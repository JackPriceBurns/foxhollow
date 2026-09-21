#include "dlls/objects/394_CCpedstal.h"

#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/gameloop_gamebit.h"
#include "main/gamebit_ids.h"
#include "main/gamebits.h"
#include "main/objseq.h"
#include "main/obj_trigger.h"
#include "sys/objects.h"

enum CcPedestalPlacementId {
    CC_PEDESTAL_FIRE_GEM_SOURCE = 0x45F1A,
    CC_PEDESTAL_FIRE_GEM_GATE_A = 0x45F1B,
    CC_PEDESTAL_FIRE_GEM_GATE_B = 0x45F1C,
};

enum CcPedestalSequence {
    CC_PEDESTAL_GATE_SEQUENCE,
    CC_PEDESTAL_SOURCE_SEQUENCE,
};

enum CcPedestalModelIndex {
    CC_PEDESTAL_GATE_INACTIVE_MODEL = 0,
    CC_PEDESTAL_GATE_ACTIVE_MODEL = 1,
    CC_PEDESTAL_SOURCE_ACTIVE_MODEL = 0,
    CC_PEDESTAL_SOURCE_IDLE_MODEL = 1,
};

enum CcPedestalTriggerId {
    CC_PEDESTAL_FIRE_GEM_TRIGGER = 0xA9,
};

enum CcPedestalPendingGameBitFlag {
    CC_PEDESTAL_PENDING_GAMEBIT_SET = 1,
};

typedef struct CcPedestalPlacement {
    ObjPlacement base;
    u8 pad18[2];
    u8 rotXByte;
    u8 pad1B[5];
} CcPedestalPlacement;

STATIC_ASSERT(sizeof(CcPedestalPlacement) == 0x20);
STATIC_ASSERT(offsetof(CcPedestalPlacement, rotXByte) == 0x1A);

typedef struct CcPedestalState CcPedestalState;
typedef void (*CcPedestalVariantUpdate)(GameObject* obj, CcPedestalState* state);

struct CcPedestalState {
    CcPedestalVariantUpdate variantUpdate;
    s16 activationGameBit;
    u8 pendingGameBitFlags;
    u8 pad0B[5];
};

STATIC_ASSERT(sizeof(CcPedestalState) == 0x10);
STATIC_ASSERT(offsetof(CcPedestalState, activationGameBit) == 0x08);
STATIC_ASSERT(offsetof(CcPedestalState, pendingGameBitFlags) == 0x0A);

static int ccPedestal_getExtraSize(void) {
    return sizeof(CcPedestalState);
}

static void ccPedestal_updateFireGemGate(GameObject* obj, CcPedestalState* state) {
    if (mainGetBit(state->activationGameBit) != 0) {
        obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
        Obj_SetActiveModelIndex(obj, CC_PEDESTAL_GATE_ACTIVE_MODEL);
    } else {
        Obj_SetActiveModelIndex(obj, CC_PEDESTAL_GATE_INACTIVE_MODEL);
        if (mainGetBit(GAMEBIT_ITEM_FireGem_Count) != 0) {
            obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_PROMPT_SUPPRESSED;
            if (ObjTrigger_IsSetById(obj, CC_PEDESTAL_FIRE_GEM_TRIGGER) != 0) {
                (*gObjectTriggerInterface)->runSequence(CC_PEDESTAL_GATE_SEQUENCE, obj, -1);
                gameBitDecrement(GAMEBIT_ITEM_FireGem_Count);
                state->pendingGameBitFlags |= CC_PEDESTAL_PENDING_GAMEBIT_SET;
            }
        } else {
            obj->anim.resetHitboxFlags |= INTERACT_FLAG_PROMPT_SUPPRESSED;
        }
    }
}

static void ccPedestal_updateFireGemSource(GameObject* obj, CcPedestalState* state) {
    if (mainGetBit(0xDC5) != 0) {
        obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
    } else {
        obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_DISABLED;
    }
    if (mainGetBit(state->activationGameBit) != 0) {
        obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
        Obj_SetActiveModelIndex(obj, CC_PEDESTAL_SOURCE_ACTIVE_MODEL);
    } else {
        Obj_SetActiveModelIndex(obj, CC_PEDESTAL_SOURCE_IDLE_MODEL);
        if (ObjTrigger_IsSet(obj) != 0) {
            (*gObjectTriggerInterface)->runSequence(CC_PEDESTAL_SOURCE_SEQUENCE, obj, -1);
            gameBitIncrement(GAMEBIT_ITEM_FireGem_Count);
            state->pendingGameBitFlags |= CC_PEDESTAL_PENDING_GAMEBIT_SET;
        }
    }
}

static void ccPedestal_update(GameObject* obj) {
    CcPedestalState* state = obj->extra;

    if (state->pendingGameBitFlags != 0) {
        mainSetBits(state->activationGameBit,
                    (state->pendingGameBitFlags & CC_PEDESTAL_PENDING_GAMEBIT_SET) != 0);
        state->pendingGameBitFlags = 0;
        if (mainGetBit(0xDF0) == 0 && mainGetBit(GAMEBIT_CC_FireGemSourceActivated) != 0) {
            mainSetBits(0xDF0, 1);
        }
    }
    state->variantUpdate(obj, state);
}

static void ccPedestal_init(GameObject* obj, const CcPedestalPlacement* placement) {
    CcPedestalState* state = obj->extra;

    obj->anim.rotX = (s16)(placement->rotXByte << 8);
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN;
    switch (placement->base.ident) {
    case CC_PEDESTAL_FIRE_GEM_SOURCE:
        state->variantUpdate = ccPedestal_updateFireGemSource;
        state->activationGameBit = GAMEBIT_CC_FireGemSourceActivated;
        Obj_SetActiveHitVolumeBounds(obj, 0, 0, 0, 0, 3);
        break;
    case CC_PEDESTAL_FIRE_GEM_GATE_A:
        state->variantUpdate = ccPedestal_updateFireGemGate;
        state->activationGameBit = GAMEBIT_CC_FireGemGateAActivated;
        break;
    case CC_PEDESTAL_FIRE_GEM_GATE_B:
        state->variantUpdate = ccPedestal_updateFireGemGate;
        state->activationGameBit = GAMEBIT_CC_FireGemGateBActivated;
        break;
    }
}

OBJECT_INIT_ADAPTER(gCCPedestalObjDescriptorInitAdapter, ccPedestal_init, obj, placement)
OBJECT_EXTRA_SIZE_ADAPTER(gCCPedestalObjDescriptorExtraSizeAdapter, ccPedestal_getExtraSize)

ObjectDescriptor gCCPedestalObjDescriptor = {
    .header = {
        .metadata = { 0, 0, 0, OBJECT_DESCRIPTOR_FLAGS_10_SLOTS },
        .acquire = NULL,
        .release = NULL,
    },
    .init = gCCPedestalObjDescriptorInitAdapter,
    .update = ccPedestal_update,
    .getExtraSize = gCCPedestalObjDescriptorExtraSizeAdapter,
};;
