#ifndef MUSYX_ARAM_H_
#define MUSYX_ARAM_H_

#include "types.h"

typedef void* (*AramUploadCallback)(u32 sourceOffset, u32 size);

extern AramUploadCallback aramUploadCallback;
extern u32 aramUploadChunkSize;

void aramInit(u32 extraSize);
void aramExit(void);
u32 aramGetBaseAddress(void);
u32 aramStoreData(void *src, u32 size);
void aramRemoveData(void *unused, u32 size);
void aramInitStreamBuffers(void);
u32 aramGetStreamBufferAddress(u8 idx, u32 *outPos);

extern u32 aramTop;
extern u32 aramWrite;

#endif /* MUSYX_ARAM_H_ */
