#include "main/dll/dll_02AD_softbody.h"
#include "main/frame_timing.h"
#include "main/object_render.h"
#include "main/objhits.h"

typedef enum SoftBodyObjectId {
    SOFT_BODY_SLOW_PHASE_OBJECT_A = 0x6AF,
    SOFT_BODY_SLOW_PHASE_OBJECT_B,
    SOFT_BODY_SLOW_PHASE_OBJECT_C
} SoftBodyObjectId;

static f32 gSoftBodySlowPhase;
static f32 gSoftBodyFastPhase;
static GameObject* gSoftBodyPhaseDriver;

int SoftBody_getExtraSize(void)
{
    return 0;
}

int SoftBody_getObjectTypeId(void)
{
    return 0;
}

void SoftBody_free(GameObject* obj)
{
    if (obj == gSoftBodyPhaseDriver)
    {
        gSoftBodyPhaseDriver = NULL;
    }
}

void SoftBody_render(GameObject* obj, int p2, int p3, int p4, int p5, s8 visible)
{
    if (visible != 0)
    {
        objRenderModelAndHitVolumes(obj, p2, p3, p4, p5, 1.0f);
    }
}

void SoftBody_hitDetect(void)
{
}

void SoftBody_update(GameObject* obj)
{
    const SoftBodyPlacement* placement = obj->anim.placementData;

    if (gSoftBodyPhaseDriver == NULL && placement->phaseDriverDisabled == 0)
    {
        gSoftBodyPhaseDriver = obj;
    }

    if (obj == gSoftBodyPhaseDriver)
    {
        gSoftBodySlowPhase = 0.001f * timeDelta + gSoftBodySlowPhase;
        while (gSoftBodySlowPhase > 1.0f)
        {
            gSoftBodySlowPhase -= 1.0f;
        }
        gSoftBodyFastPhase = 0.005f * timeDelta + gSoftBodyFastPhase;
        while (gSoftBodyFastPhase > 1.0f)
        {
            gSoftBodyFastPhase -= 1.0f;
        }
    }

    switch (obj->anim.romDefNo)
    {
    case SOFT_BODY_SLOW_PHASE_OBJECT_A:
    case SOFT_BODY_SLOW_PHASE_OBJECT_B:
    case SOFT_BODY_SLOW_PHASE_OBJECT_C:
        ObjAnim_SetCurrentMove(obj, 0, gSoftBodySlowPhase, 0);
        break;
    default:
        ObjAnim_SetCurrentMove(obj, 0, gSoftBodyFastPhase, 0);
        break;
    }
}

void SoftBody_init(GameObject* obj, const SoftBodyPlacement* placement)
{
    obj->anim.rotZ = (s16)((u32)placement->rotationZByte * 256);
    obj->anim.rotY = (s16)((u32)placement->rotationYByte * 256);
    obj->anim.rotX = (s16)((u32)placement->rotationXByte * 256);
    if (placement->scaleByte != 0)
    {
        obj->anim.rootMotionScale = (f32)(u32)placement->scaleByte / 255.0f;
        if (obj->anim.rootMotionScale == 0.0f)
        {
            obj->anim.rootMotionScale = 1.0f;
        }
        obj->anim.rootMotionScale *= obj->anim.modelInstance->rootMotionScaleBase;
    }
    obj->objectFlags |= OBJECT_OBJFLAG_HITDETECT_DISABLED;
    ObjAnim_SetCurrentMove(obj, 0, 0.0f, 0);
    if (obj->anim.hitReactState != NULL)
    {
        ObjHitsPriorityState* hitState = (ObjHitsPriorityState*)obj->anim.hitReactState;

        ObjHitbox_SetSphereRadius(&obj->anim, (s16)((f32)hitState->primaryRadius * obj->anim.rootMotionScale));
    }
}

void SoftBody_release(void)
{
}

void SoftBody_initialise(void)
{
    gSoftBodyPhaseDriver = NULL;
    gSoftBodySlowPhase = 0.0f;
    gSoftBodyFastPhase = 0.0f;
}

ObjectDescriptor gSoftBodyObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    (ObjectDescriptorCallback)SoftBody_initialise,
    (ObjectDescriptorCallback)SoftBody_release,
    0,
    (ObjectDescriptorCallback)SoftBody_init,
    (ObjectDescriptorCallback)SoftBody_update,
    (ObjectDescriptorCallback)SoftBody_hitDetect,
    (ObjectDescriptorCallback)SoftBody_render,
    (ObjectDescriptorCallback)SoftBody_free,
    (ObjectDescriptorCallback)SoftBody_getObjectTypeId,
    (ObjectDescriptorExtraSizeCallback)SoftBody_getExtraSize,
};
