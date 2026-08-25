#ifndef MAIN_DLL_DBSTEALERWORMCONTROL_STRUCT_H_
#define MAIN_DLL_DBSTEALERWORMCONTROL_STRUCT_H_

#include "types.h"
#include "game/objects/object_interface.h"
#include "main/model_engine.h"

struct GameObject;

/* DbStealerwormControl.flags14: per-frame effect-request bits, consumed and
 * cleared each tick by the fx dispatcher (dbstealerworm_processEffectFlags). */
#define DBWORM_FLAG14_ATTACK  0x1 /* strike the current target this frame */
#define DBWORM_FLAG14_FX_DUST 0x2 /* emit the small dust burst (partfx 0x345) */
#define DBWORM_FLAG14_FX_SPRAY 0x4 /* emit the large spray burst (partfx 0x343 x10) */

/* gDBstealerwormObjDescriptor from slot02 onwards: the export table sibling
   worms reach through obj->anim.dll. */
typedef struct DbStealerwormInterface
{
    OBJECT_INTERFACE_FIELDS;
    int (*getControlMode)(struct GameObject* worm);
    intptr_t (*handleMessage)(struct GameObject* worm, u8 msg, int* out);
} DbStealerwormInterface;

#define DB_STEALERWORM_INTERFACE(worm) ((DbStealerwormInterface*)*((struct GameObject*)(worm))->anim.dll)

STATIC_ASSERT(offsetof(DbStealerwormInterface, getControlMode) == 0x20);
STATIC_ASSERT(offsetof(DbStealerwormInterface, handleMessage) == 0x24);

typedef struct DbStealerwormScriptStep
{
    intptr_t code;
    intptr_t mode;
    intptr_t objGroup;
} DbStealerwormScriptStep;

typedef struct DbStealerwormScript
{
    const DbStealerwormScriptStep* steps;
    s16 stepCount;
    s16 unk6;
} DbStealerwormScript;

typedef struct DbStealerwormFlags44
{
    u8 flag80 : 1;
    u8 flag40 : 1;
    u8 flag20 : 1;
    u8 flag10 : 1;
    u8 low : 4;
} DbStealerwormFlags44;

typedef struct DbStealerwormControl
{
    const DbStealerwormScript* cfg;
    f32 unk04;
    f32 unk08;
    f32 countdown; /* countdown; init randomGetRange(10, 300) */
    f32 nextSfxTime; /* countdown threshold; on cross plays sfx, advances by randomGetRange(50,250) */
    u8 flags14; /* bits 1/2 */
    u8 flags15; /* bits 1/4 */
    u8 unk16[2];
    struct GameObject* linkedObject; /* ObjMsg target object */
    s16 msgSlotIndex; /* queued message-config slot index (-1 = none); pushed as the type-7 frame payload */
    u8 unk1E[2];
    const DbStealerwormScriptStep* routeCursor;
    RingBufferQueue* msgStack; /* queued 3-word messages */
    intptr_t msgCode; /* current message word 0: code dispatched to the player interface (frame[0]) */
    intptr_t msgMode; /* current message word 1: target-acquisition mode 0/1 (frame[1]) */
    intptr_t objGroup; /* current message word 2: ObjGroup id for FindNearest/Contains (frame[2]) */
    u8 msgAdvance; /* set to advance to / pop the next queued message next tick */
    u8 unk35[3];
    f32 spawnAccumulator; /* 0x38: accumulates on worm move-done; when over threshold, triggers a spawn-search and subtracts the threshold */
    struct GameObject* savedTargetObject;
    u8 unk40[4];
    DbStealerwormFlags44 flags44;
    u8 unk45[3];
    f32 randomTimer48; /* RandomTimer_UpdateRangeTrigger slots */
    f32 randomTimer4C;
} DbStealerwormControl;

#endif
