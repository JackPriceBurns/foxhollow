/*
 * DLL 0x1F6 (slot 502) advances object animation and responds to a linked
 * parent's rotation.
 */
#include "dlls/objects/502.h"

#include "main/frame_timing.h"
#include "main/object_render.h"
#include "sys/objects.h"

enum {
    DLL1F6_SEQUENCE_FLUTTER = 0x187,
    DLL1F6_SEQUENCE_TIED = 0x803
};

int dll502_getExtraSize(void) {
    return 0;
}

int dll502_getObjectTypeId(void) {
    return 0;
}

void dll502_free(void) {
}

static void dll502_updateTiedSwing(GameObject* obj, GameObject* parent) {
    if ((parent->objectFlags & OBJECT_OBJFLAG_PARENT_SLACK) != 0) {
        obj->anim.velocityX = 0.0f;
        return;
    }

    obj->anim.velocityX = parent->anim.rotZ * 0.5f;
    obj->anim.rotZ += obj->anim.velocityX;
}

void dll502_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (visible == 0) {
        return;
    }

    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
}

void dll502_hitDetect(void) {
}

void dll502_update(GameObject* obj) {
    if (obj->anim.romDefNo == DLL1F6_SEQUENCE_FLUTTER) {
        ObjAnim_AdvanceCurrentMove(obj, 0.007f, framesThisStep, NULL);
        return;
    }

    if (obj->anim.romDefNo == DLL1F6_SEQUENCE_TIED) {
        Obj_GetPlayerObject();
        dll502_updateTiedSwing(obj, obj->anim.parent);
        return;
    }

    ObjAnim_AdvanceCurrentMove(obj, 0.02f, framesThisStep, NULL);
}

void dll502_init(GameObject* obj, const Dll1F6PlacementView* placement) {
    if (obj->anim.romDefNo == DLL1F6_SEQUENCE_TIED) {
        return;
    }

    obj->anim.rotX = placement->rotXByte << 8;
    ObjAnim_SetCurrentMove(obj, 0, 0.0f, 0);
}

void dll502_release(void) {
}

void dll502_initialise(void) {
}

ObjectDescriptor gDll1F6ObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    dll502_initialise,
    dll502_release,
    0,
    (ObjectDescriptorCallback)dll502_init,
    (ObjectDescriptorCallback)dll502_update,
    dll502_hitDetect,
    (ObjectDescriptorCallback)dll502_render,
    dll502_free,
    (ObjectDescriptorCallback)dll502_getObjectTypeId,
    dll502_getExtraSize,
};
