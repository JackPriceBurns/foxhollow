#ifndef DLLS_OBJECTS_263_H_
#define DLLS_OBJECTS_263_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define WINDLIFT107_PLACEMENT_SIZE 0x24
#define WINDLIFT107_STATE_SIZE     0x2C

typedef enum WindLift107CarryState {
    WINDLIFT107_CARRY_IDLE = 0,
    WINDLIFT107_CARRY_GRABBING = 1,
    WINDLIFT107_CARRY_HELD = 2,
} WindLift107CarryState;

typedef enum WindLift107ThrowState {
    WINDLIFT107_THROW_NONE = 0,
    WINDLIFT107_THROW_LAUNCHED = 1,
    WINDLIFT107_THROW_DROPPED = 2,
} WindLift107ThrowState;

typedef struct WindLift107Placement {
    ObjPlacement base; /* 0x00 */
    s8 rotXParam;      /* 0x18: shifted left eight bits */
    s8 radiusParam;    /* 0x19: radius in ten-unit increments */
    u8 pad1A[2];       /* 0x1A */
    s16 cooldownParam; /* 0x1C: cooldown duration scale */
    s16 unk1E;         /* 0x1E */
    s16 leashRange;    /* 0x20 */
    u8 pad22[2];       /* 0x22 */
} WindLift107Placement;

typedef struct WindLift107State {
    s32 cooldownTimer;    /* 0x00 */
    s32 cooldownDuration; /* 0x04 */
    f32 radius;           /* 0x08 */
    s16 carryAngle;       /* 0x0C */
    s16 carryParam;       /* 0x0E */
    s16 disableTimer;     /* 0x10 */
    s16 leashRange;       /* 0x12 */
    s16 unk14;            /* 0x14 */
    s16 timer;            /* 0x16 */
    s16 unk18;            /* 0x18 */
    s16 flightTimer;      /* 0x1A */
    u8 pad1C[2];          /* 0x1C */
    s16 burstTimer;       /* 0x1E */
    u8 pad20;             /* 0x20 */
    s8 carryState;        /* 0x21: WindLift107CarryState */
    s8 carryAttached;     /* 0x22 */
    s8 throwState;        /* 0x23: WindLift107ThrowState */
    u8 pad24;             /* 0x24 */
    u8 unk25;             /* 0x25 */
    u8 glowPulse;         /* 0x26 */
    u8 unk27;             /* 0x27 */
    u8 pad28[4];          /* 0x28 */
} WindLift107State;

STATIC_ASSERT(offsetof(WindLift107Placement, base) == 0x0);
STATIC_ASSERT(offsetof(WindLift107Placement, rotXParam) == 0x18);
STATIC_ASSERT(offsetof(WindLift107Placement, radiusParam) == 0x19);
STATIC_ASSERT(offsetof(WindLift107Placement, pad1A) == 0x1A);
STATIC_ASSERT(offsetof(WindLift107Placement, cooldownParam) == 0x1C);
STATIC_ASSERT(offsetof(WindLift107Placement, unk1E) == 0x1E);
STATIC_ASSERT(offsetof(WindLift107Placement, leashRange) == 0x20);
STATIC_ASSERT(offsetof(WindLift107Placement, pad22) == 0x22);
STATIC_ASSERT(sizeof(WindLift107Placement) == WINDLIFT107_PLACEMENT_SIZE);

STATIC_ASSERT(offsetof(WindLift107State, cooldownTimer) == 0x0);
STATIC_ASSERT(offsetof(WindLift107State, cooldownDuration) == 0x4);
STATIC_ASSERT(offsetof(WindLift107State, radius) == 0x8);
STATIC_ASSERT(offsetof(WindLift107State, carryAngle) == 0xC);
STATIC_ASSERT(offsetof(WindLift107State, carryParam) == 0xE);
STATIC_ASSERT(offsetof(WindLift107State, disableTimer) == 0x10);
STATIC_ASSERT(offsetof(WindLift107State, leashRange) == 0x12);
STATIC_ASSERT(offsetof(WindLift107State, unk14) == 0x14);
STATIC_ASSERT(offsetof(WindLift107State, timer) == 0x16);
STATIC_ASSERT(offsetof(WindLift107State, unk18) == 0x18);
STATIC_ASSERT(offsetof(WindLift107State, flightTimer) == 0x1A);
STATIC_ASSERT(offsetof(WindLift107State, pad1C) == 0x1C);
STATIC_ASSERT(offsetof(WindLift107State, burstTimer) == 0x1E);
STATIC_ASSERT(offsetof(WindLift107State, pad20) == 0x20);
STATIC_ASSERT(offsetof(WindLift107State, carryState) == 0x21);
STATIC_ASSERT(offsetof(WindLift107State, carryAttached) == 0x22);
STATIC_ASSERT(offsetof(WindLift107State, throwState) == 0x23);
STATIC_ASSERT(offsetof(WindLift107State, pad24) == 0x24);
STATIC_ASSERT(offsetof(WindLift107State, unk25) == 0x25);
STATIC_ASSERT(offsetof(WindLift107State, glowPulse) == 0x26);
STATIC_ASSERT(offsetof(WindLift107State, unk27) == 0x27);
STATIC_ASSERT(offsetof(WindLift107State, pad28) == 0x28);
STATIC_ASSERT(sizeof(WindLift107State) == WINDLIFT107_STATE_SIZE);

int windLift107_getExtraSize(void);
int windLift107_getObjectTypeId(void);
void windLift107_free(GameObject* obj);
void windLift107_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void windLift107_hitDetect(GameObject* obj);
void windLift107_update(GameObject* obj);
void windLift107_init(GameObject* obj, WindLift107Placement* placement);
void windLift107_release(void);
void windLift107_initialise(void);

extern ObjectDescriptor gWindLift107ObjDescriptor;

#endif /* DLLS_OBJECTS_263_H_ */
