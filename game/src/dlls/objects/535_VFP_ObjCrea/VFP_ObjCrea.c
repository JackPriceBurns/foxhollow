/*
 * vfpobjcreator (DLL 0x217, VFP_ObjCreator) - an invisible spawner
 * object in the Volcano Force Point Temple.
 *
 * While loading is locked it periodically allocates and launches one of
 * two kinds of object setup, selected by the placement's spawnMode:
 *  - mode 1 (falling): spawns object 0x263 within a random radius around
 *    the spawner, gated on an optional game bit, with randomised spin
 *    and downward/outward velocity;
 *  - mode 6 (projectile): spawns object 0x549 burst at the spawner's
 *    position, aimed by the spawner's pitch, with launch sfx + particle
 *    trails.
 * The spawn cadence is driven by spawnTimer counting down spawnInterval.
 */
#include "dlls/object_descriptor.h"
#include "main/audio/sfx.h"
#include "main/dll/partfx_interface.h"
#include "main/vecmath.h"
#include "sys/objects/lifecycle.h"
#include "main/frame_timing.h"
#include "main/gamebits.h"
#include "sys/objects.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/dll/VF/dll_0217_vfpobjcreator.h"

#define VFP_OBJCREATOR_FALLING_MODE    1
#define VFP_OBJCREATOR_PROJECTILE_MODE 6

#define VFP_OBJCREATOR_FALLING_OBJECT_ID    0x263
#define VFP_OBJCREATOR_PROJECTILE_OBJECT_ID 0x549


int VFP_ObjCreator_getExtraSize(void)
{
    return 0xa;
}

int VFP_ObjCreator_getObjectTypeId(void)
{
    return 0x0;
}

void VFP_ObjCreator_free(void)
{
}

void VFP_ObjCreator_render(GameObject* obj, int p2, int p3, int p4, int p5, s8 visible)
{
    if (visible == 0)
        return;
}

void VFP_ObjCreator_hitDetect(void)
{
}

void VFP_ObjCreator_update(GameObject* obj)
{
    VfpObjCreatorPlacement* placement = (VfpObjCreatorPlacement*)obj->anim.placementData;
    VfpObjCreatorState* state = obj->extra;

    if (Obj_IsLoadingLocked() == 0)
    {
        return;
    }
    switch (ObjAnim_ReadPlacementS16(&obj->anim, &(placement->spawnMode)))
    {
    case 0:
        break;
    case VFP_OBJCREATOR_FALLING_MODE:
        if (mainGetBit(state->gameBit) == 0 && state->gameBit != -1)
        {
            break;
        }
        state->spawnTimer -= (s16)timeDelta;
        if (state->spawnTimer <= 0)
        {
            VfpObjCreatorSetup* setupBuf;
            GameObject* spawned;
            state->spawnTimer = state->spawnInterval;
            setupBuf = (VfpObjCreatorSetup*)Obj_AllocObjectSetup(0x28, VFP_OBJCREATOR_FALLING_OBJECT_ID);
            setupBuf->base.color[2] = 0xff;
            setupBuf->base.color[3] = 0xff;
            setupBuf->base.color[0] = 2;
            setupBuf->base.color[1] = 1;
            setupBuf->base.posX =
                obj->anim.localPosX + (f32)randomGetRange(-state->spawnRadius, state->spawnRadius);
            setupBuf->base.posY = obj->anim.localPosY;
            setupBuf->base.posZ =
                obj->anim.localPosZ + (f32)randomGetRange(-state->spawnRadius, state->spawnRadius);
            setupBuf->unk20 = 0x50;
            setupBuf->unk1E = (s16)(randomGetRange(0, 2) + 0x16a);
            setupBuf->unk22 = -1;
            setupBuf->unk18 = (s16)(randomGetRange(-0x1f4, 0x1f4) + 0x5dc);
            setupBuf->unk1A = (s16)(randomGetRange(-0x1f4, 0x1f4) + 0x5dc);
            setupBuf->unk1C = (s16)(randomGetRange(-0x1f4, 0x1f4) + 0x5dc);
            setupBuf->unk24 = 0;
            spawned = objSetupObject(&setupBuf->base, 5, obj->anim.mapEventSlot, -1,
                                      obj->anim.parent);
            if (spawned == NULL)
            {
                break;
            }
            spawned->anim.velocityY = 0.01f * (f32)randomGetRange(0, 10) + 0.1f;
            spawned->anim.velocityX = 0.2f * (f32)randomGetRange(-10, 10);
            spawned->anim.velocityZ = 0.2f * (f32)randomGetRange(-10, 10);
        }
        break;
    case VFP_OBJCREATOR_PROJECTILE_MODE:
        state->spawnTimer -= (s16)timeDelta;
        if (state->spawnTimer <= 0)
        {
            VfpObjCreatorSetup* setupBuf;
            GameObject* spawned;
            struct
            {
                s16 ang[3];
                f32 v[4];
            } launch;
            state->spawnTimer = state->spawnInterval;
            setupBuf = (VfpObjCreatorSetup*)Obj_AllocObjectSetup(0x24, VFP_OBJCREATOR_PROJECTILE_OBJECT_ID);
            setupBuf->base.posX = placement->base.posX;
            setupBuf->base.posY = placement->base.posY;
            setupBuf->base.posZ = placement->base.posZ;
            setupBuf->base.color[0] = placement->base.color[0];
            setupBuf->base.color[1] = placement->base.color[1];
            setupBuf->base.color[2] = placement->base.color[2];
            setupBuf->base.color[3] = placement->base.color[3];
            setupBuf->unk1E = -1;
            setupBuf->unk20 = -1;
            spawned = objSetupObject(&setupBuf->base, 5, obj->anim.mapEventSlot, -1,
                                      obj->anim.parent);
            if (spawned == NULL)
            {
                break;
            }
            spawned->userData2 = 0x1f4;
            {
                f32 vz;
                f32 vxy = 0.0f;
                spawned->anim.velocityY = vxy;
                spawned->anim.velocityX = vxy;
                vz = 1.0f;
                spawned->anim.velocityZ = vz;
                launch.v[1] = vxy;
                launch.v[2] = vxy;
                launch.v[3] = vxy;
                launch.v[0] = vz;
            }
            launch.ang[2] = 0;
            launch.ang[1] = 0;
            launch.ang[0] = obj->anim.rotX;
            vecRotateZXY(launch.ang, &spawned->anim.velocityX);
            Sfx_PlayFromObject(spawned, SFXTRIG_id_10c);
            (*gPartfxInterface)->spawnObject(spawned, 0x39a, NULL, 0x10002, -1, NULL);
            (*gPartfxInterface)->spawnObject(spawned, 0x39b, NULL, 0x10002, -1, NULL);
            (*gPartfxInterface)->spawnObject(spawned, 0x39c, NULL, 0x10002, -1, NULL);
        }
        break;
    }
}

void VFP_ObjCreator_init(GameObject* obj, u8* init)
{
    VfpObjCreatorPlacement* placement = (VfpObjCreatorPlacement*)init;
    VfpObjCreatorState* state = obj->extra;
    obj->anim.rotX = (s16)(placement->rotXByte << 8);
    state->gameBit = ObjAnim_ReadPlacementS16(&obj->anim, &(placement->gameBit));
    state->spawnInterval = ObjAnim_ReadPlacementS16(&obj->anim, &(placement->spawnInterval));
    state->spawnTimer = state->spawnInterval;
    state->spawnParam = placement->spawnParam;
    state->spawnRadius = placement->spawnRadius;
    obj->objectFlags |= OBJECT_OBJFLAG_HITDETECT_DISABLED;
}

void VFP_ObjCreator_release(void)
{
}

void VFP_ObjCreator_initialise(void)
{
}

OBJECT_INIT_ADAPTER(gVFP_ObjCreatorObjDescriptorInitAdapter, VFP_ObjCreator_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gVFP_ObjCreatorObjDescriptorHitDetectAdapter, VFP_ObjCreator_hitDetect)
OBJECT_FREE_ADAPTER(gVFP_ObjCreatorObjDescriptorFreeAdapter, VFP_ObjCreator_free)
OBJECT_TYPE_ID_ADAPTER(gVFP_ObjCreatorObjDescriptorTypeIdAdapter, VFP_ObjCreator_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gVFP_ObjCreatorObjDescriptorExtraSizeAdapter, VFP_ObjCreator_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gVFP_ObjCreatorObjDescriptorAcquire, VFP_ObjCreator_initialise)

ObjectDescriptor gVFP_ObjCreatorObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gVFP_ObjCreatorObjDescriptorAcquire,
        VFP_ObjCreator_release,
    },
    0,
    gVFP_ObjCreatorObjDescriptorInitAdapter,
    VFP_ObjCreator_update,
    gVFP_ObjCreatorObjDescriptorHitDetectAdapter,
    VFP_ObjCreator_render,
    gVFP_ObjCreatorObjDescriptorFreeAdapter,
    gVFP_ObjCreatorObjDescriptorTypeIdAdapter,
    gVFP_ObjCreatorObjDescriptorExtraSizeAdapter,
};
