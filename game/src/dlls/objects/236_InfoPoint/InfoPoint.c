/*
 * Information-point object (DLL slot 236 / 0xEC).
 *
 * Initialization loads the placement's game text and a shared font texture.
 * Activating the object disables the A button and runs object sequence zero.
 * Sequence events one and two toggle the class-owned sequence state.
 */
#include "dlls/objects/236_InfoPoint.h"
#include "dolphin/pad.h"
#include "main/gametext_internal.h"
#include "main/objseq.h"
#include "main/object_render.h"
#include "main/textrender_api.h"
#include "main/texture.h"
#include "main/pad_api.h"

#define INFOPOINT_FONT_TEXTURE_ASSET_ID 616

#define INFOPOINT_OBJECT_TYPE_ID       0
#define INFOPOINT_TRIGGER_SEQUENCE_ID  0
#define INFOPOINT_TRIGGER_PORT         0
#define INFOPOINT_TRIGGER_FLAGS        -1
#define INFOPOINT_INITIAL_DISPLAY_TIME 100
#define INFOPOINT_INITIAL_UNK18        2

#define INFOPOINT_SEQUENCE_EVENT_SET   1
#define INFOPOINT_SEQUENCE_EVENT_CLEAR 2
#define INFOPOINT_SEQUENCE_STATE_SET   0xFF
#define INFOPOINT_SEQUENCE_STATE_CLEAR 0

int InfoPoint_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate) {
    InfoPointState* state;
    int i;

    (void)unused;

    state = obj->extra;
    for (i = 0; i < animUpdate->eventCount; i++) {
        switch (animUpdate->eventIds[i]) {
        case INFOPOINT_SEQUENCE_EVENT_SET:
            state->sequenceState = INFOPOINT_SEQUENCE_STATE_SET;
            break;
        case INFOPOINT_SEQUENCE_EVENT_CLEAR:
            state->sequenceState = INFOPOINT_SEQUENCE_STATE_CLEAR;
            break;
        case 3:
        case 4:
            break;
        }
    }
    return 0;
}

int InfoPoint_getExtraSize(void) {
    return sizeof(InfoPointState);
}

int InfoPoint_getObjectTypeId(void) {
    return INFOPOINT_OBJECT_TYPE_ID;
}

void InfoPoint_free(GameObject* obj) {
    (void)obj;
}

void InfoPoint_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible) {
    s32 isVisible = visible;

    if (isVisible != 0) {
        objRenderModelAndHitVolumes(obj, fwdArg2, fwdArg3, fwdArg4, fwdArg5, 1.0f);
    }
}

void InfoPoint_hitDetect(GameObject* obj) {
    (void)obj;
}

void InfoPoint_update(GameObject* obj) {
    if ((obj->anim.resetHitboxFlags & INTERACT_FLAG_ACTIVATED) != 0) {
        buttonDisable(INFOPOINT_TRIGGER_PORT, PAD_BUTTON_A);
        (*gObjectTriggerInterface)->runSequence(INFOPOINT_TRIGGER_SEQUENCE_ID, obj, INFOPOINT_TRIGGER_FLAGS);
    }
}

void InfoPoint_init(GameObject* obj, InfoPointPlacement* placement) {
    InfoPointState* state;
    GameTextDef* text;

    state = obj->extra;
    obj->animEventCallback = InfoPoint_SeqFn;
    if (gInfoPointSharedResources.fontTexture == NULL) {
        gInfoPointSharedResources.fontTexture = textureLoadAsset(INFOPOINT_FONT_TEXTURE_ASSET_ID);
    }
    state->renderBounds = &gInfoPointRenderBounds;
    text = gameTextGet(placement->textId);
    state->firstString = text->strings[0];
    state->displayTimer = INFOPOINT_INITIAL_DISPLAY_TIME;
    state->text = text;
    obj->anim.rotX = (s16)((s32)placement->rotXByte << 8);
    state->unk18 = INFOPOINT_INITIAL_UNK18;
    state->unk10 = placement->unk1B;
    state->sequenceState = INFOPOINT_SEQUENCE_STATE_CLEAR;
    obj->objectFlags |= OBJECT_OBJFLAG_HITDETECT_DISABLED;
}

void InfoPoint_release(void) {
}

void InfoPoint_initialise(void) {
}

InfoPointRenderBounds gInfoPointRenderBounds = {0x50, 0x230, 0x3C, 0x190};
InfoPointSharedResources gInfoPointSharedResources = {NULL, {0, 0, 0, 0, 0}};

ObjectDescriptor gInfoPointObjDescriptor = {
    0,                                                   /* reserved0 */
    0,                                                   /* reserved1 */
    0,                                                   /* reserved2 */
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,                    /* slotCountAndFlags */
    (ObjectDescriptorCallback)InfoPoint_initialise,      /* initialise */
    (ObjectDescriptorCallback)InfoPoint_release,         /* release */
    0,                                                   /* slot02 */
    (ObjectDescriptorCallback)InfoPoint_init,            /* init */
    (ObjectDescriptorCallback)InfoPoint_update,          /* update */
    (ObjectDescriptorCallback)InfoPoint_hitDetect,       /* hitDetect */
    (ObjectDescriptorCallback)InfoPoint_render,          /* render */
    (ObjectDescriptorCallback)InfoPoint_free,            /* free */
    (ObjectDescriptorCallback)InfoPoint_getObjectTypeId, /* getObjectTypeId */
    InfoPoint_getExtraSize,                              /* getExtraSize */
};
