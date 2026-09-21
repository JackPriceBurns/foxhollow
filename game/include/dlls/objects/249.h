#ifndef DLLS_OBJECTS_249_H_
#define DLLS_OBJECTS_249_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct ProjectileSwitchPlacement {
    ObjPlacement base;        /* 0x00 */
    s16 gameBitId;            /* 0x18 */
    s16 autoResetDelayTenths; /* 0x1A */
    u8 rotYByte;              /* 0x1C: Y rotation in 1/256 turns */
    u8 scale64;               /* 0x1D: scale in 1/64 units; zero keeps the model root-motion scale */
    u8 modelIndexAndMode;     /* 0x1E: model index in bits 2-7; switch mode in bits 0-1 */
    u8 rotXByte;              /* 0x1F: X rotation in 1/256 turns */
    u8 colorR;                /* 0x20 */
    u8 colorG;                /* 0x21 */
    u8 colorB;                /* 0x22 */
    u8 renderFlags;           /* 0x23 */
} ProjectileSwitchPlacement;

typedef struct ProjectileSwitchState {
    u8 isOn;                  /* 0x00 */
    u8 pad01;                 /* 0x01 */
    s16 gameBitId;            /* 0x02 */
    f32 autoResetTimerFrames; /* 0x04 */
} ProjectileSwitchState;

enum {
    PROJECTILE_SWITCH_MODE_MASK = 3,
    PROJECTILE_SWITCH_MODE_TOGGLE = 1,
    PROJECTILE_SWITCH_MODE_TIMED_RESET = 2,
    PROJECTILE_SWITCH_RENDER_CUSTOM_COLOR = 1
};

STATIC_ASSERT(offsetof(ProjectileSwitchPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(ProjectileSwitchPlacement, gameBitId) == 0x18);
STATIC_ASSERT(offsetof(ProjectileSwitchPlacement, autoResetDelayTenths) == 0x1A);
STATIC_ASSERT(offsetof(ProjectileSwitchPlacement, rotYByte) == 0x1C);
STATIC_ASSERT(offsetof(ProjectileSwitchPlacement, scale64) == 0x1D);
STATIC_ASSERT(offsetof(ProjectileSwitchPlacement, modelIndexAndMode) == 0x1E);
STATIC_ASSERT(offsetof(ProjectileSwitchPlacement, rotXByte) == 0x1F);
STATIC_ASSERT(offsetof(ProjectileSwitchPlacement, colorR) == 0x20);
STATIC_ASSERT(offsetof(ProjectileSwitchPlacement, colorG) == 0x21);
STATIC_ASSERT(offsetof(ProjectileSwitchPlacement, colorB) == 0x22);
STATIC_ASSERT(offsetof(ProjectileSwitchPlacement, renderFlags) == 0x23);
STATIC_ASSERT(sizeof(ProjectileSwitchPlacement) == 0x24);

STATIC_ASSERT(offsetof(ProjectileSwitchState, isOn) == 0x0);
STATIC_ASSERT(offsetof(ProjectileSwitchState, pad01) == 0x1);
STATIC_ASSERT(offsetof(ProjectileSwitchState, gameBitId) == 0x2);
STATIC_ASSERT(offsetof(ProjectileSwitchState, autoResetTimerFrames) == 0x4);
STATIC_ASSERT(sizeof(ProjectileSwitchState) == 0x8);

int ProjectileSwitch_getExtraSize(void);
int ProjectileSwitch_getObjectTypeId(GameObject* obj);
void ProjectileSwitch_free(void);
void ProjectileSwitch_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void ProjectileSwitch_hitDetect(GameObject* obj);
void ProjectileSwitch_update(GameObject* obj);
void ProjectileSwitch_init(GameObject* obj, ProjectileSwitchPlacement* placement);
void ProjectileSwitch_release(void);
void ProjectileSwitch_initialise(void);

extern ObjectDescriptor gProjectileSwitchObjDescriptor;

#endif /* DLLS_OBJECTS_249_H_ */
