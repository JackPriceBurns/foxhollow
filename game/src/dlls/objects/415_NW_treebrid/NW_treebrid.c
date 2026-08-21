#include "dlls/objects/415_NW_treebrid.h"

#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/dll/partfx_interface.h"
#include "main/gamebits_api.h"
#include "main/obj_path.h"
#include "main/object_render.h"
#include "main/objseq.h"
#include "main/objtype.h"

enum NwTreeBridgeObjectGroup {
    NW_TREE_BRIDGE_TARGET_OBJECT_GROUP = 4,
};

enum NwTreeBridgeObjectId {
    NW_TREE_BRIDGE_OBJECT_NW_TREE_BRIDGE = 0x5D,
};

enum NwTreeBridgeSequenceId {
    NW_TREE_BRIDGE_SEQUENCE_FIRST,
    NW_TREE_BRIDGE_SEQUENCE_SECOND,
};

enum NwTreeBridgeTriggerId {
    NW_TREE_BRIDGE_TRIGGER_PREEMPT = 0x154,
};

enum NwTreeBridgeAnimationEvent {
    NW_TREE_BRIDGE_EVENT_LARGE_BURST = 1,
    NW_TREE_BRIDGE_EVENT_MEDIUM_BURST,
    NW_TREE_BRIDGE_EVENT_SMALL_BURST,
};

enum NwTreeBridgeParticleId {
    NW_TREE_BRIDGE_PARTICLE_LARGE_BURST = 0xCC,
    NW_TREE_BRIDGE_PARTICLE_FIRST_MEDIUM_BURST = 0xCD,
    NW_TREE_BRIDGE_PARTICLE_FIRST_SMALL_BURST = 0xCE,
    NW_TREE_BRIDGE_PARTICLE_SECOND_MEDIUM_BURST = 0xCF,
    NW_TREE_BRIDGE_PARTICLE_SECOND_SMALL_BURST = 0xD0,
    NW_TREE_BRIDGE_PARTICLE_SPECIAL_MEDIUM_BURST = 0xD3,
    NW_TREE_BRIDGE_PARTICLE_SPECIAL_SMALL_BURST = 0xD4,
};

typedef struct NwTreeBridgePlacement {
    ObjPlacement base;
    s8 initialRotX;
    s8 sequenceId;
    s16 initialRotY;
    s16 initialRotZ;
    s16 gameBit;
} NwTreeBridgePlacement;

typedef struct NwTreeBridgeState {
    s16 gameBit;
    s16 sequenceId;
    s16 preemptTriggerId;
    u8 sequenceStarted;
    u8 targetSearchAttempts;
    GameObject* pathTarget;
} NwTreeBridgeState;

STATIC_ASSERT(sizeof(NwTreeBridgePlacement) == 0x20);
STATIC_ASSERT(offsetof(NwTreeBridgePlacement, initialRotX) == 0x18);
STATIC_ASSERT(offsetof(NwTreeBridgePlacement, sequenceId) == 0x19);
STATIC_ASSERT(offsetof(NwTreeBridgePlacement, initialRotY) == 0x1A);
STATIC_ASSERT(offsetof(NwTreeBridgePlacement, initialRotZ) == 0x1C);
STATIC_ASSERT(offsetof(NwTreeBridgePlacement, gameBit) == 0x1E);

STATIC_ASSERT(sizeof(NwTreeBridgeState) == 0x10);
STATIC_ASSERT(offsetof(NwTreeBridgeState, gameBit) == 0x00);
STATIC_ASSERT(offsetof(NwTreeBridgeState, sequenceId) == 0x02);
STATIC_ASSERT(offsetof(NwTreeBridgeState, preemptTriggerId) == 0x04);
STATIC_ASSERT(offsetof(NwTreeBridgeState, sequenceStarted) == 0x06);
STATIC_ASSERT(offsetof(NwTreeBridgeState, targetSearchAttempts) == 0x07);
STATIC_ASSERT(offsetof(NwTreeBridgeState, pathTarget) == 0x08);

static void nwTreeBridge_spawnParticles(GameObject* obj, int particleId, int count) {
    for (int i = 0; i < count; i++) {
        (*gPartfxInterface)->spawnObject(obj, particleId, NULL, 1, -1, NULL);
    }
}

static int nwTreeBridge_getBurstParticleId(GameObject* obj, NwTreeBridgeState* state, u8 eventId) {
    if (obj->anim.romDefNo == NW_TREE_BRIDGE_OBJECT_NW_TREE_BRIDGE) {
        return eventId == NW_TREE_BRIDGE_EVENT_MEDIUM_BURST ? NW_TREE_BRIDGE_PARTICLE_SPECIAL_MEDIUM_BURST
                                                            : NW_TREE_BRIDGE_PARTICLE_SPECIAL_SMALL_BURST;
    }

    if (state->sequenceId == NW_TREE_BRIDGE_SEQUENCE_FIRST) {
        return eventId == NW_TREE_BRIDGE_EVENT_MEDIUM_BURST ? NW_TREE_BRIDGE_PARTICLE_FIRST_MEDIUM_BURST
                                                            : NW_TREE_BRIDGE_PARTICLE_FIRST_SMALL_BURST;
    }
    if (state->sequenceId == NW_TREE_BRIDGE_SEQUENCE_SECOND) {
        return eventId == NW_TREE_BRIDGE_EVENT_MEDIUM_BURST ? NW_TREE_BRIDGE_PARTICLE_SECOND_MEDIUM_BURST
                                                            : NW_TREE_BRIDGE_PARTICLE_SECOND_SMALL_BURST;
    }
    return -1;
}

static int nwTreeBridge_processAnimEvents(GameObject* obj, int unusedArg, ObjSeqState* animUpdate) {
    NwTreeBridgeState* state = obj->extra;

    (void)unusedArg;
    for (int i = 0; i < animUpdate->eventCount; i++) {
        u8 eventId = animUpdate->eventIds[i];

        switch (eventId) {
        case NW_TREE_BRIDGE_EVENT_LARGE_BURST:
            nwTreeBridge_spawnParticles(obj, NW_TREE_BRIDGE_PARTICLE_LARGE_BURST, 200);
            break;
        case NW_TREE_BRIDGE_EVENT_MEDIUM_BURST: {
            int particleId = nwTreeBridge_getBurstParticleId(obj, state, eventId);

            if (particleId >= 0) {
                nwTreeBridge_spawnParticles(obj, particleId, 100);
            }
            break;
        }
        case NW_TREE_BRIDGE_EVENT_SMALL_BURST: {
            int particleId = nwTreeBridge_getBurstParticleId(obj, state, eventId);

            if (particleId >= 0) {
                nwTreeBridge_spawnParticles(obj, particleId, 5);
            }
            break;
        }
        }
    }
    return 0;
}

static int nwTreeBridge_getExtraSize(void) {
    return sizeof(NwTreeBridgeState);
}

static void nwTreeBridge_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                                s8 unusedVisible) {
    NwTreeBridgeState* state = obj->extra;

    (void)unusedVisible;
    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    if (state->pathTarget != NULL) {
        Vec3f pathPosition;

        ObjPath_GetPointWorldPosition(obj, 0, &pathPosition.x, &pathPosition.y, &pathPosition.z, 0);
        state->pathTarget->anim.localPos = pathPosition;
    }
}

static void nwTreeBridge_update(GameObject* obj) {
    NwTreeBridgeState* state = obj->extra;

    if (state->targetSearchAttempts != 0) {
        f32 searchDistance = 100.0f;

        state->pathTarget = objGetNearestTypeTo(NW_TREE_BRIDGE_TARGET_OBJECT_GROUP, obj, &searchDistance);
        if (state->pathTarget != NULL) {
            state->targetSearchAttempts = 0;
        } else {
            state->targetSearchAttempts--;
        }
        return;
    }
    if (state->sequenceStarted != 0) {
        return;
    }

    if (state->preemptTriggerId != 0) {
        (*gObjectTriggerInterface)->preempt((uintptr_t)obj, state->preemptTriggerId);
        (*gObjectTriggerInterface)->runSequence(state->sequenceId, obj, 1);
        state->sequenceStarted = 1;
    } else if (mainGetBit(state->gameBit) != 0) {
        (*gObjectTriggerInterface)->runSequence(state->sequenceId, obj, -1);
        state->sequenceStarted = 1;
    }
}

static void nwTreeBridge_init(GameObject* obj, const NwTreeBridgePlacement* placement) {
    NwTreeBridgeState* state = obj->extra;

    obj->animEventCallback = nwTreeBridge_processAnimEvents;
    obj->anim.rotX = (s16)(placement->initialRotX * 0x100);
    obj->anim.rotY = ObjAnim_ReadPlacementS16(&obj->anim, &placement->initialRotY);
    obj->anim.rotZ = ObjAnim_ReadPlacementS16(&obj->anim, &placement->initialRotZ);
    state->sequenceId = placement->sequenceId;
    state->gameBit = ObjAnim_ReadPlacementS16(&obj->anim, &placement->gameBit);
    if (mainGetBit(state->gameBit) != 0) {
        state->preemptTriggerId = NW_TREE_BRIDGE_TRIGGER_PREEMPT;
    }
    state->targetSearchAttempts = 4;
}

ObjectDescriptor gNWTreeBridgeObjDescriptor = {
    .reserved0 = 0,
    .reserved1 = 0,
    .reserved2 = 0,
    .slotCountAndFlags = OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    .initialise = NULL,
    .release = NULL,
    .slot02 = NULL,
    .init = (ObjectDescriptorCallback)nwTreeBridge_init,
    .update = (ObjectDescriptorCallback)nwTreeBridge_update,
    .hitDetect = NULL,
    .render = (ObjectDescriptorCallback)nwTreeBridge_render,
    .free = NULL,
    .getObjectTypeId = NULL,
    .getExtraSize = nwTreeBridge_getExtraSize,
};
