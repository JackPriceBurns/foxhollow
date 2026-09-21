#include "dlls/objects/431_SH_swaplift.h"

#include "game/objects/object_setup.h"
#include "main/dll/dll_0000_gameui.h"
#include "main/dll/tricky.h"
#include "main/gamebits.h"
#include "main/obj_trigger.h"
#include "main/object_render.h"
#include "main/objtype.h"
#include "sys/objects.h"

typedef enum WarpStoneLiftStateId {
    WARP_STONE_LIFT_STATE_WAITING_FOR_ROCK_CANDY,
    WARP_STONE_LIFT_STATE_ROCK_CANDY_AVAILABLE,
    WARP_STONE_LIFT_STATE_ROCK_CANDY_USED
} WarpStoneLiftStateId;

struct WarpStoneLiftPlacement {
    ObjPlacement base;
    s8 rotationX;
};

typedef struct WarpStoneLiftState {
    u8 stateId;
} WarpStoneLiftState;

STATIC_ASSERT(offsetof(WarpStoneLiftPlacement, rotationX) == 0x18);
STATIC_ASSERT(sizeof(WarpStoneLiftState) == 1);

static const enum GameBitId sWarpStoneLiftStateGameBits[] = {
    GAMEBIT_ITEM_RockCandy_Got,
    GAMEBIT_ITEM_RockCandy_Used,
};

int warpstonelift_getExtraSize(void) {
    return sizeof(WarpStoneLiftState);
}

int warpstonelift_getObjectTypeId(void) {
    return 0;
}

void warpstonelift_free(void) {
}

void warpstonelift_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

void warpstonelift_hitDetect(void) {
}

void warpstonelift_update(GameObject* obj) {
    WarpStoneLiftState* state = obj->extra;
    ObjHitboxTransformState* contactState;
    int foundPlayer = 0;
    int count;
    s16 item;

    contactState = obj->anim.hitboxTransformState;
    count = contactState->contactObjectCount;
    if (count > 0) {
        for (s32 contactIndex = 0; contactIndex < count; contactIndex++) {
            GameObject* other = contactState->contactObjects[contactIndex];
            if (other->anim.classId == OBJECT_CLASS_PLAYER) {
                foundPlayer = 1;
            }
        }
    }
    if (foundPlayer != 0) {
        obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_DISABLED;
        switch (state->stateId) {
        case WARP_STONE_LIFT_STATE_WAITING_FOR_ROCK_CANDY:
        case WARP_STONE_LIFT_STATE_ROCK_CANDY_AVAILABLE:
            getYButtonItem(&item);
            if ((mainGetBit(GAMEBIT_ITEM_RockCandy_Got) != 0 && cMenuGetSelectedItem() != -1) ||
                item == GAMEBIT_ITEM_RockCandy_Got) {
                Obj_SetActiveHitVolumeBounds(obj, 0, 0, 0, 0, 4);
            } else {
                Obj_SetActiveHitVolumeBounds(obj, 0, 0, 0, 0, 2);
            }
            if (ObjTrigger_IsSetById(obj, GAMEBIT_ITEM_RockCandy_Got) != 0) {
                mainSetBits(GAMEBIT_ITEM_RockCandyRelated0886, 1);
                mainSetBits(GAMEBIT_ITEM_RockCandy_Used, 1);
                state->stateId = WARP_STONE_LIFT_STATE_ROCK_CANDY_USED;
                Obj_SetActiveHitVolumeBounds(obj, 0, 0, 0, 0, 3);
            } else if (ObjTrigger_IsSet(obj) != 0) {
                mainSetBits(GAMEBIT_SH_WarpStoneComplainingAboutGifts, 1);
            }
            break;
        case WARP_STONE_LIFT_STATE_ROCK_CANDY_USED:
            if (ObjTrigger_IsSet(obj) != 0) {
                mainSetBits(GAMEBIT_ITEM_RockCandyRelated0886, 1);
            }
            break;
        }
    } else {
        obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
    }
}

void warpstonelift_init(GameObject* obj, const WarpStoneLiftPlacement* placement) {
    WarpStoneLiftState* stateStorage = obj->extra;

    obj->anim.rotX = placement->rotationX * 256;
    obj->userData1 = 0;
    for (s32 gameBitIndex = 0; gameBitIndex < ARRAY_COUNT(sWarpStoneLiftStateGameBits); gameBitIndex++) {
        if (mainGetBit(sWarpStoneLiftStateGameBits[gameBitIndex]) != 0) {
            stateStorage->stateId = gameBitIndex + 1;
        }
    }
    switch (stateStorage->stateId) {
    case WARP_STONE_LIFT_STATE_WAITING_FOR_ROCK_CANDY:
    case WARP_STONE_LIFT_STATE_ROCK_CANDY_USED:
        Obj_SetActiveHitVolumeBounds(obj, 0, 0, 0, 0, 3);
        break;
    case WARP_STONE_LIFT_STATE_ROCK_CANDY_AVAILABLE:
        Obj_SetActiveHitVolumeBounds(obj, 0, 0, 0, 0, 4);
        break;
    }
}

void warpstonelift_release(void) {
}

void warpstonelift_initialise(void) {
}

OBJECT_INIT_ADAPTER(gWarpStoneLiftObjDescriptorInitAdapter, warpstonelift_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gWarpStoneLiftObjDescriptorHitDetectAdapter, warpstonelift_hitDetect)
OBJECT_FREE_ADAPTER(gWarpStoneLiftObjDescriptorFreeAdapter, warpstonelift_free)
OBJECT_TYPE_ID_ADAPTER(gWarpStoneLiftObjDescriptorTypeIdAdapter, warpstonelift_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gWarpStoneLiftObjDescriptorExtraSizeAdapter, warpstonelift_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gWarpStoneLiftObjDescriptorAcquire, warpstonelift_initialise)

ObjectDescriptor gWarpStoneLiftObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gWarpStoneLiftObjDescriptorAcquire,
        warpstonelift_release,
    },
    0,
    gWarpStoneLiftObjDescriptorInitAdapter,
    warpstonelift_update,
    gWarpStoneLiftObjDescriptorHitDetectAdapter,
    warpstonelift_render,
    gWarpStoneLiftObjDescriptorFreeAdapter,
    gWarpStoneLiftObjDescriptorTypeIdAdapter,
    gWarpStoneLiftObjDescriptorExtraSizeAdapter,
};
