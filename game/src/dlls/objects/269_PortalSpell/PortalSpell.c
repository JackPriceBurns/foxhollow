#include "dlls/objects/269_PortalSpell.h"

#include "main/dll/dll_80136a40.h"
#include "main/dll/player.h"
#include "main/frame_timing.h"
#include "main/gamebit_ids.h"
#include "main/gamebits.h"
#include "main/object_render.h"
#include "main/objseq.h"
#include "sys/objects.h"
#include "sys/objects/lifecycle.h"

typedef enum PortalSpellIndex {
    PORTAL_SPELL_OPEN_PORTAL = 3
} PortalSpellIndex;

typedef enum PortalSpellDoorFlag {
    PORTAL_SPELL_DOOR_OPEN = 1 << 7
} PortalSpellDoorFlag;

typedef struct PortalSpellDoorState {
    u8 unknown00[4];
    f32 openAmount;
    s32 openTimer;
    u8 flags;
    u8 unknown0D[3];
} PortalSpellDoorState;

int PortalSpellDoor_getExtraSize(void) {
    return sizeof(PortalSpellDoorState);
}

int PortalSpellDoor_getObjectTypeId(void) {
    return 0;
}

void PortalSpellDoor_free(GameObject* obj) {
    (void)obj;
}

void PortalSpellDoor_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

void PortalSpellDoor_hitDetect(void) {
}

void PortalSpellDoor_update(GameObject* obj) {
    PortalSpellDoorState* state;
    GameObject* player;
    const PortalSpellDoorPlacement* placement;

    player = Obj_GetPlayerObject();
    state = obj->extra;
    placement = obj->anim.placementData;
    if (playerHasSpell(player, PORTAL_SPELL_OPEN_PORTAL) != 0) {
        obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_PROMPT_SUPPRESSED;
    } else {
        obj->anim.resetHitboxFlags |= INTERACT_FLAG_PROMPT_SUPPRESSED;
    }
    if (state->flags & PORTAL_SPELL_DOOR_OPEN) {
        obj->anim.flags |= OBJANIM_FLAG_HIDDEN;
        if (objGetAnimState80A(player) == GAMEBIT_STAFF_ABILITY_OPEN_PORTAL) {
            playerCancelSpell(player, -1);
        }
        mainSetBits(ObjAnim_ReadPlacementS16(&obj->anim, &(placement->openedGameBit)), TRUE);
    } else if (objGetAnimState80A(player) == GAMEBIT_STAFF_ABILITY_OPEN_PORTAL &&
               state->openTimer == -1) {
        state->openTimer = 0;
    }
    if (state->openTimer != -1) {
        s32 nextTimer = state->openTimer - framesThisStep;

        state->openTimer = nextTimer;
        if (nextTimer < 0) {
            GameObject* tricky;

            obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
            (*gObjectTriggerInterface)->runSequence(0, obj, -1);
            tricky = getTrickyObject();
            if (tricky != NULL) {
                trickyImpress(tricky);
            }
            state->flags |= PORTAL_SPELL_DOOR_OPEN;
            state->openTimer = -1;
        }
    }
}

void PortalSpellDoor_init(GameObject* obj, const PortalSpellDoorPlacement* placement) {
    PortalSpellDoorState* state = obj->extra;

    obj->anim.rotX = (s16)((s32)placement->rotXByte * 256);
    obj->anim.rotY = (s16)((s32)ObjAnim_ReadPlacementS16(&obj->anim, &placement->rotY) * 256);
    obj->anim.rootMotionScale = 3.1499999f;
    state->openAmount = obj->anim.hitboxScale * obj->anim.rootMotionScale / 2.0f;
    if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &(placement->openedGameBit))) != 0) {
        obj->anim.flags |= OBJANIM_FLAG_HIDDEN;
        obj->objectFlags |= OBJECT_OBJFLAG_UPDATE_DISABLED | OBJECT_OBJFLAG_HIDDEN | OBJECT_OBJFLAG_HITDETECT_DISABLED;
    }
    state->openTimer = -1;
}

void PortalSpellDoor_release(void) {
}

void PortalSpellDoor_initialise(void) {
}

OBJECT_INIT_ADAPTER(gPortalSpellDoorObjDescriptorInitAdapter, PortalSpellDoor_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gPortalSpellDoorObjDescriptorHitDetectAdapter, PortalSpellDoor_hitDetect)
OBJECT_FREE_ADAPTER(gPortalSpellDoorObjDescriptorFreeAdapter, PortalSpellDoor_free, obj)
OBJECT_TYPE_ID_ADAPTER(gPortalSpellDoorObjDescriptorTypeIdAdapter, PortalSpellDoor_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gPortalSpellDoorObjDescriptorExtraSizeAdapter, PortalSpellDoor_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gPortalSpellDoorObjDescriptorAcquire, PortalSpellDoor_initialise)

ObjectDescriptor gPortalSpellDoorObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gPortalSpellDoorObjDescriptorAcquire,
        PortalSpellDoor_release,
    },
    0,
    gPortalSpellDoorObjDescriptorInitAdapter,
    PortalSpellDoor_update,
    gPortalSpellDoorObjDescriptorHitDetectAdapter,
    PortalSpellDoor_render,
    gPortalSpellDoorObjDescriptorFreeAdapter,
    gPortalSpellDoorObjDescriptorTypeIdAdapter,
    gPortalSpellDoorObjDescriptorExtraSizeAdapter,
};
