#include "dlls/objects/445.h"

#include "dolphin/pad.h"
#include "game/objects/object_setup.h"
#include "main/dll/player.h"
#include "main/gamebits.h"
#include "main/gametext_color.h"
#include "main/gametext_show.h"
#include "main/objprint_render.h"
#include "main/objseq.h"
#include "main/pad.h"
#include "sys/objects.h"

enum PaymentKioskConditionEvent {
    PAYMENT_KIOSK_CONDITION_CAN_AFFORD = 0x14,
    PAYMENT_KIOSK_CONDITION_CANNOT_AFFORD = 0x15,
};

enum PaymentKioskSequenceEvent {
    PAYMENT_KIOSK_SEQUENCE_EVENT_SHOW_PROMPT = 1,
    PAYMENT_KIOSK_SEQUENCE_EVENT_PAY = 2,
};

enum PaymentKioskPaymentState {
    PAYMENT_KIOSK_PAYMENT_STATE_RESOLVE,
    PAYMENT_KIOSK_PAYMENT_STATE_ACTIVE,
    PAYMENT_KIOSK_PAYMENT_STATE_PAID,
};

enum PaymentKioskTextVariant {
    PAYMENT_KIOSK_TEXT_VARIANT_PAYPOINT,
    PAYMENT_KIOSK_TEXT_VARIANT_SP_WELL,
};

enum PaymentKioskPromptState {
    PAYMENT_KIOSK_PROMPT_NONE,
    PAYMENT_KIOSK_PROMPT_APPROACH,
    PAYMENT_KIOSK_PROMPT_CANNOT_AFFORD,
};

enum PaymentKioskRomDefNo {
    PAYMENT_KIOSK_SP_WELL_ROM_DEF_NO = 0x476,
};

typedef struct PaymentKioskPlacement {
    ObjPlacement base;
    s8 rotationXByte;
    u8 unused19;
    s16 price;
    u8 unused1C[2];
    s16 completionGameBit;
    u8 unused20[4];
} PaymentKioskPlacement;

typedef struct PaymentKioskState {
    u8 paymentState;
    u8 textVariant;
    u8 promptState;
} PaymentKioskState;

typedef struct PaymentKioskTextPair {
    int approach;
    int cannotAfford;
} PaymentKioskTextPair;

STATIC_ASSERT(sizeof(PaymentKioskPlacement) == 0x24);
STATIC_ASSERT(offsetof(PaymentKioskPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(PaymentKioskPlacement, rotationXByte) == 0x18);
STATIC_ASSERT(offsetof(PaymentKioskPlacement, unused19) == 0x19);
STATIC_ASSERT(offsetof(PaymentKioskPlacement, price) == 0x1A);
STATIC_ASSERT(offsetof(PaymentKioskPlacement, unused1C) == 0x1C);
STATIC_ASSERT(offsetof(PaymentKioskPlacement, completionGameBit) == 0x1E);
STATIC_ASSERT(offsetof(PaymentKioskPlacement, unused20) == 0x20);

STATIC_ASSERT(sizeof(PaymentKioskState) == 0x03);
STATIC_ASSERT(offsetof(PaymentKioskState, paymentState) == 0x00);
STATIC_ASSERT(offsetof(PaymentKioskState, textVariant) == 0x01);
STATIC_ASSERT(offsetof(PaymentKioskState, promptState) == 0x02);

static const PaymentKioskTextPair sPaymentKioskTextPairs[] = {
    [PAYMENT_KIOSK_TEXT_VARIANT_PAYPOINT] = {.approach = 0x312, .cannotAfford = 0x34A},
    [PAYMENT_KIOSK_TEXT_VARIANT_SP_WELL] = {.approach = 0x527, .cannotAfford = -1},
};

static int paymentkiosk_testEvent(void* context, u8* unusedObject, int conditionEvent) {
    GameObject* obj = context;
    const PaymentKioskPlacement* placement = (const PaymentKioskPlacement*)obj->anim.placementData;
    PaymentKioskState* state = obj->extra;
    GameObject* player = Obj_GetPlayerObject();
    int canAfford;

    (void)unusedObject;

    if ((getButtonsJustPressed(0) & PAD_BUTTON_A) == 0) {
        return 0;
    }

    state->promptState = PAYMENT_KIOSK_PROMPT_NONE;
    canAfford = playerGetMoney(player) >= ObjAnim_ReadPlacementS16(&obj->anim, &placement->price);
    if (!canAfford) {
        state->promptState = PAYMENT_KIOSK_PROMPT_CANNOT_AFFORD;
    }

    switch (conditionEvent) {
    case PAYMENT_KIOSK_CONDITION_CAN_AFFORD:
        return canAfford;
    case PAYMENT_KIOSK_CONDITION_CANNOT_AFFORD:
        return !canAfford;
    default:
        return 0;
    }
}

static int paymentkiosk_animEventCallback(GameObject* obj, int unused, ObjSeqState* animUpdate) {
    PaymentKioskState* state = obj->extra;
    const PaymentKioskPlacement* placement = (const PaymentKioskPlacement*)obj->anim.placementData;
    GameObject* player = Obj_GetPlayerObject();

    (void)unused;

    animUpdate->conditionCallback = paymentkiosk_testEvent;
    for (int eventIndex = 0; eventIndex < animUpdate->eventCount; eventIndex++) {
        switch (animUpdate->eventIds[eventIndex]) {
        case PAYMENT_KIOSK_SEQUENCE_EVENT_PAY:
            mainSetBits(ObjAnim_ReadPlacementS16(&obj->anim, &placement->completionGameBit), 1);
            playerAddMoney(player, -ObjAnim_ReadPlacementS16(&obj->anim, &placement->price));
            state->paymentState = PAYMENT_KIOSK_PAYMENT_STATE_PAID;
            break;
        case PAYMENT_KIOSK_SEQUENCE_EVENT_SHOW_PROMPT:
            state->promptState = PAYMENT_KIOSK_PROMPT_APPROACH;
            break;
        }
    }

    gameTextSetColor(0xFF, 0xFF, 0xFF, 0xFF);
    if (state->promptState == PAYMENT_KIOSK_PROMPT_APPROACH) {
        gameTextShow(sPaymentKioskTextPairs[state->textVariant].approach);
    } else if (state->promptState == PAYMENT_KIOSK_PROMPT_CANNOT_AFFORD) {
        gameTextShow(sPaymentKioskTextPairs[state->textVariant].cannotAfford);
    }
    return 0;
}

static int paymentkiosk_getExtraSize(void) {
    return sizeof(PaymentKioskState);
}

static int paymentkiosk_getObjectTypeId(void) {
    return 1;
}

static void paymentkiosk_free(void) {
}

static void paymentkiosk_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                                s8 visible) {
    (void)obj;
    (void)renderArg2;
    (void)renderArg3;
    (void)renderArg4;
    (void)renderArg5;
    (void)visible;
}

static void paymentkiosk_hitDetect(void) {
}

static void paymentkiosk_update(GameObject* obj) {
    PaymentKioskState* state = obj->extra;
    const PaymentKioskPlacement* placement = (const PaymentKioskPlacement*)obj->anim.placementData;

    switch (state->paymentState) {
    case PAYMENT_KIOSK_PAYMENT_STATE_RESOLVE: {
        s16 completionGameBit = ObjAnim_ReadPlacementS16(&obj->anim, &placement->completionGameBit);

        state->paymentState = completionGameBit != -1 && mainGetBit(completionGameBit) != 0
                                  ? PAYMENT_KIOSK_PAYMENT_STATE_PAID
                                  : PAYMENT_KIOSK_PAYMENT_STATE_ACTIVE;
        break;
    }
    case PAYMENT_KIOSK_PAYMENT_STATE_ACTIVE:
        if ((obj->anim.resetHitboxFlags & INTERACT_FLAG_ACTIVATED) != 0) {
            (*gObjectTriggerInterface)->runSequence(0, obj, -1);
        }
        obj->anim.resetHitboxFlags &= (u8)~INTERACT_FLAG_DISABLED;
        break;
    case PAYMENT_KIOSK_PAYMENT_STATE_PAID:
        obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
        break;
    }

    state->promptState = PAYMENT_KIOSK_PROMPT_NONE;
    if ((obj->anim.modelInstance->flags & OBJDEF_FLAG_HAS_MODELS) != 0 && obj->anim.hitVolumeTransforms != NULL) {
        objUpdateHitVolumeTransforms(obj);
    }
}

static void paymentkiosk_init(GameObject* obj, const PaymentKioskPlacement* placement) {
    PaymentKioskState* state = obj->extra;

    obj->animEventCallback = paymentkiosk_animEventCallback;
    obj->anim.rotX = (s16)((s32)placement->rotationXByte * 0x100);
    state->paymentState = PAYMENT_KIOSK_PAYMENT_STATE_RESOLVE;
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN | OBJECT_OBJFLAG_HITDETECT_DISABLED;
    obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
    state->textVariant = obj->anim.romDefNo == PAYMENT_KIOSK_SP_WELL_ROM_DEF_NO ? PAYMENT_KIOSK_TEXT_VARIANT_SP_WELL
                                                                                : PAYMENT_KIOSK_TEXT_VARIANT_PAYPOINT;
}

static void paymentkiosk_release(void) {
}

static void paymentkiosk_initialise(void) {
}

OBJECT_INIT_ADAPTER(gPaymentKioskObjDescriptorInitAdapter, paymentkiosk_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gPaymentKioskObjDescriptorHitDetectAdapter, paymentkiosk_hitDetect)
OBJECT_FREE_ADAPTER(gPaymentKioskObjDescriptorFreeAdapter, paymentkiosk_free)
OBJECT_TYPE_ID_ADAPTER(gPaymentKioskObjDescriptorTypeIdAdapter, paymentkiosk_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gPaymentKioskObjDescriptorExtraSizeAdapter, paymentkiosk_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gPaymentKioskObjDescriptorAcquire, paymentkiosk_initialise)

ObjectDescriptor gPaymentKioskObjDescriptor = {
    .header = {
        .metadata = { 0, 0, 0, OBJECT_DESCRIPTOR_FLAGS_10_SLOTS },
        .acquire = gPaymentKioskObjDescriptorAcquire,
        .release = paymentkiosk_release,
    },
    .init = gPaymentKioskObjDescriptorInitAdapter,
    .update = paymentkiosk_update,
    .hitDetect = gPaymentKioskObjDescriptorHitDetectAdapter,
    .render = paymentkiosk_render,
    .free = gPaymentKioskObjDescriptorFreeAdapter,
    .getObjectTypeId = gPaymentKioskObjDescriptorTypeIdAdapter,
    .getExtraSize = gPaymentKioskObjDescriptorExtraSizeAdapter,
};;
