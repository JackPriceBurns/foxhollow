#ifndef DLLS_OBJECTS_331_CFMAINCRYST_H_
#define DLLS_OBJECTS_331_CFMAINCRYST_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define CFMAINCRYSTAL_PYLON_COUNT       3
#define CFMAINCRYSTAL_BEAM_COUNT        10
#define CFMAINCRYSTAL_CHIME_TIMER_COUNT 4

typedef enum CfMainCrystalObjectId {
    CFMAINCRYSTAL_OBJECT_ID = 0xDC,
} CfMainCrystalObjectId;

typedef enum CfMainCrystalVariant {
    CFMAINCRYSTAL_VARIANT_BEAM_CONTROLLER = 0,
    CFMAINCRYSTAL_VARIANT_POSITION_SOURCE = 1,
} CfMainCrystalVariant;

typedef struct CfMainCrystalPlacement {
    ObjPlacement base;
    s8 initialYaw;
    s8 variant;
} CfMainCrystalPlacement;

typedef struct CfMainCrystalBeam {
    f32 startX;
    f32 endX;
    f32 startY;
    f32 endY;
    f32 startZ;
    f32 endZ;
    u8 colorR;
    u8 colorG;
    u8 colorB;
    u8 active;
} CfMainCrystalBeam;

typedef struct CfMainCrystalState {
    f32 pylonX[CFMAINCRYSTAL_PYLON_COUNT];
    f32 crystalX;
    f32 pylonY[CFMAINCRYSTAL_PYLON_COUNT];
    f32 crystalY;
    f32 pylonZ[CFMAINCRYSTAL_PYLON_COUNT];
    f32 crystalZ;
    s16 pylonTimers[CFMAINCRYSTAL_PYLON_COUNT];
    s16 hasCrystalPosition;
    CfMainCrystalBeam beams[CFMAINCRYSTAL_BEAM_COUNT];
    s16 chargeTimer;
    u8 pad152[0x02];
    f32 humVolume;
    int unknown158;
    u8 chimeTimers[CFMAINCRYSTAL_CHIME_TIMER_COUNT];
} CfMainCrystalState;

STATIC_ASSERT(offsetof(CfMainCrystalPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(CfMainCrystalPlacement, initialYaw) == 0x18);
STATIC_ASSERT(offsetof(CfMainCrystalPlacement, variant) == 0x19);

STATIC_ASSERT(offsetof(CfMainCrystalBeam, startX) == 0x00);
STATIC_ASSERT(offsetof(CfMainCrystalBeam, endX) == 0x04);
STATIC_ASSERT(offsetof(CfMainCrystalBeam, startY) == 0x08);
STATIC_ASSERT(offsetof(CfMainCrystalBeam, endY) == 0x0C);
STATIC_ASSERT(offsetof(CfMainCrystalBeam, startZ) == 0x10);
STATIC_ASSERT(offsetof(CfMainCrystalBeam, endZ) == 0x14);
STATIC_ASSERT(offsetof(CfMainCrystalBeam, colorR) == 0x18);
STATIC_ASSERT(offsetof(CfMainCrystalBeam, colorG) == 0x19);
STATIC_ASSERT(offsetof(CfMainCrystalBeam, colorB) == 0x1A);
STATIC_ASSERT(offsetof(CfMainCrystalBeam, active) == 0x1B);
STATIC_ASSERT(sizeof(CfMainCrystalBeam) == 0x1C);

STATIC_ASSERT(offsetof(CfMainCrystalState, pylonX) == 0x000);
STATIC_ASSERT(offsetof(CfMainCrystalState, crystalX) == 0x00C);
STATIC_ASSERT(offsetof(CfMainCrystalState, pylonY) == 0x010);
STATIC_ASSERT(offsetof(CfMainCrystalState, crystalY) == 0x01C);
STATIC_ASSERT(offsetof(CfMainCrystalState, pylonZ) == 0x020);
STATIC_ASSERT(offsetof(CfMainCrystalState, crystalZ) == 0x02C);
STATIC_ASSERT(offsetof(CfMainCrystalState, pylonTimers) == 0x030);
STATIC_ASSERT(offsetof(CfMainCrystalState, hasCrystalPosition) == 0x036);
STATIC_ASSERT(offsetof(CfMainCrystalState, beams) == 0x038);
STATIC_ASSERT(offsetof(CfMainCrystalState, chargeTimer) == 0x150);
STATIC_ASSERT(offsetof(CfMainCrystalState, pad152) == 0x152);
STATIC_ASSERT(offsetof(CfMainCrystalState, humVolume) == 0x154);
STATIC_ASSERT(offsetof(CfMainCrystalState, unknown158) == 0x158);
STATIC_ASSERT(offsetof(CfMainCrystalState, chimeTimers) == 0x15C);
STATIC_ASSERT(sizeof(CfMainCrystalState) == 0x160);

void cfMainCrystal_updateBeams(GameObject* obj);
int cfMainCrystal_getExtraSize(void);
int cfMainCrystal_getObjectTypeId(void);
void cfMainCrystal_free(GameObject* obj);
void cfMainCrystal_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void cfMainCrystal_hitDetect(void);
void cfMainCrystal_update(GameObject* obj);
void cfMainCrystal_init(GameObject* obj, CfMainCrystalPlacement* placement);
void cfMainCrystal_release(void);
void cfMainCrystal_initialise(void);

extern ObjectDescriptor gCFMainCrystalObjDescriptor;

#endif /* DLLS_OBJECTS_331_CFMAINCRYST_H_ */
