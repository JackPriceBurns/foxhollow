#ifndef MAIN_DLL_BADDIE_CONTROL_INTERFACE_H_
#define MAIN_DLL_BADDIE_CONTROL_INTERFACE_H_

#include "game/objects/object_fwd.h"
#include "types.h"

typedef struct ObjSeqState ObjSeqState;

/* Function-pointer table exported by the baddie-control DLL (0x19); word slot N
 * here is the DLL's exported slot N + 2, defined as dll_19_<name> in
 * dlls/engine/25/25.c. Named slots are those with recovered call sites; the
 * pads are unrecovered slots. */
typedef struct BaddieControlInterface
{
    u8 pad00[0x10];
    void (*updateMovementBlend)(GameObject* obj, void* state, void* unusedState, f32 maxSpeed,
                                f32 turnSpeed); /* 0x10 */
    void (*getTargetGeometry)(GameObject* obj, GameObject* target, int divisions, u16* sectorOut,
                              u16* yawDeltaOut, u16* distanceOut); /* 0x14 */
    u8 (*getClearDirectionMask)(GameObject* obj, void* state, f32 distance); /* 0x18 */
    u8 pad1C[0x28 - 0x1C];
    void (*startHitReaction)(GameObject* obj, void* state, void* hitbox, s16 gameBit, u8* flagOut,
                             s16 substate, s16 moveMode, int animMove, s8 physicsActive); /* 0x28 */
    void (*updateGravity)(GameObject* obj, void* state, f32 gravity, s8 physicsActive); /* 0x2C */
    int (*isObjectValid)(GameObject* obj, void* state, u8 checkDead);        /* 0x30 */
    int (*updateSequenceMovement)(GameObject* obj, ObjSeqState* seq, char* state, void* moveHandlers,
                                  void* stateHandlers, s16 controlMode);     /* 0x34 */
    u8 pad38[0x3C - 0x38];
    void (*pollCameraTarget)(GameObject* obj, void* state, u16* flags, int modeA, int modeB,
                             s16 soundIdA, s16 soundIdB);                    /* 0x3C */
    void (*releaseState)(GameObject* obj, void* state, u8 flags);            /* 0x40 */
    int (*shouldDropTarget)(GameObject* obj, void* state, f32 distanceThreshold, int requireFar); /* 0x44 */
    GameObject* (*findAggroTarget)(GameObject* obj, void* state, f32 aggroRange, int angleRange); /* 0x48 */
    GameObject* (*spawnChild)(GameObject* obj, int spawnType, int unused, int alt); /* 0x4C */
    int (*updateHitReaction)(GameObject* obj, void* state, void* hitbox, s16 gameBit, int* moveTable,
                             u8* damageTable, s16 substate, void* hitPosOut); /* 0x50 */
    int (*processMessages)(GameObject* obj, void* state, void* hitbox, s16 gameBit, u8* flagOut,
                           s16 substateIdle, s16 substateActive, s16 moveMode); /* 0x54 */
    void (*initGroundBaddie)(GameObject* obj, u8* config, u8* state, int moveArg0, int moveArg1,
                             int pathFlags, u8 initFlags, f32 pathRadius); /* 0x58 */
    u8 pad5C[0x60 - 0x5C];
    f32 (*getHealthFraction)(GameObject* obj); /* 0x60 */
} BaddieControlInterface;

extern BaddieControlInterface** gBaddieControlInterface;

#endif /* MAIN_DLL_BADDIE_CONTROL_INTERFACE_H_ */
