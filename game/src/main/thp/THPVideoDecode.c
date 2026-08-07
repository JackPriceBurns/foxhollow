/*
 * THPVideoDecode - attract-movie video decoder thread and message queues.
 */
#include "global.h"
#include "main/dll/FRONT/attract_movie.h"
#include "main/dll/FRONT/picmenu.h"
#include "dolphin/os/OSInterrupt.h"
#include "dolphin/os/OSThread.h"
#include "dolphin/thp/THPDecode.h"
#include "dolphin/thp/THPPlayer.h"

enum
{
    THP_COMPONENT_VIDEO = 0
};

char gPicMenuVideoDecodeThreadArea[0x18];

extern OSMessageQueue gPicMenuDecodedTextureSetQueue;
extern OSMessageQueue gPicMenuFreeTextureSetQueue;
extern OSThread gPicMenuVideoDecodeThread;

s32 gAttractMovieIdleFrameCount;
s32 gPicMenuVideoDecodePrepareReady;
s32 gPicMenuVideoDecodeThreadCreated;

OSMessage PopDecodedTextureSet(s32 flags)
{
    OSMessage msg;
    if (OSReceiveMessage(&gPicMenuDecodedTextureSetQueue, &msg, flags) == 1)
    {
        return msg;
    }
    return (OSMessage)0;
}

void PushFreeTextureSet(OSMessage msg)
{
    OSSendMessage(&gPicMenuFreeTextureSetQueue, msg, OS_MESSAGE_NOBLOCK);
}

static void AttractMovieVideo_Decode(void* param) {
    AttractMoviePlayer* player;
    char* db;
    AttractMoviePlayer* player2;
    AttractMovieTextureSet* readMsg;
    u8* componentKind;
    u32 i;
    u32* compSizes;
    char* dvdData;
    OSMessage tmpBuf;

    db = gPicMenuVideoDecodeThreadArea;
    compSizes = (u32*)(((AttractMovieReadBuffer*)param)->ptr + 8);
    player = &gAttractMoviePlayer;

    dvdData = (char*)((AttractMovieReadBuffer*)param)->ptr + player->compInfo.mNumComponents * sizeof(u32) + 8;
    OSReceiveMessage((OSMessageQueue*)(db + 0x38), &tmpBuf, OS_MESSAGE_BLOCK);
    readMsg = tmpBuf;
    i = 0;
    player2 = &gAttractMoviePlayer;
    componentKind = (u8*)player2;

    while (i < player->compInfo.mNumComponents) {
        switch (componentKind[0x70]) {
        case THP_COMPONENT_VIDEO: {
            s32 dec = THPVideoDecode(dvdData, readMsg->yTexture,
                                     readMsg->uTexture,
                                     readMsg->vTexture, player2->thpWorkArea);
            player2->videoError = dec;
            if (dec != 0) {
                if (gPicMenuVideoDecodePrepareReady != 0) {
                    PrepareReady(0);
                    gPicMenuVideoDecodePrepareReady = 0;
                }
                OSSuspendThread((OSThread*)(db + 0x1058));
            }
            readMsg->frameNumber = ((AttractMovieReadBuffer*)param)->frameNumber;
            OSSendMessage((OSMessageQueue*)(db + 0x18), (OSMessage)readMsg, OS_MESSAGE_BLOCK);
            {
                u32 intr = OSDisableInterrupts();
                player2->videoDecodeCount++;
                OSRestoreInterrupts(intr);
            }
            gAttractMovieIdleFrameCount = 0;
            break;
        }
        }
        dvdData += *compSizes;
        compSizes++;
        componentKind++;
        i++;
    }

    if (gPicMenuVideoDecodePrepareReady != 0) {
        PrepareReady(1);
        gPicMenuVideoDecodePrepareReady = 0;
    }
}

static void* AttractMovieVideo_DecoderForOnMemory(void* param) {
    AttractMoviePlayer* player = &gAttractMoviePlayer;
    u32 frameSize = player->frameStride;
    void* cur = param;
    int i = 0;

    while (1) {
        if (player->audioExists != 0) {
            while (player->videoDecodeCount < 0) {
                {
                    u32 intr = OSDisableInterrupts();
                    player->videoDecodeCount += 1;
                    OSRestoreInterrupts(intr);
                }
                {
                    u32 cols;
                    u32 bOff = player->initReadFrame;
                    u32 sum = i + bOff;
                    u32 pos = sum % (cols = player->header.mNumFrames);
                    if (pos == cols - 1) {
                        if (!(player->playFlags & 1)) {
                            break; /* pos==cols-1, not looping: go to decode */
                        }
                        frameSize = *(u32*)cur;
                        cur = player->loopFrame;
                    } else {
                        u32 nextSize = *(u32*)cur;
                        cur = (char*)cur + frameSize;
                        frameSize = nextSize;
                    }
                }
                i++;
            }
        }

        *(s32*)(&cur + 1) = i;
        AttractMovieVideo_Decode(&cur);

        {
            u32 cols;
            u32 bOff = player->initReadFrame;
            u32 sum = i + bOff;
            u32 pos = sum % (cols = player->header.mNumFrames);
            if (pos == cols - 1) {
                if (player->playFlags & 1) {
                    frameSize = *(u32*)cur;
                    cur = player->loopFrame;
                } else {
                    OSSuspendThread(&gPicMenuVideoDecodeThread);
                }
            } else {
                u32 nextSize = *(u32*)cur;
                cur = (char*)cur + frameSize;
                frameSize = nextSize;
            }
        }
        i++;
    }
}

static void* AttractMovieVideo_Decoder(void* unused) {
    AttractMoviePlayer* player = &gAttractMoviePlayer;
    void* msg;

    while (1) {
        if (player->audioExists != 0) {
            while (player->videoDecodeCount < 0) {
                msg = PopReadedBuffer2();
                {
                    u32 cols = player->header.mNumFrames;
                    u32 bOff = player->initReadFrame;
                    u32 pos = (*(u32*)((char*)msg + 4) + bOff) % cols;
                    if (pos == cols - 1 && !(player->playFlags & 1)) {
                        AttractMovieVideo_Decode(msg);
                    }
                }
                PushFreeReadBuffer((OSMessage)msg);
                {
                    u32 intr = OSDisableInterrupts();
                    player->videoDecodeCount += 1;
                    OSRestoreInterrupts(intr);
                }
            }
        }
        if (player->audioExists != 0) {
            msg = PopReadedBuffer2();
        } else {
            msg = PopReadedBuffer();
        }
        AttractMovieVideo_Decode(msg);
        PushFreeReadBuffer((OSMessage)msg);
    }
}

void VideoDecodeThreadCancel(void)
{
    if (gPicMenuVideoDecodeThreadCreated != 0)
    {
        OSCancelThread(&gPicMenuVideoDecodeThread);
        gPicMenuVideoDecodeThreadCreated = 0;
    }
}

void VideoDecodeThreadStart(void)
{
    if (gPicMenuVideoDecodeThreadCreated != 0)
    {
        OSResumeThread(&gPicMenuVideoDecodeThread);
    }
}

BOOL CreateVideoDecodeThread(OSPriority priority, u32 onMemoryArg)
{
    char* db = gPicMenuVideoDecodeThreadArea;
    void* mbuf = db;

    if (onMemoryArg != 0)
    {
        if (!OSCreateThread((OSThread*)(db + 0x1058), AttractMovieVideo_DecoderForOnMemory, (void*)onMemoryArg,
                            (void*)(db + 0x1058), 0x1000, priority, 1))
        {
            return 0;
        }
    }
    else
    {
        if (!OSCreateThread((OSThread*)(db + 0x1058), AttractMovieVideo_Decoder, NULL, (void*)(db + 0x1058), 0x1000,
                            priority, 1))
        {
            return 0;
        }
    }

    OSInitMessageQueue((OSMessageQueue*)(db + 0x38), (void*)(db + 0x0C), 3);
    OSInitMessageQueue((OSMessageQueue*)(db + 0x18), mbuf, 3);
    gPicMenuVideoDecodeThreadCreated = 1;
    gPicMenuVideoDecodePrepareReady = 1;
    return 1;
}

OSThread gPicMenuVideoDecodeThread;
char gPicMenuVideoDecodeThreadStack[0x1000];
OSMessageQueue gPicMenuFreeTextureSetQueue;
OSMessageQueue gPicMenuDecodedTextureSetQueue;
