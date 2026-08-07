#ifndef DLLS_OBJECTS_519_WM_WORM_H_
#define DLLS_OBJECTS_519_WM_WORM_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

/* Only the accessed placement prefix is recovered; the complete retail width is not established. */
typedef struct WMWormPlacementView {
    ObjPlacement base;        /* 0x00 */
    s8 effectScale;           /* 0x18: multiplied by four */
    u8 unknown19;             /* 0x19 */
    s16 particleEffectId;     /* 0x1A */
    s16 spawnCountOrInterval; /* 0x1C: positive=count, negative=cooldown */
} WMWormPlacementView;

STATIC_ASSERT(offsetof(WMWormPlacementView, base) == 0x00);
STATIC_ASSERT(offsetof(WMWormPlacementView, effectScale) == 0x18);
STATIC_ASSERT(offsetof(WMWormPlacementView, unknown19) == 0x19);
STATIC_ASSERT(offsetof(WMWormPlacementView, particleEffectId) == 0x1A);
STATIC_ASSERT(offsetof(WMWormPlacementView, spawnCountOrInterval) == 0x1C);

/* WM_Worm_getExtraSize() allocates this complete 0x1C-byte state. */
typedef struct WMWormState {
    f32 effectScale;          /* 0x00 */
    s16 particleEffectId;     /* 0x04 */
    u8 unknown06[2];          /* 0x06 */
    s16 spawnCountOrInterval; /* 0x08 */
    u8 unknown0A[2];          /* 0x0A */
    s16 unknown0C;            /* 0x0C */
    u8 unknown0E[2];          /* 0x0E */
    f32 homeX;                /* 0x10 */
    f32 homeY;                /* 0x14 */
    f32 homeZ;                /* 0x18 */
} WMWormState;

STATIC_ASSERT(offsetof(WMWormState, effectScale) == 0x00);
STATIC_ASSERT(offsetof(WMWormState, particleEffectId) == 0x04);
STATIC_ASSERT(offsetof(WMWormState, unknown06) == 0x06);
STATIC_ASSERT(offsetof(WMWormState, spawnCountOrInterval) == 0x08);
STATIC_ASSERT(offsetof(WMWormState, unknown0A) == 0x0A);
STATIC_ASSERT(offsetof(WMWormState, unknown0C) == 0x0C);
STATIC_ASSERT(offsetof(WMWormState, unknown0E) == 0x0E);
STATIC_ASSERT(offsetof(WMWormState, homeX) == 0x10);
STATIC_ASSERT(offsetof(WMWormState, homeY) == 0x14);
STATIC_ASSERT(offsetof(WMWormState, homeZ) == 0x18);
STATIC_ASSERT(sizeof(WMWormState) == 0x1C);

int WM_Worm_getExtraSize(void);
int WM_Worm_getObjectTypeId(void);
void WM_Worm_free(GameObject* obj);
void WM_Worm_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void WM_Worm_hitDetect(void);
void WM_Worm_update(GameObject* obj);
void WM_Worm_init(GameObject* obj, const WMWormPlacementView* placement);
void WM_Worm_release(void);
void WM_Worm_initialise(void);

extern ObjectDescriptor gWM_WormObjDescriptor;

#endif /* DLLS_OBJECTS_519_WM_WORM_H_ */
