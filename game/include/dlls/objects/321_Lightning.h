#ifndef DLLS_OBJECTS_321_LIGHTNING_H_
#define DLLS_OBJECTS_321_LIGHTNING_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/lightningeffect.h"

typedef enum LightningObjectGroup {
    LIGHTNING_OBJECT_GROUP = 0x48,
} LightningObjectGroup;

typedef enum LightningPlacementFlag {
    LIGHTNING_PLACEMENT_ENABLED = 0x01,
    LIGHTNING_PLACEMENT_ALTERNATE_STYLE = 0x02,
    LIGHTNING_PLACEMENT_PERSISTENT = 0x04,
} LightningPlacementFlag;

typedef enum LightningModeFlag {
    LIGHTNING_MODE_HIT_EFFECT = 0x01,
    LIGHTNING_MODE_DIRECTIONAL_BURST = 0x02,
} LightningModeFlag;

/*
 * The setup fields through enableGameBit are evidenced by this TU. The
 * complete record extent after 0x25 is not yet proven.
 */
typedef struct LightningPlacement {
    ObjPlacement base; /* 0x00 */
    s32 linkedIdent;   /* 0x18 */
    u8 radiusX;        /* 0x1C */
    u8 radiusY;        /* 0x1D */
    u8 lifetimeBase;   /* 0x1E */
    u8 width;          /* 0x1F */
    u8 flags;          /* 0x20: LightningPlacementFlag */
    u8 mode;           /* 0x21: LightningModeFlag */
    u8 initialDelay;   /* 0x22: seconds */
    u8 repeatDelay;    /* 0x23: seconds */
    s16 enableGameBit; /* 0x24 */
} LightningPlacement;

typedef struct LightningModeBits {
    u8 unused : 4;
    u8 mode : 4; /* LightningModeFlag */
} LightningModeBits;

typedef struct LightningStateFlags {
    u8 enabled : 1;
    u8 persistent : 1;
    u8 alternateStyle : 1;
    u8 unused : 5;
} LightningStateFlags;

/* lightning_getExtraSize proves the complete 0x28-byte allocation. */
typedef struct LightningState {
    LightningEffect* effect;    /* 0x00 */
    f32 ageTimer;               /* 0x04 */
    f32 radiusX;                /* 0x08 */
    f32 radiusY;                /* 0x0C */
    f32 hitRadius;              /* 0x10 */
    f32 burstRadius;            /* 0x14 */
    f32 countdown;              /* 0x18 */
    u8 lifetimeBase;            /* 0x1C */
    u8 width;                   /* 0x1D */
    u8 pad1E[2];                /* 0x1E */
    s32 linkedIdent;            /* 0x20 */
    LightningModeBits modeBits; /* 0x24 */
    LightningStateFlags flags;  /* 0x25 */
    u8 pad26[2];                /* 0x26 */
} LightningState;

STATIC_ASSERT(offsetof(LightningPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(LightningPlacement, linkedIdent) == 0x18);
STATIC_ASSERT(offsetof(LightningPlacement, radiusX) == 0x1C);
STATIC_ASSERT(offsetof(LightningPlacement, radiusY) == 0x1D);
STATIC_ASSERT(offsetof(LightningPlacement, lifetimeBase) == 0x1E);
STATIC_ASSERT(offsetof(LightningPlacement, width) == 0x1F);
STATIC_ASSERT(offsetof(LightningPlacement, flags) == 0x20);
STATIC_ASSERT(offsetof(LightningPlacement, mode) == 0x21);
STATIC_ASSERT(offsetof(LightningPlacement, initialDelay) == 0x22);
STATIC_ASSERT(offsetof(LightningPlacement, repeatDelay) == 0x23);
STATIC_ASSERT(offsetof(LightningPlacement, enableGameBit) == 0x24);

STATIC_ASSERT(sizeof(LightningModeBits) == 0x01);
STATIC_ASSERT(sizeof(LightningStateFlags) == 0x01);
STATIC_ASSERT(offsetof(LightningState, effect) == 0x00);
STATIC_ASSERT(offsetof(LightningState, ageTimer) == 0x04);
STATIC_ASSERT(offsetof(LightningState, radiusX) == 0x08);
STATIC_ASSERT(offsetof(LightningState, radiusY) == 0x0C);
STATIC_ASSERT(offsetof(LightningState, hitRadius) == 0x10);
STATIC_ASSERT(offsetof(LightningState, burstRadius) == 0x14);
STATIC_ASSERT(offsetof(LightningState, countdown) == 0x18);
STATIC_ASSERT(offsetof(LightningState, lifetimeBase) == 0x1C);
STATIC_ASSERT(offsetof(LightningState, width) == 0x1D);
STATIC_ASSERT(offsetof(LightningState, pad1E) == 0x1E);
STATIC_ASSERT(offsetof(LightningState, linkedIdent) == 0x20);
STATIC_ASSERT(offsetof(LightningState, modeBits) == 0x24);
STATIC_ASSERT(offsetof(LightningState, flags) == 0x25);
STATIC_ASSERT(offsetof(LightningState, pad26) == 0x26);
STATIC_ASSERT(sizeof(LightningState) == 0x28);

int lightning_getExtraSize(void);
void lightning_free(GameObject* obj, int flags);
void lightning_render(GameObject* obj);
void lightning_update(GameObject* obj);
void lightning_init(GameObject* obj, LightningPlacement* placement);

extern ObjectDescriptor gLightningObjDescriptor;

#endif /* DLLS_OBJECTS_321_LIGHTNING_H_ */
