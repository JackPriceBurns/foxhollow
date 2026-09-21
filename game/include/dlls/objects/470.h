#ifndef DLLS_OBJECTS_470_H_
#define DLLS_OBJECTS_470_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

/*
 * Active EN has no object definition assigned to DLL 0x1D6. This type models
 * only the placement prefix read by the DLL's callbacks; it does not claim a
 * complete retail placement width.
 */
typedef struct Dll1D6PlacementView {
    ObjPlacement base;
    s8 rotationXByte;
    u8 unknown19;
    s16 upTimer;
    s16 downTimer;
} Dll1D6PlacementView;

/* dll_1D6_getExtraSize() allocates 0x20 bytes. */
typedef struct Dll1D6State {
    void* actionDataA; /* mmAlloc'd 40-byte LACTIONS.BIN row */
    void* actionDataB; /* row immediately following actionDataA */
    f32 hitRangeSqA;
    f32 hitRangeSqB;
    f32 bobPhase;
    f32 bobRate;
    s16 upTimer;
    s16 downTimer;
    s8 dizzyTimer;
    u8 flags;
    u8 hitRow;     /* 0x10-byte row selected from the active model data */
    u8 actionSlot; /* index into the first four gDll1D6SlotInUse entries */
} Dll1D6State;

#define DLL1D6_STATE_FLAG_DOWN_PHASE  0x01
#define DLL1D6_STATE_FLAG_HIT_ENABLED 0x02
#define DLL1D6_STATE_FLAG_BOB_ACTIVE  0x04

STATIC_ASSERT(offsetof(Dll1D6PlacementView, base) == 0x00);
STATIC_ASSERT(offsetof(Dll1D6PlacementView, rotationXByte) == 0x18);
STATIC_ASSERT(offsetof(Dll1D6PlacementView, unknown19) == 0x19);
STATIC_ASSERT(offsetof(Dll1D6PlacementView, upTimer) == 0x1A);
STATIC_ASSERT(offsetof(Dll1D6PlacementView, downTimer) == 0x1C);

STATIC_ASSERT(offsetof(Dll1D6State, actionDataA) == 0x00);
STATIC_ASSERT(offsetof(Dll1D6State, actionDataB) == 0x04);
STATIC_ASSERT(offsetof(Dll1D6State, hitRangeSqA) == 0x08);
STATIC_ASSERT(offsetof(Dll1D6State, hitRangeSqB) == 0x0C);
STATIC_ASSERT(offsetof(Dll1D6State, bobPhase) == 0x10);
STATIC_ASSERT(offsetof(Dll1D6State, bobRate) == 0x14);
STATIC_ASSERT(offsetof(Dll1D6State, upTimer) == 0x18);
STATIC_ASSERT(offsetof(Dll1D6State, downTimer) == 0x1A);
STATIC_ASSERT(offsetof(Dll1D6State, dizzyTimer) == 0x1C);
STATIC_ASSERT(offsetof(Dll1D6State, flags) == 0x1D);
STATIC_ASSERT(offsetof(Dll1D6State, hitRow) == 0x1E);
STATIC_ASSERT(offsetof(Dll1D6State, actionSlot) == 0x1F);
STATIC_ASSERT(sizeof(Dll1D6State) == 0x20);

int dll_1D6_getExtraSize(void);
int dll_1D6_getObjectTypeId(void);
void dll_1D6_free(GameObject* obj);
void dll_1D6_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dll_1D6_hitDetect(void);
void dll_1D6_update(GameObject* obj);
void dll_1D6_init(GameObject* obj, const Dll1D6PlacementView* placement);
void dll_1D6_release(void);
void dll_1D6_initialise(void);

extern s16 gDll1D6SlotTabIndex[4];
extern u8 gDll1D6SlotInUse[8];
extern ObjectDescriptor gDll1D6ObjDescriptor;

#endif /* DLLS_OBJECTS_470_H_ */
