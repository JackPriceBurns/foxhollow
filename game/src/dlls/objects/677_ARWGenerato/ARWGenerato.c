/*
 * ARWGenerato (DLL 677) - spawner used in the on-rails Arwing flight
 * sections. It holds a single countdown timer (state->spawnTimer, seeded
 * from the placement's spawnInterval) and, when the timer elapses, calls
 * one of two spawn helpers selected by the placement's spawnMode before
 * re-arming the timer.
 *
 * arwgenerato_spawnSquadronShipA / arwgenerato_spawnSquadronShipB are the two spawn modes: each allocates an
 * enemy squadron ship (object id 0x616 / 0x617), scatters it around this
 * object's position by the placement's per-axis spread, then loads it and
 * hands it a direction vector and launch speed via the sibling projectile
 * TUs.
 */
#include "main/frame_timing.h"
#include "main/vecmath.h"
#include "sys/objects.h"
#include "main/dll/ARW/dll_02A2_arwspeedstr.h"
#include "main/dll/ARW/dll_02A3.h"
#include "main/dll/ARW/dll_02A5_arwgenerato.h"
#include "main/dll/ARW/dll_02A6_arwsquadron.h"
#include "main/object_render.h"
#include "sys/objects/lifecycle.h"

/* squadron-ship object ids spawned by the generator's two modes */
#define OBJ_ID_SQUADRON_SHIP_A 0x616
#define OBJ_ID_SQUADRON_SHIP_B 0x617

/* squadron-ship spawn-setup allocation: a truncated ArwSquadronSetup covering
 * only the fields the generator seeds (base.color/pos + rotX..rotZ) */
#define SPAWN_EXTRA_SIZE 0x20

void arwgenerato_spawnSquadronShipA(GameObject* obj, ARWGeneratorState* state, ARWGeneratorSetup* setup)
{
    ArwSquadronSetup* newObj;
    Vec3f dir;

    if (Obj_IsLoadingLocked())
    {
        newObj = (ArwSquadronSetup*)Obj_AllocObjectSetup(SPAWN_EXTRA_SIZE, OBJ_ID_SQUADRON_SHIP_A);
        newObj->base.posX =
            obj->anim.localPosX +
            (f32)randomGetRange(-setup->spreadX, setup->spreadX);
        newObj->base.posY =
            obj->anim.localPosY +
            (f32)randomGetRange(-setup->spreadY, setup->spreadY);
        newObj->base.posZ =
            obj->anim.localPosZ +
            (f32)randomGetRange(-setup->spreadZ, setup->spreadZ);
        newObj->rotZByte = 0;
        newObj->rotYByte = 0;
        newObj->rotXByte = 0;
        newObj->base.color[0] = 1;
        newObj->base.color[1] = 1;
        newObj = (ArwSquadronSetup*)loadObjectAtObject(obj, &newObj->base);
        dir.x = setup->velocityX / 10.0f;
        dir.y = setup->velocityY / 10.0f;
        dir.z = setup->velocityZ / 10.0f;
        dll_2A4_setVelocity((GameObject*)(newObj), &dir);
        dll_2A4_setLifetime((GameObject*)(newObj), ObjAnim_ReadPlacementU16(&obj->anim, &(setup->projectileSpeed)));
    }
}

void arwgenerato_spawnSquadronShipB(GameObject* obj, ARWGeneratorState* state, ARWGeneratorSetup* setup)
{
    ArwSquadronSetup* newObj;
    Vec3f dir;

    if (Obj_IsLoadingLocked())
    {
        newObj = (ArwSquadronSetup*)Obj_AllocObjectSetup(SPAWN_EXTRA_SIZE, OBJ_ID_SQUADRON_SHIP_B);
        newObj->base.posX =
            obj->anim.localPosX +
            (f32)randomGetRange(-setup->spreadX, setup->spreadX);
        newObj->base.posY =
            obj->anim.localPosY +
            (f32)randomGetRange(-setup->spreadY, setup->spreadY);
        newObj->base.posZ =
            obj->anim.localPosZ +
            (f32)randomGetRange(-setup->spreadZ, setup->spreadZ);
        newObj->rotZByte = 0;
        newObj->rotYByte = 0;
        newObj->rotXByte = 0;
        newObj->base.color[0] = 1;
        newObj->base.color[1] = 1;
        newObj = (ArwSquadronSetup*)loadObjectAtObject(obj, &newObj->base);
        dir.x = setup->velocityX / 10.0f;
        dir.y = setup->velocityY / 10.0f;
        dir.z = setup->velocityZ / 10.0f;
        dll_2A3_setVelocity((GameObject*)(newObj), &dir);
        dll_2A3_setLifetime((GameObject*)(newObj), ObjAnim_ReadPlacementU16(&obj->anim, &(setup->projectileSpeed)));
    }
}

int arwgenerato_getExtraSize(void)
{
    return 4;
}

int arwgenerato_getObjectTypeId(void)
{
    return 0;
}

void arwgenerato_free(void)
{
}

void arwgenerato_render(GameObject* obj, int p2, int p3, int p4, int p5, f32 scale)
{
    objRenderModelAndHitVolumes(obj, p2, p3, p4, p5, 1.0f);
}

void arwgenerato_hitDetect(void)
{
}

void arwgenerato_update(GameObject* obj)
{
    ARWGeneratorState* state = (obj)->extra;
    ARWGeneratorSetup* mapData = (ARWGeneratorSetup*)(obj)->anim.placementData;
    f32 timer = state->spawnTimer;
    f32 thr = 0.0f;

    if (timer > thr)
    {
        state->spawnTimer = timer - timeDelta;
        if (state->spawnTimer <= thr)
        {
            switch (mapData->spawnMode)
            {
            case 0:
                arwgenerato_spawnSquadronShipB(obj, state, mapData);
                break;
            case 1:
                arwgenerato_spawnSquadronShipA(obj, state, mapData);
                break;
            }
            state->spawnTimer = (f32)(u32)ObjAnim_ReadPlacementU16(&obj->anim, &(mapData->spawnInterval));
        }
    }
}

void arwgenerato_init(GameObject* obj, ARWGeneratorSetup* setup)
{
    ARWGeneratorState* state = obj->extra;
    ARWGeneratorSetup* mapData = setup;

    state->spawnTimer = (f32)(u32)ObjAnim_ReadPlacementU16(&obj->anim, &(mapData->spawnInterval));
}

void arwgenerato_release(void)
{
}

void arwgenerato_initialise(void)
{
}

OBJECT_INIT_ADAPTER(gARWGeneratoObjDescriptorInitAdapter, arwgenerato_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gARWGeneratoObjDescriptorHitDetectAdapter, arwgenerato_hitDetect)
OBJECT_RENDER_ADAPTER(gARWGeneratoObjDescriptorRenderAdapter, arwgenerato_render, obj, arg2, arg3, arg4, arg5, visible)
OBJECT_FREE_ADAPTER(gARWGeneratoObjDescriptorFreeAdapter, arwgenerato_free)
OBJECT_TYPE_ID_ADAPTER(gARWGeneratoObjDescriptorTypeIdAdapter, arwgenerato_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gARWGeneratoObjDescriptorExtraSizeAdapter, arwgenerato_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gARWGeneratoObjDescriptorAcquire, arwgenerato_initialise)

ObjectDescriptor gARWGeneratoObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gARWGeneratoObjDescriptorAcquire,
        arwgenerato_release,
    },
    0,
    gARWGeneratoObjDescriptorInitAdapter,
    arwgenerato_update,
    gARWGeneratoObjDescriptorHitDetectAdapter,
    gARWGeneratoObjDescriptorRenderAdapter,
    gARWGeneratoObjDescriptorFreeAdapter,
    gARWGeneratoObjDescriptorTypeIdAdapter,
    gARWGeneratoObjDescriptorExtraSizeAdapter,
};
