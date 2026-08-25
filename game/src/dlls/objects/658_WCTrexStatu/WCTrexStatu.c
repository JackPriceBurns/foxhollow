#include "main/dll/partfx_interface.h"
#include "main/dll/WC/dll_0292_wctrexstatu.h"
#include "game/objects/object_setup.h"
#include "main/gamebits.h"
#include "main/mapEventTypes.h"
#include "main/objtexture.h"
#include "main/object_render.h"
#include "main/vecmath.h"
#include "main/objseq.h"

typedef enum WcTrexStatueEvent {
    WC_TREX_STATUE_EVENT_TRIGGER = 1
} WcTrexStatueEvent;

typedef enum WcTrexStatueTextureId {
    WC_TREX_STATUE_TEXTURE_TRIGGERED = 0x100
} WcTrexStatueTextureId;

typedef enum WcTrexStatueParticleId {
    WC_TREX_STATUE_PARTICLE_BANK_0 = 0x73F,
    WC_TREX_STATUE_PARTICLE_BANK_1 = 0x740
} WcTrexStatueParticleId;

typedef enum WcTrexStatueMapAct {
    WC_TREX_STATUE_MAP_ACT_RAISED = 2
} WcTrexStatueMapAct;

struct WcTrexStatuePlacement {
    ObjPlacement base;
    s8 rotationX;
    s8 modelIndex;
    u8 pad1A[4];
    s16 raisedGameBit;
    u8 pad20[4];
};

STATIC_ASSERT(offsetof(WcTrexStatuePlacement, rotationX) == 0x18);
STATIC_ASSERT(offsetof(WcTrexStatuePlacement, modelIndex) == 0x19);
STATIC_ASSERT(offsetof(WcTrexStatuePlacement, raisedGameBit) == 0x1E);
STATIC_ASSERT(sizeof(WcTrexStatuePlacement) == 0x24);

static void wctrexstatu_setTriggered(GameObject* obj) {
    ObjTextureRuntimeSlot* texture = objFindTexture(obj, 0, 0);

    if (texture != NULL) {
        texture->textureId = WC_TREX_STATUE_TEXTURE_TRIGGERED;
    }
    obj->userData1 = 1;
}

int wctrexstatu_interactCallback(GameObject* obj, int unused, ObjSeqState* animUpdate)
{
    for (s32 eventIndex = 0; eventIndex < animUpdate->eventCount; eventIndex++)
    {
        if (animUpdate->eventIds[eventIndex] == WC_TREX_STATUE_EVENT_TRIGGER)
        {
            wctrexstatu_setTriggered(obj);
        }
    }

    return 0;
}

int wctrexstatu_getExtraSize(void)
{
    return 0;
}

int wctrexstatu_getObjectTypeId(GameObject* obj)
{
    const WcTrexStatuePlacement* placement = (const WcTrexStatuePlacement*)obj->anim.placementData;
    int modelIndex = placement->modelIndex;
    int modelCount = obj->anim.modelInstance->modelCount;

    if (modelIndex >= modelCount)
    {
        modelIndex = 0;
    }
    return modelIndex * 0x800 | 0x400;
}

void wctrexstatu_free(void)
{
}

void wctrexstatu_render(GameObject* obj, int p2, int p3, int p4, int p5, s8 visible)
{
    if (visible != 0)
    {
        objRenderModelAndHitVolumes(obj, p2, p3, p4, p5, 1.0f);
    }
}

void wctrexstatu_hitDetect(GameObject* obj)
{
    if (obj->userData1 != 0 && randomGetRange(0, 5) == 0)
    {
        WcTrexStatueParticleId particleId = obj->anim.bankIndex == 0 ? WC_TREX_STATUE_PARTICLE_BANK_0
                                                                    : WC_TREX_STATUE_PARTICLE_BANK_1;
        (*gPartfxInterface)->spawnObject(obj, particleId, NULL, 2, -1, obj);
    }
}

void wctrexstatu_update(void)
{
}

void wctrexstatu_init(GameObject* obj, const WcTrexStatuePlacement* placement, int fromLoad)
{
    obj->animEventCallback = wctrexstatu_interactCallback;
    obj->anim.bankIndex = placement->modelIndex;
    if (obj->anim.bankIndex >= obj->anim.modelInstance->modelCount)
    {
        obj->anim.bankIndex = 0;
    }

    obj->anim.rotX = placement->rotationX * 256;
    if (fromLoad == 0)
    {
        if ((*gMapEventInterface)->getMapAct(obj->anim.mapEventSlot) == WC_TREX_STATUE_MAP_ACT_RAISED)
        {
            obj->anim.localPosY += 30.0f;
        }
    }

    if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->raisedGameBit)) != 0)
    {
        wctrexstatu_setTriggered(obj);
    }
}

void wctrexstatu_release(void)
{
}

void wctrexstatu_initialise(void)
{
}

OBJECT_INIT_ADAPTER(gWCTrexStatuObjDescriptorInitAdapter, wctrexstatu_init, obj, placement, flags)
OBJECT_UPDATE_ADAPTER(gWCTrexStatuObjDescriptorUpdateAdapter, wctrexstatu_update)
OBJECT_FREE_ADAPTER(gWCTrexStatuObjDescriptorFreeAdapter, wctrexstatu_free)
OBJECT_EXTRA_SIZE_ADAPTER(gWCTrexStatuObjDescriptorExtraSizeAdapter, wctrexstatu_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gWCTrexStatuObjDescriptorAcquire, wctrexstatu_initialise)

ObjectDescriptor gWCTrexStatuObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gWCTrexStatuObjDescriptorAcquire,
        wctrexstatu_release,
    },
    0,
    gWCTrexStatuObjDescriptorInitAdapter,
    gWCTrexStatuObjDescriptorUpdateAdapter,
    wctrexstatu_hitDetect,
    wctrexstatu_render,
    gWCTrexStatuObjDescriptorFreeAdapter,
    wctrexstatu_getObjectTypeId,
    gWCTrexStatuObjDescriptorExtraSizeAdapter,
};
