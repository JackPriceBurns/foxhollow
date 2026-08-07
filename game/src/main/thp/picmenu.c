/*
 * picmenu - THP movie loading and audio DMA setup.
 *
 * Opens and validates the attract-mode THP stream, reads its component
 * metadata, and initializes or shuts down the movie audio path.
 */
#include "dolphin/ai.h"
#include "main/dll/FRONT/attract_movie.h"
#include "main/dll/FRONT/n_options.h"
#include "dolphin/os/OSCache.h"
#include "dolphin/os/OSInterrupt.h"
#include "dolphin/os/OSMessage.h"
#include "dolphin/thp/THPPlayer.h"
#include "main/fileio.h"
#include "string.h"
#include "main/dll/FRONT/picmenu.h"
#include "main/attract_movie_api.h"

char sPicMenuThpMagic[] = "THP";

#define THP_VERSION_1_0 0x10000

/* per-frame component kinds in THPHeader::mCompInfoDataOffsets table */
enum
{
    THP_COMPONENT_VIDEO = 0,
    THP_COMPONENT_AUDIO = 1
};

BOOL movieLoad(const char* fileName, void* onMemory)
{
    u32 readOff;
    s32 result;
    u32 i;

    if (gAttractMovieAudioActive == 0)
    {
        return 0;
    }

    if (gAttractMoviePlayer.isOpen != 0)
    {
        return 0;
    }

    memset(&gAttractMoviePlayer.videoInfo, 0, sizeof(AttractMovieVideoInfo));
    memset(&gAttractMoviePlayer.audioInfo, 0, sizeof(AttractMovieAudioInfo));

    if (!DVDOpen(fileName, &gAttractMoviePlayer.fileInfo))
    {
        return 0;
    }

    result = DVDRead(&gAttractMoviePlayer.fileInfo, gPicMenuDvdReadBuffer, 0x40, 0);
    if (result < 0)
    {
        DVDClose(&gAttractMoviePlayer.fileInfo);
        return 0;
    }

    memcpy(&gAttractMoviePlayer.header, gPicMenuDvdReadBuffer,
           sizeof(gAttractMoviePlayer.header));

    if (strcmp(gAttractMoviePlayer.header.mMagic, sPicMenuThpMagic) != 0)
    {
        DVDClose(&gAttractMoviePlayer.fileInfo);
        return 0;
    }

    if (gAttractMoviePlayer.header.mVersion != THP_VERSION_1_0)
    {
        DVDClose(&gAttractMoviePlayer.fileInfo);
        return 0;
    }

    {
        u32 compOff = gAttractMoviePlayer.header.mCompInfoDataOffsets;

        result = DVDRead(&gAttractMoviePlayer.fileInfo, gPicMenuDvdReadBuffer, 0x20, compOff);
        if (result < 0)
        {
            DVDClose(&gAttractMoviePlayer.fileInfo);
            return 0;
        }

        memcpy(&gAttractMoviePlayer.compInfo, gPicMenuDvdReadBuffer, sizeof(THPFrameCompInfo));
        readOff = compOff + sizeof(THPFrameCompInfo);
        gAttractMoviePlayer.audioExists = 0;
    }

    for (i = 0; i < gAttractMoviePlayer.compInfo.mNumComponents; i++)
    {
        switch (gAttractMoviePlayer.compInfo.mFrameComp[i])
        {
        case THP_COMPONENT_VIDEO:
            result = DVDRead(&gAttractMoviePlayer.fileInfo, gPicMenuDvdReadBuffer, 0x20, readOff);
            if (result < 0)
            {
                DVDClose(&gAttractMoviePlayer.fileInfo);
                return 0;
            }
            memcpy(&gAttractMoviePlayer.videoInfo, gPicMenuDvdReadBuffer,
                   sizeof(AttractMovieVideoInfo));
            readOff += sizeof(AttractMovieVideoInfo);
            break;
        case THP_COMPONENT_AUDIO:
            result = DVDRead(&gAttractMoviePlayer.fileInfo, gPicMenuDvdReadBuffer, 0x20, readOff);
            if (result < 0)
            {
                DVDClose(&gAttractMoviePlayer.fileInfo);
                return 0;
            }
            memcpy(&gAttractMoviePlayer.audioInfo, gPicMenuDvdReadBuffer,
                   sizeof(AttractMovieAudioInfo));
            gAttractMoviePlayer.audioExists = 1;
            readOff += sizeof(AttractMovieAudioInfo);
            break;
        default:
            return 0;
        }
    }

    gAttractMoviePlayer.internalState = 0;
    gAttractMoviePlayer.state = 0;
    gAttractMoviePlayer.playFlags = 0;
    gAttractMoviePlayer.isOnMemory = (s32)onMemory;
    gAttractMoviePlayer.isOpen = 1;
    gAttractMoviePlayer.curVolume = 127.0f;
    gAttractMoviePlayer.targetVolume = 127.0f;
    gAttractMoviePlayer.rampCount = 0;

    return 1;
}

void AttractMovieAudio_Shutdown(void)
{
    u32 saved = OSDisableInterrupts();
    if (gAttractMovieAudioPrevDmaCallback != (AIDCallback)0)
    {
        AIRegisterDMACallback(gAttractMovieAudioPrevDmaCallback);
    }
    OSRestoreInterrupts(saved);
    gAttractMovieAudioActive = 0;
}

BOOL AttractMovieAudio_Init(int audioMode)
{
    u32 saved;
    AIDCallback oldCb;
    register AIDCallback dmaCallback;

    memset((AttractMoviePlayer*)((char*)(int)gAttractMovieAudioDmaBuffer + 0x5A0), 0, sizeof(AttractMoviePlayer));
    OSInitMessageQueue((OSMessageQueue*)((char*)(int)gAttractMovieAudioDmaBuffer + 0x50C),
                       (void*)((char*)(int)gAttractMovieAudioDmaBuffer + ATTRACT_MOVIE_AUDIO_DMA_BUFFER_BYTES), 3);

    if (!THPInit())
    {
        return 0;
    }

    saved = OSDisableInterrupts();
    gAttractMovieAudioMode = audioMode;
    gAttractMovieAudioDmaBufferIndex = 0;
    gAttractMovieAudioPendingSourceAddr = 0;
    gAttractMovieAudioMixSourceAddr = 0;
    dmaCallback = AttractMovieAudio_DmaCallback;
    oldCb = AIRegisterDMACallback(dmaCallback);
    gAttractMovieAudioPrevDmaCallback = oldCb;

    if (oldCb == (AIDCallback)0)
    {
        if (gAttractMovieAudioMode != 0)
        {
            AIRegisterDMACallback((AIDCallback)0);
            OSRestoreInterrupts(saved);
            return 0;
        }
    }

    OSRestoreInterrupts(saved);

    if (gAttractMovieAudioMode == 0)
    {
        memset((char*)(int)gAttractMovieAudioDmaBuffer, 0, ATTRACT_MOVIE_AUDIO_DMA_BUFFER_BYTES);
        DCFlushRange((char*)(int)gAttractMovieAudioDmaBuffer, ATTRACT_MOVIE_AUDIO_DMA_BUFFER_BYTES);
        AIInitDMA((u32)((char*)(int)gAttractMovieAudioDmaBuffer + gAttractMovieAudioDmaBufferIndex * ATTRACT_MOVIE_AUDIO_DMA_BUFFER_SIZE),
                  ATTRACT_MOVIE_AUDIO_DMA_BUFFER_SIZE);
        AIStartDMA();
    }

    gAttractMovieAudioActive = 1;
    return 1;
}
