#ifndef MAIN_DLL_DR_DLL_0261_DRLASERCANNON_H_
#define MAIN_DLL_DR_DLL_0261_DRLASERCANNON_H_

#include "global.h"
#include "game/objects/object.h"
#include "game/objects/object_setup.h"

typedef struct DrLaserCannonSetup
{
    ObjPlacement base;
    s8 initialYaw;
    s8 reloadFrames;
    s16 targetRange;
    s16 beamSpeed;
    s16 destroyedGameBit;
    s16 warningOffGameBit;
} DrLaserCannonSetup;

typedef struct DrLaserCannonAim
{
    u8 pad00[0x14];
    s16 yaw;
    u8 pad16[0x44 - 0x16];
    s16 pitch;
} DrLaserCannonAim;

STATIC_ASSERT(offsetof(DrLaserCannonSetup, initialYaw) == 0x18);
STATIC_ASSERT(offsetof(DrLaserCannonSetup, reloadFrames) == 0x19);
STATIC_ASSERT(offsetof(DrLaserCannonSetup, targetRange) == 0x1a);
STATIC_ASSERT(offsetof(DrLaserCannonSetup, beamSpeed) == 0x1c);
STATIC_ASSERT(offsetof(DrLaserCannonSetup, destroyedGameBit) == 0x1e);
STATIC_ASSERT(offsetof(DrLaserCannonSetup, warningOffGameBit) == 0x20);
STATIC_ASSERT(offsetof(DrLaserCannonAim, yaw) == 0x14);
STATIC_ASSERT(offsetof(DrLaserCannonAim, pitch) == 0x44);

int DR_LaserCannon_getExtraSize(void);
int DR_LaserCannon_getObjectTypeId(void);
void DR_LaserCannon_initialise(void);
void DR_LaserCannon_release(void);
void DR_LaserCannon_free(GameObject* obj);
void DR_LaserCannon_render(GameObject* obj, u32 p2, u32 p3, u32 p4, u32 p5, char visible);
GameObject* drlasercannon_getTrackedTarget(GameObject* obj, int* cooldown);
int drlasercannon_aimAtTarget(GameObject* self, GameObject* target, DrLaserCannonAim* out, int maxRate, f32* eyePos);
void DR_LaserCannon_init(GameObject* obj, DrLaserCannonSetup* setup);
void DR_LaserCannon_hitDetect(GameObject* obj);
void DR_LaserCannon_update(GameObject* obj);

extern s16 gLaserCannonMaxAimStep;
extern f32 gLaserCannonAdvanceSpeed;
extern s16 gLaserCannonPitchStep;
extern f32 lbl_803DDD68;

#endif /* MAIN_DLL_DR_DLL_0261_DRLASERCANNON_H_ */
