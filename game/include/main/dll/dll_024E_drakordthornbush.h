#ifndef MAIN_DLL_DLL_024E_DRAKORDTHORNBUSH_H_
#define MAIN_DLL_DLL_024E_DRAKORDTHORNBUSH_H_

#include "types.h"
#include "game/objects/object.h"
#include "global.h"
#include "main/byte_flags.h"
#include "main/lightningeffect.h"
#include "main/model_light.h"
#include "game/objects/object_setup.h"

typedef struct DrakordThornbushPlacement
{
    ObjPlacement base;
    s8 rotYByte;     /* 0x18: initial Y rotation in 1/256 turns (anim.rotY = byte << 8) */
    u8 spawnHealth;  /* 0x19: initial hit points */
    s16 regrowDelay; /* 0x1A: frames before regrow (0 = no respawn) */
    s16 baseRadius;  /* 0x1C: base hit-sphere radius */
    u8 pad1E[0x20 - 0x1E];
} DrakordThornbushPlacement;

typedef struct DrakordThornbushState
{
    s32 health; /* 0x00: hit points; 0 = dormant */
    u8 pad4[0x8 - 0x4];
    s32 lastHitObj;                   /* 0x08: most recent attacker, debounces re-hits */
    f32 growth;                       /* 0x0C: regrow timer / scale driver */
    f32 regrowTimer;                  /* 0x10: hit/regrow countdown */
    LightningEffect* lightningEntries[3];
    u8 pad20[0x64 - 0x20];
    ModelLightStruct* light; /* 0x64: model light handle (lightning variant) */
    f32 lightScale;                   /* 0x68: lightning scale, accumulates over time */
    void* hitTable;                   /* 0x6C: hit-reaction table pointer */
    f32 baseScale;                    /* 0x70: per-variant init scale constant */
    s32 radius;                       /* 0x74 */
    u8 pad78;
    ByteFlags flags79;                /* 0x79: bit 0x80 = spawned/regrown */
    u8 pad7A[0x7c - 0x7a];
} DrakordThornbushState;

STATIC_ASSERT(offsetof(DrakordThornbushPlacement, spawnHealth) == 0x19);
STATIC_ASSERT(offsetof(DrakordThornbushPlacement, regrowDelay) == 0x1A);
STATIC_ASSERT(offsetof(DrakordThornbushPlacement, baseRadius) == 0x1C);
STATIC_ASSERT(sizeof(DrakordThornbushPlacement) == 0x20);
STATIC_ASSERT(offsetof(DrakordThornbushState, regrowTimer) == 0x10);
STATIC_ASSERT(offsetof(DrakordThornbushState, light) == 0x64);
STATIC_ASSERT(offsetof(DrakordThornbushState, lightScale) == 0x68);
STATIC_ASSERT(offsetof(DrakordThornbushState, hitTable) == 0x6C);
STATIC_ASSERT(offsetof(DrakordThornbushState, baseScale) == 0x70);
STATIC_ASSERT(offsetof(DrakordThornbushState, radius) == 0x74);
STATIC_ASSERT(offsetof(DrakordThornbushState, flags79) == 0x79);
STATIC_ASSERT(sizeof(DrakordThornbushState) == 0x7c);

extern int gThornBushLightningHitTable[2];
extern int gThornBushThornHitTable[2];
extern f32 gThornBushLightningTimerInit;

int drakord_thornbush_getExtraSize(void);
int drakord_thornbush_getObjectTypeId(void);
void drakord_thornbush_free(GameObject* obj);
void drakord_thornbush_render(GameObject* obj, int p2, int p3, int p4, int p5, s8 vis);
void drakord_thornbush_hitDetect(GameObject* obj);
void drakord_thornbush_update(GameObject* obj);
void drakord_thornbush_init(GameObject* obj, u8* init);
void drakord_thornbush_release(void);
void drakord_thornbush_initialise(void);

#endif
