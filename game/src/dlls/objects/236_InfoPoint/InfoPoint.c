#include "dlls/objects/236_InfoPoint.h"
#include "dolphin/pad.h"
#include "main/gametext_internal.h"
#include "main/objseq.h"
#include "main/object_render.h"
#include "main/textrender_api.h"
#include "main/texture.h"
#include "main/pad_api.h"

typedef enum InfoPointResourceId {
    INFOPOINT_FONT_TEXTURE_ASSET_ID = 616
} InfoPointResourceId;

typedef enum InfoPointSequenceEvent {
    INFOPOINT_SEQUENCE_EVENT_SET = 1,
    INFOPOINT_SEQUENCE_EVENT_CLEAR
} InfoPointSequenceEvent;

typedef enum InfoPointSequenceState {
    INFOPOINT_SEQUENCE_STATE_CLEAR,
    INFOPOINT_SEQUENCE_STATE_SET = 0xFF
} InfoPointSequenceState;

typedef struct InfoPointRenderBounds {
    s32 x;
    s32 y;
    s32 width;
    s32 height;
} InfoPointRenderBounds;

typedef struct InfoPointSharedResources {
    void* fontTexture;
    u32 reserved[5];
} InfoPointSharedResources;

typedef struct InfoPointState {
    GameTextDef* text;
    char* firstString;
    InfoPointRenderBounds* renderBounds;
    s32 displayTimer;
    u8 unknownByte;
    u8 unknownBytes[5];
    s16 sequenceState;
    s32 unknownValue;
    u8 unknownTail[4];
} InfoPointState;

static InfoPointRenderBounds gInfoPointRenderBounds = {0x50, 0x230, 0x3C, 0x190};
static InfoPointSharedResources gInfoPointSharedResources;

int InfoPoint_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate) {
    (void)unused;

    InfoPointState* state = obj->extra;
    for (s32 eventIndex = 0; eventIndex < animUpdate->eventCount; eventIndex++) {
        switch (animUpdate->eventIds[eventIndex]) {
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
    return 0;
}

void InfoPoint_free(GameObject* obj) {
    (void)obj;
}

void InfoPoint_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, fwdArg2, fwdArg3, fwdArg4, fwdArg5, 1.0f);
    }
}

void InfoPoint_hitDetect(GameObject* obj) {
    (void)obj;
}

void InfoPoint_update(GameObject* obj) {
    if ((obj->anim.resetHitboxFlags & INTERACT_FLAG_ACTIVATED) != 0) {
        buttonDisable(0, PAD_BUTTON_A);
        (*gObjectTriggerInterface)->runSequence(0, obj, -1);
    }
}

void InfoPoint_init(GameObject* obj, const InfoPointPlacement* placement) {
    InfoPointState* state = obj->extra;

    obj->animEventCallback = InfoPoint_SeqFn;
    if (gInfoPointSharedResources.fontTexture == NULL) {
        gInfoPointSharedResources.fontTexture = textureLoadAsset(INFOPOINT_FONT_TEXTURE_ASSET_ID);
    }
    state->renderBounds = &gInfoPointRenderBounds;
    GameTextDef* text = gameTextGet(ObjAnim_ReadPlacementU16(&obj->anim, &placement->textId));

    state->firstString = text->strings[0];
    state->displayTimer = 100;
    state->text = text;
    obj->anim.rotX = (s16)((u32)placement->rotationXByte * 256);
    state->unknownValue = 2;
    state->unknownByte = placement->unknown1B;
    state->sequenceState = INFOPOINT_SEQUENCE_STATE_CLEAR;
    obj->objectFlags |= OBJECT_OBJFLAG_HITDETECT_DISABLED;
}

void InfoPoint_release(void) {
}

void InfoPoint_initialise(void) {
}

ObjectDescriptor gInfoPointObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    (ObjectDescriptorCallback)InfoPoint_initialise,
    (ObjectDescriptorCallback)InfoPoint_release,
    0,
    (ObjectDescriptorCallback)InfoPoint_init,
    (ObjectDescriptorCallback)InfoPoint_update,
    (ObjectDescriptorCallback)InfoPoint_hitDetect,
    (ObjectDescriptorCallback)InfoPoint_render,
    (ObjectDescriptorCallback)InfoPoint_free,
    (ObjectDescriptorCallback)InfoPoint_getObjectTypeId,
    InfoPoint_getExtraSize,
};
