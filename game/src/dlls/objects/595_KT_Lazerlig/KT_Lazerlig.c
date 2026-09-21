/*
 * KT_Lazerlig (DLL 0x253) - the point light cast by a SharpClaw laser
 * fence/wall (see ktlazerwall, DLL 0x252).
 *
 * On init it spawns a model light at the placement's position. Each
 * update tick two placement game bits decide whether the light is on and
 * how far it reaches: the first bit's value scales the distance falloff
 * (defaulting to 0x10 when set but zero), the second bit just keeps the
 * light lit. The light is freed when the object is destroyed.
 */
#include "dlls/object_descriptor.h"
#include "main/gamebits.h"
#include "main/dll/DR/dll_0253_ktlazerlight.h"
#include "main/model_light.h"

int ktlazerlight_getExtraSize(void) {
    return sizeof(KtlazerlightState);
}

int ktlazerlight_getObjectTypeId(void) {
    return 0x0;
}

void ktlazerlight_free(GameObject* obj) {
    KtlazerlightState* state = obj->extra;
    if (state->light != NULL) {
        ModelLightStruct_free(state->light);
    }
}

void ktlazerlight_render(void) {
}

void ktlazerlight_hitDetect(void) {
}

void ktlazerlight_update(GameObject* obj) {
    KtlazerlightPlacement* placement = (KtlazerlightPlacement*)obj->anim.placementData;
    KtlazerlightState* state = obj->extra;
    ModelLightStruct* light = state->light;

    s16 intensity = mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->onIntensityBit));
    if (intensity < 1 && mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->onStayLitBit)) == 0) {
        if (light != 0) {
            modelLightStruct_setEnabled(light, 0, 0.0f);
        }
        return;
    }

    if (intensity == 0) {
        intensity = 16;
    }

    if (light != 0) {
        modelLightStruct_setEnabled(light, 1, 0.0f);
        modelLightStruct_setDiffuseColor(light, 0x64, 0x6e, 0xff, 0xff);
        modelLightStruct_setDistanceAttenuation(state->light, intensity * 26, intensity * 26 + 20);
    }
}

void ktlazerlight_init(GameObject* obj, KtlazerlightPlacement* placement) {
    KtlazerlightState* state = obj->extra;
    state->light = objCreateLight(NULL, 1);
    if (state->light == NULL) {
        return;
    }

    modelLightStruct_setLightKind(state->light, MODEL_LIGHT_KIND_POINT);
    modelLightStruct_setPosition(state->light, placement->base.posX, placement->base.posY, placement->base.posZ);
    modelLightStruct_setAffectsAabbLightSelection(state->light, 1);
}

void ktlazerlight_release(void) {
}

void ktlazerlight_initialise(void) {
}

OBJECT_INIT_ADAPTER(gKtLazerlightObjDescriptorInitAdapter, ktlazerlight_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gKtLazerlightObjDescriptorHitDetectAdapter, ktlazerlight_hitDetect)
OBJECT_RENDER_ADAPTER(gKtLazerlightObjDescriptorRenderAdapter, ktlazerlight_render)
OBJECT_FREE_ADAPTER(gKtLazerlightObjDescriptorFreeAdapter, ktlazerlight_free, obj)
OBJECT_TYPE_ID_ADAPTER(gKtLazerlightObjDescriptorTypeIdAdapter, ktlazerlight_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gKtLazerlightObjDescriptorExtraSizeAdapter, ktlazerlight_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gKtLazerlightObjDescriptorAcquire, ktlazerlight_initialise)

ObjectDescriptor gKtLazerlightObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gKtLazerlightObjDescriptorAcquire,
        ktlazerlight_release,
    },
    0,
    gKtLazerlightObjDescriptorInitAdapter,
    ktlazerlight_update,
    gKtLazerlightObjDescriptorHitDetectAdapter,
    gKtLazerlightObjDescriptorRenderAdapter,
    gKtLazerlightObjDescriptorFreeAdapter,
    gKtLazerlightObjDescriptorTypeIdAdapter,
    gKtLazerlightObjDescriptorExtraSizeAdapter,
};
