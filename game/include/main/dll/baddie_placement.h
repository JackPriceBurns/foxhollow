#ifndef MAIN_DLL_BADDIE_PLACEMENT_H_
#define MAIN_DLL_BADDIE_PLACEMENT_H_

#include "game/objects/object_setup.h"
#include "global.h"

/*
 * EnemyPlacement - the 0x38 placement/setup record of an object driven by
 * the generic enemy DLL (slot 201). Baddie.c is the reader and supplies the
 * field widths; the SharpClaw encounter spawners (DFSH_ObjCre 0x179,
 * ECSH_Creato 0x191, slot 410) are the writers - each allocates one of these
 * records, fills it and hands it to objSetupObject, and the spawned child's
 * extra block is an EnemyState (enemy_getExtraSize() = 0x370).
 */
typedef struct EnemyPlacement {
    ObjPlacement base;
    s16 gameBit;
    s16 gameBit2;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
    s16 droppedItemId;
    s16 unk24;
    u8 unk26;
    s8 initialWeaponId;
    s8 objectFlagBits; /* 0x28: low 3 bits OR'd into GameObject.objectFlags */
    u8 aggroRangeByte; /* 0x29: aggro radius, shifted left 3 into EnemyState.aggroRange */
    s8 initialYaw;     /* 0x2A: restored into anim.rotX as initialYaw << 8 */
    u8 flags;          /* 0x2B: bit 3 (0x8) reloads spawn position before the trigger sequence */
    s16 respawnDelay; /* 0x2C: minutes fed to MapEvent addTime; 0 skips the off-screen respawn path */
    s8 triggerSequenceId;
    u8 pathStepByte; /* 0x2F: rom-curve advance step, divided by 255 into EnemyState.pathStep */
    s16 unk30;
    u8 hitPoints; /* 0x32: spawn hit-point count -> EnemyState.current */
    u8 unk33;
    u16 unk34; /* the SharpClaw creator spawners (DFSH_ObjCre 377, ECSH_Creato 401) write 0xFFFF; nonzero clears flags2E4 path-control bits 0x2/0x4/0x20 and sets EnemyState.flags 0x40000 */
    u8 unk36[0x38 - 0x36];
} EnemyPlacement;

STATIC_ASSERT(sizeof(EnemyPlacement) == 0x38);
STATIC_ASSERT(offsetof(EnemyPlacement, gameBit) == 0x18);
STATIC_ASSERT(offsetof(EnemyPlacement, unk1C) == 0x1C);
STATIC_ASSERT(offsetof(EnemyPlacement, droppedItemId) == 0x22);
STATIC_ASSERT(offsetof(EnemyPlacement, initialWeaponId) == 0x27);
STATIC_ASSERT(offsetof(EnemyPlacement, objectFlagBits) == 0x28);
STATIC_ASSERT(offsetof(EnemyPlacement, respawnDelay) == 0x2C);
STATIC_ASSERT(offsetof(EnemyPlacement, unk30) == 0x30);
STATIC_ASSERT(offsetof(EnemyPlacement, hitPoints) == 0x32);
STATIC_ASSERT(offsetof(EnemyPlacement, unk34) == 0x34);

#endif /* MAIN_DLL_BADDIE_PLACEMENT_H_ */
