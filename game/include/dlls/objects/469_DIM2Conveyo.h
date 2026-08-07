#ifndef DLLS_OBJECTS_469_DIM2CONVEYO_H_
#define DLLS_OBJECTS_469_DIM2CONVEYO_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

/* Every active-target retail placement is a fixed nine-word (0x24-byte) record. */
typedef struct Dim2ConveyorPlacement {
    ObjPlacement base;
    s8 rotationXByte;
    u8 unknown19;
    s16 scrollSpeed;
    u8 unknown1C[0x08];
} Dim2ConveyorPlacement;

/* dim2conveyor_getExtraSize() allocates 0x14 bytes. */
typedef struct Dim2ConveyorState {
    f32 scrollX;
    f32 scrollZ;
    u8 unknown08[0x04];
    f32 directionSwapTimer;
    s32 musicHoldTimer;
} Dim2ConveyorState;

STATIC_ASSERT(offsetof(Dim2ConveyorPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(Dim2ConveyorPlacement, rotationXByte) == 0x18);
STATIC_ASSERT(offsetof(Dim2ConveyorPlacement, unknown19) == 0x19);
STATIC_ASSERT(offsetof(Dim2ConveyorPlacement, scrollSpeed) == 0x1A);
STATIC_ASSERT(offsetof(Dim2ConveyorPlacement, unknown1C) == 0x1C);
STATIC_ASSERT(sizeof(Dim2ConveyorPlacement) == 0x24);

STATIC_ASSERT(offsetof(Dim2ConveyorState, scrollX) == 0x00);
STATIC_ASSERT(offsetof(Dim2ConveyorState, scrollZ) == 0x04);
STATIC_ASSERT(offsetof(Dim2ConveyorState, unknown08) == 0x08);
STATIC_ASSERT(offsetof(Dim2ConveyorState, directionSwapTimer) == 0x0C);
STATIC_ASSERT(offsetof(Dim2ConveyorState, musicHoldTimer) == 0x10);
STATIC_ASSERT(sizeof(Dim2ConveyorState) == 0x14);

/* gDIM2ConveyorObjDescriptor from slot02 onwards: the export table the player
   reaches through obj->anim.dll on a conveyor surface. */
typedef struct Dim2ConveyorInterface {
    void* pad00[8];
    void (*getScrollVector)(GameObject* obj, GameObject* caller, f32 unused, f32* outX, f32* outZ);
} Dim2ConveyorInterface;

#define DIM2_CONVEYOR_INTERFACE(conveyor) ((Dim2ConveyorInterface*)*((GameObject*)(conveyor))->anim.dll)

STATIC_ASSERT(offsetof(Dim2ConveyorInterface, getScrollVector) == 0x20);

void dim2conveyor_getScrollVector(GameObject* obj, GameObject* caller, f32 unused, f32* outX, f32* outZ);
int dim2conveyor_getExtraSize(void);
int dim2conveyor_getObjectTypeId(void);
void dim2conveyor_free(GameObject* obj);
void dim2conveyor_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dim2conveyor_hitDetect(void);
void dim2conveyor_update(GameObject* obj);
void dim2conveyor_init(GameObject* obj, const Dim2ConveyorPlacement* placement);
void dim2conveyor_release(void);
void dim2conveyor_initialise(void);

extern ObjectDescriptor11WithPadding gDIM2ConveyorObjDescriptor;

#endif /* DLLS_OBJECTS_469_DIM2CONVEYO_H_ */
