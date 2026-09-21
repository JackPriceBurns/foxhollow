#ifndef DLLS_OBJECTS_492_SB_SHIPGUN_H_
#define DLLS_OBJECTS_492_SB_SHIPGUN_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

/*
 * Only the placement byte consumed by this DLL is modeled. The active-target
 * retail placement width is not available, so this does not claim a total
 * record size.
 */
typedef struct SBShipGunPlacementView {
    ObjPlacement base;
    u8 unknown18;
    s8 noWakeDelay;
} SBShipGunPlacementView;

/*
 * getExtraSize establishes this complete 0x10-byte allocation. The gun caches
 * its CloudRunner target alongside its aiming and damage-state fields.
 */
typedef struct SBShipGunState {
    GameObject* cloudRunner;
    s16 yawAngle;
    s16 pitchAngle;
    s16 fireTimer;
    s8 phase;
    s8 hitCount;
    s8 health;
    u8 active;
    u8 volleyCount;
    u8 unknown0F;
} SBShipGunState;

STATIC_ASSERT(offsetof(SBShipGunPlacementView, base) == 0x00);
STATIC_ASSERT(offsetof(SBShipGunPlacementView, unknown18) == 0x18);
STATIC_ASSERT(offsetof(SBShipGunPlacementView, noWakeDelay) == 0x19);

STATIC_ASSERT(offsetof(SBShipGunState, cloudRunner) == 0x00);
STATIC_ASSERT(offsetof(SBShipGunState, yawAngle) == 0x04);
STATIC_ASSERT(offsetof(SBShipGunState, pitchAngle) == 0x06);
STATIC_ASSERT(offsetof(SBShipGunState, fireTimer) == 0x08);
STATIC_ASSERT(offsetof(SBShipGunState, phase) == 0x0A);
STATIC_ASSERT(offsetof(SBShipGunState, hitCount) == 0x0B);
STATIC_ASSERT(offsetof(SBShipGunState, health) == 0x0C);
STATIC_ASSERT(offsetof(SBShipGunState, active) == 0x0D);
STATIC_ASSERT(offsetof(SBShipGunState, volleyCount) == 0x0E);
STATIC_ASSERT(offsetof(SBShipGunState, unknown0F) == 0x0F);
STATIC_ASSERT(sizeof(SBShipGunState) == 0x10);

int SB_ShipGun_getExtraSize(void);
void SB_ShipGun_free(GameObject* obj);
void SB_ShipGun_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void SB_ShipGun_update(GameObject* obj);
void SB_ShipGun_init(GameObject* obj);

extern ObjectDescriptor gSB_ShipGunObjDescriptor;

#endif /* DLLS_OBJECTS_492_SB_SHIPGUN_H_ */
