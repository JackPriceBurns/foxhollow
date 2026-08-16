#ifndef MAIN_DLL_BONEPARTICLEEFFECT_INTERFACE_H_
#define MAIN_DLL_BONEPARTICLEEFFECT_INTERFACE_H_

#include "global.h"

typedef void (*BoneParticleEffectSpawnFn)(void* obj, int effectId, void* extraArg,
                                          int probability, void* boneSpawnData);

typedef struct BoneParticleEffectInterface
{
    void (*pad00_slots[3])(void);
    BoneParticleEffectSpawnFn spawnEffect;
} BoneParticleEffectInterface;

STATIC_ASSERT(offsetof(BoneParticleEffectInterface, spawnEffect) == 0x0C);

extern BoneParticleEffectInterface** gBoneParticleEffectInterface;

#endif /* MAIN_DLL_BONEPARTICLEEFFECT_INTERFACE_H_ */
