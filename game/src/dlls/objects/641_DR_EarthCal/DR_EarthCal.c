/* DR_EarthCal (DLL 641) */
#include "main/dll/DR/dll_0281_drearthcal.h"
#include "main/dll/player.h"
#include "main/dll/tricky.h"
#include "main/obj_trigger.h"
#include "main/objfx.h"
#include "main/objtype.h"
#include "sys/objects.h"
#include "main/objseq.h"

/* Mount object group: query nearest mount to gate the interact prompt. */
#define DREARTHCAL_MOUNT_OBJGROUP 0xa

int drearthcal_func0A(void) {
    return 1;
}

int drearthcal_getExtraSize(void) {
    return 1;
}

int drearthcal_getObjectTypeId(void) {
    return 0;
}

void drearthcal_free(void) {
}

void drearthcal_render(void) {
}

void drearthcal_hitDetect(void) {
}

void drearthcal_update(GameObject* obj) {
    struct {
        f32 _pad[3];
        f32 vec[3];
    } part;

    GameObject* player = Obj_GetPlayerObject();
    f32 searchDist = 200.0f;
    if (playerGetFocusObject(player) != NULL) {
        obj->anim.resetHitboxFlags &= ~(INTERACT_FLAG_PROMPT_SUPPRESSED | INTERACT_FLAG_DISABLED);
        if ((obj->anim.resetHitboxFlags & INTERACT_FLAG_IN_RANGE) != 0) {
            setAButtonIcon(0x15);
        }

        if (ObjTrigger_IsSet(obj) != 0) {
            (*gObjectTriggerInterface)->runSequence(1, obj, -1);
        }
    } else {
        obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
        if (obj->anim.hitboxTransformState->contactObjectCount > 0) {
            for (int i = 0; i < obj->anim.hitboxTransformState->contactObjectCount; i++) {
                GameObject* elem = obj->anim.hitboxTransformState->contactObjects[i];
                if (elem == player) {
                    obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_DISABLED;
                }
            }
        }

        if (objGetNearestTypeTo(DREARTHCAL_MOUNT_OBJGROUP, obj, &searchDist) == 0) {
            obj->anim.resetHitboxFlags |= INTERACT_FLAG_PROMPT_SUPPRESSED;
        } else {
            obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_PROMPT_SUPPRESSED;
        }

        if ((obj->anim.resetHitboxFlags & INTERACT_FLAG_IN_RANGE) != 0) {
            setAButtonIcon(0x14);
        }

        if (ObjTrigger_IsSet(obj) != 0) {
            (*gObjectTriggerInterface)->runSequence(2, obj, -1);
        }
    }

    if ((obj->objectFlags & OBJECT_OBJFLAG_RENDERED) != 0) {
        part.vec[0] = 0.0f;
        part.vec[1] = 30.0f;
        part.vec[2] = 0.0f;
        objfx_spawnArcedBurst(obj, 5, 0.75f, 2, 2, 0xf, 18.0f, 18.0f, 2.0f, &part, 0);
    }
}

void drearthcal_init(GameObject* obj, DREarthCalSetup* setup) {
    obj->anim.rotX = setup->yaw << 8;
    obj->objectFlags |= OBJECT_OBJFLAG_HITDETECT_DISABLED | OBJECT_OBJFLAG_HIDDEN;
}

void drearthcal_release(void) {
}

void drearthcal_initialise(void) {
}

OBJECT_INIT_ADAPTER(gDrEarthCalObjDescriptorInitAdapter, drearthcal_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gDrEarthCalObjDescriptorHitDetectAdapter, drearthcal_hitDetect)
OBJECT_RENDER_ADAPTER(gDrEarthCalObjDescriptorRenderAdapter, drearthcal_render)
OBJECT_FREE_ADAPTER(gDrEarthCalObjDescriptorFreeAdapter, drearthcal_free)
OBJECT_TYPE_ID_ADAPTER(gDrEarthCalObjDescriptorTypeIdAdapter, drearthcal_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gDrEarthCalObjDescriptorExtraSizeAdapter, drearthcal_getExtraSize)

typedef struct DrEarthCalObjDescriptorTypeInterface {
    OBJECT_INTERFACE_FIELDS;
    __typeof__(drearthcal_func0A)* drearthcal_func0A;
} DrEarthCalObjDescriptorTypeInterface;

struct DrEarthCalObjDescriptorType {
    ObjectDescriptorHeader header;
    DrEarthCalObjDescriptorTypeInterface interface;
};

RESOURCE_ACQUIRE_ADAPTER(gDrEarthCalObjDescriptorAcquire, drearthcal_initialise)

struct DrEarthCalObjDescriptorType gDrEarthCalObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_11_SLOTS,
        },
        gDrEarthCalObjDescriptorAcquire,
        drearthcal_release,
    },
    {
        0,
        gDrEarthCalObjDescriptorInitAdapter,
        drearthcal_update,
        gDrEarthCalObjDescriptorHitDetectAdapter,
        gDrEarthCalObjDescriptorRenderAdapter,
        gDrEarthCalObjDescriptorFreeAdapter,
        gDrEarthCalObjDescriptorTypeIdAdapter,
        gDrEarthCalObjDescriptorExtraSizeAdapter,
        drearthcal_func0A,
        0,
    },
};
