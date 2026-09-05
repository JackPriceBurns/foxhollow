/*
 * DLL 0x219 - a game-bit gated sliding object.
 *
 * Only the object with id DLL_219_MOVING_OBJECT_ID is animated; the
 * remaining ids are inert (update returns immediately). When its game
 * bit is set the object slides its local X down to
 * (placement posX - 30) at speed 0.4; when the bit
 * is clear it slides back up to placement posX at speed 0.2,
 * clamping at each end. init seeds the object's rotX and the state's
 * game bit from the placement record; free releases its expgfx source.
 */
#include "main/gamebits.h"
#include "main/dll/expgfx_interface.h"
#include "main/dll/dll_0219.h"

#define DLL_219_MOVING_OBJECT_ID   0x3a6
#define DLL_219_INERT_OBJECT_ID_LO 0x3ad
#define DLL_219_INERT_OBJECT_ID_HI 0x3ae

int dll_219_getExtraSize_ret_4(void) {
    return 0x4;
}

int dll_219_getObjectTypeId(void) {
    return 0x0;
}

void dll_219_free(GameObject* obj) {
    (*gExpgfxInterface)->freeSource2((uintptr_t)obj);
}

void dll_219_render_nop(void) {
}

void dll_219_hitDetect_nop(void) {
}

void dll_219_update(GameObject* obj) {
    switch (obj->anim.romDefNo) {
    case DLL_219_MOVING_OBJECT_ID:
        break;
    case DLL_219_INERT_OBJECT_ID_LO:
    case DLL_219_INERT_OBJECT_ID_HI:
    default:
        return;
    }

    ObjPlacement* setup = (ObjPlacement*)obj->anim.placementData;
    Dll219State* state = obj->extra;
    if (mainGetBit(state->gameBit) != 0) {
        if (obj->anim.localPosX > setup->posX - 30.0f) {
            obj->anim.localPosX -= 0.4f;
            if (obj->anim.localPosX < setup->posX - 30.0f) {
                obj->anim.localPosX = setup->posX - 30.0f;
            }
            return;
        }
    }

    if (mainGetBit(state->gameBit) == 0) {
        if (obj->anim.localPosX < setup->posX) {
            obj->anim.localPosX += 0.2f;
            if (obj->anim.localPosX > setup->posX) {
                obj->anim.localPosX = setup->posX;
            }
        }
    }
}

void dll_219_init(GameObject* obj, Dll219Setup* placement) {
    Dll219State* state = obj->extra;
    obj->anim.rotX = placement->rotX << 8;
    state->gameBit = ObjAnim_ReadPlacementS16(&obj->anim, &placement->gameBit);
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN | OBJECT_OBJFLAG_HITDETECT_DISABLED;
}

void dll_219_release_nop(void) {
}

void dll_219_initialise_nop(void) {
}

OBJECT_INIT_ADAPTER(gDll219ObjDescriptorInitAdapter, dll_219_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gDll219ObjDescriptorHitDetectAdapter, dll_219_hitDetect_nop)
OBJECT_RENDER_ADAPTER(gDll219ObjDescriptorRenderAdapter, dll_219_render_nop)
OBJECT_FREE_ADAPTER(gDll219ObjDescriptorFreeAdapter, dll_219_free, obj)
OBJECT_TYPE_ID_ADAPTER(gDll219ObjDescriptorTypeIdAdapter, dll_219_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gDll219ObjDescriptorExtraSizeAdapter, dll_219_getExtraSize_ret_4)

RESOURCE_ACQUIRE_ADAPTER(gDll219ObjDescriptorAcquire, dll_219_initialise_nop)

ObjectDescriptor gDll219ObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gDll219ObjDescriptorAcquire,
        dll_219_release_nop,
    },
    0,
    gDll219ObjDescriptorInitAdapter,
    dll_219_update,
    gDll219ObjDescriptorHitDetectAdapter,
    gDll219ObjDescriptorRenderAdapter,
    gDll219ObjDescriptorFreeAdapter,
    gDll219ObjDescriptorTypeIdAdapter,
    gDll219ObjDescriptorExtraSizeAdapter,
};
