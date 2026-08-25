/*
 * DepthOfFieldPoint (DLL 0x00C8) - placed depth-of-field focus point.
 *
 * Sequence events enable or disable the blur filter and select its area and
 * size modes. While enabled, the sequence callback keeps the filter focused
 * on the object's world position.
 */
#include "dlls/objects/200_DepthOfFieldPoint.h"
#include "game/objects/object.h"
#include "main/objseq.h"
#include "main/rcp_dolphin.h"

#define DEPTHOFFIELDPOINT_SEQEV_DISABLE       0
#define DEPTHOFFIELDPOINT_SEQEV_ENABLE        1
#define DEPTHOFFIELDPOINT_SEQEV_ENABLE_AREA   2
#define DEPTHOFFIELDPOINT_SEQEV_ENABLE_BIGGER 3

int depthoffieldpoint_SeqFn(GameObject* obj, int, ObjSeqState* animUpdate) {
    DepthOfFieldPointState* state = obj->extra;

    if (state->enabled) {
        turnOnBlurFilter(obj->anim.worldPosX, obj->anim.worldPosY, obj->anim.worldPosZ, state->useArea, state->bigger);
    }

    for (int i = 0; i < animUpdate->eventCount; i++) {
        switch (animUpdate->eventIds[i]) {
        case DEPTHOFFIELDPOINT_SEQEV_ENABLE:
            state->enabled = 1;
            state->useArea = 0;
            break;
        case DEPTHOFFIELDPOINT_SEQEV_DISABLE:
            state->enabled = 0;
            Rcp_DisableBlurFilter();
            break;
        case DEPTHOFFIELDPOINT_SEQEV_ENABLE_AREA:
            state->enabled = 1;
            state->useArea = 1;
            state->bigger = 0;
            break;
        case DEPTHOFFIELDPOINT_SEQEV_ENABLE_BIGGER:
            state->enabled = 1;
            state->bigger = 1;
            state->useArea = 0;
            break;
        }
    }
    return 0;
}

int depthoffieldpoint_getExtraSize(void) {
    return sizeof(DepthOfFieldPointState);
}

void depthoffieldpoint_update(GameObject* obj) {
    DepthOfFieldPointState* state = obj->extra;

    if (state->enabled) {
        state->enabled = 0;
        Rcp_DisableBlurFilter();
    }
}

void depthoffieldpoint_init(GameObject* obj) {
    DepthOfFieldPointState* state = obj->extra;
    state->enabled = 0;
    obj->animEventCallback = depthoffieldpoint_SeqFn;
    state->useArea = 0;
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN;
}

OBJECT_INIT_ADAPTER(gDepthOfFieldPointObjDescriptorInitAdapter, depthoffieldpoint_init, obj)
OBJECT_EXTRA_SIZE_ADAPTER(gDepthOfFieldPointObjDescriptorExtraSizeAdapter, depthoffieldpoint_getExtraSize)

ObjectDescriptor gDepthOfFieldPointObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        0,
        0,
    },
    0,
    gDepthOfFieldPointObjDescriptorInitAdapter,
    depthoffieldpoint_update,
    0,
    0,
    0,
    0,
    gDepthOfFieldPointObjDescriptorExtraSizeAdapter,
};
