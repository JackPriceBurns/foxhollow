#ifndef DLLS_OBJECTS_292_H_
#define DLLS_OBJECTS_292_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define DEATH_GAS_STATE_SIZE 0x10

/* Only the accessed placement prefix is recovered; the complete retail width is not established. */
typedef struct DeathGasPlacement {
    ObjPlacement base; /* 0x00 */
    u8 drainRate;      /* 0x18 */
    u8 fillRate;       /* 0x19 */
    s16 activeGameBit; /* 0x1A: -1 leaves the volume permanently active */
} DeathGasPlacement;

typedef struct DeathGasStateFlags {
    u8 fogStateActive : 1; /* The volume has initialized its fog lifecycle. */
    u8 airMeterActive : 1; /* The meter is draining or refilling. */
    u8 suppressFog : 1;    /* deathGasNoF leaves the global fog state untouched. */
    u8 : 5;
} DeathGasStateFlags;

typedef struct DeathGasState {
    f32 airRemaining;         /* 0x00 */
    f32 damageTimer;          /* 0x04 */
    f32 effectRadius;         /* 0x08 */
    DeathGasStateFlags flags; /* 0x0C */
    u8 pad0D[3];              /* 0x0D */
} DeathGasState;

STATIC_ASSERT(offsetof(DeathGasPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(DeathGasPlacement, drainRate) == 0x18);
STATIC_ASSERT(offsetof(DeathGasPlacement, fillRate) == 0x19);
STATIC_ASSERT(offsetof(DeathGasPlacement, activeGameBit) == 0x1A);

STATIC_ASSERT(sizeof(DeathGasStateFlags) == 0x1);
STATIC_ASSERT(offsetof(DeathGasState, airRemaining) == 0x0);
STATIC_ASSERT(offsetof(DeathGasState, damageTimer) == 0x4);
STATIC_ASSERT(offsetof(DeathGasState, effectRadius) == 0x8);
STATIC_ASSERT(offsetof(DeathGasState, flags) == 0xC);
STATIC_ASSERT(offsetof(DeathGasState, pad0D) == 0xD);
STATIC_ASSERT(sizeof(DeathGasState) == DEATH_GAS_STATE_SIZE);

int DeathGas_getExtraSize(void);
void DeathGas_free(GameObject* obj);
void DeathGas_update(GameObject* obj);
void DeathGas_init(GameObject* obj);

extern ObjectDescriptor gDeathGasObjDescriptor;

#endif /* DLLS_OBJECTS_292_H_ */
