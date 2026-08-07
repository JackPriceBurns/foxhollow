#ifndef DLLS_OBJECTS_512_H_
#define DLLS_OBJECTS_512_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

struct ObjSeqState;

/*
 * Active EN does not establish a complete retail placement width. This view
 * models only the signed rotation byte consumed by dll_200_init().
 */
typedef struct Dll200PlacementView {
    ObjPlacement base;
    s8 rotationXByte;
} Dll200PlacementView;

/* dll_200_getExtraSize() allocates the complete 0x28-byte state block. */
typedef struct Dll200State {
    f32 homeX;
    f32 homeY;
    f32 homeZ;
    f32 animationStep;
    f32 hitReactStepScale;
    f32 unknown14;
    s16 unknown18;
    u8 unknown1A[2];
    u32 unknown1C;
    s16 behaviorTimer;
    u8 behaviorMode;
    u8 previousBehaviorMode;
    u8 interactionLatch;
    u8 sequenceMode;
    u8 objectIdLow;
    u8 interactionCount;
} Dll200State;

STATIC_ASSERT(offsetof(Dll200PlacementView, base) == 0x00);
STATIC_ASSERT(offsetof(Dll200PlacementView, rotationXByte) == 0x18);

STATIC_ASSERT(offsetof(Dll200State, homeX) == 0x00);
STATIC_ASSERT(offsetof(Dll200State, homeY) == 0x04);
STATIC_ASSERT(offsetof(Dll200State, homeZ) == 0x08);
STATIC_ASSERT(offsetof(Dll200State, animationStep) == 0x0C);
STATIC_ASSERT(offsetof(Dll200State, hitReactStepScale) == 0x10);
STATIC_ASSERT(offsetof(Dll200State, unknown14) == 0x14);
STATIC_ASSERT(offsetof(Dll200State, unknown18) == 0x18);
STATIC_ASSERT(offsetof(Dll200State, unknown1A) == 0x1A);
STATIC_ASSERT(offsetof(Dll200State, unknown1C) == 0x1C);
STATIC_ASSERT(offsetof(Dll200State, behaviorTimer) == 0x20);
STATIC_ASSERT(offsetof(Dll200State, behaviorMode) == 0x22);
STATIC_ASSERT(offsetof(Dll200State, previousBehaviorMode) == 0x23);
STATIC_ASSERT(offsetof(Dll200State, interactionLatch) == 0x24);
STATIC_ASSERT(offsetof(Dll200State, sequenceMode) == 0x25);
STATIC_ASSERT(offsetof(Dll200State, objectIdLow) == 0x26);
STATIC_ASSERT(offsetof(Dll200State, interactionCount) == 0x27);
STATIC_ASSERT(sizeof(Dll200State) == 0x28);

void dll_200_updateMapAct6(GameObject* obj);
void dll_200_updateMapAct2(GameObject* obj);
void dll_200_updateMapAct1(GameObject* obj);
int dll_200_processMapAct1Events(GameObject* obj, int unusedArg2, struct ObjSeqState* animUpdate,
                                 int unusedArg4);
int dll_200_sequenceCallback(GameObject* obj, int unusedArg2, struct ObjSeqState* animUpdate, int unusedArg4);
int dll_200_getExtraSize(void);
int dll_200_getObjectTypeId(void);
void dll_200_free(void);
void dll_200_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dll_200_hitDetect(void);
void dll_200_update(GameObject* objectHandle);
void dll_200_init(GameObject* obj, const Dll200PlacementView* placement);
void dll_200_release(void);
void dll_200_initialise(void);

extern ObjectDescriptor gDll200ObjDescriptor;

#endif /* DLLS_OBJECTS_512_H_ */
