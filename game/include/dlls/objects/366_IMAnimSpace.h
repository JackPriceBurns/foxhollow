#ifndef DLLS_OBJECTS_366_IMANIMSPACE_H_
#define DLLS_OBJECTS_366_IMANIMSPACE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/objseq.h"

typedef struct IMAnimSpaceState {
    s16 blinkTimer;
    u8 submodelMask;
    u8 eventFlags;
} IMAnimSpaceState;

STATIC_ASSERT(offsetof(IMAnimSpaceState, blinkTimer) == 0x00);
STATIC_ASSERT(offsetof(IMAnimSpaceState, submodelMask) == 0x02);
STATIC_ASSERT(offsetof(IMAnimSpaceState, eventFlags) == 0x03);
STATIC_ASSERT(sizeof(IMAnimSpaceState) == 0x04);

void imAnimSpace_modelMtxCallback(void);
u32 imAnimSpace_getEventFlag(GameObject* obj);
int imAnimSpace_isSubmodelEnabled(GameObject* obj, int bitIndex);
int imAnimSpace_sequenceCallback(GameObject* obj, int unusedArg2, ObjSeqState* animUpdate);
int imAnimSpace_getExtraSize(void);
int imAnimSpace_getObjectTypeId(void);
void imAnimSpace_free(GameObject* obj);
void imAnimSpace_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void imAnimSpace_hitDetect(void);
void imAnimSpace_update(GameObject* obj);
void imAnimSpace_init(GameObject* obj);
void imAnimSpace_release(void);
void imAnimSpace_initialise(void);

extern PartFxSpawnParams gIMAnimSpacePartFxParams;
extern ObjectDescriptor13 gIMAnimSpaceObjDescriptor;

#endif /* DLLS_OBJECTS_366_IMANIMSPACE_H_ */
