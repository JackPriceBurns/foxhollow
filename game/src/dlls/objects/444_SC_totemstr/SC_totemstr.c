#include "dlls/objects/444_SC_totemstr.h"

#include "dlls/objects/438_SC_levelcon.h"
#include "dlls/objects/443_SC_totembon.h"
#include "dolphin/pad.h"
#include "main/audio/sfx.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/camera_interface.h"
#include "main/dll/CAM/dll_0001_camcontrol.h"
#include "main/dll/tricky.h"
#include "main/frame_timing.h"
#include "main/game_timer_control.h"
#include "main/gamebit_ids.h"
#include "main/gamebits.h"
#include "main/mapEventTypes.h"
#include "main/model_engine.h"
#include "main/obj_list.h"
#include "main/objanim.h"
#include "main/objseq.h"
#include "main/object_render.h"
#include "main/pad.h"
#include "main/screen_transition.h"
#include "main/vecmath.h"
#include "sys/objects.h"

enum ScTotemStrengthRomDefNo {
    SC_TOTEM_STRENGTH_OPPONENT_ROM_DEF_NO = 0x3FF,
};

enum ScTotemStrengthMoveId {
    SC_TOTEM_STRENGTH_OPPONENT_PULL_MOVE = 0,
    SC_TOTEM_STRENGTH_PLAYER_PULL_MOVE = 0x401,
};

enum ScTotemStrengthSequenceEvent {
    SC_TOTEM_STRENGTH_SEQUENCE_EVENT_ARM = 1,
    SC_TOTEM_STRENGTH_SEQUENCE_EVENT_START = 2,
    SC_TOTEM_STRENGTH_SEQUENCE_EVENT_CLEAR_LEVEL_CONTROL = 3,
    SC_TOTEM_STRENGTH_SEQUENCE_EVENT_START_LEVEL_CONTROL = 4,
    SC_TOTEM_STRENGTH_SEQUENCE_EVENT_START_PULLING = 5,
};

enum ScTotemStrengthFlag {
    SC_TOTEM_STRENGTH_FLAG_ARMED = 0x01,
    SC_TOTEM_STRENGTH_FLAG_START_REQUESTED = 0x02,
    SC_TOTEM_STRENGTH_FLAG_TRIGGER_MASK = SC_TOTEM_STRENGTH_FLAG_ARMED | SC_TOTEM_STRENGTH_FLAG_START_REQUESTED,
    SC_TOTEM_STRENGTH_FLAG_ACTIVE = 0x04,
    SC_TOTEM_STRENGTH_FLAG_WON = 0x08,
    SC_TOTEM_STRENGTH_FLAG_LOST = 0x10,
};

enum ScTotemStrengthTransitionStep {
    SC_TOTEM_STRENGTH_TRANSITION_RESET = 0,
    SC_TOTEM_STRENGTH_TRANSITION_START = 1,
    SC_TOTEM_STRENGTH_TRANSITION_FINISHED = 2,
    SC_TOTEM_STRENGTH_TRANSITION_LOST = 3,
};

enum ScTotemStrengthTrackOffset {
    SC_TOTEM_STRENGTH_TRACK_INITIAL = -0x2900,
    SC_TOTEM_STRENGTH_TRACK_WIN = -0x46DC,
    SC_TOTEM_STRENGTH_TRACK_LOSS = -0xB24,
};

enum ScTotemStrengthSequenceState {
    SC_TOTEM_STRENGTH_SEQUENCE_READY_INDEX = 0x19,
    SC_TOTEM_STRENGTH_SEQUENCE_RESULT_COMPLETE = 4,
};

typedef struct ScTotemStrengthState {
    GameObject* opponent;
    f32 unused08;
    f32 trackVelocity;
    Vec3f savedPosition;
    f32 playerSfxTimer;
    f32 mechanismSfxTimer;
    s32 trackOffset;
    s32 sequenceIndex;
    s32 previousTrackOffset;
    u8 unused30[2];
    s16 transitionStep;
    u8 flags;
    u8 unused35[3];
} ScTotemStrengthState;

STATIC_ASSERT(sizeof(ScTotemStrengthState) == 0x38);
STATIC_ASSERT(offsetof(ScTotemStrengthState, opponent) == 0x00);
STATIC_ASSERT(offsetof(ScTotemStrengthState, unused08) == 0x08);
STATIC_ASSERT(offsetof(ScTotemStrengthState, trackVelocity) == 0x0C);
STATIC_ASSERT(offsetof(ScTotemStrengthState, savedPosition) == 0x10);
STATIC_ASSERT(offsetof(ScTotemStrengthState, playerSfxTimer) == 0x1C);
STATIC_ASSERT(offsetof(ScTotemStrengthState, mechanismSfxTimer) == 0x20);
STATIC_ASSERT(offsetof(ScTotemStrengthState, trackOffset) == 0x24);
STATIC_ASSERT(offsetof(ScTotemStrengthState, sequenceIndex) == 0x28);
STATIC_ASSERT(offsetof(ScTotemStrengthState, previousTrackOffset) == 0x2C);
STATIC_ASSERT(offsetof(ScTotemStrengthState, unused30) == 0x30);
STATIC_ASSERT(offsetof(ScTotemStrengthState, transitionStep) == 0x32);
STATIC_ASSERT(offsetof(ScTotemStrengthState, flags) == 0x34);
STATIC_ASSERT(offsetof(ScTotemStrengthState, unused35) == 0x35);

static const u16 sTotemStrengthRecordGameBits[] = {
    GAMEBIT_LV_TestStrengthBestTime1,
    GAMEBIT_LV_TestStrengthBestTime2,
    GAMEBIT_LV_TestStrengthBestTime3,
};

static int sTotemStrengthDeactivateTimer;

static GameObject* sc_totemstrength_findOpponent(void) {
    int objectIndex;
    int objectCount;
    GameObject** objects = ObjList_GetObjects(&objectIndex, &objectCount);
    GameObject* opponent = NULL;

    while (objectIndex < objectCount) {
        opponent = objects[objectIndex++];
        if (opponent->anim.romDefNo == SC_TOTEM_STRENGTH_OPPONENT_ROM_DEF_NO) {
            break;
        }
    }
    return opponent;
}

static void sc_totemstrength_setLevelControlState(GameObject* self, enum ScLevelControlAnimState animEventState) {
    int objectIndex;
    int objectCount;
    GameObject** objects = ObjList_GetObjects(&objectIndex, &objectCount);

    for (; objectIndex < objectCount; objectIndex++) {
        GameObject* levelControl = objects[objectIndex];

        if (levelControl != self && levelControl->anim.romDefNo == SC_LEVEL_CONTROL_ROM_DEF_NO) {
            sc_levelcontrol_setAnimEventState(levelControl, animEventState);
            break;
        }
    }
}

static int sc_totemstrength_finishRound(GameObject* obj, ScTotemStrengthState* state, u8 resultFlag,
                                        enum ScTotemStrengthTransitionStep transitionStep) {
    state->transitionStep = transitionStep;
    state->flags &= (u8)~SC_TOTEM_STRENGTH_FLAG_TRIGGER_MASK;
    state->flags |= resultFlag;
    sc_totemstrength_setLevelControlState(obj, SC_LEVEL_CONTROL_ANIM_STATE_STOP_TREX_CHALLENGE);
    if (resultFlag == SC_TOTEM_STRENGTH_FLAG_WON) {
        sc_totembond_insertOrderedGameBit(sTotemStrengthRecordGameBits,
                                          (u16)(gameTimerGetElapsedMilliseconds() / 10.0f));
    }
    setHudForceShowMask(0);
    if (state->sequenceIndex > 0) {
        ObjSeq_takeXrotChanged(state->sequenceIndex);
    }
    (*gScreenTransitionInterface)->step(20, SCREEN_TRANSITION_BLACK);
    sTotemStrengthDeactivateTimer = 2;
    return SC_TOTEM_STRENGTH_SEQUENCE_RESULT_COMPLETE;
}

static int sc_totemstrength_animEventCallback(GameObject* obj, int unused, ObjSeqState* animUpdate) {
    ScTotemStrengthState* state = obj->extra;
    GameObject* player = Obj_GetPlayerObject();
    f32 trackDelta;

    (void)unused;

    state->flags |= SC_TOTEM_STRENGTH_FLAG_ACTIVE;
    setAButtonIcon(A_BUTTON_ICON_CONTEXT_B);
    sTotemStrengthDeactivateTimer = 0;
    state->opponent = sc_totemstrength_findOpponent();

    for (int eventIndex = 0; eventIndex < animUpdate->eventCount; eventIndex++) {
        switch (animUpdate->eventIds[eventIndex]) {
        case SC_TOTEM_STRENGTH_SEQUENCE_EVENT_ARM:
            state->flags |= SC_TOTEM_STRENGTH_FLAG_ARMED;
            break;
        case SC_TOTEM_STRENGTH_SEQUENCE_EVENT_START:
            state->flags |= SC_TOTEM_STRENGTH_FLAG_START_REQUESTED;
            state->transitionStep = SC_TOTEM_STRENGTH_TRANSITION_RESET;
            (*gObjectTriggerInterface)->setCamVars(CAMCONTROL_ACTION_TRIGGER_TYPE1, 3, 0, 0);
            break;
        case SC_TOTEM_STRENGTH_SEQUENCE_EVENT_CLEAR_LEVEL_CONTROL:
            sc_totemstrength_setLevelControlState(obj, SC_LEVEL_CONTROL_ANIM_STATE_CLEAR);
            break;
        case SC_TOTEM_STRENGTH_SEQUENCE_EVENT_START_LEVEL_CONTROL:
            sc_totemstrength_setLevelControlState(obj, SC_LEVEL_CONTROL_ANIM_STATE_START_TREX_CHALLENGE);
            break;
        case SC_TOTEM_STRENGTH_SEQUENCE_EVENT_START_PULLING:
            if (state->opponent != NULL) {
                player->anim.currentMoveProgress = 0.5f;
                state->opponent->anim.currentMoveProgress = 0.5f;
                ObjAnim_SetCurrentMove(player, SC_TOTEM_STRENGTH_PLAYER_PULL_MOVE, player->anim.currentMoveProgress, 0);
                ObjAnim_SetCurrentMove(state->opponent, SC_TOTEM_STRENGTH_OPPONENT_PULL_MOVE,
                                       state->opponent->anim.currentMoveProgress, 0);
                state->previousTrackOffset = state->trackOffset;
            }
            break;
        }
    }

    if ((state->flags & SC_TOTEM_STRENGTH_FLAG_TRIGGER_MASK) == 0 ||
        state->sequenceIndex < SC_TOTEM_STRENGTH_SEQUENCE_READY_INDEX) {
        return 0;
    }

    if ((*gCameraInterface)->getMode() != CAMCONTROL_ACTION_TRIGGER_TYPE1) {
        CamcontrolQueuedActionParam action = {
            .actionIndex = 3,
            .noBlendFlag = 1,
        };

        (*gCameraInterface)
            ->setMode(CAMCONTROL_ACTION_TRIGGER_TYPE1, 1, 3, sizeof(action), &action, 0, CAMCONTROL_QUEUE_SENTINEL);
    }
    if (player->anim.currentMove != SC_TOTEM_STRENGTH_PLAYER_PULL_MOVE) {
        ObjAnim_SetCurrentMove(player, SC_TOTEM_STRENGTH_PLAYER_PULL_MOVE, player->anim.currentMoveProgress, 0);
    }
    if (state->opponent->anim.currentMove != SC_TOTEM_STRENGTH_OPPONENT_PULL_MOVE) {
        ObjAnim_SetCurrentMove(state->opponent, SC_TOTEM_STRENGTH_OPPONENT_PULL_MOVE,
                               state->opponent->anim.currentMoveProgress, 0);
    }
    animUpdate->flags = -1;
    animUpdate->movementState = 0;
    Sfx_KeepAliveLoopedObjectSound(obj, SFXTRIG_blockscrape_lp);

    for (int frame = 0; frame < framesThisStep; frame++) {
        f32 normalizedOffset;
        f32 distanceFromCenter;
        f32 push;

        if (state->opponent == NULL) {
            return 0;
        }
        normalizedOffset = (f32)(state->trackOffset - SC_TOTEM_STRENGTH_TRACK_LOSS) /
                           (SC_TOTEM_STRENGTH_TRACK_WIN - SC_TOTEM_STRENGTH_TRACK_LOSS);
        distanceFromCenter = 2.0f * normalizedOffset - 1.0f;
        if (distanceFromCenter < 0.0f) {
            distanceFromCenter = -distanceFromCenter;
        }
        push = (1.7f * normalizedOffset + 0.2f) * distanceFromCenter + 1.0f;
        if ((getButtonsJustPressedIfNotBusy(0) & PAD_BUTTON_A) != 0 && isGameTimerDisabled() == 0) {
            state->trackVelocity -= 2.7f;
        }
        if (state->trackVelocity < -40.0f) {
            state->trackVelocity = -40.0f;
        }
        if (state->trackOffset >= SC_TOTEM_STRENGTH_TRACK_WIN && state->trackOffset <= SC_TOTEM_STRENGTH_TRACK_LOSS) {
            state->trackOffset = (s32)((f32)state->trackOffset + state->trackVelocity);
        }
        trackDelta = (f32)(state->previousTrackOffset - state->trackOffset) / 40.0f;
        if (state->trackOffset < SC_TOTEM_STRENGTH_TRACK_WIN) {
            return sc_totemstrength_finishRound(obj, state, SC_TOTEM_STRENGTH_FLAG_WON,
                                                SC_TOTEM_STRENGTH_TRANSITION_RESET);
        }
        if (state->trackOffset > SC_TOTEM_STRENGTH_TRACK_LOSS) {
            return sc_totemstrength_finishRound(obj, state, SC_TOTEM_STRENGTH_FLAG_LOST,
                                                SC_TOTEM_STRENGTH_TRANSITION_LOST);
        }
        if (state->sequenceIndex > 0) {
            (*gObjectTriggerInterface)->setXrot(state->sequenceIndex, state->trackOffset);
        }
        if (state->trackVelocity < 40.0f) {
            state->trackVelocity += 0.19f * push;
        }
        if (ObjAnim_AdvanceCurrentMove(player, (f32)(state->previousTrackOffset - state->trackOffset) / 9500.0f,
                                       timeDelta, 0) != 0 &&
            player->anim.currentMoveProgress < 0.0f) {
            player->anim.currentMoveProgress += 1.0f;
        }
        if (ObjAnim_AdvanceCurrentMove(
                state->opponent, (f32)(state->trackOffset - state->previousTrackOffset) / 9500.0f, timeDelta, 0) != 0 &&
            state->opponent->anim.currentMoveProgress < 0.0f) {
            state->opponent->anim.currentMoveProgress += 1.0f;
        }
        state->previousTrackOffset = state->trackOffset;
    }

    state->playerSfxTimer -= timeDelta;
    if (state->playerSfxTimer < 0.0f) {
        state->playerSfxTimer = trackDelta < 0.0f ? (f32)randomGetRange(40, 100) : (f32)randomGetRange(120, 240);
        Sfx_PlayFromObject(player, SFXTRIG_literun116_var);
    }
    state->mechanismSfxTimer -= timeDelta;
    if (state->mechanismSfxTimer < 0.0f) {
        state->mechanismSfxTimer = trackDelta > 0.0f ? (f32)randomGetRange(40, 100) : (f32)randomGetRange(120, 240);
        Sfx_PlayFromObject(obj, SFXTRIG_spotfox03);
    }

    f32 absoluteTrackDelta = trackDelta < 0.0f ? -trackDelta : trackDelta;
    int volume = (int)(100.0f * absoluteTrackDelta);

    if (volume > 100) {
        volume = 100;
    }
    Sfx_SetObjectSfxVolume(obj, SFXTRIG_blockscrape_lp, volume & 0xFF, 127.0f);
    return 0;
}

static int sc_totemstrength_getExtraSize(void) {
    return sizeof(ScTotemStrengthState);
}

static int sc_totemstrength_getObjectTypeId(void) {
    return 0;
}

static void sc_totemstrength_free(void) {
}

static void sc_totemstrength_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                                    s8 visible) {
    (void)visible;

    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
}

static void sc_totemstrength_hitDetect(void) {
}

static void sc_totemstrength_update(GameObject* obj) {
    ScTotemStrengthState* state = obj->extra;

    Obj_GetPlayerObject();
    mainSetBits(GAMEBIT_LV_TestStrengthSequenceActive, 0);
    if ((*gMapEventInterface)->getMapAct(SC_LEVEL_CONTROL_MAP_SWAPCIRCLE) != SC_LEVEL_CONTROL_MAP_ACT_ESCAPE_COMPLETE) {
        return;
    }

    if ((state->flags & SC_TOTEM_STRENGTH_FLAG_ACTIVE) != 0) {
        if (state->sequenceIndex > 0) {
            (*gObjectTriggerInterface)->endSequence(state->sequenceIndex);
            ObjSeq_takeXrotChanged(state->sequenceIndex);
        }
        if (sTotemStrengthDeactivateTimer-- == 0) {
            u8 flags;

            state->flags &= (u8)~SC_TOTEM_STRENGTH_FLAG_ACTIVE;
            obj->anim.localPosX = state->savedPosition.x;
            obj->anim.localPosY = state->savedPosition.y;
            obj->anim.localPosZ = state->savedPosition.z;
            state->opponent = NULL;
            obj->anim.rotX = SC_TOTEM_STRENGTH_TRACK_INITIAL;
            state->trackOffset = SC_TOTEM_STRENGTH_TRACK_INITIAL;
            flags = state->flags;
            if ((flags & SC_TOTEM_STRENGTH_FLAG_WON) != 0) {
                mainSetBits(GAMEBIT_LV_TestStrengthWon, 1);
                state->sequenceIndex = OBJECT_SEQUENCE_INDEX_NONE;
                state->flags &= (u8)~SC_TOTEM_STRENGTH_FLAG_TRIGGER_MASK;
                state->flags &= (u8)~SC_TOTEM_STRENGTH_FLAG_WON;
            } else if ((flags & SC_TOTEM_STRENGTH_FLAG_LOST) != 0) {
                state->flags = flags & (u8)~SC_TOTEM_STRENGTH_FLAG_LOST;
                state->sequenceIndex = OBJECT_SEQUENCE_INDEX_NONE;
                mainSetBits(GAMEBIT_LV_TestStrengthLost, 1);
            }
        }
    } else if ((state->flags & SC_TOTEM_STRENGTH_FLAG_START_REQUESTED) != 0) {
        switch (state->transitionStep) {
        case SC_TOTEM_STRENGTH_TRANSITION_RESET:
            obj->anim.rotX = SC_TOTEM_STRENGTH_TRACK_INITIAL;
            state->trackOffset = SC_TOTEM_STRENGTH_TRACK_INITIAL;
            state->previousTrackOffset = state->trackOffset;
            state->unused08 = 0.0f;
            state->trackVelocity = 0.0f;
            state->transitionStep = SC_TOTEM_STRENGTH_TRANSITION_START;
            state->flags &= (u8)~SC_TOTEM_STRENGTH_FLAG_ARMED;
            break;
        case SC_TOTEM_STRENGTH_TRANSITION_START:
            mainSetBits(GAMEBIT_LV_TestStrengthSequenceActive, 1);
            setHudForceShowMask(1);
            state->sequenceIndex = (*gObjectTriggerInterface)->runSequence(0, obj, -1);
            break;
        case SC_TOTEM_STRENGTH_TRANSITION_FINISHED:
        case SC_TOTEM_STRENGTH_TRANSITION_LOST:
            state->transitionStep = SC_TOTEM_STRENGTH_TRANSITION_RESET;
            break;
        }
    }
}

static void sc_totemstrength_init(GameObject* obj) {
    ScTotemStrengthState* state = obj->extra;

    obj->animEventCallback = sc_totemstrength_animEventCallback;
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN | OBJECT_OBJFLAG_HITDETECT_DISABLED;
    obj->anim.rotX = SC_TOTEM_STRENGTH_TRACK_INITIAL;
    state->trackOffset = SC_TOTEM_STRENGTH_TRACK_INITIAL;
    state->transitionStep = SC_TOTEM_STRENGTH_TRANSITION_RESET;
    state->opponent = NULL;
    state->savedPosition.x = obj->anim.localPosX;
    state->savedPosition.y = obj->anim.localPosY;
    state->savedPosition.z = obj->anim.localPosZ;
}

static void sc_totemstrength_release(void) {
}

static void sc_totemstrength_initialise(void) {
}

OBJECT_INIT_ADAPTER(gSC_totemstrengthObjDescriptorInitAdapter, sc_totemstrength_init, obj)
OBJECT_HIT_DETECT_ADAPTER(gSC_totemstrengthObjDescriptorHitDetectAdapter, sc_totemstrength_hitDetect)
OBJECT_FREE_ADAPTER(gSC_totemstrengthObjDescriptorFreeAdapter, sc_totemstrength_free)
OBJECT_TYPE_ID_ADAPTER(gSC_totemstrengthObjDescriptorTypeIdAdapter, sc_totemstrength_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gSC_totemstrengthObjDescriptorExtraSizeAdapter, sc_totemstrength_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gSC_totemstrengthObjDescriptorAcquire, sc_totemstrength_initialise)

ObjectDescriptor gSC_totemstrengthObjDescriptor = {
    .header = {
        .metadata = { 0, 0, 0, OBJECT_DESCRIPTOR_FLAGS_10_SLOTS },
        .acquire = gSC_totemstrengthObjDescriptorAcquire,
        .release = sc_totemstrength_release,
    },
    .init = gSC_totemstrengthObjDescriptorInitAdapter,
    .update = sc_totemstrength_update,
    .hitDetect = gSC_totemstrengthObjDescriptorHitDetectAdapter,
    .render = sc_totemstrength_render,
    .free = gSC_totemstrengthObjDescriptorFreeAdapter,
    .getObjectTypeId = gSC_totemstrengthObjDescriptorTypeIdAdapter,
    .getExtraSize = gSC_totemstrengthObjDescriptorExtraSizeAdapter,
};;
