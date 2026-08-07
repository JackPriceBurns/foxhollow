/*
 * CCpedstal (DLL 0x18A) - Cape Claw Fire Gem pedestals.
 *
 * The placement map ID selects the Fire Gem source or one of two consuming
 * gates. Each variant controls its model and interaction state from a
 * persistent gamebit, then queues that gamebit update when its trigger fires.
 */
#include "dlls/objects/394_CCpedstal.h"

#include "game/objects/object.h"
#include "main/gamebit_ids.h"
#include "main/gamebits_api.h"
#include "main/objseq.h"
#include "main/gameloop_gamebit_api.h"
#include "main/obj_trigger.h"
#include "sys/objects.h"

#define CC_PEDESTAL_GAMEBIT_0DC5             0xDC5
#define CC_PEDESTAL_SOURCE_ACTIVATED_GAMEBIT 0xAA
#define CC_PEDESTAL_GAMEBIT_0DF0             0xDF0
#define CC_PEDESTAL_GATE_A_ACTIVATED_GAMEBIT 0xF1
#define CC_PEDESTAL_GATE_B_ACTIVATED_GAMEBIT 0xFE

#define CC_PEDESTAL_FIRE_GEM_TRIGGER_ID 0xA9

#define CC_PEDESTAL_FIRE_GEM_SOURCE_PLACEMENT_ID 0x45F1A
#define CC_PEDESTAL_FIRE_GEM_GATE_A_PLACEMENT_ID 0x45F1B
#define CC_PEDESTAL_FIRE_GEM_GATE_B_PLACEMENT_ID 0x45F1C

#define CC_PEDESTAL_PENDING_GAMEBIT_SET 0x01

#define CC_PEDESTAL_GATE_SEQUENCE           0
#define CC_PEDESTAL_SOURCE_SEQUENCE         1
#define CC_PEDESTAL_GATE_INACTIVE_MODEL     0
#define CC_PEDESTAL_GATE_ACTIVE_MODEL       1
#define CC_PEDESTAL_SOURCE_ACTIVE_MODEL     0
#define CC_PEDESTAL_SOURCE_IDLE_MODEL       1
#define CC_PEDESTAL_ROT_X_SHIFT             8
#define CC_PEDESTAL_SOURCE_HIT_VOLUME_FLAGS 3

int ccPedestal_getExtraSize(void) {
    return sizeof(CCPedestalState);
}

/*
 * A gate consumes one Fire Gem when triggered. Its activation gamebit selects
 * the active model and disables further interaction; without a Fire Gem the
 * interaction prompt is suppressed.
 */
void ccPedestal_updateFireGemGate(GameObject* obj, CCPedestalState* state) {
    if (mainGetBit(state->activationGameBit) != 0) {
        obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
        Obj_SetActiveModelIndex(obj, CC_PEDESTAL_GATE_ACTIVE_MODEL);
    } else {
        int activationTriggered;

        Obj_SetActiveModelIndex(obj, CC_PEDESTAL_GATE_INACTIVE_MODEL);
        do {
            if (mainGetBit(GAMEBIT_ITEM_FireGem_Count) != 0) {
                obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_PROMPT_SUPPRESSED;
                if (ObjTrigger_IsSetById(obj, CC_PEDESTAL_FIRE_GEM_TRIGGER_ID) != 0) {
                    (*gObjectTriggerInterface)->runSequence(CC_PEDESTAL_GATE_SEQUENCE, obj, -1);
                    gameBitDecrement(GAMEBIT_ITEM_FireGem_Count);
                    activationTriggered = 1;
                    break;
                }
            } else {
                obj->anim.resetHitboxFlags |= INTERACT_FLAG_PROMPT_SUPPRESSED;
            }
            activationTriggered = 0;
        } while (0);

        if (activationTriggered != 0) {
            state->pendingGameBitFlags |= CC_PEDESTAL_PENDING_GAMEBIT_SET;
        }
    }
}

/*
 * The source pedestal grants one Fire Gem when triggered. A separate gamebit
 * gates interaction, while the pedestal's activation gamebit selects its
 * active model and prevents repeat collection.
 */
void ccPedestal_updateFireGemSource(GameObject* obj, CCPedestalState* state) {
    if (mainGetBit(CC_PEDESTAL_GAMEBIT_0DC5) != 0) {
        obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
    } else {
        obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_DISABLED;
    }
    if (mainGetBit(state->activationGameBit) != 0) {
        obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
        Obj_SetActiveModelIndex(obj, CC_PEDESTAL_SOURCE_ACTIVE_MODEL);
    } else {
        int activationTriggered;

        Obj_SetActiveModelIndex(obj, CC_PEDESTAL_SOURCE_IDLE_MODEL);
        if (ObjTrigger_IsSet(obj) != 0) {
            (*gObjectTriggerInterface)->runSequence(CC_PEDESTAL_SOURCE_SEQUENCE, obj, -1);
            gameBitIncrement(GAMEBIT_ITEM_FireGem_Count);
            activationTriggered = 1;
        } else {
            activationTriggered = 0;
        }
        if (activationTriggered != 0) {
            state->pendingGameBitFlags |= CC_PEDESTAL_PENDING_GAMEBIT_SET;
        }
    }
}

void ccPedestal_update(GameObject* obj) {
    CCPedestalState* state = obj->extra;

    if (state->pendingGameBitFlags != 0) {
        if ((state->pendingGameBitFlags & CC_PEDESTAL_PENDING_GAMEBIT_SET) != 0) {
            mainSetBits(state->activationGameBit, 1);
        } else {
            mainSetBits(state->activationGameBit, 0);
        }
        state->pendingGameBitFlags = 0;
        if (mainGetBit(CC_PEDESTAL_GAMEBIT_0DF0) == 0 && mainGetBit(CC_PEDESTAL_SOURCE_ACTIVATED_GAMEBIT) != 0) {
            mainSetBits(CC_PEDESTAL_GAMEBIT_0DF0, 1);
        }
    }
    state->variantUpdate(obj, state);
}

void ccPedestal_init(GameObject* obj, const CCPedestalPlacement* placement) {
    CCPedestalState* state = obj->extra;

    obj->anim.rotX = (s16)((u32)placement->rotXByte << CC_PEDESTAL_ROT_X_SHIFT);
    obj->objectFlags = (u16)(obj->objectFlags | OBJECT_OBJFLAG_HIDDEN);
    switch (placement->base.ident) {
    case CC_PEDESTAL_FIRE_GEM_SOURCE_PLACEMENT_ID:
        state->variantUpdate = ccPedestal_updateFireGemSource;
        state->activationGameBit = CC_PEDESTAL_SOURCE_ACTIVATED_GAMEBIT;
        Obj_SetActiveHitVolumeBounds(obj, 0, 0, 0, 0, CC_PEDESTAL_SOURCE_HIT_VOLUME_FLAGS);
        break;
    case CC_PEDESTAL_FIRE_GEM_GATE_A_PLACEMENT_ID:
        state->variantUpdate = ccPedestal_updateFireGemGate;
        state->activationGameBit = CC_PEDESTAL_GATE_A_ACTIVATED_GAMEBIT;
        break;
    case CC_PEDESTAL_FIRE_GEM_GATE_B_PLACEMENT_ID:
        state->variantUpdate = ccPedestal_updateFireGemGate;
        state->activationGameBit = CC_PEDESTAL_GATE_B_ACTIVATED_GAMEBIT;
        break;
    }
}

ObjectDescriptor gCCPedestalObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    0,
    0,
    0,
    (ObjectDescriptorCallback)ccPedestal_init,
    (ObjectDescriptorCallback)ccPedestal_update,
    0,
    0,
    0,
    0,
    ccPedestal_getExtraSize,
};
