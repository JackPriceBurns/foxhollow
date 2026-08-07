/* WM_seqobjec (DLL 0x01FA) */
#include "dlls/objects/506_WM_seqobjec.h"

#include "dlls/objects/504_WM_Galleon.h"
#include "game/objects/object.h"
#include "main/gamebits.h"
#include "main/objtype.h"
#include "main/objseq.h"
#include "main/object_render.h"
#include "main/screen_transition.h"

#define WM_SEQOBJECT_EVENT_TOGGLE         1
#define WM_SEQOBJECT_MODE_GALLEON         0
#define WM_SEQOBJECT_MODE_DISABLED        8
#define WM_SEQOBJECT_TRANSITION_DURATION  0x50
#define WM_SEQOBJECT_TRANSITION_COUNTDOWN 0x14

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
    return sizeof(WMSeqObjectState);
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
    int objectCount;
    int transitionCountdown;
    GameObject** objects;
    int galleonFound;
    int objectIndex;
    int mode;
    const WMSeqObjectPlacementView* placement;

    placement = (const WMSeqObjectPlacementView*)obj->anim.placementData;
    mode = placement->mode;
    switch (mode) {
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

        objects = (GameObject**)objGetAllOfType(6, &objectCount);
        galleonFound = 0;
        for (objectIndex = 0; objectIndex < objectCount; objectIndex++) {
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
                (*gScreenTransitionInterface)->step(WM_SEQOBJECT_TRANSITION_DURATION, SCREEN_TRANSITION_BLACK);
            }
        } else {
            obj->userData2 = WM_SEQOBJECT_TRANSITION_COUNTDOWN;
            (*gScreenTransitionInterface)->step(WM_SEQOBJECT_TRANSITION_DURATION, SCREEN_TRANSITION_BLACK);
        }

        transitionCountdown = obj->userData2 - 1;
        obj->userData2 = transitionCountdown;
        if (transitionCountdown < 0) {
            obj->userData2 = 0;
        }
        break;
    }
}

void WM_seqobject_init(GameObject* obj, const WMSeqObjectPlacementView* placement) {
    s16 angle;

    angle = (s16)((s32)placement->rotationXByte << 8);
    obj->anim.rotX = angle;
    obj->animEventCallback = WM_seqobject_SeqFn;
    obj->userData2 = WM_SEQOBJECT_TRANSITION_COUNTDOWN;
}

void WM_seqobject_release(void) {
}

void WM_seqobject_initialise(void) {
}

ObjectDescriptor gWM_seqobjectObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    WM_seqobject_initialise,
    WM_seqobject_release,
    0,
    (ObjectDescriptorCallback)WM_seqobject_init,
    (ObjectDescriptorCallback)WM_seqobject_update,
    WM_seqobject_hitDetect,
    (ObjectDescriptorCallback)WM_seqobject_render,
    WM_seqobject_free,
    (ObjectDescriptorCallback)WM_seqobject_getObjectTypeId,
    WM_seqobject_getExtraSize,
};
