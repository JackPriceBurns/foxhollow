#ifndef DLLS_OBJECTS_323_FESEQOBJECT_H_
#define DLLS_OBJECTS_323_FESEQOBJECT_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "main/objseq.h"

/* FEseqobject_getExtraSize proves the complete one-byte allocation. */
typedef struct FEseqobjectState {
    u8 unused;
} FEseqobjectState;

STATIC_ASSERT(sizeof(FEseqobjectState) == 0x01);

int FEseqobject_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate);
int FEseqobject_getExtraSize(void);
int FEseqobject_getObjectTypeId(void);
void FEseqobject_free(void);
void FEseqobject_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void FEseqobject_hitDetect(void);
void FEseqobject_update(GameObject* obj);
void FEseqobject_init(GameObject* obj);
void FEseqobject_release(void);
void FEseqobject_initialise(void);

extern ObjectDescriptor gFEseqobjectObjDescriptor;

#endif /* DLLS_OBJECTS_323_FESEQOBJECT_H_ */
