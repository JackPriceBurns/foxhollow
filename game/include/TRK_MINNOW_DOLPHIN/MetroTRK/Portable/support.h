#ifndef METROTRK_PORTABLE_SUPPORT_H
#define METROTRK_PORTABLE_SUPPORT_H

#include "PowerPC_EABI_Support/MetroTRK/trk.h"
#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif

DSError TRKSuppAccessFile(u32 file_handle, u8* data, size_t* count,
                          u8* io_result, BOOL needReply, BOOL read);
DSError TRKRequestSend(TRKBuffer* msgBuf, int* bufferId, u32 p1, u32 p2, int p3);
DSError HandleOpenFileSupportRequest(const char* path, u8 replyError, u32* fileHandle,
                                     u8* ioResult);
DSError HandleCloseFileSupportRequest(int replyError, u8* ioResult);
DSError HandlePositionFileSupportRequest(u32 replyErr, u32 filePosition, u8 positionType,
                                         u8* ioResult);

#ifdef __cplusplus
}
#endif

#endif /* METROTRK_PORTABLE_SUPPORT_H */
