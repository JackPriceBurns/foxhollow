/*
 * DLL 0x1F5 (slot 501) provides shared trigger-sequence behavior for several
 * object definitions.
 */
#include "dlls/objects/501.h"

#include "main/dll/dll_0004_dummy04.h"
#include "main/dll/objfx.h"
#include "main/model_light.h"
#include "main/obj_list.h"
#include "main/object_render.h"
#include "sys/objects/lifecycle.h"

#include "main/frame_timing.h"
#include "main/objseq.h"
#define DLL1F5_OBJECT_TYPE_ID           0xB
#define DLL1F5_FIRE_SEQ_ID              0x171
#define DLL1F5_SEQ_INDEX_PENDING        -2
#define DLL1F5_SEQUENCE_OBJECT_CLASS_ID 0x10
#define DLL1F5_TRAIL_MODE               4
#define DLL1F5_TRAIL_EFFECT_ID          389
#define DLL1F5_TRAIL_EFFECT_PARAM       5


typedef struct Dll1F5TrackedState {
    f32 value;
    u8 flag;
    u8 unknown05[3];
} Dll1F5TrackedState;

STATIC_ASSERT(offsetof(Dll1F5TrackedState, value) == 0x00);
STATIC_ASSERT(offsetof(Dll1F5TrackedState, flag) == 0x04);
STATIC_ASSERT(offsetof(Dll1F5TrackedState, unknown05) == 0x05);
STATIC_ASSERT(sizeof(Dll1F5TrackedState) == 0x08);

Dll1F5TrackedState gDll1F5TrackedState;

static void dll501_resetTrackedState(void) {
    gDll1F5TrackedState.value = 0.0f;
    gDll1F5TrackedState.flag = 0;
}

int dll501_getExtraSize(void) {
    return sizeof(Dll1F5State);
}

int dll501_getObjectTypeId(void) {
    return DLL1F5_OBJECT_TYPE_ID;
}

void dll501_free(GameObject* obj) {
    Dll1F5State* state = obj->extra;
    int light;

    (*gObjectTriggerInterface)->freeState((u8*)state);
    gTitleMenuControlInterfaceCopy->vtable->func05(obj, 0xffff, 0, 0, 0);
    light = obj->userData2;
    if (light != 0) {
        ModelLightStruct_free((ModelLightStruct*)light);
    }
}

void dll501_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 unusedVisible) {
    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    if (obj->anim.romDefNo == DLL1F5_FIRE_SEQ_ID) {
        objfx_spawnFlaggedTrailBurst(obj, 0.11f, DLL1F5_TRAIL_MODE, DLL1F5_TRAIL_EFFECT_ID, DLL1F5_TRAIL_EFFECT_PARAM,
                                     NULL);
    }
}

void dll501_hitDetect(void) {
}

void dll501_update(GameObject* obj) {
    int groupId;
    GameObject** objects;
    int i;
    int objectCount;
    GameObject* current;
    GameObject* linkedObject;
    int groupId2;
    int sameGroupCount;

    if (obj->anim.placementData == NULL) {
        return;
    }
    if (((Dll1F5PlacementView*)obj->anim.placementData)->segmentIndex == -1) {
        return;
    }

    i = (*gObjectTriggerInterface)->update((u8*)obj, framesThisStepUnclamped);
    if (i == 0) {
        return;
    }
    if (obj->seqIndex != DLL1F5_SEQ_INDEX_PENDING) {
        return;
    }

    groupId = ((Dll1F5State*)obj->extra)->sequence.slot;
    linkedObject = 0;
    objects = ObjList_GetObjects(&i, &objectCount);
    sameGroupCount = 0;
    i = 0;
    groupId2 = groupId;
    groupId2 |= groupId;
    while (i < objectCount) {
        current = objects[i];
        if (current->seqIndex == groupId) {
            linkedObject = current;
        }
        if (current->seqIndex == DLL1F5_SEQ_INDEX_PENDING &&
            current->anim.classId == DLL1F5_SEQUENCE_OBJECT_CLASS_ID &&
            groupId2 == ((Dll1F5State*)current->extra)->sequence.slot) {
            sameGroupCount++;
        }
        i++;
    }

    if (sameGroupCount <= 1 && (void*)linkedObject != NULL && linkedObject->seqIndex != -1) {
        linkedObject->seqIndex = -1;
        (*gObjectTriggerInterface)->endSequence(groupId2);
    }
    obj->seqIndex = -1;
    Obj_FreeObject(obj);
}

void dll501_init(GameObject* obj, Dll1F5PlacementView* placement) {
    Dll1F5State* state;
    int light;
    int chainIndex;

    state = obj->extra;
    state->sequence.gameBit = placement->gameBit;
    state->sequence.flags = -1;
    state->sequence.posOffsetDecay = 1.0f / (1.0f + (f32)placement->dampingDivisor);
    state->sequence.curveId = -1;

    chainIndex = obj->userData1;
    if (chainIndex == 0 && placement->segmentIndex != 1) {
        (*gObjectTriggerInterface)->loadAnimData((u8*)state, (u8*)placement);
        obj->userData1 = placement->segmentIndex + 1;
    } else if (chainIndex != 0 && placement->segmentIndex != chainIndex - 1) {
        (*gObjectTriggerInterface)->freeState((u8*)state);
        if (placement->segmentIndex != -1) {
            (*gObjectTriggerInterface)->loadAnimData((u8*)state, (u8*)placement);
        }
        obj->userData1 = placement->segmentIndex + 1;
    }

    if (obj->anim.romDefNo == DLL1F5_FIRE_SEQ_ID) {
        light = (int)objCreateLight(obj, 1);
        if ((u32)light != 0) {
            modelLightStruct_setLightKind((ModelLightStruct*)light, MODEL_LIGHT_KIND_POINT);
            modelLightStruct_setDiffuseColor((ModelLightStruct*)light, 200, 60, 0, 0);
            modelLightStruct_setDistanceAttenuation((ModelLightStruct*)light, 30.0f, 80.0f);
        }
        obj->userData2 = light;
    }

    dll501_resetTrackedState();
}

void dll501_release(void) {
}

void dll501_initialise(void) {
}

ObjectDescriptor gDll1F5ObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    dll501_initialise,
    dll501_release,
    0,
    (ObjectDescriptorCallback)dll501_init,
    (ObjectDescriptorCallback)dll501_update,
    dll501_hitDetect,
    (ObjectDescriptorCallback)dll501_render,
    (ObjectDescriptorCallback)dll501_free,
    (ObjectDescriptorCallback)dll501_getObjectTypeId,
    dll501_getExtraSize,
};
