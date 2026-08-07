#ifndef MAIN_DLL_DLL_02BB_GFLEVELCON_H_
#define MAIN_DLL_DLL_02BB_GFLEVELCON_H_

#include "global.h"
#include "game/objects/object.h"
#include "dlls/object_descriptor.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

/* Spawn-setup buffer for the arwing-projectile children (defNos
 * 0x80d/0x7e4/0x859). Reuses ObjPlacement's pos/color head and adds the
 * class-specific launch fields at 0x18/0x19/0x1a (all u8 stores per asm). */
typedef struct GfProjectileSetup
{
    ObjPlacement head; /* 0x00 */
    u8 roll;           /* 0x18: cleared to 0 */
    u8 pitch;          /* 0x19 */
    u8 yawHi;          /* 0x1a */
} GfProjectileSetup;

/* Two views over the same 0x10-byte obj->extra allocation.
 * findLinkedObjects stores the three linked object handles as s32 ids;
 * the sequence handler accesses the scroll handles as pointers and the
 * final word as the prompt countdown.
 */
typedef struct GfLevelconFindLinkedObjectsState
{
    s32 light;
    s32 scrollA;
    s32 scrollB;
    u8 padC[0x10 - 0xC];
} GfLevelconFindLinkedObjectsState;

typedef struct GfLevelconHandleScriptEventsState
{
    void* light;
    void* scrollA;
    void* scrollB;
    f32 promptTimer;
} GfLevelconHandleScriptEventsState;

typedef struct GfHitState
{
    u8 pad0[0x88];
    int mode;
    u8 pad1[0x16];
    s16 pitchVel;
    s16 rollVel;
    u8 pad2[8];
    u8 hits[4];
    u8 timer[4];
    u8 pad3[3];
    u8 texState[3];
} GfHitState;

STATIC_ASSERT(offsetof(GfHitState, mode) == 0x88);
STATIC_ASSERT(offsetof(GfHitState, pitchVel) == 0xA2);
STATIC_ASSERT(offsetof(GfHitState, hits[0]) == 0xAE);
STATIC_ASSERT(offsetof(GfHitState, timer[0]) == 0xB2);
STATIC_ASSERT(offsetof(GfHitState, texState[0]) == 0xB9);

extern ObjectDescriptor gGF_LevelConObjDescriptor;

int gf_levelcon_SeqFn(GameObject* obj, int eventId, ObjSeqState* animUpdate);
int gf_levelcon_getExtraSize(void);
int gf_levelcon_getObjectTypeId(void);
void gf_levelcon_hitDetect(void);
void gf_levelcon_initialise(void);
void gf_levelcon_release(void);
void gf_levelcon_free(void);
void gf_levelcon_update(GameObject* obj);
void gf_levelcon_render(GameObject* obj, int p2, int p3, int p4, int p5, s8 visible);
void gf_levelcon_init(GameObject* obj);
void gf_levelcon_findLinkedObjects(GameObject* obj);

#endif /* MAIN_DLL_DLL_02BB_GFLEVELCON_H_ */
