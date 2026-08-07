#ifndef DLLS_OBJECTS_377_DFSH_OBJCRE_H_
#define DLLS_OBJECTS_377_DFSH_OBJCRE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct DFSHObjCreatorPlacement {
    ObjPlacement base;
    u8 unknown18[0x1E - 0x18];
    s8 initialYaw;
    s8 triggerGameBitOffset;
} DFSHObjCreatorPlacement;

typedef struct DFSHObjCreatorState {
    s16 spawnTimer;
    s16 spawnTimerRate;
} DFSHObjCreatorState;

STATIC_ASSERT(offsetof(DFSHObjCreatorPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(DFSHObjCreatorPlacement, unknown18) == 0x18);
STATIC_ASSERT(offsetof(DFSHObjCreatorPlacement, initialYaw) == 0x1E);
STATIC_ASSERT(offsetof(DFSHObjCreatorPlacement, triggerGameBitOffset) == 0x1F);

STATIC_ASSERT(sizeof(DFSHObjCreatorState) == 0x04);
STATIC_ASSERT(offsetof(DFSHObjCreatorState, spawnTimer) == 0x00);
STATIC_ASSERT(offsetof(DFSHObjCreatorState, spawnTimerRate) == 0x02);

extern ObjectDescriptor gDFSHObjCreatorObjDescriptor;

int dfshObjCreator_getExtraSize(void);
int dfshObjCreator_getObjectTypeId(void);
void dfshObjCreator_free(void);
void dfshObjCreator_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dfshObjCreator_hitDetect(void);
void dfshObjCreator_update(GameObject* obj);
void dfshObjCreator_init(GameObject* obj, const DFSHObjCreatorPlacement* placement);
void dfshObjCreator_release(void);
void dfshObjCreator_initialise(void);

#endif /* DLLS_OBJECTS_377_DFSH_OBJCRE_H_ */
