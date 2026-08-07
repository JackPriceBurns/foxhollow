/*
 * Object DLL 0x184 advances animation sequences and manages child objects
 * requested by sequence events.
 */
#include "dlls/objects/388.h"

#include "main/dll/dll_0004_dummy04.h"
#include "main/frame_timing.h"
#include "main/object_render.h"
#include "sys/objects/lifecycle.h"
#include "main/audio/sfx_stop_channel_api.h"
#include "main/obj_link.h"
#include "main/obj_list.h"
#include "sys/objects.h"
#include "main/objseq.h"

#define DLL_184_ANIM_EVENT_CREATE_CHILD  1
#define DLL_184_ANIM_EVENT_REMOVE_CHILD  2
#define DLL_184_CHILD_SETUP_ID           0x30B
#define DLL_184_OBJECT_TYPE_ID           0xB
#define DLL_184_CLASS_ID                 0x10
#define DLL_184_OBJECT_SLOT              0x64
#define DLL_184_SHADOW_TINT_A            0x64
#define DLL_184_SHADOW_TINT_B            0x96
#define DLL_184_OBJECT_SOUND_CHANNEL_ALL 0x7F

int dll_184_handleAnimEvents(GameObject* obj, const ObjSeqState* sequence) {
    int i;
    GameObject* child;
    Dll184ChildSetup* childSetup;

    for (i = 0; i < sequence->eventCount; i++) {
        u8 eventId = sequence->eventIds[i];

        switch (eventId) {
        case DLL_184_ANIM_EVENT_CREATE_CHILD:
            obj->userData2 = DLL_184_CHILD_SETUP_ID;
            child = obj->childObjs[0];
            if (child != NULL) {
                ObjLink_DetachChild(obj, child);
                Obj_FreeObject(child);
            }
            childSetup = (Dll184ChildSetup*)Obj_AllocObjectSetup(sizeof(Dll184ChildSetup), obj->userData2);
            child = objSetupObject(&childSetup->base, 4, obj->anim.mapEventSlot, -1, obj->anim.parent);
            ObjLink_AttachChild(obj, child, 0);
            break;
        case DLL_184_ANIM_EVENT_REMOVE_CHILD:
            child = obj->childObjs[0];
            if (child != NULL) {
                ObjLink_DetachChild(obj, child);
                Obj_FreeObject(child);
            }
            obj->userData2 = -1;
            break;
        }
    }
    return 0;
}

int dll_184_getExtraSize(void) {
    return sizeof(Dll184State);
}

int dll_184_getObjectTypeId(void) {
    return DLL_184_OBJECT_TYPE_ID;
}

void dll_184_free(GameObject* obj) {
    Dll184State* state;
    GameObject* child;

    state = obj->extra;
    child = obj->childObjs[0];
    if (child != NULL) {
        ObjLink_DetachChild(obj, child);
        Obj_FreeObject(child);
    }
    (*gObjectTriggerInterface)->freeState((u8*)state);
    gTitleMenuControlInterfaceCopy->vtable->func05((void*)obj, 0xffff, 0, 0, 0);
    Sfx_StopObjectChannel(obj, DLL_184_OBJECT_SOUND_CHANNEL_ALL);
}

void dll_184_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    s32 isVisible = visible;

    if (isVisible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

void dll_184_hitDetect(void) {
}

void dll_184_update(GameObject* obj) {
    const Dll184Placement* placement;
    int slot;
    int sequenceSlotExt;
    int participantCount;
    GameObject** objectList;
    Dll184State* state;
    GameObject* sequenceObject;
    int i;
    int objectCount;

    state = obj->extra;
    placement = (const Dll184Placement*)obj->anim.placementData;
    if (placement != NULL && placement->animDataIndex != -1) {
        i = (*gObjectTriggerInterface)->update((u8*)obj, (f32)(u32)framesThisStep);
        dll_184_handleAnimEvents(obj, &state->sequence);
        if (i != 0 && obj->seqIndex == -2) {
            slot = state->sequence.slot;
            sequenceObject = NULL;
            objectList = ObjList_GetObjects(&i, &objectCount);
            participantCount = 0;
            for (i = 0, sequenceSlotExt = (int)(s8)slot; i < objectCount; i++) {
                GameObject* otherObject = *objectList;

                if (otherObject->seqIndex == slot) {
                    sequenceObject = otherObject;
                }
                if (otherObject->seqIndex == -2 && otherObject->anim.classId == DLL_184_CLASS_ID &&
                    sequenceSlotExt == ((Dll184State*)otherObject->extra)->sequence.slot) {
                    participantCount++;
                }
                objectList++;
            }
            if (participantCount <= 1 && sequenceObject != NULL && sequenceObject->seqIndex != -1) {
                sequenceObject->seqIndex = -1;
                (*gObjectTriggerInterface)->endSequence(sequenceSlotExt);
            }
            obj->seqIndex = -1;
        }
    }
}

void dll_184_init(GameObject* obj, const Dll184Placement* placement) {
    u8* sequenceData;
    int cachedAnimDataIndexPlusOne;
    Dll184State* state;

    obj->animEventCallback = NULL;
    objSetSlot(obj, DLL_184_OBJECT_SLOT);
    sequenceData = obj->extra;
    state = (Dll184State*)sequenceData;
    state->sequence.gameBit = placement->sequenceGameBit;
    state->sequence.flags = -1;
    state->sequence.posOffsetDecay = 1.0f / (1.0f + (f32)(u32)placement->positionDamping);
    state->sequence.curveId = -1;
    state->sequence.animEntries = NULL;
    state->sequence.cmds = NULL;
    obj->userData2 = -1;
    cachedAnimDataIndexPlusOne = obj->userData1;
    if (cachedAnimDataIndexPlusOne == 0 && placement->animDataIndex != 1) {
        (*gObjectTriggerInterface)->loadAnimData(sequenceData, (u8*)placement);
        obj->userData1 = placement->animDataIndex + 1;
    } else if (cachedAnimDataIndexPlusOne != 0 && placement->animDataIndex != cachedAnimDataIndexPlusOne - 1) {
        (*gObjectTriggerInterface)->freeState(sequenceData);
        if (placement->animDataIndex != -1) {
            (*gObjectTriggerInterface)->loadAnimData(sequenceData, (u8*)placement);
        }
        obj->userData1 = placement->animDataIndex + 1;
    }
    if (obj->anim.modelState != NULL) {
        obj->anim.modelState->shadowTintA = DLL_184_SHADOW_TINT_A;
        obj->anim.modelState->shadowTintB = DLL_184_SHADOW_TINT_B;
    }
}

void dll_184_release(void) {
}

void dll_184_initialise(void) {
}

ObjectDescriptor gDll184ObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    (ObjectDescriptorCallback)dll_184_initialise,
    (ObjectDescriptorCallback)dll_184_release,
    0,
    (ObjectDescriptorCallback)dll_184_init,
    (ObjectDescriptorCallback)dll_184_update,
    (ObjectDescriptorCallback)dll_184_hitDetect,
    (ObjectDescriptorCallback)dll_184_render,
    (ObjectDescriptorCallback)dll_184_free,
    (ObjectDescriptorCallback)dll_184_getObjectTypeId,
    dll_184_getExtraSize,
};
