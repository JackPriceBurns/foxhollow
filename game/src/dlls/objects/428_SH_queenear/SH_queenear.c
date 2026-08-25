#include "dlls/objects/428_SH_queenear.h"

#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/audio/sfx.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/dll/dll_0000_gameui.h"
#include "main/dll/player.h"
#include "main/dll/tricky.h"
#include "main/frame_timing.h"
#include "main/gamebits.h"
#include "main/mapEvent.h"
#include "main/mapEventTypes.h"
#include "main/obj_trigger.h"
#include "main/objprint_anim.h"
#include "main/objprint_character.h"
#include "main/objseq.h"
#include "main/objtype.h"
#include "main/vecmath.h"
#include "sys/objects.h"
#include "sys/objects/lifecycle.h"

enum QueenEarthWalkerStateId {
    QUEEN_EARTH_WALKER_WAITING_FOR_RETURN,
    QUEEN_EARTH_WALKER_READY_TO_FEED,
    QUEEN_EARTH_WALKER_FINISHING_FEED,
    QUEEN_EARTH_WALKER_FED,
    QUEEN_EARTH_WALKER_DEPARTING,
};

enum QueenEarthWalkerFlag {
    QUEEN_EARTH_WALKER_STARTED = 1 << 0,
    QUEEN_EARTH_WALKER_TARGETING_PLAYER = 1 << 1,
    QUEEN_EARTH_WALKER_LOOK_LATCHED = 1 << 2,
    QUEEN_EARTH_WALKER_EYES_CLOSED = 1 << 3,
    QUEEN_EARTH_WALKER_SUPPRESS_IDLE_SEQUENCE = 1 << 4,
    QUEEN_EARTH_WALKER_EVENTS_INITIALIZED = 1 << 5,
};

enum QueenEarthWalkerAnimEvent {
    QUEEN_EARTH_WALKER_EVENT_CLOSE_EYES,
    QUEEN_EARTH_WALKER_EVENT_OPEN_EYES,
    QUEEN_EARTH_WALKER_EVENT_BEGIN_TARGETING,
    QUEEN_EARTH_WALKER_EVENT_END_TARGETING,
};

enum QueenEarthWalkerMapAct {
    QUEEN_EARTH_WALKER_ACT_RETURN = 1,
    QUEEN_EARTH_WALKER_ACT_FEED,
    QUEEN_EARTH_WALKER_ACT_MOON_PASS_KEY_A,
    QUEEN_EARTH_WALKER_ACT_MOON_PASS_KEY_B,
    QUEEN_EARTH_WALKER_ACT_PORTAL,
    QUEEN_EARTH_WALKER_ACT_SPELL,
    QUEEN_EARTH_WALKER_ACT_BERRY,
    QUEEN_EARTH_WALKER_ACT_DEPARTURE,
};

enum QueenEarthWalkerObjectGroup {
    QUEEN_EARTH_WALKER_TARGET_GROUP = 0xF,
};

enum QueenEarthWalkerSequence {
    QUEEN_EARTH_WALKER_SEQUENCE_ACT_1 = 0,
    QUEEN_EARTH_WALKER_SEQUENCE_RETURNED = 1,
    QUEEN_EARTH_WALKER_SEQUENCE_FEED_REACTION_A = 3,
    QUEEN_EARTH_WALKER_SEQUENCE_FEED_REACTION_B = 4,
    QUEEN_EARTH_WALKER_SEQUENCE_FEED_COMPLETE = 5,
    QUEEN_EARTH_WALKER_SEQUENCE_FED = 6,
    QUEEN_EARTH_WALKER_SEQUENCE_DEPARTURE = 7,
};

enum QueenEarthWalkerSequenceTarget {
    QUEEN_EARTH_WALKER_ACT_1_TARGET = 0x1324,
    QUEEN_EARTH_WALKER_QUEEN_TARGET = 0x18F6,
    QUEEN_EARTH_WALKER_DEPARTURE_TARGET = 0x6A4,
};

enum QueenEarthWalkerSequenceFlag {
    QUEEN_EARTH_WALKER_SEQUENCE_DEFAULT_FLAGS = -1,
    QUEEN_EARTH_WALKER_SEQUENCE_QUEEN_FLAGS = 1,
    QUEEN_EARTH_WALKER_SEQUENCE_DEPARTURE_FLAGS = 8,
    QUEEN_EARTH_WALKER_SEQUENCE_ACT_1_FLAGS = 0x10,
};

enum QueenEarthWalkerHitVolumeMode {
    QUEEN_EARTH_WALKER_HIT_VOLUME_NEAR_TRICKY = 2,
    QUEEN_EARTH_WALKER_HIT_VOLUME_FEED = 4,
};

enum QueenEarthWalkerSpell {
    QUEEN_EARTH_WALKER_OPEN_PORTAL_SPELL = 3,
};

enum QueenEarthWalkerAudioChannel {
    QUEEN_EARTH_WALKER_LOOPING_SFX_CHANNEL = 0x7F,
};

typedef struct QueenEarthWalkerAnimation {
    s16 moveId;
    f32 stepScale;
} QueenEarthWalkerAnimation;

typedef struct QueenEarthWalkerEventTable {
    u8 count;
    u8 sequenceIds[5];
} QueenEarthWalkerEventTable;

typedef struct QueenEarthWalkerPlacement {
    ObjPlacement base;
    s8 yawByte;
} QueenEarthWalkerPlacement;

typedef struct QueenEarthWalkerState {
    u8 stateId;
    u8 unused01;
    u8 flags;
    u8 unused03[5];
    CharacterEyeAnimState eyeAnimState;
    u8 unused30[8];
    const QueenEarthWalkerEventTable* eventTable;
    f32 attackTimer;
} QueenEarthWalkerState;

STATIC_ASSERT(offsetof(QueenEarthWalkerPlacement, yawByte) == 0x18);
STATIC_ASSERT(offsetof(QueenEarthWalkerState, stateId) == 0x00);
STATIC_ASSERT(offsetof(QueenEarthWalkerState, flags) == 0x02);
STATIC_ASSERT(offsetof(QueenEarthWalkerState, eyeAnimState) == 0x08);
STATIC_ASSERT(offsetof(QueenEarthWalkerState, eventTable) ==
              offsetof(QueenEarthWalkerState, eyeAnimState) + sizeof(CharacterEyeAnimState) + 8);
STATIC_ASSERT(offsetof(QueenEarthWalkerState, attackTimer) ==
              offsetof(QueenEarthWalkerState, eventTable) + sizeof(void*));

static const QueenEarthWalkerEventTable sQueenEarthWalkerEventTableAct1 = {1, {0}};
static const QueenEarthWalkerEventTable sQueenEarthWalkerEventTableAct2 = {1, {0x14}};
static const QueenEarthWalkerEventTable sQueenEarthWalkerEventTableFed = {2, {0x0C, 0x0A}};
static const QueenEarthWalkerEventTable sQueenEarthWalkerEventTableFeed = {1, {0x0E}};
static const QueenEarthWalkerEventTable sQueenEarthWalkerEventTablePortalDefault = {1, {0x0F}};
static const QueenEarthWalkerEventTable sQueenEarthWalkerEventTablePortalReady = {1, {0x10}};
static const QueenEarthWalkerEventTable sQueenEarthWalkerEventTableSpell = {1, {0x11}};
static const QueenEarthWalkerEventTable sQueenEarthWalkerEventTableBerry = {1, {0x12}};
static const QueenEarthWalkerEventTable sQueenEarthWalkerEventTableDeparture = {1, {0x13}};
static const QueenEarthWalkerEventTable sQueenEarthWalkerEventTableComplete = {5, {7, 8, 9, 0x0A, 0x0B}};

static const QueenEarthWalkerAnimation sQueenEarthWalkerAnimations[] = {
    [QUEEN_EARTH_WALKER_WAITING_FOR_RETURN] = {34, 0.005f}, [QUEEN_EARTH_WALKER_READY_TO_FEED] = {34, 0.005f},
    [QUEEN_EARTH_WALKER_FINISHING_FEED] = {34, 0.005f},     [QUEEN_EARTH_WALKER_FED] = {5, 0.01f},
    [QUEEN_EARTH_WALKER_DEPARTING] = {28, 0.005f},
};

static f32 queenEarthWalker_xzDistanceSquared(const Vec3f* a, const Vec3f* b) {
    f32 dx = a->x - b->x;
    f32 dz = a->z - b->z;

    return dx * dx + dz * dz;
}

static void queenEarthWalker_lookAtPlayer(GameObject* obj, QueenEarthWalkerState* state) {
    GameObject* player = Obj_GetPlayerObject();

    state->eyeAnimState.lookAtActive = 1;
    state->eyeAnimState.lookAtPosX = player->anim.localPosX;
    state->eyeAnimState.lookAtPosY = player->anim.localPosY;
    state->eyeAnimState.lookAtPosZ = player->anim.localPosZ;
    characterHeadLookCalm(obj, (s16*)&state->eyeAnimState, 0.0f);
}

static void queenEarthWalker_updateEyes(GameObject* obj, QueenEarthWalkerState* state) {
    if (state->flags & QUEEN_EARTH_WALKER_EYES_CLOSED) {
        characterCloseEyes(obj, &state->eyeAnimState);
    } else {
        characterDoEyeAnims(obj, &state->eyeAnimState);
    }
}

static int queenEarthWalker_processAnimEvents(GameObject* obj, int unusedArg, ObjSeqState* animUpdate) {
    QueenEarthWalkerState* state = obj->extra;

    (void)unusedArg;

    if (!(state->flags & QUEEN_EARTH_WALKER_EVENTS_INITIALIZED)) {
        Sfx_StopObjectChannel(obj, QUEEN_EARTH_WALKER_LOOPING_SFX_CHANNEL);
        state->flags &= ~QUEEN_EARTH_WALKER_SUPPRESS_IDLE_SEQUENCE;
        state->flags |= QUEEN_EARTH_WALKER_EVENTS_INITIALIZED;
    }

    for (int i = 0; i < animUpdate->eventCount; i++) {
        switch (animUpdate->eventIds[i]) {
        case QUEEN_EARTH_WALKER_EVENT_CLOSE_EYES:
            state->flags |= QUEEN_EARTH_WALKER_EYES_CLOSED;
            break;
        case QUEEN_EARTH_WALKER_EVENT_OPEN_EYES:
            state->flags &= ~QUEEN_EARTH_WALKER_EYES_CLOSED;
            break;
        case QUEEN_EARTH_WALKER_EVENT_BEGIN_TARGETING:
            state->flags |= QUEEN_EARTH_WALKER_TARGETING_PLAYER;
            break;
        case QUEEN_EARTH_WALKER_EVENT_END_TARGETING:
            state->flags &= ~QUEEN_EARTH_WALKER_TARGETING_PLAYER;
            animUpdate->flags |= OBJSEQ_APPLY_JOINT_ROTATION_TRACKS | OBJSEQ_APPLY_TEXTURE_SCROLL_TRACK;
            break;
        }
    }

    u8 flags = state->flags;
    if (flags & QUEEN_EARTH_WALKER_TARGETING_PLAYER) {
        if (!(flags & QUEEN_EARTH_WALKER_LOOK_LATCHED)) {
            animUpdate->flags &= ~OBJSEQ_APPLY_JOINT_ROTATION_TRACKS;
            queenEarthWalker_lookAtPlayer(obj, state);
        }
        animUpdate->flags &= ~OBJSEQ_APPLY_TEXTURE_SCROLL_TRACK;
        queenEarthWalker_updateEyes(obj, state);
    }

    return 0;
}

static void queenEarthWalker_updatePortal(GameObject* obj, QueenEarthWalkerState* state) {
    GameObject* player = Obj_GetPlayerObject();

    obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_DISABLED;
    if (mainGetBit(GAMEBIT_SH_QueenPortalComplete) != 0) {
        state->eventTable = &sQueenEarthWalkerEventTableComplete;
    } else if (mainGetBit(GAMEBIT_SH_Related023C) != 0) {
        state->eventTable = &sQueenEarthWalkerEventTablePortalReady;
    } else if (mainGetBit(GAMEBIT_STAFF_ABILITY_OPEN_PORTAL) != 0) {
        obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
        if (playerHasSpell(player, QUEEN_EARTH_WALKER_OPEN_PORTAL_SPELL) != 0 &&
            queenEarthWalker_xzDistanceSquared((Vec3f*)&player->anim.worldPosX, (Vec3f*)&obj->anim.worldPosX) < 10000.0f) {
            mainSetBits(GAMEBIT_SH_OpenPortalRequested, 1);
        }
    } else if (mainGetBit(GAMEBIT_SH_RescuedEggs) != 0) {
        state->eventTable = &sQueenEarthWalkerEventTableComplete;
    } else {
        state->eventTable = &sQueenEarthWalkerEventTablePortalDefault;
    }

    queenEarthWalker_lookAtPlayer(obj, state);
}

static void queenEarthWalker_updateFeeding(GameObject* obj, QueenEarthWalkerState* state) {
    switch (state->stateId) {
    case QUEEN_EARTH_WALKER_WAITING_FOR_RETURN:
        if (mainGetBit(GAMEBIT_SH_ReturnedToQueen) != 0) {
            (*gObjectTriggerInterface)
                ->runSequence(QUEEN_EARTH_WALKER_SEQUENCE_RETURNED, obj, QUEEN_EARTH_WALKER_SEQUENCE_DEFAULT_FLAGS);
            state->stateId = QUEEN_EARTH_WALKER_READY_TO_FEED;
        }
        break;
    case QUEEN_EARTH_WALKER_READY_TO_FEED: {
        s16 triggerId;

        obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_DISABLED;
        if (cMenuGetSelectedItem() == -1 &&
            (getYButtonItem(&triggerId) == 0 || triggerId != GAMEBIT_ITEM_WhiteShroom_Count)) {
            GameObject* tricky = getTrickyObject();

            if (tricky != NULL &&
                queenEarthWalker_xzDistanceSquared((Vec3f*)&tricky->anim.worldPosX, (Vec3f*)&obj->anim.worldPosX) < 22500.0f) {
                Obj_SetActiveHitVolumeBounds(obj, 0, 0, 0, 0, QUEEN_EARTH_WALKER_HIT_VOLUME_NEAR_TRICKY);
            } else {
                obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
            }
            break;
        }

        Obj_SetActiveHitVolumeBounds(obj, 0, 0, 0, 0, QUEEN_EARTH_WALKER_HIT_VOLUME_FEED);
        if (ObjTrigger_IsSetById(obj, GAMEBIT_ITEM_WhiteShroom_Count) != 0) {
            state->flags |= QUEEN_EARTH_WALKER_SUPPRESS_IDLE_SEQUENCE;
            s32 total = mainGetBit(GAMEBIT_ITEM_WhiteShroom_Count);
            total += mainGetBit(GAMEBIT_ITEM_WhiteGrubTub_Used);
            mainSetBits(GAMEBIT_ITEM_WhiteShroom_Count, 0);
            mainSetBits(GAMEBIT_ITEM_WhiteGrubTub_Used, total);
            if (total != 6) {
                state->flags |= QUEEN_EARTH_WALKER_TARGETING_PLAYER;
                int sequence = randomGetRange(0, 1) != 0 ? QUEEN_EARTH_WALKER_SEQUENCE_FEED_REACTION_A
                                                         : QUEEN_EARTH_WALKER_SEQUENCE_FEED_REACTION_B;
                (*gObjectTriggerInterface)->runSequence(sequence, obj, QUEEN_EARTH_WALKER_SEQUENCE_DEFAULT_FLAGS);
            } else {
                (*gObjectTriggerInterface)
                    ->runSequence(QUEEN_EARTH_WALKER_SEQUENCE_FEED_COMPLETE, obj,
                                  QUEEN_EARTH_WALKER_SEQUENCE_DEFAULT_FLAGS);
                state->stateId = QUEEN_EARTH_WALKER_FINISHING_FEED;
            }
        }
        break;
    }
    case QUEEN_EARTH_WALKER_FINISHING_FEED:
        (*gObjectTriggerInterface)
            ->runSequence(QUEEN_EARTH_WALKER_SEQUENCE_FED, obj, QUEEN_EARTH_WALKER_SEQUENCE_DEFAULT_FLAGS);
        mainSetBits(GAMEBIT_SH_QueenFed, 1);
        state->stateId = QUEEN_EARTH_WALKER_FED;
        break;
    case QUEEN_EARTH_WALKER_FED:
        Obj_SetActiveHitVolumeBounds(obj, 0, 0, 0, 0, QUEEN_EARTH_WALKER_HIT_VOLUME_NEAR_TRICKY);
        state->flags &= ~QUEEN_EARTH_WALKER_LOOK_LATCHED;
        state->flags &= ~QUEEN_EARTH_WALKER_EYES_CLOSED;
        state->eventTable = &sQueenEarthWalkerEventTableFed;
        queenEarthWalker_lookAtPlayer(obj, state);
        break;
    default:
        break;
    }
}

static int queenEarthWalker_getExtraSize(void) {
    return sizeof(QueenEarthWalkerState);
}

static void queenEarthWalker_selectEventTable(GameObject* obj, QueenEarthWalkerState* state, u8 action) {
    switch (action) {
    case QUEEN_EARTH_WALKER_ACT_FEED:
        queenEarthWalker_updateFeeding(obj, state);
        break;
    case QUEEN_EARTH_WALKER_ACT_MOON_PASS_KEY_A:
    case QUEEN_EARTH_WALKER_ACT_MOON_PASS_KEY_B:
        state->eventTable = mainGetBit(GAMEBIT_ITEM_MoonPassKey_Got) != 0 ? &sQueenEarthWalkerEventTableComplete
                                                                          : &sQueenEarthWalkerEventTableFeed;
        queenEarthWalker_lookAtPlayer(obj, state);
        break;
    case QUEEN_EARTH_WALKER_ACT_PORTAL:
        queenEarthWalker_updatePortal(obj, state);
        break;
    case QUEEN_EARTH_WALKER_ACT_SPELL:
        state->eventTable = mainGetBit(GAMEBIT_ITEM_BigScarabBag_Got) != 0 ? &sQueenEarthWalkerEventTableComplete
                                                                           : &sQueenEarthWalkerEventTableSpell;
        queenEarthWalker_lookAtPlayer(obj, state);
        break;
    case QUEEN_EARTH_WALKER_ACT_BERRY:
        state->eventTable = mainGetBit(GAMEBIT_SH_ThornTailRelated0199) != 0 ? &sQueenEarthWalkerEventTableComplete
                                                                             : &sQueenEarthWalkerEventTableBerry;
        queenEarthWalker_lookAtPlayer(obj, state);
        break;
    case QUEEN_EARTH_WALKER_ACT_DEPARTURE:
        queenEarthWalker_lookAtPlayer(obj, state);
        break;
    default:
        break;
    }
}

static void queenEarthWalker_startAct(GameObject* obj, QueenEarthWalkerState* state, u8 action) {
    switch (action) {
    case QUEEN_EARTH_WALKER_ACT_RETURN: {
        GameObject* target = objGetNearestTypeTo(QUEEN_EARTH_WALKER_TARGET_GROUP, obj, NULL);

        (*gObjectTriggerInterface)->preempt((uintptr_t)target, QUEEN_EARTH_WALKER_ACT_1_TARGET);
        (*gObjectTriggerInterface)
            ->runSequence(QUEEN_EARTH_WALKER_SEQUENCE_RETURNED, target, QUEEN_EARTH_WALKER_SEQUENCE_ACT_1_FLAGS);
        state->flags |= QUEEN_EARTH_WALKER_LOOK_LATCHED | QUEEN_EARTH_WALKER_EYES_CLOSED;
        state->eventTable = &sQueenEarthWalkerEventTableAct1;
        break;
    }
    case QUEEN_EARTH_WALKER_ACT_FEED:
        if (mainGetBit(GAMEBIT_ITEM_WhiteGrubTub_Used) == 6) {
            (*gObjectTriggerInterface)->preempt((uintptr_t)obj, QUEEN_EARTH_WALKER_QUEEN_TARGET);
            (*gObjectTriggerInterface)
                ->runSequence(QUEEN_EARTH_WALKER_SEQUENCE_FED, obj, QUEEN_EARTH_WALKER_SEQUENCE_QUEEN_FLAGS);
            state->stateId = QUEEN_EARTH_WALKER_FED;
        } else {
            if (mainGetBit(GAMEBIT_SH_ReturnedToQueen) != 0) {
                state->stateId = QUEEN_EARTH_WALKER_READY_TO_FEED;
            }
            state->flags |= QUEEN_EARTH_WALKER_LOOK_LATCHED | QUEEN_EARTH_WALKER_EYES_CLOSED;
            state->eventTable = &sQueenEarthWalkerEventTableAct2;
        }
        break;
    case QUEEN_EARTH_WALKER_ACT_MOON_PASS_KEY_A:
    case QUEEN_EARTH_WALKER_ACT_MOON_PASS_KEY_B:
    case QUEEN_EARTH_WALKER_ACT_PORTAL:
    case QUEEN_EARTH_WALKER_ACT_SPELL:
    case QUEEN_EARTH_WALKER_ACT_BERRY:
        (*gObjectTriggerInterface)->preempt((uintptr_t)obj, QUEEN_EARTH_WALKER_QUEEN_TARGET);
        (*gObjectTriggerInterface)
            ->runSequence(QUEEN_EARTH_WALKER_SEQUENCE_FED, obj, QUEEN_EARTH_WALKER_SEQUENCE_QUEEN_FLAGS);
        state->stateId = QUEEN_EARTH_WALKER_FED;
        break;
    case QUEEN_EARTH_WALKER_ACT_DEPARTURE: {
        GameObject* target = objGetNearestTypeTo(QUEEN_EARTH_WALKER_TARGET_GROUP, obj, NULL);

        (*gObjectTriggerInterface)->preempt((uintptr_t)target, QUEEN_EARTH_WALKER_DEPARTURE_TARGET);
        (*gObjectTriggerInterface)
            ->runSequence(QUEEN_EARTH_WALKER_SEQUENCE_DEPARTURE, target, QUEEN_EARTH_WALKER_SEQUENCE_DEPARTURE_FLAGS);
        state->stateId = QUEEN_EARTH_WALKER_DEPARTING;
        state->eventTable = &sQueenEarthWalkerEventTableDeparture;
        break;
    }
    default:
        break;
    }

    state->flags |= QUEEN_EARTH_WALKER_STARTED;
}

static void queenEarthWalker_update(GameObject* obj) {
    QueenEarthWalkerState* state = obj->extra;

    state->flags &= ~QUEEN_EARTH_WALKER_EVENTS_INITIALIZED;
    u8 action = (*gMapEventInterface)->getMapAct(obj->anim.mapEventSlot);
    if (!(state->flags & QUEEN_EARTH_WALKER_STARTED)) {
        queenEarthWalker_startAct(obj, state, action);
        return;
    }

    queenEarthWalker_selectEventTable(obj, state, action);
    queenEarthWalker_updateEyes(obj, state);

    const QueenEarthWalkerAnimation* animation = &sQueenEarthWalkerAnimations[state->stateId];
    if (obj->anim.currentMove != animation->moveId) {
        ObjAnim_SetCurrentMove(obj, animation->moveId, 0.0f, 0);
    }
    ObjAnim_AdvanceCurrentMove(obj, animation->stepScale, timeDelta, NULL);

    u8 flags = state->flags;
    if (!(flags & QUEEN_EARTH_WALKER_SUPPRESS_IDLE_SEQUENCE)) {
        state->flags &= ~QUEEN_EARTH_WALKER_TARGETING_PLAYER;
        if (ObjTrigger_IsSet(obj) != 0 && obj->anim.hitVolumeBounds->flags != QUEEN_EARTH_WALKER_HIT_VOLUME_FEED) {
            u8 eventIndex = randomGetRange(1, state->eventTable->count);
            state->flags |= QUEEN_EARTH_WALKER_TARGETING_PLAYER;
            (*gObjectTriggerInterface)
                ->runSequence(state->eventTable->sequenceIds[eventIndex - 1], obj,
                              QUEEN_EARTH_WALKER_SEQUENCE_DEFAULT_FLAGS);
        }
    }

    if (RandomTimer_UpdateRangeTrigger(&state->attackTimer, 2.0f, 5.0f) != 0) {
        Sfx_PlayFromObject(obj, SFXTRIG_thorntail);
    }
}

static void queenEarthWalker_init(GameObject* obj, QueenEarthWalkerPlacement* placement) {
    obj->anim.rotX = (s16)((s32)placement->yawByte * 0x100);
    obj->animEventCallback = queenEarthWalker_processAnimEvents;
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN;
}

OBJECT_INIT_ADAPTER(gSH_queenearthwalkerObjDescriptorInitAdapter, queenEarthWalker_init, obj, placement)
OBJECT_EXTRA_SIZE_ADAPTER(gSH_queenearthwalkerObjDescriptorExtraSizeAdapter, queenEarthWalker_getExtraSize)

ObjectDescriptor gSH_queenearthwalkerObjDescriptor = {
    .header = {
        .metadata = { 0, 0, 0, OBJECT_DESCRIPTOR_FLAGS_10_SLOTS },
        .acquire = NULL,
        .release = NULL,
    },
    .slot02 = NULL,
    .init = gSH_queenearthwalkerObjDescriptorInitAdapter,
    .update = queenEarthWalker_update,
    .hitDetect = NULL,
    .render = NULL,
    .free = NULL,
    .getObjectTypeId = NULL,
    .getExtraSize = gSH_queenearthwalkerObjDescriptorExtraSizeAdapter,
};;
