#ifndef DLLS_OBJECTS_503_SB_SHIPGUNB_H_
#define DLLS_OBJECTS_503_SB_SHIPGUNB_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct SBShipGunBrokeState {
    u8 unknown00;
} SBShipGunBrokeState;

/*
 * Only the placement prefix consumed by this DLL is modeled. The active-target
 * retail placement width is not yet evidenced.
 */
typedef struct SBShipGunBrokePlacementView {
    ObjPlacement base;
    u8 unknown18[0x1E - 0x18];
    s16 destroyedGameBit;
} SBShipGunBrokePlacementView;

STATIC_ASSERT(offsetof(SBShipGunBrokeState, unknown00) == 0x00);
STATIC_ASSERT(sizeof(SBShipGunBrokeState) == 0x01);

STATIC_ASSERT(offsetof(SBShipGunBrokePlacementView, base) == 0x00);
STATIC_ASSERT(offsetof(SBShipGunBrokePlacementView, unknown18) == 0x18);
STATIC_ASSERT(offsetof(SBShipGunBrokePlacementView, destroyedGameBit) == 0x1E);

int SB_ShipGunBroke_getExtraSize(void);
int SB_ShipGunBroke_getObjectTypeId(void);
void SB_ShipGunBroke_free(void);
void SB_ShipGunBroke_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5);
void SB_ShipGunBroke_hitDetect(void);
void SB_ShipGunBroke_update(GameObject* obj);
void SB_ShipGunBroke_init(void);
void SB_ShipGunBroke_release(void);
void SB_ShipGunBroke_initialise(void);

extern ObjectDescriptor gSB_ShipGunBrokeObjDescriptor;

#endif /* DLLS_OBJECTS_503_SB_SHIPGUNB_H_ */
