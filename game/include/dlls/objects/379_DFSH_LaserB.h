#ifndef DLLS_OBJECTS_379_DFSH_LASERB_H_
#define DLLS_OBJECTS_379_DFSH_LASERB_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/dll/dll_0081_modgfx.h"
#include "main/texture.h"


typedef struct DFSHLaserBeamPlacement {
    ObjPlacement base;
    s8 initialYaw;
    u8 proximityMode;
    s16 beamRange;
    s16 cyclePeriod;
    s16 disableGameBit;
} DFSHLaserBeamPlacement;

typedef struct DFSHLaserBeamState {
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
    f32 messagePayload[3];
    f32 knockbackTargetX;
    f32 knockbackTargetY;
    f32 knockbackTargetZ;
    u8 modgfxAttached;
    u8 blastPhase;
    u8 proximityMode;
    u8 unknown4B;
} DFSHLaserBeamState;

STATIC_ASSERT(offsetof(DFSHLaserBeamPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(DFSHLaserBeamPlacement, initialYaw) == 0x18);
STATIC_ASSERT(offsetof(DFSHLaserBeamPlacement, proximityMode) == 0x19);
STATIC_ASSERT(offsetof(DFSHLaserBeamPlacement, beamRange) == 0x1A);
STATIC_ASSERT(offsetof(DFSHLaserBeamPlacement, cyclePeriod) == 0x1C);
STATIC_ASSERT(offsetof(DFSHLaserBeamPlacement, disableGameBit) == 0x1E);

STATIC_ASSERT(sizeof(DFSHLaserBeamState) == 0x4C);
STATIC_ASSERT(offsetof(DFSHLaserBeamState, beamTexture) == 0x00);
STATIC_ASSERT(offsetof(DFSHLaserBeamState, beamY) == 0x04);
STATIC_ASSERT(offsetof(DFSHLaserBeamState, beamY2) == 0x08);
STATIC_ASSERT(offsetof(DFSHLaserBeamState, beamX) == 0x0C);
STATIC_ASSERT(offsetof(DFSHLaserBeamState, beamX2) == 0x10);
STATIC_ASSERT(offsetof(DFSHLaserBeamState, beamZ) == 0x14);
STATIC_ASSERT(offsetof(DFSHLaserBeamState, beamZ2) == 0x18);
STATIC_ASSERT(offsetof(DFSHLaserBeamState, beamVolumeScale) == 0x1C);
STATIC_ASSERT(offsetof(DFSHLaserBeamState, unknown20) == 0x20);
STATIC_ASSERT(offsetof(DFSHLaserBeamState, beamState) == 0x24);
STATIC_ASSERT(offsetof(DFSHLaserBeamState, beamBlocked) == 0x25);
STATIC_ASSERT(offsetof(DFSHLaserBeamState, heightOffset) == 0x26);
STATIC_ASSERT(offsetof(DFSHLaserBeamState, damageCooldown) == 0x27);
STATIC_ASSERT(offsetof(DFSHLaserBeamState, hitStrength) == 0x28);
STATIC_ASSERT(offsetof(DFSHLaserBeamState, blockTimer) == 0x2A);
STATIC_ASSERT(offsetof(DFSHLaserBeamState, cycleTimer) == 0x2C);
STATIC_ASSERT(offsetof(DFSHLaserBeamState, warmupThreshold) == 0x2E);
STATIC_ASSERT(offsetof(DFSHLaserBeamState, messagePayload) == 0x30);
STATIC_ASSERT(offsetof(DFSHLaserBeamState, knockbackTargetX) == 0x3C);
STATIC_ASSERT(offsetof(DFSHLaserBeamState, knockbackTargetY) == 0x40);
STATIC_ASSERT(offsetof(DFSHLaserBeamState, knockbackTargetZ) == 0x44);
STATIC_ASSERT(offsetof(DFSHLaserBeamState, modgfxAttached) == 0x48);
STATIC_ASSERT(offsetof(DFSHLaserBeamState, blastPhase) == 0x49);
STATIC_ASSERT(offsetof(DFSHLaserBeamState, proximityMode) == 0x4A);
STATIC_ASSERT(offsetof(DFSHLaserBeamState, unknown4B) == 0x4B);

extern Dll81Interface** gDFSHLaserBeamEffectResource;
extern ObjectDescriptor gDFSHLaserBeamObjDescriptor;

int dfshLaserBeam_getExtraSize(void);
int dfshLaserBeam_getObjectTypeId(void);
void dfshLaserBeam_free(GameObject* obj);
void dfshLaserBeam_render(void);
void dfshLaserBeam_hitDetect(void);
void dfshLaserBeam_update(GameObject* obj);
void dfshLaserBeam_init(GameObject* obj, const DFSHLaserBeamPlacement* placement);
void dfshLaserBeam_release(void);
void dfshLaserBeam_initialise(void);

#endif /* DLLS_OBJECTS_379_DFSH_LASERB_H_ */
