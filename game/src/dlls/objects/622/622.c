/*
 * DLL 622 - the swinging chain/shackle that hangs from a
 * path point. drshackle_renderAtPathPoint orients the chain along the segment
 * between two model joints, drshackle_update binds the per-slot path
 * objects (ObjGroup 0x17) the chain rides, and drshackle_hitDetect plays
 * a distance-scaled footstep-style rattle when active.
 *
 * The 0x1A flag byte is a BitFlags8 whose b0 = "active" (chain visible
 * and rattling).
 */
#include "dolphin/mtx/vec.h"
#include "main/objtype.h"
#include "main/object_render.h"
#include "main/objseq.h"
#include "dlls/object_descriptor.h"
#include "dlls/objects/373_DFropenode.h"

#include "main/audio/sfx_trigger_ids.h"

#include "main/dll/DR/dll_026E_drshackle.h"
#include "main/dll/DR/dr_types.h"
#include "main/vecmath.h"
#include "main/audio/sfx.h"
#include "main/gamebits.h"
#include "main/model.h"
#include "main/obj_path.h"
#include "main/objprint_render.h"
#include "sys/objects.h"
#include "dolphin/mtx.h"

int lbl_803DDD70;
int gDrShackleRotZOffset = -32768;

#define DRSHACKLE_OBJGROUP  0x37

static inline ObjModel* DrShackle_GetActiveModel(GameObject* obj)
{
    return Obj_GetActiveModel(obj);
}

int drshackle_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate)
{
    DrshackleState* state = obj->extra;
    GameObject* pathObj = state->pathSlots[0];
    int i;
    if (pathObj != 0)
    {
        pathObj->anim.localPosX = obj->anim.localPosX;
        pathObj->anim.localPosY = obj->anim.localPosY;
        pathObj->anim.localPosZ = obj->anim.localPosZ;
    }
    for (i = 0; i < animUpdate->eventCount; i++)
    {
        switch (animUpdate->eventIds[i])
        {
        case 1:
            state->flags1A.b0 = 0;
            break;
        case 2:
            state->flags1A.b0 = 1;
            break;
        }
    }
    return 0;
}

int drshackle_getAttachSlot(GameObject* obj)
{
    DrshacklePlacement* placement = (DrshacklePlacement*)obj->anim.placementData;
    return placement->attachSlot;
}

int drshackle_renderAtPathPoint(GameObject* obj, GameObject* owner, int b, int c, int d, int e, int f)
{
    ObjModel* model;
    ObjAttachPoint* attachPoint;
    int joint1;
    u8* p = obj->extra;
    DrshacklePlacement* q = (DrshacklePlacement*)obj->anim.placementData;
    f32 jointPos[3];
    f32 parentPos[3];
    int i;
    BitFlags8* bf = &((DrshackleState*)p)->flags1A;
    ObjAnimComponent* objAnim = &obj->anim;
    s16 quarterTurns;

    if (bf->b0 == 0)
    {
        return 1;
    }
    ((DrshackleState*)p)->savedPos.x = obj->anim.localPosX;
    ((DrshackleState*)p)->savedPos.y = obj->anim.localPosY;
    ((DrshackleState*)p)->savedPos.z = obj->anim.localPosZ;

    attachPoint = &owner->anim.modelInstance->attachPoints[b];
    joint1 = attachPoint->joints[objAnim->bankIndex];
    model = DrShackle_GetActiveModel(owner);

    obj->anim.rotZ = 0;
    obj->anim.rotY = 0;
    ObjModel_CopyJointTranslation((u8*)model, joint1, jointPos);
    ObjModel_CopyJointTranslation((u8*)model, *(s8*)(model->file->jointData + joint1 * 28), parentPos);
    PSVECSubtract((Vec*)parentPos, (Vec*)jointPos, (Vec*)jointPos);

    quarterTurns = ObjAnim_ReadPlacementS16(&obj->anim, &q->quarterTurns);
    if (quarterTurns != 0)
    {
        obj->anim.rotZ =
            (s16)((quarterTurns << 14) + getAngle(jointPos[2], jointPos[0]));
        obj->anim.rotY = (s16)getAngle(jointPos[2], jointPos[1]);
    }
    else
    {
        f32 savedY = jointPos[1];
        f32 mag;
        jointPos[1] = 0.0f;
        mag = PSVECMag((Vec*)jointPos);
        obj->anim.rotZ = (s16)(gDrShackleRotZOffset + getAngle(jointPos[0], jointPos[2]));
        obj->anim.rotY = (s16)(lbl_803DDD70 + getAngle(mag, savedY));
        objSetCurrentMatrix((MtxPtr)ObjPath_GetPointModelMtx(owner, b));
    }
    ObjPath_GetPointWorldPosition(owner, b, &obj->anim.localPosX, &obj->anim.localPosY, &obj->anim.localPosZ,
                                  0);
    objRenderModelAndHitVolumes(obj, c, d, e, f, 1.0f);

    for (i = 0; i < ((DrshackleState*)p)->slotCount; i++)
    {
        GameObject* entry = ((DrshackleState*)p)->pathSlots[i];
        if (entry != NULL)
        {
            int pathPoint = i == 0 ? ((DrshackleState*)p)->pathPointA : ((DrshackleState*)p)->pathPointB;
            ObjPath_GetPointWorldPosition(obj, pathPoint, &entry->anim.localPosX,
                                          &entry->anim.localPosY, &entry->anim.localPosZ, 0);
        }
    }
    return 0;
}

int drshackle_getExtraSize(void)
{
    return sizeof(DrshackleState);
}

int drshackle_getObjectTypeId(void)
{
    return 0x0;
}

void drshackle_free(GameObject* obj)
{
    objFreeObjectType(obj, DRSHACKLE_OBJGROUP);
}

void drshackle_render(GameObject* obj, u32 p2, u32 p3, u32 p4, u32 p5, char visible)
{
    u8* state = obj->extra;
    int i;
    if (((DrshackleState*)state)->flags1A.b0 == 0 && visible != 0)
    {
        objRenderModelAndHitVolumes(obj, p2, p3, p4, p5, 1.0f);
        for (i = 0; i < ((DrshackleState*)state)->slotCount; i++)
        {
            GameObject* entry = ((DrshackleState*)state)->pathSlots[i];
            if (entry != 0)
            {
                int pathPoint = i == 0 ? ((DrshackleState*)state)->pathPointA : ((DrshackleState*)state)->pathPointB;
                ObjPath_GetPointWorldPosition(obj, pathPoint, &entry->anim.localPosX,
                                              &entry->anim.localPosY,
                                              &entry->anim.localPosZ, 0);
            }
        }
    }
}

void drshackle_hitDetect(GameObject* obj)
{
    DrshackleState* state = obj->extra;
    if (Sfx_IsPlayingFromObjectChannel(obj, 1) == 0 && state->flags1A.b0 != 0)
    {
        Vec vec;
        int n;
        PSVECSubtract((Vec*)&obj->anim.localPosX, &state->savedPos, &vec);
        n = 0xc8 - (int)(30.0f * PSVECMag(&vec));
        if (randomGetRange(0, (n < 1) ? 1 : ((n > 0xc8) ? 0xc8 : n)) == 0)
        {
            Sfx_PlayFromObject(obj, SFXTRIG_dn_boar1_c_1b3);
        }
    }
}

void drshackle_update(GameObject* obj)
{
    DrshackleState* state = obj->extra;
    DrshacklePlacement* placement = (DrshacklePlacement*)obj->anim.placementData;
    int count;
    int j;
    GameObject** list;
    s16 pathObjGroupBase = ObjAnim_ReadPlacementS16(
        &obj->anim, &placement->pathObjGroupBase);
    s16 activeGameBit = ObjAnim_ReadPlacementS16(
        &obj->anim, &placement->activeGameBit);
    if (pathObjGroupBase != 0 && state->pathSlots[0] == 0)
    {
        list = objGetAllOfType(DFROPENODE_OBJECT_GROUP, &count);
        while (count-- != 0)
        {
            DrshacklePlacement* subPlacement = (DrshacklePlacement*)(*list)->anim.placementData;
            for (j = 0; j < state->slotCount; j++)
            {
                if ((u8)subPlacement->startPathPoint == pathObjGroupBase + j * 4)
                {
                    state->pathSlots[j] = *list;
                    (*gObjectTriggerInterface)->runSequence(0, state->pathSlots[j], -1);
                }
            }
            list++;
        }
    }
    if (state->flags1A.b0 != 0)
    {
        state->flags1A.b0 = (mainGetBit(activeGameBit) == 0);
    }
}

void drshackle_init(GameObject* obj, char* arg)
{
    DrshackleState* state = (obj)->extra;
    DrshacklePlacement* placement = (DrshacklePlacement*)arg;
    s16 activeGameBit = ObjAnim_ReadPlacementS16(
        &obj->anim, &placement->activeGameBit);
    s16 quarterTurns = ObjAnim_ReadPlacementS16(
        &obj->anim, &placement->quarterTurns);
    objAddObjectType(obj, DRSHACKLE_OBJGROUP);
    state->flags1A.b0 = (mainGetBit(activeGameBit) == 0);
    state->pathPointA = placement->startPathPoint % 2;
    (obj)->animEventCallback = drshackle_SeqFn;
    if (quarterTurns == 1)
    {
        state->slotCount = 2;
        state->pathPointB = 1 - state->pathPointA;
    }
    else
    {
        state->slotCount = 1;
    }
}

void drshackle_release(void)
{
}

void drshackle_initialise(void)
{
}

OBJECT_INIT_ADAPTER(gDrShackleObjDescriptorInitAdapter, drshackle_init, obj, placement)
OBJECT_RENDER_ADAPTER(gDrShackleObjDescriptorRenderAdapter, drshackle_render, obj, arg2, arg3, arg4, arg5, visible)
OBJECT_FREE_ADAPTER(gDrShackleObjDescriptorFreeAdapter, drshackle_free, obj)
OBJECT_TYPE_ID_ADAPTER(gDrShackleObjDescriptorTypeIdAdapter, drshackle_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gDrShackleObjDescriptorExtraSizeAdapter, drshackle_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gDrShackleObjDescriptorAcquire, drshackle_initialise)

DrshackleDescriptor gDrShackleObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_12_SLOTS,
        },
        gDrShackleObjDescriptorAcquire,
        drshackle_release,
    },
    {
        0,
        gDrShackleObjDescriptorInitAdapter,
        drshackle_update,
        drshackle_hitDetect,
        gDrShackleObjDescriptorRenderAdapter,
        gDrShackleObjDescriptorFreeAdapter,
        gDrShackleObjDescriptorTypeIdAdapter,
        gDrShackleObjDescriptorExtraSizeAdapter,
        drshackle_renderAtPathPoint,
        drshackle_getAttachSlot,
    },
};
