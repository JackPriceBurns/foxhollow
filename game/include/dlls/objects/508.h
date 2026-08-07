#ifndef DLLS_OBJECTS_508_H_
#define DLLS_OBJECTS_508_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/dll/dll_0081_modgfx.h"
#include "main/texture.h"


/*
 * Only the placement prefix consumed by this DLL is modeled. The active-target
 * retail placement width has not been established, so this does not claim a
 * complete record size.
 */
typedef struct LaserBeamPlacementView {
    ObjPlacement base;
    s8 initialYaw;
    u8 beamKind;
    s16 beamRange;
    s16 cyclePeriod;
    s16 disableGameBit;
} LaserBeamPlacementView;

/* LaserBeam_getExtraSize() establishes the complete allocation size. */
typedef struct LaserBeamState {
    Texture* beamTexture;
    f32 beamY;
    f32 beamY2;
    f32 beamX;
    f32 beamX2;
    f32 beamZ;
    f32 beamZ2;
    f32 beamVolumeScale;
    u8 unknown20[0x24 - 0x20];
    u8 beamState;
    u8 beamBlocked;
    s8 heightOffset;
    s8 damageCooldown;
    s16 hitStrength;
    s16 blockTimer;
    s16 cycleTimer;
    s16 warmupThreshold;
    s16 cyclePeriod;
    s16 effectHandle;
    f32 messagePayload[3];
    f32 knockbackTargetX;
    f32 knockbackTargetY;
    f32 knockbackTargetZ;
    u8 modgfxAttached;
    u8 blastPhase;
    u8 beamKind;
    u8 unknown4F;
} LaserBeamState;

STATIC_ASSERT(offsetof(LaserBeamPlacementView, base) == 0x00);
STATIC_ASSERT(offsetof(LaserBeamPlacementView, initialYaw) == 0x18);
STATIC_ASSERT(offsetof(LaserBeamPlacementView, beamKind) == 0x19);
STATIC_ASSERT(offsetof(LaserBeamPlacementView, beamRange) == 0x1A);
STATIC_ASSERT(offsetof(LaserBeamPlacementView, cyclePeriod) == 0x1C);
STATIC_ASSERT(offsetof(LaserBeamPlacementView, disableGameBit) == 0x1E);

STATIC_ASSERT(offsetof(LaserBeamState, beamTexture) == 0x00);
STATIC_ASSERT(offsetof(LaserBeamState, beamY) == 0x04);
STATIC_ASSERT(offsetof(LaserBeamState, beamY2) == 0x08);
STATIC_ASSERT(offsetof(LaserBeamState, beamX) == 0x0C);
STATIC_ASSERT(offsetof(LaserBeamState, beamX2) == 0x10);
STATIC_ASSERT(offsetof(LaserBeamState, beamZ) == 0x14);
STATIC_ASSERT(offsetof(LaserBeamState, beamZ2) == 0x18);
STATIC_ASSERT(offsetof(LaserBeamState, beamVolumeScale) == 0x1C);
STATIC_ASSERT(offsetof(LaserBeamState, unknown20) == 0x20);
STATIC_ASSERT(offsetof(LaserBeamState, beamState) == 0x24);
STATIC_ASSERT(offsetof(LaserBeamState, beamBlocked) == 0x25);
STATIC_ASSERT(offsetof(LaserBeamState, heightOffset) == 0x26);
STATIC_ASSERT(offsetof(LaserBeamState, damageCooldown) == 0x27);
STATIC_ASSERT(offsetof(LaserBeamState, hitStrength) == 0x28);
STATIC_ASSERT(offsetof(LaserBeamState, blockTimer) == 0x2A);
STATIC_ASSERT(offsetof(LaserBeamState, cycleTimer) == 0x2C);
STATIC_ASSERT(offsetof(LaserBeamState, warmupThreshold) == 0x2E);
STATIC_ASSERT(offsetof(LaserBeamState, cyclePeriod) == 0x30);
STATIC_ASSERT(offsetof(LaserBeamState, effectHandle) == 0x32);
STATIC_ASSERT(offsetof(LaserBeamState, messagePayload) == 0x34);
STATIC_ASSERT(offsetof(LaserBeamState, knockbackTargetX) == 0x40);
STATIC_ASSERT(offsetof(LaserBeamState, knockbackTargetY) == 0x44);
STATIC_ASSERT(offsetof(LaserBeamState, knockbackTargetZ) == 0x48);
STATIC_ASSERT(offsetof(LaserBeamState, modgfxAttached) == 0x4C);
STATIC_ASSERT(offsetof(LaserBeamState, blastPhase) == 0x4D);
STATIC_ASSERT(offsetof(LaserBeamState, beamKind) == 0x4E);
STATIC_ASSERT(offsetof(LaserBeamState, unknown4F) == 0x4F);
STATIC_ASSERT(sizeof(LaserBeamState) == 0x50);

int LaserBeam_getExtraSize(void);
int LaserBeam_getObjectTypeId(void);
void LaserBeam_free(GameObject* obj);
void LaserBeam_render(void);
void LaserBeam_hitDetect(void);
void LaserBeam_update(GameObject* obj);
void LaserBeam_init(GameObject* obj, const LaserBeamPlacementView* placement);
void LaserBeam_release(void);
void LaserBeam_initialise(void);

extern Dll81Interface** gLaserBeamObjModgfxResource;
extern ObjectDescriptor gLaserBeamObjDescriptor;

#endif /* DLLS_OBJECTS_508_H_ */
