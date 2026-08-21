#include "dolphin/mtx/vec.h"
#include "main/camera.h"
#include "main/dll/WM/dll_0215_wmnewcrystal.h"
#include "main/dll/partfx_interface.h"
#include "main/gamebits.h"
#include "main/gamebit_ids.h"
#include "main/mapEventTypes.h"
#include "main/obj_path.h"
#include "main/objfx.h"
#include "main/object_render.h"
#include "main/objseq.h"

typedef enum WmNewCrystalEvent {
    WMNEWCRYSTAL_EVENT_DETONATE = 1,
    WMNEWCRYSTAL_EVENT_STOP_GREEN_BURSTS = 2
} WmNewCrystalEvent;

typedef enum WmNewCrystalObjectId {
    WMNEWCRYSTAL_OBJECT_BLUE = 0x783,
    WMNEWCRYSTAL_OBJECT_GREEN = 0x784
} WmNewCrystalObjectId;

typedef enum WmNewCrystalParticleId {
    WMNEWCRYSTAL_PARTICLE_ID = 0x7ed
} WmNewCrystalParticleId;

typedef struct WmNewCrystalBurstOrigin {
    u8 pad00[6];
    s16 pathPoint;
    u8 pad08[4];
    Vec3f position;
} WmNewCrystalBurstOrigin;

typedef struct WmNewCrystalState {
    ObjFxCrystalOrbitState primaryOrbit;
    ObjFxCrystalOrbitState secondaryOrbit;
    u8 greenBurstsActive;
} WmNewCrystalState;

STATIC_ASSERT(offsetof(WmNewCrystalBurstOrigin, pathPoint) == 0x06);
STATIC_ASSERT(offsetof(WmNewCrystalBurstOrigin, position) == 0x0C);
STATIC_ASSERT(sizeof(WmNewCrystalBurstOrigin) == 0x18);
STATIC_ASSERT(offsetof(WmNewCrystalState, secondaryOrbit) == 0x34);
STATIC_ASSERT(offsetof(WmNewCrystalState, greenBurstsActive) == 0x68);
STATIC_ASSERT(sizeof(WmNewCrystalState) == 0x6C);

int WM_newcrystal_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate)
{
    WmNewCrystalState* state;
    WmNewCrystalBurstOrigin burstOrigin;
    Vec cameraDelta;

    state = obj->extra;
    for (s32 eventIndex = 0; eventIndex < animUpdate->eventCount; eventIndex++)
    {
        switch (animUpdate->eventIds[eventIndex])
        {
        case WMNEWCRYSTAL_EVENT_DETONATE:
            PSVECSubtract(&Camera_GetCurrent()->position, &obj->anim.localPos, &cameraDelta);
            PSVECNormalize(&cameraDelta, &cameraDelta);
            PSVECScale(&cameraDelta, &cameraDelta, 100.0f);
            PSVECAdd(&obj->anim.localPos, &cameraDelta, &obj->anim.localPos);
            obj->anim.worldPosX = obj->anim.localPosX;
            obj->anim.worldPosY = obj->anim.localPosY;
            obj->anim.worldPosZ = obj->anim.localPosZ;
            spawnExplosion(obj, 100.0f, 1, 1, 0, 0, 0, 0, 0);
            obj->anim.flags = obj->anim.flags | OBJANIM_FLAG_HIDDEN;
            if (obj->anim.romDefNo == WMNEWCRYSTAL_OBJECT_BLUE)
            {
                mainSetBits(GAMEBIT_WM_KrystalCrystalized, 0);
            }
            break;
        case WMNEWCRYSTAL_EVENT_STOP_GREEN_BURSTS:
            state->greenBurstsActive = 0;
            break;
        }
    }

    if (mainGetBit(GAMEBIT_WM_KrystalCrystalized) == 0)
    {
        return 0;
    }

    if (obj->anim.romDefNo == WMNEWCRYSTAL_OBJECT_BLUE)
    {
        if (mainGetBit(GAMEBIT_WM_KrystalRelated0E49) == 0)
        {
            (*gPartfxInterface)->spawnObject(obj, WMNEWCRYSTAL_PARTICLE_ID, NULL, 2, -1, NULL);
            (*gPartfxInterface)->spawnObject(obj, WMNEWCRYSTAL_PARTICLE_ID, &burstOrigin, 2, -1, NULL);
        }
        objfx_spawnCrystalOrbitEffects(obj, &state->primaryOrbit, 640.0f, 36.0f, -60.0f, 5.0f, 100.0f, 1);
        objfx_spawnCrystalOrbitEffects(obj, &state->secondaryOrbit, 640.0f, 36.0f, 60.0f, 5.0f, 0.0f, 1);
    }
    else if (obj->anim.romDefNo == WMNEWCRYSTAL_OBJECT_GREEN && state->greenBurstsActive != 0)
    {
        ObjPath_GetPointLocalPosition(obj, 0, &burstOrigin.position.x, &burstOrigin.position.y,
                                      &burstOrigin.position.z);
        PSVECScale(&burstOrigin.position, &burstOrigin.position, obj->anim.rootMotionScale);
        burstOrigin.pathPoint = 1;
        objfx_spawnDirectionalBurst(obj, 5, 2.0f, 1, 1, 10, 4.0f, &burstOrigin, 0);

        ObjPath_GetPointLocalPosition(obj, 1, &burstOrigin.position.x, &burstOrigin.position.y,
                                      &burstOrigin.position.z);
        PSVECScale(&burstOrigin.position, &burstOrigin.position, obj->anim.rootMotionScale);
        burstOrigin.pathPoint = 0;
        objfx_spawnDirectionalBurst(obj, 5, 2.0f, 1, 1, 10, 4.0f, &burstOrigin, 0);
    }
    return 0;
}

int WM_newcrystal_getExtraSize(void)
{
    return sizeof(WmNewCrystalState);
}

int WM_newcrystal_getObjectTypeId(void)
{
    return 0x0;
}

void WM_newcrystal_free(void)
{
}

void WM_newcrystal_render(GameObject* obj, int p2, int p3, int p4, int p5, s8 visible)
{
    objRenderModelAndHitVolumes(obj, p2, p3, p4, p5, 1.0f);
}

void WM_newcrystal_hitDetect(void)
{
}

void WM_newcrystal_update(void)
{
}

void WM_newcrystal_init(GameObject* obj, ObjPlacement* unused)
{
    WmNewCrystalState* state = obj->extra;
    obj->animEventCallback = WM_newcrystal_SeqFn;
    if ((*gMapEventInterface)->getMapAct(obj->anim.mapEventSlot) > 1)
    {
        mainSetBits(GAMEBIT_WM_KrystalCrystalized, 1);
        state->greenBurstsActive = 1;
    }
}

void WM_newcrystal_release(void)
{
}

void WM_newcrystal_initialise(void)
{
}

ObjectDescriptor gWM_newcrystalObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    WM_newcrystal_initialise,
    WM_newcrystal_release,
    0,
    (ObjectDescriptorCallback)WM_newcrystal_init,
    WM_newcrystal_update,
    WM_newcrystal_hitDetect,
    (ObjectDescriptorCallback)WM_newcrystal_render,
    WM_newcrystal_free,
    (ObjectDescriptorCallback)WM_newcrystal_getObjectTypeId,
    WM_newcrystal_getExtraSize,
};
