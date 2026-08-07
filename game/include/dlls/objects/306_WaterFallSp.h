#ifndef DLLS_OBJECTS_306_WATERFALLSP_H_
#define DLLS_OBJECTS_306_WATERFALLSP_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef enum WaterFallSprayFlag {
    WATERFALLSPRAY_FLAG_EFFECT_320 = 0x01,
    WATERFALLSPRAY_FLAG_EFFECT_321 = 0x02,
    WATERFALLSPRAY_FLAG_EFFECT_322 = 0x04,
    WATERFALLSPRAY_FLAG_EFFECT_351 = 0x08,
    WATERFALLSPRAY_FLAG_SFX_DISABLED = 0x10,
} WaterFallSprayFlag;

/*
 * All 53 active retail EN placements use this complete fixed-width
 * 0x28-byte record.
 */
typedef struct WaterFallSprayPlacement {
    ObjPlacement base; /* 0x00 */
    s16 enableGameBit; /* 0x18: -1 means always enabled */
    s8 initialRotZ;    /* 0x1A: shifted left by eight */
    s8 initialRotY;    /* 0x1B: shifted left by eight */
    s8 initialRotX;    /* 0x1C: shifted left by eight */
    u8 randomExtentX;  /* 0x1D */
    u8 randomExtentZ;  /* 0x1E */
    u8 randomExtentY;  /* 0x1F */
    u8 triggerRadius;  /* 0x20: multiplied by 16; zero is unlimited */
    u8 pad21[2];       /* 0x21 */
    u8 flags;          /* 0x23: WaterFallSprayFlag */
    u8 emitCount;      /* 0x24: also determines the negated cooldown */
    u8 pad25[3];       /* 0x25 */
} WaterFallSprayPlacement;

/* WaterFallSpray_getExtraSize proves the complete 0x08-byte allocation. */
typedef struct WaterFallSprayState {
    u32 sfxIdA; /* 0x00 */
    u32 sfxIdB; /* 0x04 */
} WaterFallSprayState;

STATIC_ASSERT(offsetof(WaterFallSprayPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(WaterFallSprayPlacement, enableGameBit) == 0x18);
STATIC_ASSERT(offsetof(WaterFallSprayPlacement, initialRotZ) == 0x1A);
STATIC_ASSERT(offsetof(WaterFallSprayPlacement, initialRotY) == 0x1B);
STATIC_ASSERT(offsetof(WaterFallSprayPlacement, initialRotX) == 0x1C);
STATIC_ASSERT(offsetof(WaterFallSprayPlacement, randomExtentX) == 0x1D);
STATIC_ASSERT(offsetof(WaterFallSprayPlacement, randomExtentZ) == 0x1E);
STATIC_ASSERT(offsetof(WaterFallSprayPlacement, randomExtentY) == 0x1F);
STATIC_ASSERT(offsetof(WaterFallSprayPlacement, triggerRadius) == 0x20);
STATIC_ASSERT(offsetof(WaterFallSprayPlacement, pad21) == 0x21);
STATIC_ASSERT(offsetof(WaterFallSprayPlacement, flags) == 0x23);
STATIC_ASSERT(offsetof(WaterFallSprayPlacement, emitCount) == 0x24);
STATIC_ASSERT(offsetof(WaterFallSprayPlacement, pad25) == 0x25);
STATIC_ASSERT(sizeof(WaterFallSprayPlacement) == 0x28);

STATIC_ASSERT(offsetof(WaterFallSprayState, sfxIdA) == 0x00);
STATIC_ASSERT(offsetof(WaterFallSprayState, sfxIdB) == 0x04);
STATIC_ASSERT(sizeof(WaterFallSprayState) == 0x08);

void WaterFallSpray_free(GameObject* obj);
void WaterFallSpray_init(GameObject* obj, WaterFallSprayPlacement* placement);
void WaterFallSpray_render(void);
void WaterFallSpray_update(GameObject* obj);
int WaterFallSpray_getExtraSize(void);
int WaterFallSpray_sequenceCallback(GameObject* obj);

extern ObjectDescriptor gWaterFallSprayObjDescriptor;

#endif /* DLLS_OBJECTS_306_WATERFALLSP_H_ */
