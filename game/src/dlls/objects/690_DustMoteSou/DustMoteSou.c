#include "main/dll_000A_expgfx.h"
#include "main/dll/dll_02B2_dustmotesou.h"
#include "game/objects/object_setup.h"
#include "main/gamebits.h"
#include "main/objfx.h"

typedef enum DustMoteSourceVariant {
    DUST_MOTE_SOURCE_TAIL_LIGHT = 0x807,
    DUST_MOTE_SOURCE_FIREWORK = 0x80E
} DustMoteSourceVariant;

typedef enum DustMoteSourceBurstMode {
    DUST_MOTE_BURST_BOX,
    DUST_MOTE_BURST_ARCED
} DustMoteSourceBurstMode;

struct DustMoteSourcePlacement {
    ObjPlacement base;
    u8 rotationZ;
    u8 rotationY;
    u8 rotationX;
    u8 effectId;
    u8 effectParamA;
    u8 effectParamB;
    u8 pad1E[2];
    f32 scale;
    s16 gameBit;
    u8 spreadX;
    u8 spreadY;
    u8 spreadZ;
    u8 spawnChance;
    u8 burstMode;
    u8 pad2B[5];
};

STATIC_ASSERT(offsetof(DustMoteSourcePlacement, rotationZ) == 0x18);
STATIC_ASSERT(offsetof(DustMoteSourcePlacement, effectId) == 0x1B);
STATIC_ASSERT(offsetof(DustMoteSourcePlacement, scale) == 0x20);
STATIC_ASSERT(offsetof(DustMoteSourcePlacement, gameBit) == 0x24);
STATIC_ASSERT(offsetof(DustMoteSourcePlacement, spreadX) == 0x26);
STATIC_ASSERT(offsetof(DustMoteSourcePlacement, spawnChance) == 0x29);
STATIC_ASSERT(offsetof(DustMoteSourcePlacement, burstMode) == 0x2A);
STATIC_ASSERT(sizeof(DustMoteSourcePlacement) == 0x30);

int dustmotesou_getExtraSize(void)
{
    return 0;
}

int dustmotesou_getObjectTypeId(void)
{
    return 0;
}

void dustmotesou_free(GameObject* obj)
{
    (*gExpgfxInterface)->freeSource2((uintptr_t)obj);
}

void dustmotesou_render(GameObject* obj, int p2, int p3, int p4, int p5, s8 visible)
{
    if (visible == 0)
    {
        return;
    }
}

void dustmotesou_hitDetect(void)
{
}

void dustmotesou_update(GameObject* source)
{
    const DustMoteSourcePlacement* placement = (const DustMoteSourcePlacement*)source->anim.placementData;
    f32 scale = ObjAnim_ReadPlacementF32(&source->anim, &placement->scale);
    s16 gameBit = ObjAnim_ReadPlacementS16(&source->anim, &placement->gameBit);

    if (gameBit != -1 && mainGetBit(gameBit) == 0)
    {
        return;
    }
    if (source->anim.romDefNo == DUST_MOTE_SOURCE_TAIL_LIGHT)
    {
        if (placement->effectId == 0 || placement->effectParamA == 0)
        {
            return;
        }
        objfx_spawnMaskedHitEffect(source, scale, placement->effectId, placement->effectParamA,
                                   placement->effectParamB, NULL);
        return;
    }
    if (source->anim.romDefNo == DUST_MOTE_SOURCE_FIREWORK)
    {
        if (placement->effectId == 0 || placement->effectParamA == 0)
        {
            return;
        }
        objfx_spawnHitEffectBurst(source, scale, placement->effectId, placement->effectParamA,
                                  placement->effectParamB, NULL);
        return;
    }
    if (placement->effectId == 0 || placement->effectParamA == 0 || placement->effectParamB == 0)
    {
        return;
    }
    if (placement->burstMode == DUST_MOTE_BURST_BOX)
    {
        objfx_spawnBoxBurst(source, placement->effectId, scale, placement->effectParamA,
                            placement->effectParamB, placement->spawnChance, placement->spreadX,
                            placement->spreadY, placement->spreadZ, NULL, 0);
    }
    else if (placement->burstMode == DUST_MOTE_BURST_ARCED)
    {
        objfx_spawnArcedBurst(source, placement->effectId, scale, placement->effectParamA,
                             placement->effectParamB, placement->spawnChance, placement->spreadX,
                             placement->spreadY, placement->spreadZ, NULL, 0);
    }
    else
    {
        objfx_spawnDirectionalBurst(source, placement->effectId, scale, placement->effectParamA,
                                    placement->effectParamB, placement->spawnChance, placement->spreadX, NULL, 0);
    }
}

void dustmotesou_init(GameObject* source, const DustMoteSourcePlacement* placement)
{
    source->anim.rotZ = placement->rotationZ * 256;
    source->anim.rotY = placement->rotationY * 256;
    source->anim.rotX = placement->rotationX * 256;
    source->objectFlags |= OBJECT_OBJFLAG_HITDETECT_DISABLED;
}

void dustmotesou_release(void)
{
}

void dustmotesou_initialise(void)
{
}

OBJECT_INIT_ADAPTER(gDustMoteSouObjDescriptorInitAdapter, dustmotesou_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gDustMoteSouObjDescriptorHitDetectAdapter, dustmotesou_hitDetect)
OBJECT_FREE_ADAPTER(gDustMoteSouObjDescriptorFreeAdapter, dustmotesou_free, obj)
OBJECT_TYPE_ID_ADAPTER(gDustMoteSouObjDescriptorTypeIdAdapter, dustmotesou_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gDustMoteSouObjDescriptorExtraSizeAdapter, dustmotesou_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gDustMoteSouObjDescriptorAcquire, dustmotesou_initialise)

ObjectDescriptor gDustMoteSouObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gDustMoteSouObjDescriptorAcquire,
        dustmotesou_release,
    },
    0,
    gDustMoteSouObjDescriptorInitAdapter,
    dustmotesou_update,
    gDustMoteSouObjDescriptorHitDetectAdapter,
    dustmotesou_render,
    gDustMoteSouObjDescriptorFreeAdapter,
    gDustMoteSouObjDescriptorTypeIdAdapter,
    gDustMoteSouObjDescriptorExtraSizeAdapter,
};
