/* WM_seqobjec (DLL 0x01FA) */
#include "dlls/objects/506_WM_seqobjec.h"

#include "dlls/objects/504_WM_Galleon.h"
#include "game/objects/object.h"
#include "main/gamebits.h"
#include "main/objtype.h"
#include "main/objseq.h"
#include "main/object_render.h"
#include "main/screen_transition.h"

typedef enum WMSeqObjectEvent {
    WM_SEQOBJECT_EVENT_TOGGLE = 1,
} WMSeqObjectEvent;

typedef enum WMSeqObjectMode {
    WM_SEQOBJECT_MODE_GALLEON = 0,
    WM_SEQOBJECT_MODE_DISABLED = 8,
} WMSeqObjectMode;

u8 gWMSeqObjectToggleState;

int WM_seqobject_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate) {
    int eventIndex;

    (void)obj;
    (void)unused;
    for (eventIndex = 0; eventIndex < animUpdate->eventCount; eventIndex++) {
        if (animUpdate->eventIds[eventIndex] == WM_SEQOBJECT_EVENT_TOGGLE) {
            gWMSeqObjectToggleState = 1 - gWMSeqObjectToggleState;
        }
    }
    animUpdate->curEventId = 0;
    animUpdate->movementState = 0;
    return 0;
}

int WM_seqobject_getExtraSize(void) {
    return sizeof(u8);
}

int WM_seqobject_getObjectTypeId(void) {
    return 0;
}

void WM_seqobject_free(void) {
}

void WM_seqobject_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

void WM_seqobject_hitDetect(void) {
}

void WM_seqobject_update(GameObject* obj) {
    const WMSeqObjectPlacement* placement = (const WMSeqObjectPlacement*)obj->anim.placementData;

    switch (placement->mode) {
    case WM_SEQOBJECT_MODE_DISABLED:
        break;
    case WM_SEQOBJECT_MODE_GALLEON:
        if (obj->userData1 != 0) {
            return;
        }
        if (mainGetBit(GAMEBIT_WM_GalleonRelated00A4) != 0) {
            return;
        }
        if (mainGetBit(GAMEBIT_WM_Galleon_despawn) != 0) {
            return;
        }

        int objectCount;
        GameObject** objects = (GameObject**)objGetAllOfType(6, &objectCount);
        int galleonFound = 0;
        for (int objectIndex = 0; objectIndex < objectCount; objectIndex++) {
            if (objects[objectIndex]->anim.romDefNo == WM_GALLEON_OBJECT_ID) {
                galleonFound = 1;
            }
        }

        if (galleonFound != 0) {
            if (obj->userData2 == 0) {
                (*gObjectTriggerInterface)->runSequence(0, obj, -1);
                obj->userData1 = 1;
                mainSetBits(GAMEBIT_WM_GalleonRelated00A4, 1);
            } else {
                (*gScreenTransitionInterface)->step(0x50, SCREEN_TRANSITION_BLACK);
            }
        } else {
            obj->userData2 = 0x14;
            (*gScreenTransitionInterface)->step(0x50, SCREEN_TRANSITION_BLACK);
        }

        obj->userData2--;
        if (obj->userData2 < 0) {
            obj->userData2 = 0;
        }
        break;
    }
}

void WM_seqobject_init(GameObject* obj, const WMSeqObjectPlacement* placement) {
    obj->anim.rotX = (s16)((s32)placement->rotationXByte << 8);
    obj->animEventCallback = WM_seqobject_SeqFn;
    obj->userData2 = 0x14;
}

void WM_seqobject_release(void) {
}

void WM_seqobject_initialise(void) {
}

OBJECT_INIT_ADAPTER(gWM_seqobjectObjDescriptorInitAdapter, WM_seqobject_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gWM_seqobjectObjDescriptorHitDetectAdapter, WM_seqobject_hitDetect)
OBJECT_FREE_ADAPTER(gWM_seqobjectObjDescriptorFreeAdapter, WM_seqobject_free)
OBJECT_TYPE_ID_ADAPTER(gWM_seqobjectObjDescriptorTypeIdAdapter, WM_seqobject_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gWM_seqobjectObjDescriptorExtraSizeAdapter, WM_seqobject_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gWM_seqobjectObjDescriptorAcquire, WM_seqobject_initialise)

ObjectDescriptor gWM_seqobjectObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gWM_seqobjectObjDescriptorAcquire,
        WM_seqobject_release,
    },
    0,
    gWM_seqobjectObjDescriptorInitAdapter,
    WM_seqobject_update,
    gWM_seqobjectObjDescriptorHitDetectAdapter,
    WM_seqobject_render,
    gWM_seqobjectObjDescriptorFreeAdapter,
    gWM_seqobjectObjDescriptorTypeIdAdapter,
    gWM_seqobjectObjDescriptorExtraSizeAdapter,
};
