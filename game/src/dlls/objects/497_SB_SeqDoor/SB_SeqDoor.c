/* SB_SeqDoor (DLL 0x01F1) - a sequence-controlled Ship Battle door. */
#include "dlls/objects/497_SB_SeqDoor.h"

#include "main/gamebits.h"
#include "main/object_render.h"
#include "main/objseq.h"

#define SB_SEQDOOR_SEQ_ID        0x173
#define SB_SEQDOOR_OPEN_SEQUENCE 0

int SB_SeqDoor_SeqFn(GameObject* obj, int unusedArg, ObjSeqState* animUpdate) {
    if (obj->anim.romDefNo != SB_SEQDOOR_SEQ_ID) {
        animUpdate->flags = -2;
    }
    animUpdate->movementState = 0;
    return 0;
}

int SB_SeqDoor_getExtraSize(void) {
    return 0;
}

int SB_SeqDoor_getObjectTypeId(void) {
    return 0;
}

void SB_SeqDoor_free(void) {
}

void SB_SeqDoor_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

void SB_SeqDoor_hitDetect(void) {
}

void SB_SeqDoor_update(GameObject* obj) {
    if (obj->anim.romDefNo == SB_SEQDOOR_SEQ_ID) {
        if (obj->userData1 == 0) {
            if (mainGetBit(GAMEBIT_SB_DoorOpen) != 0u) {
                (*gObjectTriggerInterface)->runSequence(SB_SEQDOOR_OPEN_SEQUENCE, obj, -1);
                obj->userData1 = 1;
            }
        }
    }
    obj->anim.resetHitboxFlags |= INTERACT_FLAG_PROMPT_SUPPRESSED;
}

void SB_SeqDoor_init(GameObject* obj, const SBSeqDoorPlacementView* placement) {
    s8 bankSelect;

    obj->animEventCallback = SB_SeqDoor_SeqFn;
    obj->anim.rotX = (s16)((s32)placement->rotXByte << 8);
    bankSelect = placement->bankSelect;
    obj->anim.bankIndex = (s8)(((u32)-bankSelect | (u32)bankSelect) >> 31);
}

void SB_SeqDoor_release(void) {
}

void SB_SeqDoor_initialise(void) {
}

ObjectDescriptor gSB_SeqDoorObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    SB_SeqDoor_initialise,
    SB_SeqDoor_release,
    0,
    (ObjectDescriptorCallback)SB_SeqDoor_init,
    (ObjectDescriptorCallback)SB_SeqDoor_update,
    SB_SeqDoor_hitDetect,
    (ObjectDescriptorCallback)SB_SeqDoor_render,
    SB_SeqDoor_free,
    (ObjectDescriptorCallback)SB_SeqDoor_getObjectTypeId,
    SB_SeqDoor_getExtraSize,
};
