#ifndef MAIN_OBJFX_H_
#define MAIN_OBJFX_H_

#include "dolphin/mtx/vec_types.h"
#include "global.h"
#include "game/objects/object.h"
#include "main/objfx_hit_emitter.h"

typedef struct ObjFxCrystalOrbitState {
    Vec3s rotation;
    s16 pad06;
    f32 radius;
    f32 amplitudes[4];
    s16 waveAngles[4];
    s16 waveSpeeds[4];
    s16 spinAngles[4];
} ObjFxCrystalOrbitState;

STATIC_ASSERT(sizeof(ObjFxCrystalOrbitState) == 0x34);

void objDoHitParticleFx(GameObject* obj, f32 scale, void* origin, u8 type, void* light);
void objfx_spawnCrystalOrbitEffects(GameObject* obj, ObjFxCrystalOrbitState* state, f32 period, f32 xMul, f32 yMul,
                                    f32 xOff, f32 yOff, u8 flags);
void objfx_spawnRandomBurst(GameObject* obj, u8 type, u8 count, void* origin, f32 mult, u8 flagByte);
void objfx_spawnMaskedHitEffect(GameObject* obj, f32 scale, u8 type, u8 mode, u8 mask, void* origin);
void objfx_spawnLightPulse(GameObject* obj, f32 radius, int type, int colorIndex, int mode, f32 intensity, void* light);
void objfx_spawnDirectionalBurst(GameObject* obj, u8 idx, f32 scale, u8 kind, u8 mode, u8 chance, f32 mult,
                                 void* origin, int flags);
void objfx_spawnArcedBurst(GameObject* obj, u8 idx, f32 scale, u8 kind, u8 mode, int chance, f32 angleBase,
                           f32 angleLow, f32 angleHigh, void* origin, int flags);
void objfx_spawnBoxBurst(GameObject* obj, u8 idx, f32 scale, u8 kind, u8 mode, u8 chance, f32 scaleX, f32 scaleY,
                         f32 scaleZ, void* origin, int flags);
void projectileDoParticleFx(GameObject* obj, f32 scale, int mode);
void itemPickupDoParticleFx(GameObject* obj, f32 scale, int mode, u8 count);
void objfx_spawnPulseBurst(GameObject* obj, f32 scale, int type, int count, int mode, const Vec* offset);
void spawnExplosion(GameObject* source, f32 scale, u8 kind, u8 flag4, u8 flag8, u8 flag10, u8 doShake, u8 flag20,
                    u8 initialFlags);

void objfx_spawnHitEffectBurst(GameObject* obj, f32 scale, u8 effect, u8 variant, u8 count, GameObject* origin);

#endif /* MAIN_OBJFX_H_ */
