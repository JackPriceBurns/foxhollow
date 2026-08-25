/*
 * MMSH_Scales (DLL 0x18D) advances object-trigger sequences and spawns the
 * scalessword child used by the Moon Mountain Pass sequence.
 */
#include "dlls/objects/397_MMSH_Scales.h"

#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/dll/dll_0004_dummy04.h"
#include "main/frame_timing.h"
#include "main/obj_list.h"
#include "main/object_render.h"
#include "main/objseq.h"
#include "main/objtype.h"
#include "sys/objects.h"
#include "sys/objects/lifecycle.h"

typedef struct MmshScalesPlacement {
    ObjPlacement base;
    s16 animDataIndex;
    s16 sequenceGameBit;
    u8 pad1C[8];
    u8 positionDamping;
} MmshScalesPlacement;

STATIC_ASSERT(offsetof(MmshScalesPlacement, animDataIndex) == 0x18);
STATIC_ASSERT(offsetof(MmshScalesPlacement, sequenceGameBit) == 0x1A);
STATIC_ASSERT(offsetof(MmshScalesPlacement, positionDamping) == 0x24);

typedef struct MmshScalesState {
    ObjSeqState sequence;
    u8 trailingState[8];
} MmshScalesState;

STATIC_ASSERT(sizeof(MmshScalesState) == 0x168);
STATIC_ASSERT(offsetof(MmshScalesState, trailingState) == 0x160);

typedef struct MmshScalesChildSetup {
    ObjPlacement base;
    u8 pad18[12];
} MmshScalesChildSetup;

STATIC_ASSERT(sizeof(MmshScalesChildSetup) == 0x24);

static int mmshScales_getExtraSize(void) {
    return sizeof(MmshScalesState);
}

static int mmshScales_getObjectTypeId(void) {
    return 0xB;
}

static void mmshScales_free(GameObject* obj, int keepChild) {
    GameObject* child;

    (*gObjectTriggerInterface)->freeState(obj->extra);
    gTitleMenuControlInterfaceCopy->vtable->func05(obj, 0xffff, 0, 0, 0);
    child = obj->childObjs[0];
    if (child != NULL && keepChild == 0) {
        Obj_FreeObject(child);
    }
}

static void mmshScales_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                              s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

static void mmshScales_hitDetect(void) {
}

static void mmshScales_update(GameObject* obj) {
    const MmshScalesPlacement* placement = (const MmshScalesPlacement*)obj->anim.placementData;
    MmshScalesState* state = obj->extra;
    GameObject** objects;
    GameObject* sequenceOwner;
    int sequenceResult;
    int sequenceSlot;
    int siblingCount;
    int firstObjectIndex;
    int objectCount;

    if (placement != NULL && ObjAnim_ReadPlacementS16(&obj->anim, &placement->animDataIndex) != -1) {
        sequenceResult = (*gObjectTriggerInterface)->update((u8*)obj, (f32)(u32)framesThisStepUnclamped);
        if (sequenceResult != 0 && obj->seqIndex == OBJECT_SEQUENCE_INDEX_PENDING) {
            sequenceSlot = state->sequence.slot;
            sequenceOwner = NULL;
            objects = ObjList_GetObjects(&firstObjectIndex, &objectCount);
            siblingCount = 0;
            for (int objectIndex = firstObjectIndex; objectIndex < objectCount; objectIndex++) {
                GameObject* other = objects[objectIndex];

                if (other->seqIndex == sequenceSlot) {
                    sequenceOwner = other;
                }
                if (other->seqIndex == OBJECT_SEQUENCE_INDEX_PENDING &&
                    other->anim.classId == OBJECT_CLASS_SEQUENCE &&
                    sequenceSlot == ((MmshScalesState*)other->extra)->sequence.slot) {
                    siblingCount++;
                }
            }
            if (siblingCount <= 1 && sequenceOwner != NULL &&
                sequenceOwner->seqIndex != OBJECT_SEQUENCE_INDEX_NONE) {
                sequenceOwner->seqIndex = OBJECT_SEQUENCE_INDEX_NONE;
                (*gObjectTriggerInterface)->endSequence(sequenceSlot);
            }
            obj->seqIndex = OBJECT_SEQUENCE_INDEX_NONE;
            Obj_FreeObject(obj);
        }
    }
}

static void mmshScales_init(GameObject* obj, const MmshScalesPlacement* placement) {
    MmshScalesState* state = obj->extra;
    MmshScalesChildSetup* childSetup;
    intptr_t cachedAnimDataIndexPlusOne;
    s16 animDataIndex;

    state->sequence.gameBit = ObjAnim_ReadPlacementS16(&obj->anim, &placement->sequenceGameBit);
    state->sequence.flags = -1;
    state->sequence.posOffsetDecay = 1.0f / (1.0f + placement->positionDamping);
    state->sequence.curveId = -1;
    animDataIndex = ObjAnim_ReadPlacementS16(&obj->anim, &placement->animDataIndex);
    cachedAnimDataIndexPlusOne = obj->userData1;
    if (cachedAnimDataIndexPlusOne == 0 && animDataIndex != 1) {
        (*gObjectTriggerInterface)->loadAnimData((u8*)state, (u8*)placement, &obj->anim);
        obj->userData1 = animDataIndex + 1;
    } else if (cachedAnimDataIndexPlusOne != 0 && animDataIndex != cachedAnimDataIndexPlusOne - 1) {
        (*gObjectTriggerInterface)->freeState((u8*)state);
        if (animDataIndex != -1) {
            (*gObjectTriggerInterface)->loadAnimData((u8*)state, (u8*)placement, &obj->anim);
        }
        obj->userData1 = animDataIndex + 1;
    }
    if (Obj_IsLoadingLocked() == 0) {
        return;
    }
    childSetup = (MmshScalesChildSetup*)Obj_AllocObjectSetup(sizeof(MmshScalesChildSetup), 0x1B8);
    childSetup->base.posX = obj->anim.localPosX;
    childSetup->base.posY = obj->anim.localPosY;
    childSetup->base.posZ = obj->anim.localPosZ;
    childSetup->base.color[0] = 0x20;
    childSetup->base.color[1] = 0x04;
    childSetup->base.color[3] = 0xFF;
    obj->childObjs[0] = objSetupObject(&childSetup->base, 5, -1, -1, NULL);
    obj->childObjs[0]->anim.rootMotionScale *= 2.0f;
}

static void mmshScales_release(void) {
}

static void mmshScales_initialise(void) {
}

OBJECT_INIT_ADAPTER(gMMSHScalesObjDescriptorInitAdapter, mmshScales_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gMMSHScalesObjDescriptorHitDetectAdapter, mmshScales_hitDetect)
OBJECT_TYPE_ID_ADAPTER(gMMSHScalesObjDescriptorTypeIdAdapter, mmshScales_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gMMSHScalesObjDescriptorExtraSizeAdapter, mmshScales_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gMMSHScalesObjDescriptorAcquire, mmshScales_initialise)

ObjectDescriptor gMMSHScalesObjDescriptor = {
    .header = {
        .metadata = { 0, 0, 0, OBJECT_DESCRIPTOR_FLAGS_10_SLOTS },
        .acquire = gMMSHScalesObjDescriptorAcquire,
        .release = mmshScales_release,
    },
    .init = gMMSHScalesObjDescriptorInitAdapter,
    .update = mmshScales_update,
    .hitDetect = gMMSHScalesObjDescriptorHitDetectAdapter,
    .render = mmshScales_render,
    .free = mmshScales_free,
    .getObjectTypeId = gMMSHScalesObjDescriptorTypeIdAdapter,
    .getExtraSize = gMMSHScalesObjDescriptorExtraSizeAdapter,
};;
