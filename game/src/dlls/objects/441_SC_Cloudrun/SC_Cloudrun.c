#include "dlls/objects/441_SC_Cloudrun.h"

#include "game/objects/object_setup.h"
#include "main/audio/sfx.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/dll/dll_0004_dummy04.h"
#include "main/dll/dll_02B1_cmbsrc.h"
#include "main/frame_timing.h"
#include "main/obj_link.h"
#include "main/obj_list.h"
#include "main/object_render.h"
#include "main/objseq.h"
#include "main/objtype.h"
#include "sys/objects.h"
#include "sys/objects/lifecycle.h"

enum ScCloudrunnerAEvent {
    SC_CLOUDRUNNER_A_EVENT_CREATE_CHILD = 0,
    SC_CLOUDRUNNER_A_EVENT_DEACTIVATE_CHILD = 1,
    SC_CLOUDRUNNER_A_EVENT_REMOVE_CHILD = 2,
};

enum ScCloudrunnerAAnimDataIndex {
    SC_CLOUDRUNNER_A_ANIM_DATA_NONE = -1,
    SC_CLOUDRUNNER_A_DEFAULT_ANIM_DATA_INDEX = 1,
};

typedef struct ScCloudrunnerAPlacement {
    ObjPlacement base;
    s16 animDataIndex;
    s16 sequenceGameBit;
    u8 unused1C[8];
    u8 positionDamping;
} ScCloudrunnerAPlacement;

typedef struct ScCloudrunnerAState {
    ObjSeqState sequence;
    u8 trailingState[8];
} ScCloudrunnerAState;

STATIC_ASSERT(sizeof(ScCloudrunnerAPlacement) == 0x28);
STATIC_ASSERT(offsetof(ScCloudrunnerAPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(ScCloudrunnerAPlacement, animDataIndex) == 0x18);
STATIC_ASSERT(offsetof(ScCloudrunnerAPlacement, sequenceGameBit) == 0x1A);
STATIC_ASSERT(offsetof(ScCloudrunnerAPlacement, unused1C) == 0x1C);
STATIC_ASSERT(offsetof(ScCloudrunnerAPlacement, positionDamping) == 0x24);

STATIC_ASSERT(sizeof(ScCloudrunnerAState) == 0x168);
STATIC_ASSERT(offsetof(ScCloudrunnerAState, sequence) == 0x000);
STATIC_ASSERT(offsetof(ScCloudrunnerAState, trailingState) == 0x160);

static int sc_cloudrunnera_getExtraSize(void) {
    return sizeof(ScCloudrunnerAState);
}

static int sc_cloudrunnera_getObjectTypeId(void) {
    return 0xB;
}

static void sc_cloudrunnera_free(GameObject* obj) {
    ScCloudrunnerAState* state = obj->extra;

    (*gObjectTriggerInterface)->freeState((u8*)&state->sequence);
    gTitleMenuControlInterfaceCopy->vtable->func05(obj, 0xFFFF, 0, 0, 0);
}

static void sc_cloudrunnera_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                                   s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

static void sc_cloudrunnera_hitDetect(void) {
}

static void sc_cloudrunnera_update(GameObject* obj) {
    const ScCloudrunnerAPlacement* placement = (const ScCloudrunnerAPlacement*)obj->anim.placementData;
    ScCloudrunnerAState* state = obj->extra;
    ObjSeqState* sequence = &state->sequence;

    if (placement == NULL) {
        return;
    }
    if (ObjAnim_ReadPlacementS16(&obj->anim, &placement->animDataIndex) == SC_CLOUDRUNNER_A_ANIM_DATA_NONE) {
        return;
    }
    int sequenceResult = (*gObjectTriggerInterface)->update((u8*)obj, (f32)(u32)framesThisStepUnclamped);
    if (sequenceResult != 0 && obj->seqIndex == OBJECT_SEQUENCE_INDEX_PENDING) {
        int firstObjectIndex;
        int objectCount;
        GameObject** objects = ObjList_GetObjects(&firstObjectIndex, &objectCount);
        GameObject* sequenceOwner = NULL;
        int sequenceSlot = sequence->slot;
        int participantCount = 0;

        for (int objectIndex = firstObjectIndex; objectIndex < objectCount; objectIndex++) {
            GameObject* otherObject = objects[objectIndex];
            s16 sequenceIndex = otherObject->seqIndex;

            if (sequenceIndex == sequenceSlot) {
                sequenceOwner = otherObject;
            }
            if (sequenceIndex == OBJECT_SEQUENCE_INDEX_PENDING &&
                otherObject->anim.classId == OBJECT_CLASS_SEQUENCE) {
                sequence = otherObject->extra;
                if (sequenceSlot == sequence->slot) {
                    participantCount++;
                }
            }
        }
        if (participantCount <= 1 && sequenceOwner != NULL &&
            sequenceOwner->seqIndex != OBJECT_SEQUENCE_INDEX_NONE) {
            sequenceOwner->seqIndex = OBJECT_SEQUENCE_INDEX_NONE;
            (*gObjectTriggerInterface)->endSequence(sequenceSlot);
        }
        obj->seqIndex = OBJECT_SEQUENCE_INDEX_NONE;
    }

    for (int eventIndex = 0; eventIndex < sequence->eventCount; eventIndex++) {
        switch (sequence->eventIds[eventIndex]) {
        case SC_CLOUDRUNNER_A_EVENT_CREATE_CHILD: {
            CmbSrcMapData* setup;
            GameObject* child;

            if (obj->childObjs[0] != NULL) {
                break;
            }
            if (Obj_IsLoadingLocked() == 0) {
                break;
            }
            setup = (CmbSrcMapData*)Obj_AllocObjectSetup(sizeof(*setup), CMBSRC_SEQ_DEFAULT);
            setup->colorIndex = 0x9;
            setup->effectMode = 0;
            setup->pulseSubMode = 0;
            setup->radius = 1.0f;
            setup->colorDistance = 0xff;
            setup->effectDistance = 0xff;
            setup->pulseDistance = 0xff;
            setup->gameBit = -1;
            setup->base.color[0] = 2;
            setup->base.color[1] = 1;
            setup->base.color[2] = 0xff;
            setup->base.color[3] = 0xff;
            setup->flags = CMBSRC_MAP_START_ACTIVE;
            setup->behaviorFlags = 0;
            child = objSetupObject(&setup->base, 5, obj->anim.mapEventSlot, -1, obj->anim.parent);
            child->anim.flags |= OBJANIM_FLAG_HIDDEN;
            ObjLink_AttachChild(obj, child, 0);
            Sfx_PlayFromObject(obj, SFXTRIG_en_cvdrip1c);
            break;
        }
        case SC_CLOUDRUNNER_A_EVENT_DEACTIVATE_CHILD: {
            if (obj->childObjs[0] != NULL) {
                cmbsrc_setExternalActive(obj->childObjs[0], 0);
            }
            break;
        }
        case SC_CLOUDRUNNER_A_EVENT_REMOVE_CHILD: {
            GameObject* child = obj->childObjs[0];

            if (child != NULL) {
                ObjLink_DetachChild(obj, child);
                Obj_FreeObject(child);
            }
            break;
        }
        }
    }
    {
        GameObject* child = obj->childObjs[0];

        if (child != NULL) {
            child->anim.rotZ = obj->anim.rotZ;
            child->anim.rotY = (s16)(obj->anim.rotY + 0xE38);
            child->anim.rotX = (s16)(obj->anim.rotX - 0x8000);
        }
    }
}

static void sc_cloudrunnera_init(GameObject* obj, const ScCloudrunnerAPlacement* placement) {
    ScCloudrunnerAState* state = obj->extra;
    ObjSeqState* sequence = &state->sequence;
    s16 animDataIndex = ObjAnim_ReadPlacementS16(&obj->anim, &placement->animDataIndex);
    intptr_t cachedAnimDataIndexPlusOne;

    objSetSlot(obj, 0x64);
    sequence->gameBit = ObjAnim_ReadPlacementS16(&obj->anim, &placement->sequenceGameBit);
    sequence->flags = -1;
    sequence->posOffsetDecay = 1.0f / (1.0f + (f32)(u32)placement->positionDamping);
    sequence->curveId = -1;
    obj->userData2 = 0;

    cachedAnimDataIndexPlusOne = obj->userData1;
    if (cachedAnimDataIndexPlusOne == 0 && animDataIndex != SC_CLOUDRUNNER_A_DEFAULT_ANIM_DATA_INDEX) {
        (*gObjectTriggerInterface)->loadAnimData((u8*)sequence, (u8*)placement, &obj->anim);
        obj->userData1 = animDataIndex + 1;
    } else if (cachedAnimDataIndexPlusOne != 0 && animDataIndex != cachedAnimDataIndexPlusOne - 1) {
        (*gObjectTriggerInterface)->freeState((u8*)sequence);
        if (animDataIndex != SC_CLOUDRUNNER_A_ANIM_DATA_NONE) {
            (*gObjectTriggerInterface)->loadAnimData((u8*)sequence, (u8*)placement, &obj->anim);
        }
        obj->userData1 = animDataIndex + 1;
    }
    if (obj->anim.modelState != NULL) {
        obj->anim.modelState->shadowTintA = 0x64;
        obj->anim.modelState->shadowTintB = 0x96;
    }
}

static void sc_cloudrunnera_release(void) {
}

static void sc_cloudrunnera_initialise(void) {
}

OBJECT_INIT_ADAPTER(gSC_CloudrunnerAObjDescriptorInitAdapter, sc_cloudrunnera_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gSC_CloudrunnerAObjDescriptorHitDetectAdapter, sc_cloudrunnera_hitDetect)
OBJECT_FREE_ADAPTER(gSC_CloudrunnerAObjDescriptorFreeAdapter, sc_cloudrunnera_free, obj)
OBJECT_TYPE_ID_ADAPTER(gSC_CloudrunnerAObjDescriptorTypeIdAdapter, sc_cloudrunnera_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gSC_CloudrunnerAObjDescriptorExtraSizeAdapter, sc_cloudrunnera_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gSC_CloudrunnerAObjDescriptorAcquire, sc_cloudrunnera_initialise)

ObjectDescriptor gSC_CloudrunnerAObjDescriptor = {
    .header = {
        .metadata = { 0, 0, 0, OBJECT_DESCRIPTOR_FLAGS_10_SLOTS },
        .acquire = gSC_CloudrunnerAObjDescriptorAcquire,
        .release = sc_cloudrunnera_release,
    },
    .init = gSC_CloudrunnerAObjDescriptorInitAdapter,
    .update = sc_cloudrunnera_update,
    .hitDetect = gSC_CloudrunnerAObjDescriptorHitDetectAdapter,
    .render = sc_cloudrunnera_render,
    .free = gSC_CloudrunnerAObjDescriptorFreeAdapter,
    .getObjectTypeId = gSC_CloudrunnerAObjDescriptorTypeIdAdapter,
    .getExtraSize = gSC_CloudrunnerAObjDescriptorExtraSizeAdapter,
};;
