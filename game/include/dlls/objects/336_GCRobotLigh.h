#ifndef DLLS_OBJECTS_336_GCROBOTLIGH_H_
#define DLLS_OBJECTS_336_GCROBOTLIGH_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "main/modellight_api.h"

typedef struct GcRobotLightBeamStatusFlags {
    u8 playerCaught : 1;
    u8 unused : 7;
} GcRobotLightBeamStatusFlags;

typedef struct GcRobotLightBeamState {
    ModelLightStruct* pointLight;
    int unknown4;
    GcRobotLightBeamStatusFlags statusFlags;
    u8 pad9[0x03];
} GcRobotLightBeamState;

STATIC_ASSERT(sizeof(GcRobotLightBeamStatusFlags) == 0x01);

STATIC_ASSERT(offsetof(GcRobotLightBeamState, pointLight) == 0x00);
STATIC_ASSERT(offsetof(GcRobotLightBeamState, unknown4) == 0x04);
STATIC_ASSERT(offsetof(GcRobotLightBeamState, statusFlags) == 0x08);
STATIC_ASSERT(offsetof(GcRobotLightBeamState, pad9) == 0x09);
STATIC_ASSERT(sizeof(GcRobotLightBeamState) == 0x0C);

int gcRobotLightBeam_isPlayerCaught(GameObject* obj);
int gcRobotLightBeam_getExtraSize(void);
int gcRobotLightBeam_getObjectTypeId(void);
void gcRobotLightBeam_free(GameObject* obj);
void gcRobotLightBeam_render(void);
void gcRobotLightBeam_hitDetect(GameObject* obj);
void gcRobotLightBeam_update(GameObject* obj);
void gcRobotLightBeam_init(GameObject* obj);
void gcRobotLightBeam_release(void);
void gcRobotLightBeam_initialise(void);

extern ObjectDescriptor10WithPadding gGCRobotLightBeamObjDescriptor;

#endif /* DLLS_OBJECTS_336_GCROBOTLIGH_H_ */
