#include "dlls/objects/438_SC_levelcon.h"

#include "main/gamebit_latch.h"
#include "main/audio/music.h"
#include "main/audio/music_trigger_ids.h"
#include "main/audio/sfx.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/dll/dll_0000_gameui.h"
#include "main/dll/savegame_load.h"
#include "main/frame_timing.h"
#include "main/game_timer_control.h"
#include "main/gamebit_ids.h"
#include "main/gamebits.h"
#include "main/gametext_show.h"
#include "main/lightmap.h"
#include "main/map_load.h"
#include "main/mapEventTypes.h"
#include "main/object_render.h"
#include "main/objseq.h"
#include "main/pi_dolphin.h"
#include "main/rcp_dolphin.h"
#include "main/render_envfx.h"
#include "main/screen_transition.h"
#include "main/sky.h"
#include "main/sky_interface.h"
#include "sys/objects.h"

enum ScLevelControlObjectGroup {
    SC_LEVEL_CONTROL_OBJECT_GROUP_FOG_CLEAR = 1,
    SC_LEVEL_CONTROL_OBJECT_GROUP_CAPTURED = 2,
    SC_LEVEL_CONTROL_OBJECT_GROUP_FOG_DENSE = 5,
    SC_LEVEL_CONTROL_OBJECT_GROUP_CHALLENGE_GATE = 0xA,
};

enum ScLevelControlEnvironmentEffect {
    SC_LEVEL_CONTROL_ENVIRONMENT_EFFECT_4F = 0x4F,
    SC_LEVEL_CONTROL_ENVIRONMENT_EFFECT_50 = 0x50,
    SC_LEVEL_CONTROL_ENVIRONMENT_EFFECT_51 = 0x51,
    SC_LEVEL_CONTROL_ENVIRONMENT_EFFECT_245 = 0x245,
    SC_LEVEL_CONTROL_ENVIRONMENT_EFFECT_246 = 0x246,
};

enum ScLevelControlAnimEvent {
    SC_LEVEL_CONTROL_ANIM_EVENT_SET_STATE_7 = 1,
    SC_LEVEL_CONTROL_ANIM_EVENT_START_TIMED_CHALLENGE = 2,
    SC_LEVEL_CONTROL_ANIM_EVENT_OPEN_TIMER_PROMPT = 3,
};

enum ScLevelControlAnimEventFlag {
    SC_LEVEL_CONTROL_ANIM_EVENTS_PROCESSED = 1 << 0,
    SC_LEVEL_CONTROL_TIMER_PROMPT_PENDING = 1 << 1,
};

enum ScLevelControlStatusFlag {
    SC_LEVEL_CONTROL_CHALLENGE_GATE_GROUP_ENABLED = 1 << 7,
};

enum ScLevelControlReloadMode {
    SC_LEVEL_CONTROL_RELOAD_ENVIRONMENT = 1,
    SC_LEVEL_CONTROL_RELOAD_ENVIRONMENT_IMMEDIATELY = 2,
};

enum ScLevelControlTimerId {
    SC_LEVEL_CONTROL_TIMER = 0x1D,
};

enum ScLevelControlTextId {
    SC_LEVEL_CONTROL_HELP_TEXT = 0x429,
};

enum ScLevelControlTransitionId {
    SC_LEVEL_CONTROL_BLACK_TRANSITION = 0x73,
};

typedef struct ScLevelControlInterface {
    ObjectInterface base;
    void (*applyAnimEventState)(GameObject* obj, enum ScLevelControlAnimState animEventState);
    u8 (*getAnimEventState)(GameObject* obj);
} ScLevelControlInterface;

STATIC_ASSERT(offsetof(ScLevelControlInterface, applyAnimEventState) == sizeof(ObjectInterface));
STATIC_ASSERT(offsetof(ScLevelControlInterface, getAnimEventState) ==
              sizeof(ObjectInterface) + sizeof(ObjectInterfaceCallback));

typedef struct ScLevelControlState {
    f32 fogNear;
    f32 fogNearTarget;
    f32 fogNearStep;
    f32 helpTextTimer;
    f32 exitTimer;
    f32 fadeTimer;
    int musicLatches;
    u8 totemComboIndex;
    u8 animEventState;
    u8 playerMapCell;
    u8 animEventFlags;
    u8 musicTriggerId;
    s8 ambientMusicTriggerId;
    u8 statusFlags;
    u8 unused23;
} ScLevelControlState;

STATIC_ASSERT(sizeof(ScLevelControlState) == 0x24);
STATIC_ASSERT(offsetof(ScLevelControlState, fogNear) == 0x00);
STATIC_ASSERT(offsetof(ScLevelControlState, fogNearTarget) == 0x04);
STATIC_ASSERT(offsetof(ScLevelControlState, fogNearStep) == 0x08);
STATIC_ASSERT(offsetof(ScLevelControlState, helpTextTimer) == 0x0C);
STATIC_ASSERT(offsetof(ScLevelControlState, exitTimer) == 0x10);
STATIC_ASSERT(offsetof(ScLevelControlState, fadeTimer) == 0x14);
STATIC_ASSERT(offsetof(ScLevelControlState, musicLatches) == 0x18);
STATIC_ASSERT(offsetof(ScLevelControlState, totemComboIndex) == 0x1C);
STATIC_ASSERT(offsetof(ScLevelControlState, animEventState) == 0x1D);
STATIC_ASSERT(offsetof(ScLevelControlState, playerMapCell) == 0x1E);
STATIC_ASSERT(offsetof(ScLevelControlState, animEventFlags) == 0x1F);
STATIC_ASSERT(offsetof(ScLevelControlState, musicTriggerId) == 0x20);
STATIC_ASSERT(offsetof(ScLevelControlState, ambientMusicTriggerId) == 0x21);
STATIC_ASSERT(offsetof(ScLevelControlState, statusFlags) == 0x22);

static const u16 sScLevelControlTotemTreeSequence[] = {
    GAMEBIT_SC_TotemTreeHit1,
    GAMEBIT_SC_TotemTreeHit2,
    GAMEBIT_SC_TotemTreeHit3,
};

static void sc_levelcontrol_applyAnimEventState(GameObject* obj, enum ScLevelControlAnimState animEventState);

void sc_levelcontrol_setAnimEventState(GameObject* obj, enum ScLevelControlAnimState animEventState) {
    ScLevelControlInterface* interface = (ScLevelControlInterface*)*obj->anim.dll;

    interface->applyAnimEventState(obj, animEventState);
}

static void sc_levelcontrol_resetTotemPoles(void) {
    mainSetBits(GAMEBIT_SC_TotemPoleFrontLit, 0);
    mainSetBits(GAMEBIT_SC_TotemPoleLeftLit, 0);
    mainSetBits(GAMEBIT_SC_TotemPoleRightLit, 0);
    mainSetBits(GAMEBIT_SC_TotemPoleRearLit, 0);
}

static void sc_levelcontrol_loadEnvironmentEffects(int immediately) {
    int (*loadEffect)(void*, void*, u16, int) = immediately ? getEnvfxActImmediately : getEnvfxAct;
    static const u16 environmentEffects[] = {
        SC_LEVEL_CONTROL_ENVIRONMENT_EFFECT_4F,
        SC_LEVEL_CONTROL_ENVIRONMENT_EFFECT_50,
        SC_LEVEL_CONTROL_ENVIRONMENT_EFFECT_245,
    };

    for (int i = 0; i < ARRAY_COUNT(environmentEffects); i++) {
        loadEffect(NULL, NULL, environmentEffects[i], 0);
    }
    if ((*gMapEventInterface)
            ->getObjGroupStatus(SC_LEVEL_CONTROL_MAP_SWAPCIRCLE, SC_LEVEL_CONTROL_OBJECT_GROUP_FOG_DENSE) != 0) {
        loadEffect(NULL, NULL, SC_LEVEL_CONTROL_ENVIRONMENT_EFFECT_246, 0);
    } else {
        loadEffect(NULL, NULL, SC_LEVEL_CONTROL_ENVIRONMENT_EFFECT_51, 0);
    }
}

static void sc_levelcontrol_finishTimedChallenge(GameObject* obj) {
    ScLevelControlState* state = obj->extra;

    Obj_GetPlayerObject();
    if (state->animEventState != SC_LEVEL_CONTROL_ANIM_STATE_TIMED_CHALLENGE) {
        return;
    }

    mainSetBits(0x60F, 1);
    if (!isGameTimerDisabled()) {
        return;
    }

    if (mainGetBit(0x7A) != 0) {
        mainSetBits(0x85, 1);
    }
    state->exitTimer = 120.0f;
    state->animEventState = SC_LEVEL_CONTROL_ANIM_STATE_IDLE;
    Sfx_PlayFromObject(NULL, SFXTRIG_id_10a);
    Music_Trigger(MUSICTRIG_CRF_Suspense, 0);
}

static void sc_levelcontrol_updateTotemTreeSequence(ScLevelControlState* state) {
    for (int i = 0; i < ARRAY_COUNT(sScLevelControlTotemTreeSequence); i++) {
        u16 hitGameBit = sScLevelControlTotemTreeSequence[i];

        if (mainGetBit(hitGameBit) == 0) {
            continue;
        }

        mainSetBits(hitGameBit, 0);
        if (sScLevelControlTotemTreeSequence[state->totemComboIndex] == hitGameBit) {
            state->totemComboIndex++;
        } else {
            state->totemComboIndex = 0;
        }
        break;
    }

    if (state->totemComboIndex >= ARRAY_COUNT(sScLevelControlTotemTreeSequence)) {
        mainSetBits(GAMEBIT_SC_TotemTreeSequenceComplete, 1);
        state->totemComboIndex = 0;
    }
}

static int sc_levelcontrol_processAnimEventsCallback(GameObject* obj, int unused, ObjSeqState* animUpdate) {
    ScLevelControlState* state = obj->extra;

    (void)unused;

    animUpdate->movementState = 0;
    for (int i = 0; i < animUpdate->eventCount; i++) {
        switch (animUpdate->eventIds[i]) {
        case SC_LEVEL_CONTROL_ANIM_EVENT_SET_STATE_7:
            sc_levelcontrol_applyAnimEventState(obj, SC_LEVEL_CONTROL_ANIM_STATE_EVENT_1);
            break;
        case SC_LEVEL_CONTROL_ANIM_EVENT_START_TIMED_CHALLENGE:
            sc_levelcontrol_applyAnimEventState(obj, SC_LEVEL_CONTROL_ANIM_STATE_TIMED_CHALLENGE);
            break;
        case SC_LEVEL_CONTROL_ANIM_EVENT_OPEN_TIMER_PROMPT:
            state->animEventFlags |= SC_LEVEL_CONTROL_TIMER_PROMPT_PENDING;
            break;
        }
    }
    state->animEventFlags |= SC_LEVEL_CONTROL_ANIM_EVENTS_PROCESSED;
    mainSetBits(0x60F, 0);
    sc_levelcontrol_finishTimedChallenge(obj);
    return 0;
}

static u8 sc_levelcontrol_getAnimEventState(GameObject* obj) {
    return ((ScLevelControlState*)obj->extra)->animEventState;
}

static void sc_levelcontrol_applyAnimEventState(GameObject* obj, enum ScLevelControlAnimState animEventState) {
    ScLevelControlState* state = obj->extra;

    state->animEventState = animEventState;
    switch (state->animEventState) {
    case SC_LEVEL_CONTROL_ANIM_STATE_CLEAR:
        state->animEventState = SC_LEVEL_CONTROL_ANIM_STATE_IDLE;
        break;
    case SC_LEVEL_CONTROL_ANIM_STATE_TIMED_CHALLENGE:
        mainSetBits(0x2B8, 1);
        mainSetBits(0x4BD, 0);
        mainSetBits(0x85, 0);
        gameTimerInit(SC_LEVEL_CONTROL_TIMER, 0x96);
        Music_Trigger(MUSICTRIG_CRF_Suspense, 1);
        timerSetToCountUp();
        break;
    case SC_LEVEL_CONTROL_ANIM_STATE_START_TREX_CHALLENGE:
        gameTimerInit(SC_LEVEL_CONTROL_TIMER, 0x3C);
        state->animEventState = SC_LEVEL_CONTROL_ANIM_STATE_IDLE;
        Music_Trigger(MUSICTRIG_trex_chase, 1);
        timerSetToCountUp();
        break;
    case SC_LEVEL_CONTROL_ANIM_STATE_FINISH_TIMED_CHALLENGE:
        Music_Trigger(MUSICTRIG_CRF_Suspense, 0);
        state->animEventState = SC_LEVEL_CONTROL_ANIM_STATE_IDLE;
        state->fadeTimer = 120.0f;
        gameTimerStop();
        break;
    case SC_LEVEL_CONTROL_ANIM_STATE_STOP_TREX_CHALLENGE:
        state->animEventState = SC_LEVEL_CONTROL_ANIM_STATE_IDLE;
        Music_Trigger(MUSICTRIG_trex_chase, 0);
        gameTimerStop();
        break;
    }
}

static int sc_levelcontrol_getExtraSize(void) {
    return sizeof(ScLevelControlState);
}

static int sc_levelcontrol_getObjectTypeId(void) {
    return 0;
}

static void sc_levelcontrol_free(GameObject* obj) {
    (void)obj;

    gameTimerStop();
    disableHeavyFog();
    Music_Trigger(MUSICTRIG_PU3_Adventure_c4, 0);
    Music_Trigger(MUSICTRIG_Teleport, 0);
    Music_Trigger(MUSICTRIG_CRF_Suspense, 0);
    Music_Trigger(MUSICTRIG_fox_arwing, 0);
    Music_Trigger(MUSICTRIG_trex_chase, 0);
}

static void sc_levelcontrol_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                                   s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

static void sc_levelcontrol_hitDetect(void) {
}

static void sc_levelcontrol_update(GameObject* obj) {
    ScLevelControlState* state = obj->extra;
    GameObject* player = Obj_GetPlayerObject();

    if (obj->userData1 != 0) {
        skySetSlotFlag80(7, 0);
        skySetEnvFxFlags(0);
        sc_levelcontrol_loadEnvironmentEffects(obj->userData1 == SC_LEVEL_CONTROL_RELOAD_ENVIRONMENT_IMMEDIATELY);
        obj->userData1 = 0;
    }
    if ((state->statusFlags & SC_LEVEL_CONTROL_CHALLENGE_GATE_GROUP_ENABLED) == 0 &&
        mainGetBit(GAMEBIT_LV_ChallengeGate2Complete) != 0) {
        (*gMapEventInterface)
            ->setObjGroupStatus(SC_LEVEL_CONTROL_MAP_SWAPCIRCLE, SC_LEVEL_CONTROL_OBJECT_GROUP_CHALLENGE_GATE, 1);
        state->statusFlags |= SC_LEVEL_CONTROL_CHALLENGE_GATE_GROUP_ENABLED;
    }
    if (state->playerMapCell != SC_LEVEL_CONTROL_MAP_SWAPCIRCLE) {
        if (coordsToMapCell(player->anim.localPosX, player->anim.localPosZ) == SC_LEVEL_CONTROL_MAP_SWAPCIRCLE) {
            u8 mapAct = (*gMapEventInterface)->getMapAct(SC_LEVEL_CONTROL_MAP_SWAPCIRCLE);
            Obj_GetPlayerObject();
            switch (mapAct) {
            case SC_LEVEL_CONTROL_MAP_ACT_SPELLSTONE_INSERTED:
                if (mainGetBit(GAMEBIT_ITEM_SpellStone2_Used) != 0) {
                    (*gMapEventInterface)
                        ->setMapAct(SC_LEVEL_CONTROL_MAP_SWAPCIRCLE, SC_LEVEL_CONTROL_MAP_ACT_SPELLSTONE_USED);
                }
                break;
            case SC_LEVEL_CONTROL_MAP_ACT_SPELLSTONE_USED:
            case 3:
            case 4:
            case 5:
                if (mainGetBit(GAMEBIT_LV_EscapedFromPole) != 0) {
                    (*gMapEventInterface)
                        ->setMapAct(SC_LEVEL_CONTROL_MAP_SWAPCIRCLE, SC_LEVEL_CONTROL_MAP_ACT_ESCAPE_COMPLETE);
                }
                break;
            }
        } else {
            return;
        }
    }
    if (state->fadeTimer && (player->objectFlags & OBJECT_OBJFLAG_PARENT_SLACK) == 0) {
        if (state->fadeTimer == 120.0f) {
            (*gScreenTransitionInterface)->start(SC_LEVEL_CONTROL_BLACK_TRANSITION, SCREEN_TRANSITION_BLACK);
        }
        state->fadeTimer -= timeDelta;
        if (state->fadeTimer <= 0.0f) {
            state->fadeTimer = 0.0f;
            state->exitTimer = 0.0f;
            mainSetBits(0x2B8, 0);
            mainSetBits(0x4BD, 1);
            sc_levelcontrol_resetTotemPoles();
            mainSetBits(0x63E, 1);
            mainSetBits(0x7CF, 1);
        }
    } else if (state->exitTimer && (player->objectFlags & OBJECT_OBJFLAG_PARENT_SLACK) == 0) {
        if (state->exitTimer == 120.0f) {
            (*gScreenTransitionInterface)->start(SC_LEVEL_CONTROL_BLACK_TRANSITION, SCREEN_TRANSITION_BLACK);
        }
        state->exitTimer -= timeDelta;
        if (state->exitTimer <= 0.0f) {
            mainSetBits(0x640, 1);
            state->exitTimer = 0.0f;
            mainSetBits(0x2B8, 0);
            mainSetBits(0x4BD, 1);
            sc_levelcontrol_resetTotemPoles();
        }
    }
    state->playerMapCell = coordsToMapCell(player->anim.localPosX, player->anim.localPosZ);
    if (mainGetBit(0xCDC) != 0) {
        if (state->helpTextTimer > 0.0f) {
            gameTextShow(SC_LEVEL_CONTROL_HELP_TEXT);
            state->helpTextTimer -= timeDelta;
            if (state->helpTextTimer < 0.0f) {
                state->helpTextTimer = 0.0f;
            }
        }
        if ((*gMapEventInterface)
                ->getObjGroupStatus(SC_LEVEL_CONTROL_MAP_SWAPCIRCLE, SC_LEVEL_CONTROL_OBJECT_GROUP_FOG_CLEAR) != 0) {
            state->fogNearTarget = -1000.0f;
            state->fogNearStep = 0.35f;
        } else if ((*gMapEventInterface)
                       ->getObjGroupStatus(SC_LEVEL_CONTROL_MAP_SWAPCIRCLE, SC_LEVEL_CONTROL_OBJECT_GROUP_FOG_DENSE) !=
                   0) {
            state->fogNearTarget = -1200.0f;
            state->fogNearStep = -0.35f;
            if (obj->userData2 != 0) {
                skySetLightIndex(1, 1.0f);
                obj->userData2 = 0;
            }
        } else {
            state->fogNearTarget = -1000.0f;
            state->fogNearStep = 0.35f;
        }
    } else {
        state->fogNearTarget = -1080.0f;
        state->fogNearStep = -0.35f;
    }
    if (state->fogNearTarget != state->fogNear) {
        state->fogNear = state->fogNearStep * timeDelta + state->fogNear;
        if (state->fogNearStep < 0.0f) {
            if (state->fogNear < state->fogNearTarget) {
                state->fogNear = state->fogNearTarget;
            }
        } else if (state->fogNear > state->fogNearTarget) {
            state->fogNear = state->fogNearTarget;
        }
        enableHeavyFog(50.0f + state->fogNear, state->fogNear, 1000.0f, 0.1f, 0.0005f, 0);
    }
    sc_levelcontrol_updateTotemTreeSequence(state);
    if ((state->animEventFlags & SC_LEVEL_CONTROL_ANIM_EVENTS_PROCESSED) != 0) {
        state->animEventFlags &= ~SC_LEVEL_CONTROL_ANIM_EVENTS_PROCESSED;
        mainSetBits(0x60F, 1);
        if (mainGetBit(0x7A) == 0) {
            if (mainGetBit(0x627) != 0 && mainGetBit(0x63E) != 0) {
                mainSetBits(GAMEBIT_LV_DoneTests, 1);
            }
        } else if (mainGetBit(GAMEBIT_LV_DoneTests) != 0) {
            mainSetBits(0x85, 1);
        }
    }
    if (state->animEventState == SC_LEVEL_CONTROL_ANIM_STATE_IDLE) {
        if (mainGetBit(0x60E) != 0) {
            mainSetBits(0x60E, 0);
            timeListPromptOpen();
        }
    } else if (state->animEventState == SC_LEVEL_CONTROL_ANIM_STATE_TIMED_CHALLENGE) {
        if (mainGetBit(0x60E) != 0) {
            mainSetBits(0x60E, 0);
            gameTimerStop();
            if (mainGetBit(0x7A) != 0) {
                mainSetBits(0x85, 1);
            }
            state->exitTimer = 120.0f;
            (*gScreenTransitionInterface)->start(SC_LEVEL_CONTROL_BLACK_TRANSITION, SCREEN_TRANSITION_BLACK);
            state->animEventState = SC_LEVEL_CONTROL_ANIM_STATE_IDLE;
            Sfx_PlayFromObject(NULL, SFXTRIG_id_10a);
        }
    }
    if (mainGetBit(GAMEBIT_ITEM_LVBlock2_Used) != 0) {
        mainSetBits(0x612, 1);
        mainSetBits(0x90b, 1);
        mainSetBits(0x87, 1);
    }
    if (mainGetBit(GAMEBIT_ITEM_LVBlock3_Used) != 0) {
        mainSetBits(0x2C6, 1);
        mainSetBits(0x2CE, 1);
        mainSetBits(0xBDC, 1);
    }
    if (mainGetBit(GAMEBIT_ITEM_LVBlock1_Used) != 0) {
        mainSetBits(0xBDF, 1);
        mainSetBits(0xBE1, 1);
        mainSetBits(0xBE3, 1);
    }
    sc_levelcontrol_finishTimedChallenge(obj);
    if (mainGetBit(0x4D0) == 0) {
        if (mainGetBit(GAMEBIT_LV_CapturedByLightFoot) != 0) {
            mainSetBits(0x4D0, 1);
            (*gMapEventInterface)
                ->setObjGroupStatus(SC_LEVEL_CONTROL_MAP_SWAPCIRCLE, SC_LEVEL_CONTROL_OBJECT_GROUP_CAPTURED, 1);
            warpToMap(0x50, 0);
            (*gMapEventInterface)
                ->setObjGroupStatus(SC_LEVEL_CONTROL_MAP_SWAPCIRCLE, SC_LEVEL_CONTROL_OBJECT_GROUP_FOG_CLEAR, 0);
        }
    }
    if ((*gSkyInterface)->getSunPosition(0) != 0) {
        if (state->musicTriggerId != MUSICTRIG_PU1_Mysterious) {
            state->musicTriggerId = MUSICTRIG_PU1_Mysterious;
            Music_Trigger(MUSICTRIG_PU1_Mysterious, 1);
        }
        if (state->ambientMusicTriggerId != -1) {
            state->ambientMusicTriggerId = -1;
            Music_Trigger(MUSICTRIG_fox_arwing, 0);
        }
    } else {
        if (state->musicTriggerId != MUSICTRIG_KP_Text) {
            state->musicTriggerId = MUSICTRIG_KP_Text;
            Music_Trigger(MUSICTRIG_KP_Text, 1);
        }
        if (state->ambientMusicTriggerId != MUSICTRIG_fox_arwing) {
            state->ambientMusicTriggerId = MUSICTRIG_fox_arwing;
            Music_Trigger(MUSICTRIG_fox_arwing, 1);
        }
    }
    GameBitLatch_Update(&state->musicLatches, 1, -1, -1, 0xE1E, MUSICTRIG_Teleport);
    GameBitLatch_Update(&state->musicLatches, 2, -1, -1, GAMEBIT_SHRINE_MUSIC_LOCK, MUSICTRIG_PU3_Adventure_c4);
    if ((state->animEventFlags & SC_LEVEL_CONTROL_TIMER_PROMPT_PENDING) != 0) {
        mainSetBits(0x60E, 1);
        state->animEventFlags &= ~SC_LEVEL_CONTROL_TIMER_PROMPT_PENDING;
    }
}

static void sc_levelcontrol_init(GameObject* obj) {
    ScLevelControlState* state = obj->extra;

    state->statusFlags &= ~SC_LEVEL_CONTROL_CHALLENGE_GATE_GROUP_ENABLED;
    state->playerMapCell = 0xFF;
    state->animEventState = SC_LEVEL_CONTROL_ANIM_STATE_IDLE;
    obj->animEventCallback = sc_levelcontrol_processAnimEventsCallback;
    mainSetBits(0x60F, 1);
    mainSetBits(0x2B8, 0);
    mainSetBits(0x4BD, 1);
    sc_levelcontrol_resetTotemPoles();
    state->helpTextTimer = 300.0f;
    state->fogNear = -1200.0f;
    state->fogNearTarget = -1200.0f;
    state->fogNearStep = -0.35f;
    enableHeavyFog(50.0f + state->fogNear, state->fogNear, 1000.0f, 0.1f, 0.0005f, 0);
    if (mainGetBit(0x7A) != 0) {
        mainSetBits(0x85, 1);
    }
    unlockLevel(mapGetDirIdx(SC_LEVEL_CONTROL_MAP_SWAPCIRCLE), 0, 0);
    if (getSaveGameLoadStatus() != 0) {
        obj->userData1 = SC_LEVEL_CONTROL_RELOAD_ENVIRONMENT_IMMEDIATELY;
    } else {
        obj->userData1 = SC_LEVEL_CONTROL_RELOAD_ENVIRONMENT;
    }
    obj->userData2 = 1;
}

static void sc_levelcontrol_release(void) {
}

static void sc_levelcontrol_initialise(void) {
}

OBJECT_INIT_ADAPTER(gSC_levelcontrolObjDescriptorInitAdapter, sc_levelcontrol_init, obj)
OBJECT_HIT_DETECT_ADAPTER(gSC_levelcontrolObjDescriptorHitDetectAdapter, sc_levelcontrol_hitDetect)
OBJECT_FREE_ADAPTER(gSC_levelcontrolObjDescriptorFreeAdapter, sc_levelcontrol_free, obj)
OBJECT_TYPE_ID_ADAPTER(gSC_levelcontrolObjDescriptorTypeIdAdapter, sc_levelcontrol_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gSC_levelcontrolObjDescriptorExtraSizeAdapter, sc_levelcontrol_getExtraSize)

typedef struct SC_levelcontrolObjDescriptorTypeInterface {
    OBJECT_INTERFACE_FIELDS;
    __typeof__(sc_levelcontrol_applyAnimEventState)* sc_levelcontrol_applyAnimEventState;
    __typeof__(sc_levelcontrol_getAnimEventState)* sc_levelcontrol_getAnimEventState;
} SC_levelcontrolObjDescriptorTypeInterface;

struct SC_levelcontrolObjDescriptorType {
    ObjectDescriptorHeader header;
    SC_levelcontrolObjDescriptorTypeInterface interface;
};

RESOURCE_ACQUIRE_ADAPTER(gSC_levelcontrolObjDescriptorAcquire, sc_levelcontrol_initialise)

struct SC_levelcontrolObjDescriptorType gSC_levelcontrolObjDescriptor = {
    {
        .metadata[3] = OBJECT_DESCRIPTOR_FLAGS_12_SLOTS,
        .acquire = gSC_levelcontrolObjDescriptorAcquire,
        .release = sc_levelcontrol_release,
    },
    {
        .init = gSC_levelcontrolObjDescriptorInitAdapter,
        .update = sc_levelcontrol_update,
        .hitDetect = gSC_levelcontrolObjDescriptorHitDetectAdapter,
        .render = sc_levelcontrol_render,
        .free = gSC_levelcontrolObjDescriptorFreeAdapter,
        .getObjectTypeId = gSC_levelcontrolObjDescriptorTypeIdAdapter,
        .getExtraSize = gSC_levelcontrolObjDescriptorExtraSizeAdapter,
        .sc_levelcontrol_applyAnimEventState = sc_levelcontrol_applyAnimEventState,
        .sc_levelcontrol_getAnimEventState = sc_levelcontrol_getAnimEventState,
    },
};
