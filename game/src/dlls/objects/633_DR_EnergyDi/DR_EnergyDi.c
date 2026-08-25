/*
 * DR_EnergyDi (DLL 633) - an energy-disc dressing object whose
 * activation is driven by two placement game bits.
 *
 * While the "active" game bit (placement 0x20) is set the disc plays a
 * one-shot servo whir on its first frame active, forces its texture to
 * the energised id and scrolls the texture's T coordinate each step.
 * When the "move" game bit (placement 0x1E) is set the disc switches to
 * animation move 1.0f. init seeds the spawn rotation from the
 * placement and primes the activated/texture state from the active bit.
 */
#include "main/dll/DR/dll_0279_drenergydisc.h"
#include "main/audio/sfx.h"
#include "main/frame_timing.h"
#include "main/gamebits.h"
#include "main/objtexture.h"
#include "dlls/object_descriptor.h"
#include "main/audio/sfx_trigger_ids.h"

int gDREnergyDiscScrollSpeedT = -400;

int DR_EnergyDisc_getExtraSize(void) {
    return 1;
}

int DR_EnergyDisc_getObjectTypeId(void) {
    return 0;
}

void DR_EnergyDisc_free(void) {
}

void DR_EnergyDisc_render(void) {
}

void DR_EnergyDisc_hitDetect(void) {
}

void DR_EnergyDisc_update(GameObject* obj) {
    DrEnergyDiscState* state = obj->extra;
    DrenergydiscPlacement* setup = (DrenergydiscPlacement*)obj->anim.placementData;

    if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &setup->activeGameBit)) != 0) {
        if (state->activated == 0) {
            state->activated = 1;
            Sfx_PlayFromObject(obj, SFXTRIG_id_30c);
        }

        ObjTextureRuntimeSlot* texture = objFindTexture(obj, 0, 0);
        if (texture != NULL) {
            texture->textureId = 0x100;
        }

        texture = objFindTexture(obj, 0, 0);
        if (texture != NULL) {
            texture->offsetT = texture->offsetT + gDREnergyDiscScrollSpeedT * framesThisStep;
            if (texture->offsetT < -0x1000) {
                texture->offsetT = 0;
            }
        }
    }

    if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &(setup->moveGameBit))) != 0) {
        ObjAnim_SetCurrentMove(obj, 0, 1.0f, 0);
    }
}

void DR_EnergyDisc_init(GameObject* obj, DrenergydiscPlacement* setup) {
    ObjTextureRuntimeSlot* texture;
    DrEnergyDiscState* state = obj->extra;
    obj->anim.rotX = setup->rotXByte << 8;
    if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &setup->activeGameBit)) != 0) {
        state->activated = 1;
        Sfx_PlayFromObject(obj, SFXTRIG_id_30c);
        texture = objFindTexture(obj, 0, 0);
        if (texture != NULL) {
            texture->textureId = 0x100;
        }
    } else {
        state->activated = 0;
        texture = objFindTexture(obj, 0, 0);
        if (texture != NULL) {
            texture->textureId = 0;
        }
    }
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN | OBJECT_OBJFLAG_HITDETECT_DISABLED;
}

void DR_EnergyDisc_release(void) {
}

void DR_EnergyDisc_initialise(void) {
}

OBJECT_INIT_ADAPTER(gDrEnergyDiscObjDescriptorInitAdapter, DR_EnergyDisc_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gDrEnergyDiscObjDescriptorHitDetectAdapter, DR_EnergyDisc_hitDetect)
OBJECT_RENDER_ADAPTER(gDrEnergyDiscObjDescriptorRenderAdapter, DR_EnergyDisc_render)
OBJECT_FREE_ADAPTER(gDrEnergyDiscObjDescriptorFreeAdapter, DR_EnergyDisc_free)
OBJECT_TYPE_ID_ADAPTER(gDrEnergyDiscObjDescriptorTypeIdAdapter, DR_EnergyDisc_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gDrEnergyDiscObjDescriptorExtraSizeAdapter, DR_EnergyDisc_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gDrEnergyDiscObjDescriptorAcquire, DR_EnergyDisc_initialise)

ObjectDescriptor gDrEnergyDiscObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gDrEnergyDiscObjDescriptorAcquire,
        DR_EnergyDisc_release,
    },
    0,
    gDrEnergyDiscObjDescriptorInitAdapter,
    DR_EnergyDisc_update,
    gDrEnergyDiscObjDescriptorHitDetectAdapter,
    gDrEnergyDiscObjDescriptorRenderAdapter,
    gDrEnergyDiscObjDescriptorFreeAdapter,
    gDrEnergyDiscObjDescriptorTypeIdAdapter,
    gDrEnergyDiscObjDescriptorExtraSizeAdapter,
};
