#include "main/dll/dll_0243_dbholecontrol1.h"
#include "main/dll/baddie_state.h"
#include "main/dll/dbholecontrol1state_struct.h"
#include "main/gamebit_ids.h"
#include "main/gamebits.h"
#include "main/lightmap.h"
#include "main/obj_message.h"
#include "main/object_render.h"
#include "main/object_update_list.h"
#include "main/objseq.h"
#include "main/objtype.h"
#include "string.h"
#include "sys/objects.h"
#include "sys/objects/lifecycle.h"

typedef enum DbHoleControlSequenceEvent {
    DB_HOLE_CONTROL_SEQUENCE_EVENT_SPAWN_CHILD = 1
} DbHoleControlSequenceEvent;

typedef enum DbHoleControlObjectId {
    DB_HOLE_CONTROL_CHILD_OBJECT_ID = 1337
} DbHoleControlObjectId;

typedef enum DbHoleControlEggObjectGroup {
    DB_HOLE_CONTROL_EGG_OBJECT_GROUP = 0x24
} DbHoleControlEggObjectGroup;

typedef struct DbHoleControlChildPlacement {
    GroundBaddiePlacement placement;
    u8 unknown34[4];
} DbHoleControlChildPlacement;

STATIC_ASSERT(sizeof(DbHoleControlChildPlacement) == 0x38);

int lbl_803DDCE0;

int dbholecontrol1_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate) {
    const DbHoleControl1Placement* placement = obj->anim.placementData;

    for (s32 eventIndex = 0; eventIndex < animUpdate->eventCount; eventIndex++) {
        switch (animUpdate->eventIds[eventIndex]) {
        case DB_HOLE_CONTROL_SEQUENCE_EVENT_SPAWN_CHILD: {
            const DbHoleControlChildPlacement* childTemplate;
            DbHoleControlChildPlacement* childPlacement;

            if (mainGetBit((s32)placement->triggerSequenceId + 2601) != 0) {
                continue;
            }
            if (Obj_IsLoadingLocked() == 0) {
                continue;
            }
            childTemplate = mapRomListFindItem(0x4658A, 0, 0, 0, 0);
            if (childTemplate == NULL) {
                continue;
            }
            childPlacement = Obj_AllocObjectSetup(sizeof(*childPlacement), DB_HOLE_CONTROL_CHILD_OBJECT_ID);
            memcpy(childPlacement, childTemplate, sizeof(*childPlacement));
            childPlacement->placement.gameBitA = fhReadBES16(&childPlacement->placement.gameBitA);
            childPlacement->placement.gameBitC = fhReadBES16(&childPlacement->placement.gameBitC);
            childPlacement->placement.gameBitD = fhReadBES16(&childPlacement->placement.gameBitD);
            childPlacement->placement.soundIdB = fhReadBES16(&childPlacement->placement.soundIdB);
            childPlacement->placement.soundIdA = fhReadBES16(&childPlacement->placement.soundIdA);
            childPlacement->placement.triggerId = fhReadBES16(&childPlacement->placement.triggerId);
            childPlacement->placement.unk24 = fhReadBES16(&childPlacement->placement.unk24);
            childPlacement->placement.respawnDelay = fhReadBES16(&childPlacement->placement.respawnDelay);
            childPlacement->placement.gameBitB = fhReadBES16(&childPlacement->placement.gameBitB);
            childPlacement->placement.base.posX = obj->anim.localPosX;
            childPlacement->placement.base.posY = obj->anim.localPosY;
            childPlacement->placement.base.posZ = obj->anim.localPosZ;
            childPlacement->placement.base.ident = -1;
            childPlacement->placement.gameBitC = GAMEBIT_Always1;
            loadObjectAtObject(obj, &childPlacement->placement.base);
            break;
        }
        }
    }

    if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->hideGameBit)) != 0 || lbl_803DDCE0 != 0) {
        s32 objectCount;
        GameObject** objects = objGetAllOfType(DB_HOLE_CONTROL_EGG_OBJECT_GROUP, &objectCount);

        ObjMsg_SendToObjects(0, 3, obj, 17, 0);
        for (s32 objectIndex = 0; objectIndex < objectCount; objectIndex++) {
            objFreeObjectType(objects[objectIndex], DB_HOLE_CONTROL_EGG_OBJECT_GROUP);
        }
        return 4;
    }
    return 0;
}

int dbholecontrol1_getExtraSize(void) {
    return sizeof(DbHoleControl1State);
}
int dbholecontrol1_getObjectTypeId(void) {
    return 0;
}

void dbholecontrol1_free(GameObject* obj) {
    objFreeObjectType(obj, DBHOLE_CONTROL1_OBJECT_GROUP);
}

void dbholecontrol1_render(GameObject* obj, int p2, int p3, int p4, int p5, s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, p2, p3, p4, p5, 1.0f);
    }
}

void dbholecontrol1_hitDetect(void) {
}

void dbholecontrol1_update(GameObject* obj) {

    const DbHoleControl1Placement* placement = obj->anim.placementData;

    if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->hideGameBit)) != 0) {
        Obj_RemoveFromUpdateList(obj);
        obj->anim.flags |= OBJANIM_FLAG_HIDDEN;
    } else if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->triggerGameBit)) != 0) {
        (*gObjectTriggerInterface)->runSequence(placement->triggerSequenceId, obj, -1);
    }
}

void dbholecontrol1_init(GameObject* obj, const DbHoleControl1Placement* placement) {
    DbHoleControl1State* state = obj->extra;

    objAddObjectType(obj, DBHOLE_CONTROL1_OBJECT_GROUP);
    obj->anim.rotX = (s16)((s32)placement->rotXByte * 256);
    obj->animEventCallback = dbholecontrol1_SeqFn;
    state->gameBitA = ObjAnim_ReadPlacementS16(&obj->anim, &placement->gameBitA);
    state->gameBitB = ObjAnim_ReadPlacementS16(&obj->anim, &placement->gameBitB);
}

void dbholecontrol1_release(void) {
}

void dbholecontrol1_initialise(void) {
}

OBJECT_INIT_ADAPTER(gDBHoleControl1ObjDescriptorInitAdapter, dbholecontrol1_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gDBHoleControl1ObjDescriptorHitDetectAdapter, dbholecontrol1_hitDetect)
OBJECT_FREE_ADAPTER(gDBHoleControl1ObjDescriptorFreeAdapter, dbholecontrol1_free, obj)
OBJECT_TYPE_ID_ADAPTER(gDBHoleControl1ObjDescriptorTypeIdAdapter, dbholecontrol1_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gDBHoleControl1ObjDescriptorExtraSizeAdapter, dbholecontrol1_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gDBHoleControl1ObjDescriptorAcquire, dbholecontrol1_initialise)

ObjectDescriptor gDBHoleControl1ObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gDBHoleControl1ObjDescriptorAcquire,
        dbholecontrol1_release,
    },
    0,
    gDBHoleControl1ObjDescriptorInitAdapter,
    dbholecontrol1_update,
    gDBHoleControl1ObjDescriptorHitDetectAdapter,
    dbholecontrol1_render,
    gDBHoleControl1ObjDescriptorFreeAdapter,
    gDBHoleControl1ObjDescriptorTypeIdAdapter,
    gDBHoleControl1ObjDescriptorExtraSizeAdapter,
};
