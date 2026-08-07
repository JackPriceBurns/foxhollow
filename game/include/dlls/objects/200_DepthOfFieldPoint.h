#ifndef DLLS_OBJECTS_200_DEPTHOFFIELDPOINT_H_
#define DLLS_OBJECTS_200_DEPTHOFFIELDPOINT_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "main/objseq.h"

typedef struct DepthOfFieldPointState {
    u8 enabled : 1; /* 0x00 bit 0: blur filter currently active */
    u8 pad00 : 7;
    u8 useArea; /* 0x01: turnOnBlurFilter useArea argument */
    u8 bigger;  /* 0x02: turnOnBlurFilter bigger argument */
} DepthOfFieldPointState;

STATIC_ASSERT(offsetof(DepthOfFieldPointState, useArea) == 0x1);
STATIC_ASSERT(offsetof(DepthOfFieldPointState, bigger) == 0x2);
STATIC_ASSERT(sizeof(DepthOfFieldPointState) == 0x3);

int depthoffieldpoint_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate);
int depthoffieldpoint_getExtraSize(void);
void depthoffieldpoint_update(GameObject* obj);
void depthoffieldpoint_init(GameObject* obj);

extern ObjectDescriptor gDepthOfFieldPointObjDescriptor;

#endif /* DLLS_OBJECTS_200_DEPTHOFFIELDPOINT_H_ */
