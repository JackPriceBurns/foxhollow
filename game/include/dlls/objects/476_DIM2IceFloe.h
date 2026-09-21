#ifndef DLLS_OBJECTS_476_DIM2ICEFLOE_H_
#define DLLS_OBJECTS_476_DIM2ICEFLOE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/curve_types.h"

/*
 * Only the fields consumed by this DLL are modeled. The active-target retail
 * placement files are unavailable, so this does not claim a complete record
 * width.
 */
typedef struct Dim2IceFloePlacementView {
    ObjPlacement base;
    s8 initialRotationXByte;
    u8 unknown19[3];
    s16 pathStep;
} Dim2IceFloePlacementView;

typedef struct Dim2IceFloeCompletionFlags {
    u8 finished : 1;
    u8 unused : 7;
} Dim2IceFloeCompletionFlags;

/* dim2icefloe_getExtraSize() allocates the complete 0xBC-byte block. */
typedef struct Dim2IceFloeState {
    Curve curve;
    GameObject* followedObject;
    u32 targetObjectId;
    f32 pathStep;
    f32 yawJitter;
    f32 bobRate;
    f32 bobBase;
    u16 bobPhase;
    u8 flags;
    u8 unknownB7;
    u8 paused;
    Dim2IceFloeCompletionFlags completion;
    u8 unknownBA[2];
} Dim2IceFloeState;

STATIC_ASSERT(offsetof(Dim2IceFloePlacementView, base) == 0x00);
STATIC_ASSERT(offsetof(Dim2IceFloePlacementView, initialRotationXByte) == 0x18);
STATIC_ASSERT(offsetof(Dim2IceFloePlacementView, unknown19) == 0x19);
STATIC_ASSERT(offsetof(Dim2IceFloePlacementView, pathStep) == 0x1C);

STATIC_ASSERT(sizeof(Dim2IceFloeCompletionFlags) == 0x01);

STATIC_ASSERT(offsetof(Dim2IceFloeState, curve) == 0x00);
STATIC_ASSERT(offsetof(Dim2IceFloeState, followedObject) == 0x9C);
STATIC_ASSERT(offsetof(Dim2IceFloeState, targetObjectId) == 0xA0);
STATIC_ASSERT(offsetof(Dim2IceFloeState, pathStep) == 0xA4);
STATIC_ASSERT(offsetof(Dim2IceFloeState, yawJitter) == 0xA8);
STATIC_ASSERT(offsetof(Dim2IceFloeState, bobRate) == 0xAC);
STATIC_ASSERT(offsetof(Dim2IceFloeState, bobBase) == 0xB0);
STATIC_ASSERT(offsetof(Dim2IceFloeState, bobPhase) == 0xB4);
STATIC_ASSERT(offsetof(Dim2IceFloeState, flags) == 0xB6);
STATIC_ASSERT(offsetof(Dim2IceFloeState, unknownB7) == 0xB7);
STATIC_ASSERT(offsetof(Dim2IceFloeState, paused) == 0xB8);
STATIC_ASSERT(offsetof(Dim2IceFloeState, completion) == 0xB9);
STATIC_ASSERT(offsetof(Dim2IceFloeState, unknownBA) == 0xBA);
STATIC_ASSERT(sizeof(Dim2IceFloeState) == 0xBC);

int dim2icefloe_getExtraSize(void);
int dim2icefloe_getObjectTypeId(void);
void dim2icefloe_free(void);
void dim2icefloe_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dim2icefloe_hitDetect(void);
void dim2icefloe_update(GameObject* obj);
void dim2icefloe_init(GameObject* obj, Dim2IceFloePlacementView* placement);
void dim2icefloe_release(void);
void dim2icefloe_initialise(void);

extern ObjectDescriptor gDIM2IceFloeObjDescriptor;

#endif /* DLLS_OBJECTS_476_DIM2ICEFLOE_H_ */
