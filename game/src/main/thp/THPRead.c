/*
 * THPRead - attract-movie DVD reader thread and message queues.
 */
#include "global.h"
#include "main/dll/FRONT/attract_movie.h"
#include "dolphin/os/OSThread.h"
#include "dolphin/thp/THPPlayer.h"

void PushReadedBuffer2(OSMessage msg);
OSMessage PopReadedBuffer2(void);
void PushFreeReadBuffer(OSMessage msg);
OSMessage PopReadedBuffer(void);
void ReadThreadCancel(void);
void ReadThreadStart(void);
BOOL CreateReadThread(OSPriority priority);

char gPicMenuReadThreadArea[0x1000];
OSThread gPicMenuReadThread;

extern OSMessageQueue gPicMenuReadedBuffer2Queue;
extern OSMessageQueue gPicMenuReadedBufferQueue;
extern OSMessageQueue gPicMenuFreeReadBufferQueue;
extern OSMessage gPicMenuFreeReadBufferMessages[10];
extern OSMessage gPicMenuReadedBufferMessages[10];
extern OSMessage gPicMenuReadedBuffer2Messages[10];

s32 gPicMenuReadThreadCreated;

void PushReadedBuffer2(OSMessage msg)
{
    OSSendMessage(&gPicMenuReadedBuffer2Queue, msg, OS_MESSAGE_BLOCK);
}

OSMessage PopReadedBuffer2(void)
{
    OSMessage msg;
    OSReceiveMessage(&gPicMenuReadedBuffer2Queue, &msg, OS_MESSAGE_BLOCK);
    return msg;
}

void PushFreeReadBuffer(OSMessage msg)
{
    OSSendMessage(&gPicMenuFreeReadBufferQueue, msg, OS_MESSAGE_BLOCK);
}

OSMessage PopReadedBuffer(void)
{
    OSMessage msg;
    OSReceiveMessage(&gPicMenuReadedBufferQueue, &msg, OS_MESSAGE_BLOCK);
    return msg;
}

static void* THPRead_Reader(void* unused) {
    AttractMovieReadBuffer* req;
    u32 readOff;
    u32 readSize;
    int i;

    i = 0;
    readOff = gAttractMoviePlayer.initOffset;
    readSize = gAttractMoviePlayer.initReadSize;

    while (1) {
        OSMessage msgVal;
        s32 res;

        OSReceiveMessage(&gPicMenuFreeReadBufferQueue, &msgVal, OS_MESSAGE_BLOCK);
        req = (AttractMovieReadBuffer*)msgVal;

        res = DVDReadPrio(&gAttractMoviePlayer.fileInfo, req->ptr, readSize, readOff, 2);
        if (res != (s32)readSize) {
            if (res == -1) {
                gAttractMoviePlayer.dvdError = -1;
            }
            if (i == 0) {
                PrepareReady(0);
            }
            OSSuspendThread(&gPicMenuReadThread);
        }

        req->frameNumber = i;
        OSSendMessage(&gPicMenuReadedBufferQueue, (OSMessage)req, OS_MESSAGE_BLOCK);

        readOff += readSize;
        readSize = fhSwap32(*(u32*)req->ptr);

        {
            u32 cols = gAttractMoviePlayer.header.mNumFrames;
            u32 bOff = gAttractMoviePlayer.initReadFrame;
            u32 pos = (i + bOff) % cols;
            if (pos == cols - 1) {
                if (gAttractMoviePlayer.playFlags & 1) {
                    readOff = gAttractMoviePlayer.header.mMovieDataOffsets;
                } else {
                    OSSuspendThread(&gPicMenuReadThread);
                }
            }
        }
        i++;
    }
}

void ReadThreadCancel(void)
{
    if (gPicMenuReadThreadCreated != 0)
    {
        OSCancelThread(&gPicMenuReadThread);
        gPicMenuReadThreadCreated = 0;
    }
}

void ReadThreadStart(void)
{
    if (gPicMenuReadThreadCreated != 0)
    {
        OSResumeThread(&gPicMenuReadThread);
    }
}

BOOL CreateReadThread(OSPriority priority)
{
    char* stack = gPicMenuReadThreadArea + sizeof(gPicMenuReadThreadArea);

    if (!OSCreateThread(&gPicMenuReadThread, THPRead_Reader, NULL, stack, 0x1000, priority, 1))
    {
        return 0;
    }

    OSInitMessageQueue(&gPicMenuFreeReadBufferQueue, gPicMenuFreeReadBufferMessages, 10);
    OSInitMessageQueue(&gPicMenuReadedBufferQueue, gPicMenuReadedBufferMessages, 10);
    OSInitMessageQueue(&gPicMenuReadedBuffer2Queue, gPicMenuReadedBuffer2Messages, 10);
    gPicMenuReadThreadCreated = 1;
    return 1;
}

OSMessageQueue gPicMenuFreeReadBufferQueue;
OSMessageQueue gPicMenuReadedBufferQueue;
OSMessageQueue gPicMenuReadedBuffer2Queue;
OSMessage gPicMenuFreeReadBufferMessages[10];
OSMessage gPicMenuReadedBufferMessages[10];
OSMessage gPicMenuReadedBuffer2Messages[10];
