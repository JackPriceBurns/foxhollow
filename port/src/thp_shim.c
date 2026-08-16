#include <dolphin/types.h>
#include <dolphin/thp/THPAudio.h>
#include <dolphin/thp/THPDecode.h>
#include "main/dll/FRONT/attract_movie.h"
#include "foxhollow_compat.h"

#include <setjmp.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <jpeglib.h>

typedef struct THPJpegError
{
    struct jpeg_error_mgr base;
    jmp_buf jump;
} THPJpegError;

static u8* sDecodedFrame;
static size_t sDecodedFrameSize;
static u8* sJpegFrame;
static size_t sJpegFrameSize;
static size_t sCompressedFrameSize;

typedef struct THPRGBTexture
{
    u8* rgba;
    size_t size;
} THPRGBTexture;

static THPRGBTexture sRGBTextures[3];

static u32 thpReadU32(const void* ptr)
{
    u32 value;
    memcpy(&value, ptr, sizeof(value));
    return fhSwap32(value);
}

static void thpJpegErrorExit(j_common_ptr info)
{
    THPJpegError* error = (THPJpegError*)info->err;
    longjmp(error->jump, 1);
}

void fhTHPVideoSetCompressedSize(u32 size)
{
    sCompressedFrameSize = size;
}

void* fhTHPVideoGetRGB(const void* yTexture)
{
    u32 i;

    for (i = 0; i < 3; i++)
    {
        if (gAttractMoviePlayer.textureSet[i].yTexture == yTexture)
        {
            return sRGBTextures[i].rgba;
        }
    }
    return NULL;
}

static THPRGBTexture* thpGetRGBTexture(const void* yTexture, size_t size)
{
    THPRGBTexture* texture = NULL;
    u32 i;

    for (i = 0; i < 3; i++)
    {
        if (gAttractMoviePlayer.textureSet[i].yTexture == yTexture)
        {
            texture = &sRGBTextures[i];
            break;
        }
    }
    if (texture == NULL)
    {
        return NULL;
    }
    if (size > texture->size)
    {
        u8* resized = realloc(texture->rgba, size);
        if (resized == NULL)
        {
            return NULL;
        }
        texture->rgba = resized;
        texture->size = size;
    }
    return texture;
}

static u8 thpClampColor(s32 value)
{
    if (value < 0)
    {
        return 0;
    }
    if (value > 255)
    {
        return 255;
    }
    return (u8)value;
}

static size_t thpBuildJpeg(const u8* source)
{
    size_t sourceOffset = 2;
    size_t scanOffset = 0;
    size_t outputOffset;
    size_t needed;

    while (sourceOffset + 4 <= sCompressedFrameSize)
    {
        size_t markerLength;
        if (source[sourceOffset] != 0xff)
        {
            return 0;
        }
        while (sourceOffset < sCompressedFrameSize && source[sourceOffset] == 0xff)
        {
            sourceOffset++;
        }
        if (sourceOffset >= sCompressedFrameSize)
        {
            return 0;
        }
        if (source[sourceOffset] == 0xda)
        {
            markerLength = ((size_t)source[sourceOffset + 1] << 8) | source[sourceOffset + 2];
            scanOffset = sourceOffset + 1 + markerLength;
            break;
        }
        if (source[sourceOffset] == 0xd8 || (source[sourceOffset] >= 0xd0 && source[sourceOffset] <= 0xd7))
        {
            sourceOffset++;
            continue;
        }
        markerLength = ((size_t)source[sourceOffset + 1] << 8) | source[sourceOffset + 2];
        if (markerLength < 2)
        {
            return 0;
        }
        sourceOffset += 1 + markerLength;
    }

    if (scanOffset == 0 || scanOffset > sCompressedFrameSize)
    {
        return 0;
    }

    needed = sCompressedFrameSize * 2 + 2;
    if (needed > sJpegFrameSize)
    {
        u8* resized = realloc(sJpegFrame, needed);
        if (resized == NULL)
        {
            return 0;
        }
        sJpegFrame = resized;
        sJpegFrameSize = needed;
    }

    memcpy(sJpegFrame, source, scanOffset);
    outputOffset = scanOffset;
    for (sourceOffset = scanOffset; sourceOffset < sCompressedFrameSize; sourceOffset++)
    {
        sJpegFrame[outputOffset++] = source[sourceOffset];
        if (source[sourceOffset] == 0xff)
        {
            sJpegFrame[outputOffset++] = 0;
        }
    }
    sJpegFrame[outputOffset++] = 0xff;
    sJpegFrame[outputOffset++] = 0xd9;
    return outputOffset;
}

static void thpJpegEmitMessage(j_common_ptr info, int level)
{
    (void)info;
    (void)level;
}

static size_t thpTileOffset(u32 width, u32 x, u32 y)
{
    return (((size_t)(y >> 2) * (width >> 3) + (x >> 3)) << 5) + ((y & 3) << 3) + (x & 7);
}

BOOL THPInit(void)
{
    return TRUE;
}

s32 THPVideoDecode(void* file, void* tileY, void* tileU, void* tileV, void* work)
{
    struct jpeg_decompress_struct info;
    THPJpegError error;
    const u8* source = file;
    size_t sourceSize;
    size_t decodedSize;
    THPRGBTexture* rgbTexture;
    u32 width;
    u32 height;
    u32 x;
    u32 y;

    (void)work;
    if (source == NULL || tileY == NULL || tileU == NULL || tileV == NULL)
    {
        return 27;
    }

    sourceSize = thpBuildJpeg(source);
    if (sourceSize == 0)
    {
        return 3;
    }

    info.err = jpeg_std_error(&error.base);
    error.base.error_exit = thpJpegErrorExit;
    error.base.emit_message = thpJpegEmitMessage;
    if (setjmp(error.jump) != 0)
    {
        jpeg_destroy_decompress(&info);
        return 3;
    }

    jpeg_create_decompress(&info);
    jpeg_mem_src(&info, sJpegFrame, sourceSize);
    jpeg_read_header(&info, TRUE);
    info.out_color_space = JCS_YCbCr;
    jpeg_start_decompress(&info);

    width = info.output_width;
    height = info.output_height;
    if (info.output_components != 3 || width != gAttractMoviePlayer.videoInfo.xSize ||
        height != gAttractMoviePlayer.videoInfo.ySize || (width & 15) != 0 || (height & 7) != 0)
    {
        jpeg_destroy_decompress(&info);
        return 19;
    }

    decodedSize = (size_t)width * height * 3;
    if (decodedSize > sDecodedFrameSize)
    {
        u8* resized = realloc(sDecodedFrame, decodedSize);
        if (resized == NULL)
        {
            jpeg_destroy_decompress(&info);
            return 6;
        }
        sDecodedFrame = resized;
        sDecodedFrameSize = decodedSize;
    }

    while (info.output_scanline < height)
    {
        JSAMPROW row = sDecodedFrame + (size_t)info.output_scanline * width * 3;
        jpeg_read_scanlines(&info, &row, 1);
    }
    jpeg_finish_decompress(&info);
    jpeg_destroy_decompress(&info);

    rgbTexture = thpGetRGBTexture(tileY, (size_t)width * height * 4);
    if (rgbTexture == NULL)
    {
        return 6;
    }

    for (y = 0; y < height; y++)
    {
        for (x = 0; x < width; x++)
        {
            const u8* yuv = sDecodedFrame + ((size_t)y * width + x) * 3;
            s32 cb = (s32)yuv[1] - 128;
            s32 cr = (s32)yuv[2] - 128;
            u8* rgba = rgbTexture->rgba + ((size_t)y * width + x) * 4;

            ((u8*)tileY)[thpTileOffset(width, x, y)] = yuv[0];
            rgba[0] = thpClampColor((s32)yuv[0] + ((91881 * cr) >> 16));
            rgba[1] = thpClampColor((s32)yuv[0] - ((22554 * cb + 46802 * cr) >> 16));
            rgba[2] = thpClampColor((s32)yuv[0] + ((116130 * cb) >> 16));
            rgba[3] = 255;
        }
    }

    for (y = 0; y < height / 2; y++)
    {
        for (x = 0; x < width / 2; x++)
        {
            const u8* row0 = sDecodedFrame + ((size_t)y * 2 * width + x * 2) * 3;
            const u8* row1 = row0 + (size_t)width * 3;
            size_t offset = thpTileOffset(width / 2, x, y);
            ((u8*)tileU)[offset] = (u8)((row0[1] + row0[4] + row1[1] + row1[4] + 2) >> 2);
            ((u8*)tileV)[offset] = (u8)((row0[2] + row0[5] + row1[2] + row1[5] + 2) >> 2);
        }
    }

    return 0;
}

typedef struct THPAudioCursor
{
    const u8* data;
    u32 nibble;
    u8 predictor;
    u8 scale;
} THPAudioCursor;

static void thpAudioCursorInit(THPAudioCursor* cursor, const u8* data)
{
    cursor->data = data + 1;
    cursor->nibble = 2;
    cursor->predictor = (data[0] >> 4) & 7;
    cursor->scale = data[0] & 15;
}

static s32 thpAudioNextSample(THPAudioCursor* cursor)
{
    s32 sample;

    if ((cursor->nibble & 15) == 0)
    {
        cursor->predictor = (cursor->data[0] >> 4) & 7;
        cursor->scale = cursor->data[0] & 15;
        cursor->data++;
        cursor->nibble += 2;
    }
    if ((cursor->nibble & 1) != 0)
    {
        sample = (s8)(cursor->data[0] << 4) >> 4;
        cursor->data++;
    }
    else
    {
        sample = (s8)cursor->data[0] >> 4;
    }
    cursor->nibble++;
    return sample;
}

static s16 thpAudioDecodeSample(THPAudioCursor* cursor, const u8* coefficients, s16* yn1, s16* yn2)
{
    s64 value = (s64)fhReadBES16(coefficients + (cursor->predictor * 4) + 2) * *yn2;
    value += (s64)fhReadBES16(coefficients + (cursor->predictor * 4)) * *yn1;
    value += ((s64)thpAudioNextSample(cursor) << cursor->scale) << 11;
    value = (value + 1024) >> 11;
    if (value > 32767)
    {
        value = 32767;
    }
    else if (value < -32768)
    {
        value = -32768;
    }
    *yn2 = *yn1;
    *yn1 = (s16)value;
    return (s16)value;
}

u32 THPAudioDecode(s16* buffer, u8* audioFrame, s32 flag)
{
    u32 channelOffset;
    u32 sampleCount;
    const u8* leftCoefficients;
    const u8* rightCoefficients;
    const u8* leftData;
    const u8* rightData;
    THPAudioCursor leftCursor;
    THPAudioCursor rightCursor;
    s16 leftYn1;
    s16 leftYn2;
    s16 rightYn1;
    s16 rightYn2;
    u32 i;

    if (buffer == NULL || audioFrame == NULL)
    {
        return 0;
    }

    channelOffset = thpReadU32(audioFrame);
    sampleCount = thpReadU32(audioFrame + 4);
    leftCoefficients = audioFrame + 8;
    rightCoefficients = leftCoefficients + 32;
    leftYn1 = fhReadBES16(audioFrame + 72);
    leftYn2 = fhReadBES16(audioFrame + 74);
    rightYn1 = fhReadBES16(audioFrame + 76);
    rightYn2 = fhReadBES16(audioFrame + 78);
    leftData = audioFrame + sizeof(THPAudioRecordHeader);
    rightData = leftData + channelOffset;
    thpAudioCursorInit(&leftCursor, leftData);
    if (channelOffset != 0)
    {
        thpAudioCursorInit(&rightCursor, rightData);
    }

    for (i = 0; i < sampleCount; i++)
    {
        s16 left = thpAudioDecodeSample(&leftCursor, leftCoefficients, &leftYn1, &leftYn2);
        s16 right = left;
        if (channelOffset != 0)
        {
            right = thpAudioDecodeSample(&rightCursor, rightCoefficients, &rightYn1, &rightYn2);
        }
        if (flag == 1)
        {
            buffer[i] = right;
            buffer[sampleCount + i] = left;
        }
        else
        {
            buffer[i * 2] = right;
            buffer[i * 2 + 1] = left;
        }
    }

    return sampleCount;
}
