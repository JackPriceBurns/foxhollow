/* VFP_lavapoo (DLL 0x0226) */
#include "dlls/object_descriptor.h"
#include "dolphin/math.h"
#include "game/objects/object_setup.h"
#include "main/audio/sfx.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/dll/partfx_interface.h"
#include "main/frame_timing.h"
#include "main/object_render.h"
#include "main/objprint.h"
#include "main/objtexture.h"
#include "main/vecmath.h"

#define VFP_LAVAPOOL_PARTFX          0x3a2

static const f32 gVfpLavaPoolPi = 3.1415927f;

f32 gVfpLavaPoolWaveSin;

typedef struct VfpLavaPoolMapData
{
    ObjPlacement base;
    u8 pad18[2];
    s16 amplitudeDivisor; /* 0x1a: inverse scale for the randomized wave amplitude */
} VfpLavaPoolMapData;

typedef struct VfpLavaPoolState
{
    u8 pad00[4];
    s16 timerA;      /* 0x04 (init 7000) */
    s16 timerB;      /* 0x06 (init 2000) */
    f32 amplitude;   /* 0x08 */
    f32 phase;       /* 0x0C */
    f32 speedFactor; /* 0x10 */
    u8 pad14[4];
} VfpLavaPoolState;

STATIC_ASSERT(offsetof(VfpLavaPoolMapData, amplitudeDivisor) == 0x1A);
STATIC_ASSERT(sizeof(VfpLavaPoolState) == 0x18);
STATIC_ASSERT(offsetof(VfpLavaPoolState, timerA) == 0x04);
STATIC_ASSERT(offsetof(VfpLavaPoolState, timerB) == 0x06);
STATIC_ASSERT(offsetof(VfpLavaPoolState, amplitude) == 0x08);
STATIC_ASSERT(offsetof(VfpLavaPoolState, phase) == 0x0C);
STATIC_ASSERT(offsetof(VfpLavaPoolState, speedFactor) == 0x10);

void VFP_lavapool_updateWave(GameObject* obj)
{
    VfpLavaPoolState* state;
    VfpLavaPoolMapData* mapData;
    f32 speed;
    f32 phase;
    ObjTextureRuntimeSlot* tex;
    f32 scrollT;
    f32 waveScale;
    s16 amplitudeDivisor;
    struct
    {
        u8 pad[8];
        f32 value;
        f32 unused[2];
    } parm;

    state = obj->extra;
    mapData = (VfpLavaPoolMapData*)obj->anim.placementData;
    speed = (f32)(u32)obj->anim.alpha;
    state->phase += timeDelta * ((100.0f * state->speedFactor) / 100.0f);
    if (state->phase > 32767.0f)
    {
        amplitudeDivisor = ObjAnim_ReadPlacementS16(
            &obj->anim, &mapData->amplitudeDivisor);
        if (amplitudeDivisor == 0)
        {
            amplitudeDivisor = 500;
        }
        state->speedFactor = (f32)randomGetRange(0x32, 100);
        state->amplitude = 1.0f / ((f32)(int)amplitudeDivisor / (f32)randomGetRange(0x15e, 800));
        state->phase = 0.0f;
        Sfx_PlayFromObject(obj, SFXTRIG_id_111);
        speed = 255.0f;
    }
    gVfpLavaPoolWaveSin = mathSinf((gVfpLavaPoolPi * (f32)(s16)(int)state->phase) / 32768.0f);
    waveScale = 0.8f * state->amplitude;
    obj->anim.rootMotionScale = 0.2f * state->amplitude + waveScale * gVfpLavaPoolWaveSin;
    phase = state->phase;
    if (phase > 4767.0f && phase < 20767.0f)
    {
        parm.value = state->amplitude;
        if (obj->objectFlags & OBJECT_OBJFLAG_RENDERED)
        {
            (*gPartfxInterface)->spawnObject((void*)obj, VFP_LAVAPOOL_PARTFX, &parm, 2, -1, NULL);
        }
    }
    phase = state->phase;
    if (phase > 16767.0f)
    {
        speed = (f32)(s16)(int)(255.0f * gVfpLavaPoolWaveSin);
    }
    if (phase < 2000.0f)
    {
        speed = 255.0f * (phase / 2000.0f);
    }
    obj->anim.alpha = ((speed < 0.0f) ? 0.0f : ((speed > 255.0f) ? 255.0f : speed));
    tex = objFindTexture(obj, 0, 0);
    if (tex != NULL)
    {
        scrollT = (f32)(int)tex->offsetT;
        scrollT += 100.0f;
        if (scrollT >= 10000.0f)
        {
            scrollT -= 10000.0f;
        }
        tex->offsetT = (s16)scrollT;
    }
    tex = objFindTexture(obj, 1, 0);
    if (tex != NULL)
    {
        scrollT = (f32)(int)tex->offsetT;
        scrollT += 50.0f;
        if (scrollT >= 10000.0f)
        {
            scrollT -= 10000.0f;
        }
        tex->offsetT = (s16)scrollT;
    }
}

int VFP_lavapool_animEventCallback(void)
{
    return 0x1;
}

int VFP_lavapool_getExtraSize_ret_24(void)
{
    return 0x18;
}

int VFP_lavapool_getObjectTypeId(void)
{
    return 0x0;
}

void VFP_lavapool_free_nop(void)
{
}

void VFP_lavapool_render(GameObject* obj, int p1, int p2, int p3, int p4, s8 visible)
{
    if (visible != 0)
    {
        objSetColorFilter(0xff, 0xe6, 0xd7);
        objRenderModelAndHitVolumes(obj, p1, p2, p3, p4, 1.0f);
    }
}

void VFP_lavapool_hitDetect_nop(void)
{
}

void VFP_lavapool_update(GameObject* obj)
{
    VFP_lavapool_updateWave(obj);
}

void VFP_lavapool_init(GameObject* obj, VfpLavaPoolMapData* mapData)
{
    VfpLavaPoolState* state;
    s16 amplitudeDivisor;

    state = obj->extra;
    obj->animEventCallback = VFP_lavapool_animEventCallback;
    state->timerA = 7000;
    state->timerB = 2000;
    amplitudeDivisor = ObjAnim_ReadPlacementS16(
        &obj->anim, &mapData->amplitudeDivisor);
    if (amplitudeDivisor == 0)
    {
        amplitudeDivisor = 500;
    }
    obj->anim.rootMotionScale =
        1.0f /
        ((f32)(int)amplitudeDivisor / (f32)randomGetRange(600, 1000));
    state->amplitude = obj->anim.rootMotionScale;
    state->speedFactor = (f32)randomGetRange(0x32, 100);
}

void VFP_lavapool_release_nop(void)
{
}

void VFP_lavapool_initialise_nop(void)
{
}

OBJECT_INIT_ADAPTER(gVFP_lavapoolObjDescriptorInitAdapter, VFP_lavapool_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gVFP_lavapoolObjDescriptorHitDetectAdapter, VFP_lavapool_hitDetect_nop)
OBJECT_FREE_ADAPTER(gVFP_lavapoolObjDescriptorFreeAdapter, VFP_lavapool_free_nop)
OBJECT_TYPE_ID_ADAPTER(gVFP_lavapoolObjDescriptorTypeIdAdapter, VFP_lavapool_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gVFP_lavapoolObjDescriptorExtraSizeAdapter, VFP_lavapool_getExtraSize_ret_24)

RESOURCE_ACQUIRE_ADAPTER(gVFP_lavapoolObjDescriptorAcquire, VFP_lavapool_initialise_nop)

ObjectDescriptor gVFP_lavapoolObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gVFP_lavapoolObjDescriptorAcquire,
        VFP_lavapool_release_nop,
    },
    0,
    gVFP_lavapoolObjDescriptorInitAdapter,
    VFP_lavapool_update,
    gVFP_lavapoolObjDescriptorHitDetectAdapter,
    VFP_lavapool_render,
    gVFP_lavapoolObjDescriptorFreeAdapter,
    gVFP_lavapoolObjDescriptorTypeIdAdapter,
    gVFP_lavapoolObjDescriptorExtraSizeAdapter,
};
