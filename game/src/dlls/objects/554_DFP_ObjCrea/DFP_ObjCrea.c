#include "game/objects/object_setup.h"
#include "main/gamebits.h"
#include "main/frame_timing.h"
#include "sys/objects/lifecycle.h"
#include "sys/objects.h"
#include "main/dll/DF/dll_022A_dfpobjcreator.h"

typedef enum DfpObjectCreatorBehavior {
    DFP_OBJECT_CREATOR_PERIODIC = 7
} DfpObjectCreatorBehavior;

typedef enum DfpObjectCreatorObjectId {
    DFP_OBJECT_CREATOR_CHILD = 0x71B,
    DFP_OBJECT_CREATOR_CHILD_DEFINITION = 0xDC
} DfpObjectCreatorObjectId;

struct DfpObjectCreatorPlacement {
    ObjPlacement base;
    s16 gameBit;
    s16 behavior;
    s16 spawnPeriod;
    s8 rotationXAndChildValue;
    s8 parameter12;
    u8 parameter14;
    u8 pad21[3];
    s16 parameter24;
    u8 pad26[10];
};

typedef struct DfpObjectCreatorChildPlacement {
    ObjPlacement base;
    u8 pad18[2];
    s16 objectDefinitionId;
    u8 pad1C[2];
    s16 gameBit;
    s16 secondaryGameBit;
    u8 pad22[2];
} DfpObjectCreatorChildPlacement;

typedef struct DfpObjectCreatorState {
    GameObject* spawnedObject;
    u8 padAfterPointer[8];
    s16 gameBit;
    s16 spawnPeriod;
    s16 spawnTimer;
    s16 parameter12;
    s16 parameter14;
    s16 parameter16;
    u8 padTail[4];
} DfpObjectCreatorState;

STATIC_ASSERT(offsetof(DfpObjectCreatorPlacement, gameBit) == 0x18);
STATIC_ASSERT(offsetof(DfpObjectCreatorPlacement, behavior) == 0x1A);
STATIC_ASSERT(offsetof(DfpObjectCreatorPlacement, spawnPeriod) == 0x1C);
STATIC_ASSERT(offsetof(DfpObjectCreatorPlacement, rotationXAndChildValue) == 0x1E);
STATIC_ASSERT(sizeof(DfpObjectCreatorPlacement) == 0x30);
STATIC_ASSERT(offsetof(DfpObjectCreatorChildPlacement, objectDefinitionId) == 0x1A);
STATIC_ASSERT(offsetof(DfpObjectCreatorChildPlacement, gameBit) == 0x1E);
STATIC_ASSERT(offsetof(DfpObjectCreatorChildPlacement, secondaryGameBit) == 0x20);
STATIC_ASSERT(sizeof(DfpObjectCreatorChildPlacement) == 0x24);

int DFP_ObjCreator_getExtraSize(void)
{
    return sizeof(DfpObjectCreatorState);
}
int DFP_ObjCreator_getObjectTypeId(void)
{
    return 0x0;
}

void DFP_ObjCreator_free(GameObject* obj, int flag)
{
    DfpObjectCreatorState* state = obj->extra;
    if (flag == 0)
    {
        if (state->spawnedObject != NULL)
        {
            Obj_FreeObject(state->spawnedObject);
            state->spawnedObject = NULL;
        }
    }
}

void DFP_ObjCreator_render(GameObject* obj, int p2, int p3, int p4, int p5, s8 visible)
{
    if (visible == 0)
        return;
}

void DFP_ObjCreator_hitDetect(void)
{
}

void DFP_ObjCreator_update(GameObject* obj)
{

    const DfpObjectCreatorPlacement* placement = (const DfpObjectCreatorPlacement*)obj->anim.placementData;
    DfpObjectCreatorState* state = obj->extra;
    DfpObjectCreatorChildPlacement* childPlacement;
    GameObject* newObj;

    if (Obj_IsLoadingLocked() != 0)
    {
        switch (ObjAnim_ReadPlacementS16(&obj->anim, &placement->behavior))
        {
        case DFP_OBJECT_CREATOR_PERIODIC:
            state->spawnTimer -= (s16)timeDelta;
            if (state->spawnTimer <= 0 && mainGetBit(state->gameBit) != 0)
            {
                state->spawnTimer = state->spawnPeriod;
                childPlacement =
                    (DfpObjectCreatorChildPlacement*)Obj_AllocObjectSetup(sizeof(*childPlacement),
                                                                          DFP_OBJECT_CREATOR_CHILD);
                childPlacement->base.posX = placement->base.posX;
                childPlacement->base.posY = placement->base.posY;
                childPlacement->base.posZ = placement->base.posZ;
                childPlacement->base.color[0] = placement->base.color[0];
                childPlacement->base.color[1] = placement->base.color[1];
                childPlacement->base.color[2] = placement->base.color[2];
                childPlacement->base.color[3] = placement->base.color[3];
                childPlacement->gameBit = -1;
                childPlacement->secondaryGameBit = -1;
                childPlacement->objectDefinitionId = DFP_OBJECT_CREATOR_CHILD_DEFINITION;
                newObj = objSetupObject(&childPlacement->base, 5, obj->anim.mapEventSlot, -1, obj->anim.parent);
                newObj->userData1 = placement->rotationXAndChildValue;
            }
            break;
        }
    }
}

void DFP_ObjCreator_init(GameObject* obj, const DfpObjectCreatorPlacement* placement)
{
    DfpObjectCreatorState* state = obj->extra;
    obj->anim.rotX = placement->rotationXAndChildValue * 256;
    state->gameBit = ObjAnim_ReadPlacementS16(&obj->anim, &placement->gameBit);
    state->spawnPeriod = ObjAnim_ReadPlacementS16(&obj->anim, &placement->spawnPeriod);
    state->spawnTimer = state->spawnPeriod;
    state->parameter12 = placement->parameter12;
    state->parameter14 = placement->parameter14 * 2;
    state->parameter16 = 100;
}

void DFP_ObjCreator_release(void)
{
}

void DFP_ObjCreator_initialise(void)
{
}

ObjectDescriptor gDFP_ObjCreatorObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    (ObjectDescriptorCallback)DFP_ObjCreator_initialise,
    (ObjectDescriptorCallback)DFP_ObjCreator_release,
    0,
    (ObjectDescriptorCallback)DFP_ObjCreator_init,
    (ObjectDescriptorCallback)DFP_ObjCreator_update,
    (ObjectDescriptorCallback)DFP_ObjCreator_hitDetect,
    (ObjectDescriptorCallback)DFP_ObjCreator_render,
    (ObjectDescriptorCallback)DFP_ObjCreator_free,
    (ObjectDescriptorCallback)DFP_ObjCreator_getObjectTypeId,
    DFP_ObjCreator_getExtraSize,
};
