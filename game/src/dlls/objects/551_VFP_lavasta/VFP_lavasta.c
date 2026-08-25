/* VFP_lavasta (DLL 0x0227) */
#include "dlls/object_descriptor.h"
#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/dll/dll_00A6_modgfx.h"
#include "main/dll/expgfx_interface.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/partfx_interface.h"
#include "main/frame_timing.h"
#include "main/resource.h"
#include "main/vecmath.h"

typedef enum VfpLavaStarParticleId {
    VFP_LAVA_STAR_PARTICLE_ID = 0x3A4,
} VfpLavaStarParticleId;

static DllA6Interface** sVfpLavaPoolEffectResource;

typedef struct VfpLavaStarState
{
    f32 verticalVelocity;
    f32 delayRangeMin;
    f32 delayRangeMax;
    s16 gameBit;
    s16 effectTimer;
    u8 particleToggle;
    u8 pad11[3];
} VfpLavaStarState;

typedef struct VfpLavaStarPlacement
{
    ObjPlacement base;
    u8 pad18[2];
    s16 heightOffset;
    u8 pad1C[2];
    s16 gameBit;
} VfpLavaStarPlacement;

STATIC_ASSERT(sizeof(VfpLavaStarState) == 0x14);
STATIC_ASSERT(offsetof(VfpLavaStarState, verticalVelocity) == 0x00);
STATIC_ASSERT(offsetof(VfpLavaStarState, delayRangeMin) == 0x04);
STATIC_ASSERT(offsetof(VfpLavaStarState, delayRangeMax) == 0x08);
STATIC_ASSERT(offsetof(VfpLavaStarState, gameBit) == 0x0C);
STATIC_ASSERT(offsetof(VfpLavaStarState, effectTimer) == 0x0E);
STATIC_ASSERT(offsetof(VfpLavaStarState, particleToggle) == 0x10);
STATIC_ASSERT(offsetof(VfpLavaStarPlacement, heightOffset) == 0x1A);
STATIC_ASSERT(offsetof(VfpLavaStarPlacement, gameBit) == 0x1E);

int VFP_lavastar_getExtraSize(void)
{
    return sizeof(VfpLavaStarState);
}

int VFP_lavastar_getObjectTypeId(void)
{
    return 0;
}

void VFP_lavastar_free(GameObject* obj)
{
    (*gExpgfxInterface)->freeSource2((uintptr_t)obj);
    (*gModgfxInterface)->freeSourceEffects((void*)obj);
}

void VFP_lavastar_render(void)
{
}

void VFP_lavastar_hitDetect(void)
{
}

void VFP_lavastar_update(GameObject* obj)
{
    const VfpLavaStarPlacement* placement = (const VfpLavaStarPlacement*)obj->anim.placementData;
    VfpLavaStarState* state = obj->extra;

    obj->anim.localPosY += timeDelta * state->verticalVelocity;
    if (obj->anim.localPosY > 900.0f + placement->base.posY)
    {
        state->verticalVelocity = 0.1f * (f32)randomGetRange(5, 0x14);
        obj->anim.localPosY = placement->base.posY;
    }
    state->effectTimer += (s16)timeDelta;
    if (sVfpLavaPoolEffectResource != NULL && state->effectTimer >= 0x28)
    {
        (*sVfpLavaPoolEffectResource)->spawn(obj, 0, NULL, 4);
        state->effectTimer = 0;
    }
    if (state->particleToggle == 0)
    {
        (*gPartfxInterface)->spawnObject(obj, VFP_LAVA_STAR_PARTICLE_ID, NULL, 2, -1, NULL);
    }
    state->particleToggle ^= 1;
}

void VFP_lavastar_init(GameObject* obj, const VfpLavaStarPlacement* placement)
{
    VfpLavaStarState* state = obj->extra;

    state->gameBit = ObjAnim_ReadPlacementS16(&obj->anim, &placement->gameBit);
    state->verticalVelocity = 0.1f * (f32)randomGetRange(10, 0x19);
    state->effectTimer = 0x14;
    obj->anim.localPosY =
        placement->base.posY + (f32)ObjAnim_ReadPlacementS16(&obj->anim, &placement->heightOffset);
    obj->objectFlags |= OBJECT_OBJFLAG_HITDETECT_DISABLED;
    state->delayRangeMin = (f32)randomGetRange(0x1e, 0x3c);
    state->delayRangeMax = (f32)randomGetRange(100, 200);
}

void VFP_lavastar_release(void)
{
    Resource_Release(sVfpLavaPoolEffectResource);
    sVfpLavaPoolEffectResource = NULL;
}

void VFP_lavastar_initialise(void)
{
    sVfpLavaPoolEffectResource = Resource_Acquire(0xA6, 1);
}

OBJECT_INIT_ADAPTER(gVFP_lavastarObjDescriptorInitAdapter, VFP_lavastar_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gVFP_lavastarObjDescriptorHitDetectAdapter, VFP_lavastar_hitDetect)
OBJECT_RENDER_ADAPTER(gVFP_lavastarObjDescriptorRenderAdapter, VFP_lavastar_render)
OBJECT_FREE_ADAPTER(gVFP_lavastarObjDescriptorFreeAdapter, VFP_lavastar_free, obj)
OBJECT_TYPE_ID_ADAPTER(gVFP_lavastarObjDescriptorTypeIdAdapter, VFP_lavastar_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gVFP_lavastarObjDescriptorExtraSizeAdapter, VFP_lavastar_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gVFP_lavastarObjDescriptorAcquire, VFP_lavastar_initialise)

ObjectDescriptor gVFP_lavastarObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gVFP_lavastarObjDescriptorAcquire,
        VFP_lavastar_release,
    },
    0,
    gVFP_lavastarObjDescriptorInitAdapter,
    VFP_lavastar_update,
    gVFP_lavastarObjDescriptorHitDetectAdapter,
    gVFP_lavastarObjDescriptorRenderAdapter,
    gVFP_lavastarObjDescriptorFreeAdapter,
    gVFP_lavastarObjDescriptorTypeIdAdapter,
    gVFP_lavastarObjDescriptorExtraSizeAdapter,
};
