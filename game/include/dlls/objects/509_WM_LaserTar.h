#ifndef DLLS_OBJECTS_509_WM_LASERTAR_H_
#define DLLS_OBJECTS_509_WM_LASERTAR_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

/* An active-target retail placement proves the complete 0x24-byte record. */
typedef struct WMLaserTargetPlacement {
    ObjPlacement base;
    u8 unknown18[2];
    s16 cooldown;
    u8 unknown1C[2];
    s16 toggleGameBit;
    s16 pairedGameBit;
    u8 unknown22[2];
} WMLaserTargetPlacement;

/* WM_LaserTarget_getExtraSize() allocates the complete four-byte state. */
typedef struct WMLaserTargetState {
    s16 cooldown;
    u8 toggleQueued;
    u8 unknown03;
} WMLaserTargetState;

STATIC_ASSERT(offsetof(WMLaserTargetPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(WMLaserTargetPlacement, unknown18) == 0x18);
STATIC_ASSERT(offsetof(WMLaserTargetPlacement, cooldown) == 0x1A);
STATIC_ASSERT(offsetof(WMLaserTargetPlacement, unknown1C) == 0x1C);
STATIC_ASSERT(offsetof(WMLaserTargetPlacement, toggleGameBit) == 0x1E);
STATIC_ASSERT(offsetof(WMLaserTargetPlacement, pairedGameBit) == 0x20);
STATIC_ASSERT(offsetof(WMLaserTargetPlacement, unknown22) == 0x22);
STATIC_ASSERT(sizeof(WMLaserTargetPlacement) == 0x24);

STATIC_ASSERT(offsetof(WMLaserTargetState, cooldown) == 0x00);
STATIC_ASSERT(offsetof(WMLaserTargetState, toggleQueued) == 0x02);
STATIC_ASSERT(offsetof(WMLaserTargetState, unknown03) == 0x03);
STATIC_ASSERT(sizeof(WMLaserTargetState) == 0x04);

int WM_LaserTarget_getExtraSize(void);
int WM_LaserTarget_getObjectTypeId(void);
void WM_LaserTarget_free(void);
void WM_LaserTarget_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void WM_LaserTarget_hitDetect(void);
void WM_LaserTarget_update(GameObject* obj);
void WM_LaserTarget_init(GameObject* obj, const WMLaserTargetPlacement* placement);
void WM_LaserTarget_release(void);
void WM_LaserTarget_initialise(void);

extern ObjectDescriptor gWM_LaserTargetObjDescriptor;

#endif /* DLLS_OBJECTS_509_WM_LASERTAR_H_ */
