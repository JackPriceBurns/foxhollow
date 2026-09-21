#include "main/audio/sfx.h"
#include "main/game_timer_control.h"
#include "main/objhits.h"
#include "sys/objects.h"
#include "main/mapEvent.h"
#include "dlls/objects/562_DFP_RotateP.h"
#include "main/dll/dll_02B1_cmbsrc.h"
#include "main/gamebits.h"
#include "main/frame_timing.h"
#include "main/vecmath.h"
#include "sys/objects/lifecycle.h"
#include "main/mapEventTypes.h"
#include "main/objseq.h"

#define DFP_ROTATEP_EFFECT_RING_COUNT       4
#define DFP_ROTATEP_EFFECT_HANDLES_PER_RING 2
#define DFP_ROTATEP_MODE_SEQUENCE           2
#define DFP_ROTATEP_RING_START_SFX          0x459
#define DFP_ROTATEP_TIMEOUT_RESET_SFX       0x1CE
#define DFP_ROTATEP_GAMEBIT_RING_ACTIVE     0xEDF
#define DFP_ROTATEP_RING_VISUAL_SETUP_SIZE  0x2C
#define DFP_ROTATEP_RING_VISUAL_OBJECT_ID   CMBSRC_SEQ_DEFAULT
#define DFP_ROTATEP_RING_HIT_SETUP_SIZE     4
#define DFP_ROTATEP_RING_HIT_OBJECT_ID      0x71C
#define DFP_ROTATEP_RING_SETUP_MODE         5
#define DFP_ROTATEP_EFFECT_RING_ROT_STEP    0x3FFF

#define DFP_ROTATEP_COMPLETE_RING_COUNT     4
#define DFP_ROTATEP_TIMER_ID                0x1D
#define DFP_ROTATEP_TIMER_SHORT_FRAMES      0x96
#define DFP_ROTATEP_TIMER_LONG_FRAMES       0xB4
#define DFP_ROTATEP_MODE_SINGLE             1
#define DFP_ROTATEP_GAMEBIT_SINGLE_COMPLETE 0x9F7
#define DFP_ROTATEP_SFX_COMPLETE            0x7E
#define DFP_ROTATEP_SFX_TIMEOUT_RESET       0x1CE
#define DFP_ROTATEP_SFX_RING_HIT            0x409
#define DFP_ROTATEP_HIT_TYPE_RING_TARGET    0x13

GameObject* gDFP_RotatePEffectHandles[8];

static const s16 sDFPRotatePColorIndices[4] = {4, 5, 6, 11};

static void DFP_RotateP_updateEffectHandlePosition(GameObject* handle, GameObject* obj, s16* rotation, int angleStep)
{
    if (handle != NULL)
    {
        handle->anim.localPosX = 0.0f;
        handle->anim.localPosY = 60.0f;
        handle->anim.localPosZ = 93.0f;
        rotation[0] = (s16)(obj->anim.rotX + angleStep);
        vecRotateZXY(rotation, &handle->anim.localPosX);
        handle->anim.localPosX += obj->anim.localPosX;
        handle->anim.localPosY += obj->anim.localPosY;
        handle->anim.localPosZ += obj->anim.localPosZ;
    }
}

void DFP_RotateP_updateEffectHandleRing(GameObject* obj)
{
    struct
    {
        s16 rotation[4];
        f32 baseVec[4];
    } buf;
    GameObject** handles;
    DFPRotatePState* state = (DFPRotatePState*)obj->extra;
    s16 i;

    if (state->flags.bit10 != 0 && state->flags.bit20 == 0 && state->variantSfxTimer > 0x32)
    {
        Sfx_KeepAliveLoopedObjectSound(obj, DFP_ROTATEP_RING_START_SFX);
        if ((*gMapEventInterface)->getMapAct(obj->anim.mapEventSlot) == DFP_ROTATEP_MODE_SEQUENCE)
        {
            obj->anim.rotX += (int)((1.0f + state->ringCount) * (30.0f * timeDelta));
        }
        else
        {
            obj->anim.rotX += (int)(30.0f * timeDelta);
        }
    }

    if (state->variantSfxTimer != 0 && state->flags.bit10 != 0)
    {
        state->variantSfxTimer -= (s16)timeDelta;
        if (state->variantSfxTimer <= 0)
        {
            state->variantSfxTimer = 200;
        }
    }

    buf.baseVec[1] = 0.0f;
    buf.baseVec[2] = 0.0f;
    buf.baseVec[3] = 0.0f;
    buf.baseVec[0] = 1.0f;
    buf.rotation[1] = buf.rotation[2] = 0;
    handles = gDFP_RotatePEffectHandles;

    for (i = 0; i < DFP_ROTATEP_EFFECT_RING_COUNT; i++)
    {
        DFP_RotateP_updateEffectHandlePosition(handles[i * DFP_ROTATEP_EFFECT_HANDLES_PER_RING], obj, buf.rotation,
                                               i * DFP_ROTATEP_EFFECT_RING_ROT_STEP);
        DFP_RotateP_updateEffectHandlePosition(handles[i * DFP_ROTATEP_EFFECT_HANDLES_PER_RING + 1], obj, buf.rotation,
                                               i * DFP_ROTATEP_EFFECT_RING_ROT_STEP);
    }
}

int DFP_RotateP_ensureEffectHandlePair(GameObject* obj, u8 ringIndex)
{
    GameObject** pair;
    CmbSrcMapData* setup;

    if (Obj_IsLoadingLocked() == 0)
    {
        return 0;
    }

    pair = &gDFP_RotatePEffectHandles[(ringIndex & 0xff) * DFP_ROTATEP_EFFECT_HANDLES_PER_RING];
    if (pair[0] == NULL)
    {
        setup = (CmbSrcMapData*)Obj_AllocObjectSetup(DFP_ROTATEP_RING_VISUAL_SETUP_SIZE,
                                                     DFP_ROTATEP_RING_VISUAL_OBJECT_ID);
        setup->base.color[2] = 0xff;
        setup->base.color[3] = 0xff;
        setup->base.color[0] = 2;
        setup->base.color[1] = 1;
        setup->base.posX = obj->anim.localPosX;
        setup->base.posY = obj->anim.localPosY;
        setup->base.posZ = obj->anim.localPosZ;
        setup->gameBit = -1;
        setup->rotX = 0;
        setup->rotZ = 0;
        setup->rotY = 0;
        if ((*gMapEventInterface)->getMapAct(obj->anim.mapEventSlot) == DFP_ROTATEP_MODE_SEQUENCE)
        {
            setup->colorIndex = sDFPRotatePColorIndices[ringIndex & 0xff];
        }
        else
        {
            setup->colorIndex = (u8)sDFPRotatePColorIndices[3];
        }
        setup->effectMode = 0;
        setup->pulseSubMode = 0;
        setup->colorDistance = 0x64;
        setup->effectDistance = 0;
        setup->pulseDistance = 0;
        setup->radius = 0.5f;
        setup->flags = 0xd2;
        setup->behaviorFlags = 0;
        pair[0] = objSetupObject(&setup->base, DFP_ROTATEP_RING_SETUP_MODE, obj->anim.mapEventSlot, -1,
                                 obj->anim.parent);
    }

    if (pair[1] == NULL)
    {
        setup = (CmbSrcMapData*)Obj_AllocObjectSetup(DFP_ROTATEP_RING_HIT_SETUP_SIZE, DFP_ROTATEP_RING_HIT_OBJECT_ID);
        setup->base.color[2] = 0xff;
        setup->base.color[3] = 0xff;
        setup->base.color[0] = 2;
        setup->base.color[1] = 1;
        setup->base.posX = obj->anim.localPosX;
        setup->base.posY = obj->anim.localPosY;
        setup->base.posZ = obj->anim.localPosZ;
        pair[1] = objSetupObject(&setup->base, DFP_ROTATEP_RING_SETUP_MODE, obj->anim.mapEventSlot, -1,
                                 obj->anim.parent);
    }

    return 1;
}

int DFP_RotateP_activateEffectHandleRing(GameObject* obj, int unused, ObjSeqState* animUpdate)
{
    DFPRotatePState* state = (DFPRotatePState*)obj->extra;
    int i;

    state->flags.bit80 = 1;
    gameTimerStop();
    for (i = 0; i < animUpdate->eventCount; i++)
    {
        switch ((int)animUpdate->eventIds[i])
        {
        case 1:
            state->flags.bit10 = 1;
            state->ringCount = 0;
            mainSetBits(state->activationEventId, 0);
            mainSetBits(DFP_ROTATEP_GAMEBIT_RING_ACTIVE, 1);
            for (i = 0; i < DFP_ROTATEP_EFFECT_RING_COUNT; i++)
            {
                DFP_RotateP_ensureEffectHandlePair(obj, i);
            }
            state->flags.bit40 = 1;
            break;
        }
    }

    DFP_RotateP_updateEffectHandleRing(obj);
    return 0;
}

int DFP_RotateP_getExtraSize(void)
{
    return 0xa;
}
int DFP_RotateP_getObjectTypeId(void)
{
    return 0x0;
}

void DFP_RotateP_free(GameObject* obj, int flag)
{
    GameObject** handles;
    s16 i;

    if (flag == 0)
    {
        handles = gDFP_RotatePEffectHandles;
        for (i = 0; i < DFP_ROTATEP_EFFECT_RING_COUNT; i++)
        {
            if (handles[i * DFP_ROTATEP_EFFECT_HANDLES_PER_RING] != 0)
            {
                Obj_FreeObject(handles[i * DFP_ROTATEP_EFFECT_HANDLES_PER_RING]);
            }
            handles[i * DFP_ROTATEP_EFFECT_HANDLES_PER_RING] = 0;
            if (handles[i * DFP_ROTATEP_EFFECT_HANDLES_PER_RING + 1] != 0)
            {
                Obj_FreeObject(handles[i * DFP_ROTATEP_EFFECT_HANDLES_PER_RING + 1]);
            }
            handles[i * DFP_ROTATEP_EFFECT_HANDLES_PER_RING + 1] = 0;
            Sfx_PlayFromObject(obj, DFP_ROTATEP_TIMEOUT_RESET_SFX);
        }
    }
    gameTimerStop();
}

void DFP_RotateP_render(void)
{
}

void DFP_RotateP_hitDetect(void)
{
}

void DFP_RotateP_update(GameObject* obj)
{
    GameObject** handles;
    s16 i;
    s16 hitType;
    u8 mode;
    DFPRotatePState* state;
    DFPRotatePStateFlags* flags;
    GameObject* hitObj;

    state = (DFPRotatePState*)obj->extra;
    flags = &state->flags;
    if ((flags->bit20 == 0) && (mainGetBit(state->eventId) == 0))
    {
        if (state->ringCount == DFP_ROTATEP_COMPLETE_RING_COUNT)
        {
            Sfx_PlayFromObject(0, DFP_ROTATEP_SFX_COMPLETE);
            flags->bit20 = 1;
            flags->bit10 = 0;
            flags->bit40 = 0;
            mainSetBits(state->eventId, 1);
            mainSetBits(DFP_ROTATEP_GAMEBIT_RING_ACTIVE, 0);
            mode = (*gMapEventInterface)->getMapAct(obj->anim.mapEventSlot);
            if (mode == DFP_ROTATEP_MODE_SINGLE)
            {
                mainSetBits(DFP_ROTATEP_GAMEBIT_SINGLE_COMPLETE, 1);
            }
            gameTimerStop();
        }
        else
        {
            if (flags->bit80 != 0)
            {
                flags->bit80 = 0;
                if (flags->bit10 != 0)
                {
                    mode = (*gMapEventInterface)->getMapAct(obj->anim.mapEventSlot);
                    if (mode == DFP_ROTATEP_MODE_SINGLE)
                    {
                        gameTimerInit(DFP_ROTATEP_TIMER_ID, DFP_ROTATEP_TIMER_SHORT_FRAMES);
                    }
                    else
                    {
                        gameTimerInit(DFP_ROTATEP_TIMER_ID, DFP_ROTATEP_TIMER_LONG_FRAMES);
                    }
                    timerSetToCountUp();
                }
            }
            if (isGameTimerDisabled() != 0)
            {
                handles = gDFP_RotatePEffectHandles;
                for (i = 0; i < DFP_ROTATEP_EFFECT_RING_COUNT; i++)
                {
                    if (handles[i * DFP_ROTATEP_EFFECT_HANDLES_PER_RING] != 0)
                    {
                        Obj_FreeObject(handles[i * DFP_ROTATEP_EFFECT_HANDLES_PER_RING]);
                    }
                    handles[i * DFP_ROTATEP_EFFECT_HANDLES_PER_RING] = 0;
                    if (handles[i * DFP_ROTATEP_EFFECT_HANDLES_PER_RING + 1] != 0)
                    {
                        Obj_FreeObject(handles[i * DFP_ROTATEP_EFFECT_HANDLES_PER_RING + 1]);
                    }
                    handles[i * DFP_ROTATEP_EFFECT_HANDLES_PER_RING + 1] = 0;
                    Sfx_PlayFromObject(obj, DFP_ROTATEP_SFX_TIMEOUT_RESET);
                }
                state->ringCount = 0;
                flags->bit40 = 0;
                flags->bit10 = 0;
                mainSetBits(DFP_ROTATEP_GAMEBIT_RING_ACTIVE, 0);
            }
            DFP_RotateP_updateEffectHandleRing(obj);
            handles = gDFP_RotatePEffectHandles;
            for (i = 0; i < DFP_ROTATEP_EFFECT_RING_COUNT; i++)
            {
                if (handles[i * DFP_ROTATEP_EFFECT_HANDLES_PER_RING] != 0)
                {
                    hitObj = NULL;
                    hitType = ObjHits_GetPriorityHit(handles[i * DFP_ROTATEP_EFFECT_HANDLES_PER_RING + 1], &hitObj, 0x0,
                                                     0x0);
                    if (hitType == DFP_ROTATEP_HIT_TYPE_RING_TARGET)
                    {
                        mode = (*gMapEventInterface)->getMapAct(obj->anim.mapEventSlot);
                        if ((mode == DFP_ROTATEP_MODE_SINGLE) || (hitObj->userData1 == i))
                        {
                            if (handles[i * DFP_ROTATEP_EFFECT_HANDLES_PER_RING] != 0)
                            {
                                Obj_FreeObject(handles[i * DFP_ROTATEP_EFFECT_HANDLES_PER_RING]);
                            }
                            handles[i * DFP_ROTATEP_EFFECT_HANDLES_PER_RING] = 0;
                            if (handles[i * DFP_ROTATEP_EFFECT_HANDLES_PER_RING + 1] != 0)
                            {
                                Obj_FreeObject(handles[i * DFP_ROTATEP_EFFECT_HANDLES_PER_RING + 1]);
                            }
                            handles[i * DFP_ROTATEP_EFFECT_HANDLES_PER_RING + 1] = 0;
                            Sfx_PlayFromObject(0, DFP_ROTATEP_SFX_RING_HIT);
                            state->ringCount++;
                        }
                    }
                }
            }
        }
    }
    return;
}

void DFP_RotateP_init(GameObject* obj, DFPRotatePPlacement* placement)
{
    DFPRotatePState* state;

    state = (DFPRotatePState*)obj->extra;
    obj->anim.rotX = (s16)(placement->rotXByte << 8);
    obj->animEventCallback = (void*)DFP_RotateP_activateEffectHandleRing;
    state->config19 = placement->unknown19;
    state->eventId = ObjAnim_ReadPlacementS16(&obj->anim, &(placement->eventGameBit));
    state->activationEventId = ObjAnim_ReadPlacementS16(&obj->anim, &(placement->activationGameBit));
    state->variantSfxTimer = 1;
    gDFP_RotatePEffectHandles[0] = 0;
    gDFP_RotatePEffectHandles[1] = 0;
    gDFP_RotatePEffectHandles[2] = 0;
    gDFP_RotatePEffectHandles[3] = 0;
    gDFP_RotatePEffectHandles[4] = 0;
    gDFP_RotatePEffectHandles[5] = 0;
    gDFP_RotatePEffectHandles[6] = 0;
    gDFP_RotatePEffectHandles[7] = 0;
    gameTimerStop();
    if (mainGetBit(state->eventId) != 0)
    {
        state->flags.bit20 = 1;
    }
    obj->objectFlags = obj->objectFlags | (OBJECT_OBJFLAG_HITDETECT_DISABLED | OBJECT_OBJFLAG_HIDDEN);
}

void DFP_RotateP_release(void)
{
}

void DFP_RotateP_initialise(void)
{
}

OBJECT_INIT_ADAPTER(gDFP_RotatePObjDescriptorInitAdapter, DFP_RotateP_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gDFP_RotatePObjDescriptorHitDetectAdapter, DFP_RotateP_hitDetect)
OBJECT_RENDER_ADAPTER(gDFP_RotatePObjDescriptorRenderAdapter, DFP_RotateP_render)
OBJECT_TYPE_ID_ADAPTER(gDFP_RotatePObjDescriptorTypeIdAdapter, DFP_RotateP_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gDFP_RotatePObjDescriptorExtraSizeAdapter, DFP_RotateP_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gDFP_RotatePObjDescriptorAcquire, DFP_RotateP_initialise)

ObjectDescriptor gDFP_RotatePObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gDFP_RotatePObjDescriptorAcquire,
        DFP_RotateP_release,
    },
    0,
    gDFP_RotatePObjDescriptorInitAdapter,
    DFP_RotateP_update,
    gDFP_RotatePObjDescriptorHitDetectAdapter,
    gDFP_RotatePObjDescriptorRenderAdapter,
    DFP_RotateP_free,
    gDFP_RotatePObjDescriptorTypeIdAdapter,
    gDFP_RotatePObjDescriptorExtraSizeAdapter,
};
