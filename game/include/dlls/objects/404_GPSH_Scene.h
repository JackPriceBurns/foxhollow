#ifndef DLLS_OBJECTS_404_GPSH_SCENE_H_
#define DLLS_OBJECTS_404_GPSH_SCENE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct GPSHScenePlacement {
    ObjPlacement base;
    s8 initialYaw;
    u8 unknown19[0x24 - 0x19];
} GPSHScenePlacement;

STATIC_ASSERT(sizeof(GPSHScenePlacement) == 0x24);
STATIC_ASSERT(offsetof(GPSHScenePlacement, base) == 0x00);
STATIC_ASSERT(offsetof(GPSHScenePlacement, initialYaw) == 0x18);
STATIC_ASSERT(offsetof(GPSHScenePlacement, unknown19) == 0x19);

extern ObjectDescriptor gGPSHSceneObjDescriptor;

int gpshScene_getExtraSize(void);
int gpshScene_getObjectTypeId(void);
void gpshScene_free(void);
void gpshScene_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void gpshScene_hitDetect(void);
void gpshScene_update(void);
void gpshScene_init(GameObject* obj, const GPSHScenePlacement* placement);
void gpshScene_release(void);
void gpshScene_initialise(void);

#endif /* DLLS_OBJECTS_404_GPSH_SCENE_H_ */
