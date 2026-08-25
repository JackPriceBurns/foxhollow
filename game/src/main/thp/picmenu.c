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
#include "main/attract_movie.h"

char sPicMenuThpMagic[] = "THP";

#define THP_VERSION_1_0 0x10000

/* per-frame component kinds in THPHeader::mCompInfoDataOffsets table */
enum
{
    THP_COMPONENT_VIDEO = 0,
    THP_COMPONENT_AUDIO = 1
};

static u32 movieReadU32(const void* ptr)
{
    u32 value;
    memcpy(&value, ptr, sizeof(value));
    return fhSwap32(value);
}

static f32 movieReadF32(const void* ptr)
{
    union
    {
        u32 bits;
        f32 value;
    } result;
    result.bits = movieReadU32(ptr);
    return result.value;
}

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

    gAttractMoviePlayer.header.mVersion = movieReadU32(gPicMenuDvdReadBuffer + 0x04);
    gAttractMoviePlayer.header.mBufferSize = movieReadU32(gPicMenuDvdReadBuffer + 0x08);
    gAttractMoviePlayer.header.mAudioMaxSamples = movieReadU32(gPicMenuDvdReadBuffer + 0x0c);
    gAttractMoviePlayer.header.mFrameRate = movieReadF32(gPicMenuDvdReadBuffer + 0x10);
    gAttractMoviePlayer.header.mNumFrames = movieReadU32(gPicMenuDvdReadBuffer + 0x14);
    gAttractMoviePlayer.header.mFirstFrameSize = movieReadU32(gPicMenuDvdReadBuffer + 0x18);
    gAttractMoviePlayer.header.mMovieDataSize = movieReadU32(gPicMenuDvdReadBuffer + 0x1c);
    gAttractMoviePlayer.header.mCompInfoDataOffsets = movieReadU32(gPicMenuDvdReadBuffer + 0x20);
    gAttractMoviePlayer.header.mOffsetDataOffsets = movieReadU32(gPicMenuDvdReadBuffer + 0x24);
    gAttractMoviePlayer.header.mMovieDataOffsets = movieReadU32(gPicMenuDvdReadBuffer + 0x28);
    gAttractMoviePlayer.header.mFinalFrameDataOffsets = movieReadU32(gPicMenuDvdReadBuffer + 0x2c);

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
        gAttractMoviePlayer.compInfo.mNumComponents = movieReadU32(gPicMenuDvdReadBuffer);
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
            gAttractMoviePlayer.videoInfo.xSize = movieReadU32(gPicMenuDvdReadBuffer);
            gAttractMoviePlayer.videoInfo.ySize = movieReadU32(gPicMenuDvdReadBuffer + 4);
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
            gAttractMoviePlayer.audioInfo.channelCount = movieReadU32(gPicMenuDvdReadBuffer);
            gAttractMoviePlayer.audioInfo.frequency = movieReadU32(gPicMenuDvdReadBuffer + 4);
            gAttractMoviePlayer.audioInfo.sampleCount = movieReadU32(gPicMenuDvdReadBuffer + 8);
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
    gAttractMoviePlayer.isOnMemory = (intptr_t)onMemory;
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

    memset(&gAttractMoviePlayer, 0, sizeof(AttractMoviePlayer));
    OSInitMessageQueue(&gAttractMovieSpentTextureSetQueue,
                       (void*)(gAttractMovieAudioDmaBuffer + ATTRACT_MOVIE_AUDIO_DMA_BUFFER_BYTES), 3);

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
        memset(gAttractMovieAudioDmaBuffer, 0, ATTRACT_MOVIE_AUDIO_DMA_BUFFER_BYTES);
        DCFlushRange(gAttractMovieAudioDmaBuffer, ATTRACT_MOVIE_AUDIO_DMA_BUFFER_BYTES);
        AIInitDMA((uintptr_t)(gAttractMovieAudioDmaBuffer + gAttractMovieAudioDmaBufferIndex * ATTRACT_MOVIE_AUDIO_DMA_BUFFER_SIZE),
                  ATTRACT_MOVIE_AUDIO_DMA_BUFFER_SIZE);
        AIStartDMA();
    }

    gAttractMovieAudioActive = 1;
    return 1;
}
