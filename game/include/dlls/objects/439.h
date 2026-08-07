#ifndef DLLS_OBJECTS_439_H_
#define DLLS_OBJECTS_439_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define SC_MUSIC_TREE_AMBIENT_EFFECT_COUNT 3

typedef struct ScMusicTreePlacement {
    ObjPlacement base;
    u8 rotZByte;
    u8 rotYByte;
    u8 rotXByte;
    u8 hearRadiusHalf;
    f32 scale;
    u8 colorR;
    u8 colorG;
    u8 colorB;
    u8 flags;
} ScMusicTreePlacement;

typedef struct ScMusicTreeState {
    int ambientEffectHandles[SC_MUSIC_TREE_AMBIENT_EFFECT_COUNT];
    f32 ambientEffectPositions[SC_MUSIC_TREE_AMBIENT_EFFECT_COUNT][3];
    f32 proximityBurstTimer;
    f32 animationStep;
    f32 effectScale;
    f32 proximityCooldown;
    f32 hitCooldown;
    f32 hitEffectCooldown;
    u16 hearRadius;
    u16 previousDistance;
    u8 flags;
    u8 unknown4D[0x50 - 0x4D];
} ScMusicTreeState;

STATIC_ASSERT(offsetof(ScMusicTreePlacement, base) == 0x00);
STATIC_ASSERT(offsetof(ScMusicTreePlacement, rotZByte) == 0x18);
STATIC_ASSERT(offsetof(ScMusicTreePlacement, rotYByte) == 0x19);
STATIC_ASSERT(offsetof(ScMusicTreePlacement, rotXByte) == 0x1A);
STATIC_ASSERT(offsetof(ScMusicTreePlacement, hearRadiusHalf) == 0x1B);
STATIC_ASSERT(offsetof(ScMusicTreePlacement, scale) == 0x1C);
STATIC_ASSERT(offsetof(ScMusicTreePlacement, colorR) == 0x20);
STATIC_ASSERT(offsetof(ScMusicTreePlacement, colorG) == 0x21);
STATIC_ASSERT(offsetof(ScMusicTreePlacement, colorB) == 0x22);
STATIC_ASSERT(offsetof(ScMusicTreePlacement, flags) == 0x23);

STATIC_ASSERT(offsetof(ScMusicTreeState, ambientEffectHandles) == 0x00);
STATIC_ASSERT(offsetof(ScMusicTreeState, ambientEffectPositions) == 0x0C);
STATIC_ASSERT(offsetof(ScMusicTreeState, proximityBurstTimer) == 0x30);
STATIC_ASSERT(offsetof(ScMusicTreeState, animationStep) == 0x34);
STATIC_ASSERT(offsetof(ScMusicTreeState, effectScale) == 0x38);
STATIC_ASSERT(offsetof(ScMusicTreeState, proximityCooldown) == 0x3C);
STATIC_ASSERT(offsetof(ScMusicTreeState, hitCooldown) == 0x40);
STATIC_ASSERT(offsetof(ScMusicTreeState, hitEffectCooldown) == 0x44);
STATIC_ASSERT(offsetof(ScMusicTreeState, hearRadius) == 0x48);
STATIC_ASSERT(offsetof(ScMusicTreeState, previousDistance) == 0x4A);
STATIC_ASSERT(offsetof(ScMusicTreeState, flags) == 0x4C);
STATIC_ASSERT(offsetof(ScMusicTreeState, unknown4D) == 0x4D);
STATIC_ASSERT(sizeof(ScMusicTreeState) == 0x50);

void sc_musictree_spawnAmbientEffect(GameObject* obj, ScMusicTreeState* state, int unused, s8 index);
void sc_musictree_handleHitObject(GameObject* obj, ScMusicTreeState* state, int unusedEffectType);
int sc_musictree_getExtraSize(void);
int sc_musictree_getObjectTypeId(void);
void sc_musictree_free(void);
void sc_musictree_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void sc_musictree_hitDetect(void);
void sc_musictree_update(GameObject* obj);
void sc_musictree_init(GameObject* obj, ScMusicTreePlacement* placement);
void sc_musictree_release(void);
void sc_musictree_initialise(void);

extern ObjectDescriptor gSC_MusicTreeObjDescriptor;

#endif /* DLLS_OBJECTS_439_H_ */
