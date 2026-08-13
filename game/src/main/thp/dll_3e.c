/*
 * dll_3e - THP attract-mode movie playback (FRONT/attract_movie).
 *
 * Drives the streamed THP attract movie via the VI post-retrace callback:
 * PlayControl() runs each retrace, pops a decoded texture set from the
 * decode pipeline, paces it against the audio track (single-field /
 * even-field / odd-field cadence from playFlags), recycles the previously
 * displayed set back to its message queue, and detects end-of-movie /
 * loop completion.
 *
 * THPPlayerPlay/THPPlayerStop arm and tear down the player (state machine
 * in AttractMoviePlayer.state / internalState, with worker threads for
 * video decode, audio decode and DVD read).
 *
 * prepareAttractMode() seeks to a movie within the attract package
 * (offset table indexed by movieIndex), spins up the decode/read threads,
 * primes the message queues (InitAllMessageQueue) and installs the
 * retrace callback.
 */
#include "global.h"
#include "dolphin/ai.h"
#include "dolphin/os.h"
#include "dolphin/vi/vifuncs.h"
#include "main/dll/FRONT/dll_3B.h"
#include "main/dll/FRONT/n_options.h"
#include "main/attract_movie_api.h"
#include "main/fileio.h"
#include "main/audio_decode_thread.h"
#include "main/dll/FRONT/picmenu.h"
#include "main/dll/dll_3e_api.h"
#include "dolphin/thp/THPDraw.h"
#include "dolphin/thp/THPDecode.h"

enum {
    THP_PLAY_LOOP = 1,
    THP_PLAY_EVEN_FIELD = 2,
    THP_PLAY_ODD_FIELD = 4
};

extern OSMessageQueue gAttractMoviePrepareReadyQueue;
static void InitAllMessageQueue(void);

u8 gAttractMovieLoopCompleted;
OSMessage lbl_803DD67C;
u32 gAttractMovieAudioDmaBufferIndex;
uintptr_t gAttractMovieAudioPendingSourceAddr;
uintptr_t gAttractMovieAudioMixSourceAddr;
s32 gAttractMovieAudioMode;
AIDCallback gAttractMovieAudioPrevDmaCallback;
static VIRetraceCallback OldVIPostCallback;

#ifdef TARGET_PC
extern void fhTHPVideoSetCompressedSize(u32 size);

static u32 sPcMovieReadOffset;
static u32 sPcMovieReadSize;
static u32 sPcMovieFrame;
static u32 sPcMovieTexture;
static OSTime sPcMovieNextFrameTime;
static OSTime sPcMovieFrameTicks;

static BOOL DecodeNextMovieFramePC(void)
{
    AttractMoviePlayer* player = &gAttractMoviePlayer;
    AttractMovieReadBuffer* readBuffer = &player->readBuffer[0];
    AttractMovieTextureSet* textureSet = &player->textureSet[sPcMovieTexture];
    u32* componentSizes;
    u8* componentData;
    BOOL decoded = FALSE;
    u32 i;

    if (DVDRead(&player->fileInfo, readBuffer->ptr, sPcMovieReadSize, sPcMovieReadOffset) != (s32)sPcMovieReadSize)
    {
        player->dvdError = -1;
        return FALSE;
    }

    componentSizes = (u32*)(readBuffer->ptr + 8);
    componentData = readBuffer->ptr + 8 + player->compInfo.mNumComponents * sizeof(u32);
    for (i = 0; i < player->compInfo.mNumComponents; i++)
    {
        u32 componentSize = fhSwap32(componentSizes[i]);
        if (player->compInfo.mFrameComp[i] == 0)
        {
            s32 decodeError;
            fhTHPVideoSetCompressedSize(componentSize);
            decodeError = THPVideoDecode(componentData, textureSet->yTexture, textureSet->uTexture,
                                         textureSet->vTexture, player->thpWorkArea);
            if (decodeError != 0 && player->curTextureSet == NULL)
            {
                player->videoError = decodeError;
                return FALSE;
            }
            decoded = decodeError == 0;
        }
        else if (player->compInfo.mFrameComp[i] == 1)
        {
            AttractMovieAudio_DecodeFramePC(componentData, sPcMovieFrame);
        }
        componentData += componentSize;
    }

    player->curAudioTrack = sPcMovieFrame;
    if (decoded)
    {
        player->videoError = 0;
        textureSet->frameNumber = sPcMovieFrame;
        player->curTextureSet = textureSet;
        sPcMovieTexture = (sPcMovieTexture + 1) % 3;
    }
    sPcMovieReadOffset += sPcMovieReadSize;
    sPcMovieReadSize = fhSwap32(*(u32*)readBuffer->ptr);
    sPcMovieFrame++;
    if (sPcMovieFrame >= player->header.mNumFrames)
    {
        sPcMovieFrame = 0;
        sPcMovieReadOffset = player->header.mMovieDataOffsets;
        sPcMovieReadSize = player->header.mFirstFrameSize;
        gAttractMovieLoopCompleted = 1;
    }
    return TRUE;
}
#endif

static void PlayControl(u32 retraceCount) {
    AttractMovieTextureSet* decodedTexture;
    s32 frame;
    int allowPop;
    s32 modResult;

    if (OldVIPostCallback != NULL) {
        OldVIPostCallback(retraceCount);
    }

    decodedTexture = (AttractMovieTextureSet*)-1;
    if (gAttractMoviePlayer.isOpen == 0) {
        return;
    }
    if (gAttractMoviePlayer.state != 2) {
        return;
    }
    if ((gAttractMoviePlayer.dvdError != 0) || (gAttractMoviePlayer.videoError != 0)) {
        gAttractMoviePlayer.internalState = 5;
        gAttractMoviePlayer.state = 5;
        return;
    }

#ifdef TARGET_PC
    OSTime now;

    gAttractMovieIdleFrameCount = 0;
    gAttractMoviePlayer.retraceCount++;
    if (gAttractMoviePlayer.retraceCount == 0)
    {
        gAttractMoviePlayer.internalState = 2;
        return;
    }
    now = OSGetTime();
    if (now >= sPcMovieNextFrameTime)
    {
        DecodeNextMovieFramePC();
        sPcMovieNextFrameTime += sPcMovieFrameTicks;
        if (now - sPcMovieNextFrameTime >= sPcMovieFrameTicks)
        {
            sPcMovieNextFrameTime = now + sPcMovieFrameTicks;
        }
    }
    return;
#endif

    if ((gAttractMoviePlayer.retraceCount == 0) &&
        ((gAttractMoviePlayer.internalState == 0) || (gAttractMoviePlayer.internalState == 4))) {
        gAttractMoviePlayer.internalState = 2;
    }
    gAttractMoviePlayer.retraceCount++;

    if ((gAttractMoviePlayer.internalState == 0) || (gAttractMoviePlayer.internalState == 4)) {
        do {
            if ((gAttractMoviePlayer.playFlags & THP_PLAY_EVEN_FIELD) != 0) {
                if (VIGetNextField() == 0) {
                    allowPop = 1;
                    break;
                }
            } else if ((gAttractMoviePlayer.playFlags & THP_PLAY_ODD_FIELD) != 0) {
                if (VIGetNextField() == 1) {
                    allowPop = 1;
                    break;
                }
            } else {
                allowPop = 1;
                break;
            }
            allowPop = 0;
        } while (0);

        if (allowPop != 0) {
            if (gAttractMoviePlayer.audioExists != 0) {
                frame = gAttractMoviePlayer.curAudioTrack - gAttractMoviePlayer.curVideoNumber;
                if (frame <= 1) {
                    decodedTexture = (AttractMovieTextureSet*)PopDecodedTextureSet(0);
                    if (gAttractMoviePlayer.videoDecodeCount > frame) {
                        gAttractMoviePlayer.videoDecodeCount--;
                    }
                } else {
                    gAttractMoviePlayer.internalState = 2;
                }
            } else {
                decodedTexture = (AttractMovieTextureSet*)PopDecodedTextureSet(0);
                gAttractMoviePlayer.internalState = 2;
            }
        } else {
            gAttractMoviePlayer.retraceCount = -1;
        }
    } else if (ProperTimingForGettingNextFrame() != 0) {
        if (gAttractMoviePlayer.audioExists != 0) {
            frame = gAttractMoviePlayer.curAudioTrack - gAttractMoviePlayer.curVideoNumber;
            if (frame <= 1) {
                decodedTexture = (AttractMovieTextureSet*)PopDecodedTextureSet(0);
                if (gAttractMoviePlayer.videoDecodeCount > frame) {
                    gAttractMoviePlayer.videoDecodeCount--;
                }
            }
        } else {
            decodedTexture = (AttractMovieTextureSet*)PopDecodedTextureSet(0);
        }
    }

    if ((decodedTexture != NULL) && (decodedTexture != (AttractMovieTextureSet*)-1)) {
        gAttractMoviePlayer.curAudioTrack = decodedTexture->frameNumber;
        if (gAttractMoviePlayer.curTextureSet != NULL) {
            OSSendMessage(&gAttractMovieSpentTextureSetQueue, (OSMessage)gAttractMoviePlayer.curTextureSet,
                          OS_MESSAGE_NOBLOCK);
        }
        gAttractMoviePlayer.curTextureSet = decodedTexture;
    }

    if ((gAttractMoviePlayer.playFlags & THP_PLAY_LOOP) == 0) {
        if (gAttractMoviePlayer.audioExists != 0) {
            modResult = (gAttractMoviePlayer.curVideoNumber + gAttractMoviePlayer.initReadFrame) %
                        gAttractMoviePlayer.header.mNumFrames;
            if ((modResult == (gAttractMoviePlayer.header.mNumFrames - 1)) &&
                (gAttractMoviePlayer.dispTextureSet == NULL)) {
                modResult = (gAttractMoviePlayer.curAudioTrack + gAttractMoviePlayer.initReadFrame) %
                            gAttractMoviePlayer.header.mNumFrames;
                if ((modResult == (gAttractMoviePlayer.header.mNumFrames - 1)) && (decodedTexture == NULL)) {
                    gAttractMoviePlayer.internalState = 3;
                    gAttractMoviePlayer.state = 3;
                }
            }
        } else {
            u32 numFrames;
            modResult = (gAttractMoviePlayer.curAudioTrack + gAttractMoviePlayer.initReadFrame) %
                        (numFrames = gAttractMoviePlayer.header.mNumFrames);
            if ((modResult == (numFrames - 1)) && (decodedTexture == NULL)) {
                gAttractMoviePlayer.internalState = 3;
                gAttractMoviePlayer.state = 3;
            }
        }
    } else {
        u32 numFrames;
        modResult = (gAttractMoviePlayer.curAudioTrack + gAttractMoviePlayer.initReadFrame) %
                    (numFrames = gAttractMoviePlayer.header.mNumFrames);
        if (modResult == (numFrames - 1)) {
            gAttractMovieLoopCompleted = 1;
        }
    }
}

void THPPlayerStop(void) {
    OSMessage msg;

    if ((gAttractMoviePlayer.isOpen != 0) && (gAttractMoviePlayer.state != 0)) {
        gAttractMoviePlayer.internalState = 0;
        gAttractMoviePlayer.state = 0;
        VISetPostRetraceCallback(OldVIPostCallback);

        if (gAttractMoviePlayer.isOnMemory == 0) {
            DVDCancel((DVDCommandBlock*)&gAttractMoviePlayer.fileInfo);
            ReadThreadCancel();
        }

        VideoDecodeThreadCancel();
        if (gAttractMoviePlayer.audioExists != 0) {
            AudioDecodeThreadCancel();
        }

        while (
            ((OSReceiveMessage(&gAttractMovieSpentTextureSetQueue, &msg, OS_MESSAGE_NOBLOCK) == TRUE) ? msg : NULL) !=
            NULL) {
        }

        gAttractMoviePlayer.curVolume = gAttractMoviePlayer.targetVolume;
        gAttractMoviePlayer.rampCount = 0;
        gAttractMoviePlayer.dvdError = 0;
        gAttractMoviePlayer.videoError = 0;
    }
}

BOOL THPPlayerPlay(void) {
    if ((gAttractMoviePlayer.isOpen != 0) && ((gAttractMoviePlayer.state == 1) || (gAttractMoviePlayer.state == 4))) {
        gAttractMoviePlayer.state = 2;
        gAttractMoviePlayer.prevCount = 0;
        gAttractMoviePlayer.curCount = 0;
        gAttractMoviePlayer.retraceCount = -1;
        return TRUE;
    }
    return FALSE;
}

BOOL prepareAttractMode(u32 movieIndex, s32 playFlags) {
    AttractMoviePlayer* player;
    s32 readyMsg;
    uintptr_t startOffset;
    extern char gPicMenuDvdReadBuffer[0x40];

    player = &gAttractMoviePlayer;
    gAttractMovieLoopCompleted = 0;

    if (player->isOpen != 0 && player->state == 0) {
        if ((s32)movieIndex > 0) {
            u32 offsetTable = player->header.mOffsetDataOffsets;

            if (offsetTable == 0) {
                return FALSE;
            }
            if (player->header.mNumFrames > movieIndex) {
                if (DVDRead(&player->fileInfo, gPicMenuDvdReadBuffer, 0x20,
                            offsetTable + ((movieIndex - 1) * sizeof(u32))) < 0) {
                    return FALSE;
                }

                player->initOffset = player->header.mMovieDataOffsets + fhSwap32(*(u32*)gPicMenuDvdReadBuffer);
                player->initReadFrame = movieIndex;
                player->initReadSize = fhSwap32(*(u32*)(gPicMenuDvdReadBuffer + 4)) -
                                       fhSwap32(*(u32*)gPicMenuDvdReadBuffer);
            } else {
                return FALSE;
            }
        } else {
            player->initOffset = player->header.mMovieDataOffsets;
            player->initReadSize = player->header.mFirstFrameSize;
            player->initReadFrame = movieIndex;
        }

        player->playFlags = playFlags;
        player->videoDecodeCount = 0;

#ifdef TARGET_PC
        sPcMovieReadOffset = player->initOffset;
        sPcMovieReadSize = player->initReadSize;
        sPcMovieFrame = player->initReadFrame;
        sPcMovieTexture = 0;
        sPcMovieFrameTicks = (OSTime)((f64)OS_TIMER_CLOCK / player->header.mFrameRate);
        sPcMovieNextFrameTime = OSGetTime() + sPcMovieFrameTicks;
        player->curAudioTrack = 0;
        player->curVideoNumber = 0;
        player->curTextureSet = NULL;
        player->dispTextureSet = NULL;
        if (player->audioExists != 0)
        {
            AttractMovieAudio_InitQueuesPC();
        }
        if (!DecodeNextMovieFramePC())
        {
            return FALSE;
        }
        player->state = 1;
        player->internalState = 0;
        OldVIPostCallback = VISetPostRetraceCallback(PlayControl);
        return TRUE;
#endif

        if (player->isOnMemory != 0) {
            if (DVDRead(&player->fileInfo, player->loopFrame, player->header.mMovieDataSize,
                        player->header.mMovieDataOffsets) < 0) {
                return FALSE;
            }
            startOffset = ((uintptr_t)player->loopFrame + player->initOffset) - player->header.mMovieDataOffsets;
            CreateVideoDecodeThread(0xf, startOffset);
            if (player->audioExists != 0) {
                CreateAudioDecodeThread(0xc, (void*)startOffset);
            }
        } else {
            CreateVideoDecodeThread(0xf, 0);
            if (player->audioExists != 0) {
                CreateAudioDecodeThread(0xc, NULL);
            }
            CreateReadThread(8);
        }

        InitAllMessageQueue();
        VideoDecodeThreadStart();
        if (player->audioExists != 0) {
            AudioDecodeThreadStart();
        }
        if (player->isOnMemory == 0) {
            ReadThreadStart();
        }

        OSReceiveMessage(&gAttractMoviePrepareReadyQueue, (OSMessage*)&readyMsg, OS_MESSAGE_BLOCK);
        if (readyMsg == 0) {
            return FALSE;
        }
        player->state = 1;
        player->internalState = 0;
        player->curAudioTrack = 0;
        player->curVideoNumber = 0;
        player->curTextureSet = NULL;
        player->dispTextureSet = NULL;
        OldVIPostCallback = VISetPostRetraceCallback(PlayControl);
        return TRUE;
    }
    return FALSE;
}

void PrepareReady(void* msg) {
    OSSendMessage(&gAttractMoviePrepareReadyQueue, msg, OS_MESSAGE_BLOCK);
}

static void InitAllMessageQueue(void) {
    AttractMoviePlayer* player;
    s32 i;

    player = &gAttractMoviePlayer;
    if (player->isOnMemory == 0) {
        for (i = 0; i < 10; i++) {
            PushFreeReadBuffer((OSMessage)&player->readBuffer[i]);
        }
    }

    i = 0;
    player = &gAttractMoviePlayer;
    do {
        PushFreeTextureSet((OSMessage)&player->textureSet[i]);
        i++;
    } while (i < 3);

    if (gAttractMoviePlayer.audioExists != 0) {
        i = 0;
        do {
            PushFreeAudioBuffer((OSMessage)&player->audioBuffer[i]);
            i++;
        } while (i < 3);
    }

    OSInitMessageQueue(&gAttractMoviePrepareReadyQueue, &lbl_803DD67C, 1);
}
