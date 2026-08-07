#ifndef MAIN_DLL_DLL_0262_DRAKORMISSILE_H_
#define MAIN_DLL_DLL_0262_DRAKORMISSILE_H_

#include "game/objects/object.h"
#include "main/modellight_api.h"
#include "game/objects/object_setup.h"

#define DRAKORMISSILE_RENDER_TRAIL_COUNT 5

typedef struct DrakorMissileSetup
{
    ObjPlacement base;
    u8 velocityX;
    u8 velocityY;
    u8 velocityZ;
} DrakorMissileSetup;

typedef struct DrakorMissileState
{
    ModelLightStruct* light;
    u8 state;
    u8 flags;
    u8 pad06[2];
    int timer;
    f32 fadeTime;
    u16 trailYaw[DRAKORMISSILE_RENDER_TRAIL_COUNT];
    u16 trailYawStep[DRAKORMISSILE_RENDER_TRAIL_COUNT];
    u16 trailPitch[DRAKORMISSILE_RENDER_TRAIL_COUNT];
    u16 trailPitchStep[DRAKORMISSILE_RENDER_TRAIL_COUNT];
} DrakorMissileState;

STATIC_ASSERT(offsetof(DrakorMissileSetup, base.posX) == 0x08);
STATIC_ASSERT(offsetof(DrakorMissileSetup, velocityX) == 0x18);
STATIC_ASSERT(sizeof(DrakorMissileState) == 0x38);

void drakormissile_startActiveLaunch(GameObject* obj);
void drakormissile_startStraightLaunch(GameObject* obj, GameObject* from, GameObject* target, f32 speed);
int drakormissile_getExtraSize(void);
int drakormissile_getObjectTypeId(void);
void drakormissile_hitDetect(void);
void drakormissile_initialise(void);
void drakormissile_release(void);
void drakormissile_update(GameObject* o);
int drakormissile_isFadingOut(GameObject* obj);
void drakormissile_abortStraightFlight(GameObject* obj);
void drakormissile_requestFree(GameObject* obj);
void drakormissile_free(GameObject* obj);
void drakormissile_render(GameObject* obj, int p2, int p3, int p4, int p5, s8 visible);
void drakormissile_init(GameObject* obj, DrakorMissileSetup* setup);

extern f32 gDrakorMissileVelocityDamping;
extern f32 gDrakorMissileSteerGain;
extern f32 gDrakorMissileInterceptSpeedBias;
extern f32 gDrakorMissileProximityDetonateDist;

#endif /* MAIN_DLL_DLL_0262_DRAKORMISSILE_H_ */
