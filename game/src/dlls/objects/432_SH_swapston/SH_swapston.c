#include "dlls/objects/432_SH_swapston.h"

#include "dolphin/pad.h"
#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/dll/partfx_interface.h"
#include "main/frame_timing.h"
#include "main/gamebit_ids.h"
#include "main/gamebits.h"
#include "main/maketex_random.h"
#include "main/maketex_sequence.h"
#include "main/mapEvent.h"
#include "main/map_load.h"
#include "main/model.h"
#include "main/model_engine.h"
#include "main/model_engine_ui.h"
#include "main/object_render.h"
#include "main/objtype.h"
#include "main/obj_link.h"
#include "main/obj_path.h"
#include "main/obj_query.h"
#include "main/objseq.h"
#include "main/objfx.h"
#include "main/objhits.h"
#include "main/objprint_anim.h"
#include "main/objprint.h"
#include "main/objprint_character.h"
#include "main/pad.h"
#include "main/pi_dolphin.h"
#include "main/rcp_dolphin.h"
#include "main/shader.h"
#include "main/textrender.h"
#include "main/vecmath.h"
#include "sys/objects.h"
#include "main/mapEventTypes.h"
#include "sys/objects/lifecycle.h"
#include "main/dll/player.h"
#include "main/dll/player_spirit.h"
#include "main/dll/dll_0000_gameui.h"
#include "main/audio/sfx.h"
#include "main/audio/stream.h"
#include "main/audio/audio_control.h"

enum WarpStoneDustFlag {
    WARPSTONE_DUST_BURST_READY = 1 << 1,
    WARPSTONE_DUST_ACTIVE = 1 << 2,
};

enum WarpStoneBehaviorFlag {
    WARPSTONE_SFX_FIRED = 1 << 4,
    WARPSTONE_LOOK_AT_PLAYER = 1 << 6,
};

enum WarpStoneSequenceFlag {
    WARPSTONE_SEQUENCE_PROBE_SUCCEEDED = 1 << 0,
    WARPSTONE_SEQUENCE_HAS_SPELL_STONE = 1 << 1,
};

enum WarpStoneMove {
    WARPSTONE_MOVE_IDLE = 0,
    WARPSTONE_MOVE_TURN_RIGHT = 0x16,
    WARPSTONE_MOVE_TURN_FAR_RIGHT = 0x17,
    WARPSTONE_MOVE_TURN_LEFT = 0x18,
    WARPSTONE_MOVE_TURN_FAR_LEFT = 0x19,
    WARPSTONE_MOVE_YAWN = 0x1A,
    WARPSTONE_MOVE_MUMBLE = 0x1B,
};

enum WarpStoneAnimEvent {
    WARPSTONE_EVENT_LEFT_SPARK_A = 1,
    WARPSTONE_EVENT_RIGHT_SPARK_A,
    WARPSTONE_EVENT_LEFT_SPARK_B,
    WARPSTONE_EVENT_RIGHT_SPARK_B,
    WARPSTONE_EVENT_LANTERN_SWING = 9,
};

enum WarpStoneEffectId {
    WARPSTONE_DUST_PUFF_EFFECT = 0x7CA,
    WARPSTONE_DUST_CLOUD_EFFECT = 0x7D2,
};

enum WarpStoneObjectGroup {
    WARPSTONE_TARGET_OBJECT_GROUP = 8,
};

typedef struct WarpStonePlacement {
    ObjPlacement base;
    u8 unused18[2];
    u8 initialYaw;
} WarpStonePlacement;

typedef struct WarpStoneDustEffectParams {
    s16 flags;
    s16 count;
    s16 effectType;
    s16 radius;
    f32 scale;
    Vec position;
} WarpStoneDustEffectParams;

typedef struct WarpStoneState {
    GameObject* child;
    f32 dustEffectTimer;
    u8 pathPointIndex;
    u8 sequenceToggle;
    u8 sequenceFlags;
    u8 unused0F;
    u8 activated;
    u8 unused11;
    s16 sequenceGameBit;
    s16 resetGameBit;
    u8 unused16[2];
    ObjSoundState soundState;
    CharacterEyeAnimState eyeAnimState;
    u8 unused70[8];
    ObjJointTrackPair headAimState;
    u8 dustEffectFlags;
    u8 behaviorFlags;
    u8 unusedDA[2];
} WarpStoneState;

STATIC_ASSERT(offsetof(WarpStonePlacement, initialYaw) == sizeof(ObjPlacement) + 2);
STATIC_ASSERT(offsetof(WarpStoneState, dustEffectTimer) == sizeof(GameObject*));
STATIC_ASSERT(offsetof(WarpStoneState, soundState) == offsetof(WarpStoneState, resetGameBit) + sizeof(s16) + 2);
STATIC_ASSERT(offsetof(WarpStoneState, eyeAnimState) == offsetof(WarpStoneState, soundState) + sizeof(ObjSoundState));
STATIC_ASSERT(offsetof(WarpStoneState, headAimState) ==
              offsetof(WarpStoneState, eyeAnimState) + sizeof(CharacterEyeAnimState) + 8);
STATIC_ASSERT(offsetof(WarpStoneState, dustEffectFlags) ==
              offsetof(WarpStoneState, headAimState) + sizeof(ObjJointTrackPair));
STATIC_ASSERT(offsetof(WarpStoneState, behaviorFlags) == offsetof(WarpStoneState, dustEffectFlags) + 1);
STATIC_ASSERT(sizeof(WarpStoneState) == 0xE0);

static ObjAnimEventList sWarpStoneAnimEvents;
static int sWarpStoneMenuState;
static int sWarpStoneLookToggleChance = 300;
static int sWarpStoneHeadAimMode = 1;
static int sWarpStoneHeadAimHeightOffset = 200;
static s16 sWarpStoneHeadYawOffset = 0x800;
static int sWarpStoneMumbleChance = 3;
static int sWarpStoneYawnChance = 4;
static int sWarpStoneMenuUpEnabled = 1;
static s16 sWarpStoneHeadPitchOffset;
static s16 sWarpStoneYawBias;

static void warpstone_updateDustEffects(GameObject* obj) {
    GameObject* player = Obj_GetPlayerObject();
    WarpStoneState* state = obj->extra;

    WarpStoneDustEffectParams effectParams;
    effectParams.position.x = 0.0f;
    effectParams.position.y = 55.0f;
    effectParams.position.z = 0.0f;
    effectParams.effectType = 0xC0E;
    effectParams.count = 1;

    if ((state->dustEffectFlags & WARPSTONE_DUST_ACTIVE) == 0) {
        return;
    }

    if (state->dustEffectTimer < 120.0f) {
        if ((f32)randomGetRange(0, 0x1E0) < state->dustEffectTimer / 2.0f) {
            (*gPartfxInterface)->spawnObject(player, WARPSTONE_DUST_PUFF_EFFECT, &effectParams, 2, -1, NULL);
        }

        state->dustEffectTimer += timeDelta;
        return;
    }

    if (state->dustEffectTimer < 360.0f) {
        if ((f32)randomGetRange(0, 0x1E0) < state->dustEffectTimer / 3.0f) {
            (*gPartfxInterface)->spawnObject(player, WARPSTONE_DUST_PUFF_EFFECT, &effectParams, 2, -1, NULL);
        }

        effectParams.radius = 0x28;
        effectParams.flags = 0;
        effectParams.scale = 0.0009f * ((state->dustEffectTimer - 120.0f) / 240.0f);
        (*gPartfxInterface)->spawnObject(player, WARPSTONE_DUST_CLOUD_EFFECT, &effectParams, 2, -1, NULL);
        state->dustEffectFlags |= WARPSTONE_DUST_BURST_READY;
        state->dustEffectTimer += timeDelta;
        return;
    }

    if (state->dustEffectTimer < 420.0f) {
        if ((f32)randomGetRange(0, 0x1E0) < state->dustEffectTimer / 2.0f) {
            (*gPartfxInterface)->spawnObject(player, WARPSTONE_DUST_PUFF_EFFECT, &effectParams, 2, -1, NULL);
        }

        if ((state->dustEffectFlags & WARPSTONE_DUST_BURST_READY) != 0) {
            state->dustEffectFlags &= ~WARPSTONE_DUST_BURST_READY;
            effectParams.radius = 0x46;
            effectParams.scale = 0.00036f;
            for (int i = 0; i < 15; i++) {
                (*gPartfxInterface)->spawnObject(player, WARPSTONE_DUST_CLOUD_EFFECT, &effectParams, 2, -1, NULL);
            }
        }

        state->dustEffectTimer += timeDelta;
        return;
    }

    if (!(state->dustEffectTimer < 480.0f)) {
        state->dustEffectTimer = 0.0f;
        state->dustEffectFlags &= ~WARPSTONE_DUST_ACTIVE;
        state->dustEffectTimer += timeDelta;
        return;
    }

    state->dustEffectTimer += timeDelta;
}

static u32 warpstone_advanceAnimEvents(GameObject* lantern, f32 moveStepScale) {
    int pointIndex = 0;
    sWarpStoneAnimEvents.triggerCount = 0;
    sWarpStoneAnimEvents.rootCurveValid = 0;
    u32 advanceResult = ObjAnim_AdvanceCurrentMove(lantern, moveStepScale, timeDelta, &sWarpStoneAnimEvents);
    if (sWarpStoneAnimEvents.rootCurveValid != 0) {
        lantern->anim.rotX += sWarpStoneAnimEvents.rootPitch;
    }

    for (int i = 0; i < sWarpStoneAnimEvents.triggerCount; i++) {
        switch (sWarpStoneAnimEvents.triggeredIds[i]) {
        case WARPSTONE_EVENT_LEFT_SPARK_A:
            pointIndex = 1;
            break;
        case WARPSTONE_EVENT_RIGHT_SPARK_A:
            pointIndex = 2;
            break;
        case WARPSTONE_EVENT_LEFT_SPARK_B:
            pointIndex = 1;
            break;
        case WARPSTONE_EVENT_RIGHT_SPARK_B:
            pointIndex = 2;
            break;
        case WARPSTONE_EVENT_LANTERN_SWING:
            Sfx_PlayFromObject(lantern, SFXTRIG_swapstone_move_short);
            break;
        case 0:
        case 5:
        case 6:
        case 7:
        case 8:
        default:
            break;
        }
    }

    if (pointIndex != 0) {
        f32 posX, posY, posZ;
        ObjPath_GetPointWorldPosition(lantern, pointIndex - 1, &posX, &posY, &posZ, 0);
        if (!(lantern->anim.currentMove == WARPSTONE_MOVE_MUMBLE && lantern->anim.currentMoveProgress < 0.8f)) {
            Sfx_PlayAtPositionFromObject(lantern, posX, posY, posZ, 0x415);
        }
    }

    return advanceResult;
}

static u32 warpstoneProbePlayerAnimState(void) {
    (*gMapEventInterface)->getCurChar();
    GameObject* playerObj = Obj_GetPlayerObject();
    objGetAnimStateFlags(playerObj, 0xff);
    return 2;
}

static int warpstone_testEvent(void* context, u8* object, int option) {
    s8 horizontal;
    s8 vertical;

    (void)context;
    (void)object;
    Obj_GetPlayerObject();
    padGetAnalogInput(0, &horizontal, &vertical);

    switch (option) {
    case 0x14:
        if (horizontal < 0) {
            loadMapAndParent(0x42);
            unlockLevel(0, 0, 1);
            lockLevel(mapGetDirIdx(0x42), 0);
            lockLevel(mapGetDirIdx(7), 1);
            (*gMapEventInterface)->setMapAct(0x42, 1);
            Sfx_PlayFromObject(0, SFXTRIG_menu_pause_up);
            return 1;
        }
        break;

    case 0x15:
        if (vertical > 0 && sWarpStoneMenuUpEnabled == 0) {
            Sfx_PlayFromObject(0, SFXTRIG_menu_pause_up);
            return 1;
        }
        break;

    case 0x16:
        if (horizontal > 0 && playerHasKrazoaSpirit(1, 0) != 0) {
            loadMapAndParent(0x42);
            lockLevel(mapGetDirIdx(0x42), 0);
            lockLevel(mapGetDirIdx(7), 1);
            if (mainGetBit(GAMEBIT_ITEM_TestCombatSpirit_Got) != 0) {
                (*gMapEventInterface)->setMapAct(0x42, 2);
            } else if (mainGetBit(GAMEBIT_ITEM_SpiritTestFear_Got) != 0) {
                (*gMapEventInterface)->setMapAct(0x42, 2);
            } else if (mainGetBit(GAMEBIT_ITEM_SpiritTestStrength_Got) != 0) {
                (*gMapEventInterface)->setMapAct(0x42, 2);
            } else if (mainGetBit(GAMEBIT_ITEM_Spirit5_Got) != 0) {
                (*gMapEventInterface)->setMapAct(0x42, 2);
            }
            Sfx_PlayFromObject(0, SFXTRIG_menu_pause_up);
            return 1;
        }
        break;

    case 0x17: {
        int hasSpirit = playerHasKrazoaSpirit(1, 0);
        if (horizontal > 0 && hasSpirit == 0) {
            Sfx_PlayFromObject(0, SFXTRIG_menu_pause_up);
            return 1;
        }
        break;
    }

    case 0x18:
        sWarpStoneMenuState = 1;
        if (vertical > 0) {
            loadMapAndParent(9);
            lockLevel(mapGetDirIdx(9), 0);
            lockLevel(mapGetDirIdx(7), 1);
            Sfx_PlayFromObject(0, SFXTRIG_menu_pause_up);
            return 1;
        }
        break;

    case 0x19:
        if ((getButtonsJustPressed(0) & PAD_BUTTON_B) != 0) {
            unlockLevel(0, 0, 1);
            mapUnload(mapGetDirIdx(0x42), 0x20000000);
            mapUnload(mapGetDirIdx(0x17), 0x20000000);
            Sfx_PlayFromObject(0, SFXTRIG_menu_pause_down);
            return 1;
        }
        break;
    }

    return 0;
}

static void warpstone_loadBaseUi(void* context, u8* object) {
    (void)context;
    (void)object;
    loadUiDll(1);
}

static int warpstone_SeqFn(GameObject* obj, u32 unused, ObjSeqState* animObj) {
    WarpStoneState* state = obj->extra;
    ObjSeqState* animUpdate = animObj;

    (void)unused;

    if (animatedObjGetSeqId(animUpdate) == 0x35f) {
        ObjSeq_SetSlotValue(animUpdate, 0x2648);
        if (getCurUiDll() != 0x10) {
            loadUiDll(0x10);
        }
    }

    GameObject* child = state->child;
    if (child != NULL) {
        ObjAnim_AdvanceCurrentMove(child, obj->anim.currentMoveProgress - child->anim.currentMoveProgress, timeDelta,
                                   NULL);
    }

    animUpdate->conditionCallback = warpstone_testEvent;
    animUpdate->freeCallback = warpstone_loadBaseUi;

    if (animUpdate->movementState != 0) {
        state->sequenceFlags &= ~(WARPSTONE_SEQUENCE_PROBE_SUCCEEDED | WARPSTONE_SEQUENCE_HAS_SPELL_STONE);
        if ((s32)warpstoneProbePlayerAnimState() != 0) {
            state->sequenceFlags |= WARPSTONE_SEQUENCE_PROBE_SUCCEEDED;
        }

        int hit;
        if (mainGetBit(GAMEBIT_ITEM_WaterSpellStone1_Got) != 0) {
            hit = 1;
        } else if (mainGetBit(GAMEBIT_ITEM_FireSpellStone1_Got) != 0) {
            hit = 1;
        } else {
            hit = 0;
        }
        if (hit) {
            state->sequenceFlags |= WARPSTONE_SEQUENCE_HAS_SPELL_STONE;
        }
        animUpdate->movementState = 0;

        if (mainGetBit(state->sequenceGameBit) != 0 && animatedObjGetSeqId(animObj) == 0x35f) {
            AudioStream_CancelPrepared();
            seqClearTaskTexts();
            AudioStream_Nop(0);
            animUpdate->sequenceControlFlags |= OBJSEQ_CONTROL_SET_LATCH_A;
        }
    }

    for (int i = 0; i < animUpdate->eventCount; i++) {
        switch (animUpdate->eventIds[i]) {
        case 0x17:
            state->dustEffectFlags |= WARPSTONE_DUST_ACTIVE;
            Sfx_PlayFromObject(0, SFXTRIG_id_420);
            break;

        case 3:
            state->pathPointIndex = 0;
            break;

        case 4:
            state->pathPointIndex = 1;
            break;

        case 6:
            CMenu_SetFadeCounter(0);
            loadUiDll(1);
            warpToMap(0x7e, 1);
            break;

        case 7:
            CMenu_SetFadeCounter(0);
            loadUiDll(1);
            mainSetBits(GAMEBIT_SH_WarpStoneRelated0884, 1);
            warpToMap(0x7e, 1);
            break;

        case 0xa:
            state->sequenceToggle = state->sequenceToggle ^ 1;
            break;

        case 9:
            (*gMapEventInterface)->setMapAct(0x17, 1);
            (*gMapEventInterface)->setMapAct(0xe, 2);
            CMenu_SetFadeCounter(0);
            loadUiDll(1);
            break;

        case 0xc:
            CMenu_SetFadeCounter(0);
            loadUiDll(1);
            warpToMap(0x33, 0);
            break;

        case 0xd:
            subtitleStop();
            __attribute__((fallthrough));
        case 0xe:
        case 0xf:
        case 0x10:
        case 0x11:
            if (getCurUiDll() == 0x10) {
                UiDllVTable** uiDll = getCurUiDllInterface();
                (*uiDll)->setState(animUpdate->eventIds[i] - 0xd);
            }
            mainSetBits(state->sequenceGameBit, 1);
            mainSetBits(GAMEBIT_SH_SawWarpStoneIntro, 1);
            break;

        case 0x12:
            (*gMapEventInterface)->setObjGroupStatus(7, 0xa, 0);
            break;

        case 0x14:
            unlockLevel(0, 0, 1);
            break;

        case 0x15:
            unlockLevel(0, 0, 1);
            mapUnload(mapGetDirIdx(0x42), 0x20000000);
            break;

        case 0x16:
            unlockLevel(0, 0, 1);
            mapUnload(mapGetDirIdx(0x42), 0x20000000);
            break;
        }
    }

    warpstone_updateDustEffects(obj);
    return 0;
}

static int warpstone_getExtraSize(void) {
    return sizeof(WarpStoneState);
}

static int warpstone_getObjectTypeId(void) {
    return 0x48;
}

static void warpstone_free(GameObject* obj, int mode) {
    WarpStoneState* state = obj->extra;

    if (state->child != NULL && mode == 0) {
        ObjLink_DetachChild(obj, state->child);
        Obj_FreeObject(state->child);
    }
}

static void warpstone_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                             s8 visible) {
    if (visible == 0) {
        return;
    }

    WarpStoneState* state = obj->extra;
    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    GameObject* player = Obj_GetPlayerObject();
    if (player != NULL && playerIsSequenceRenderSuppressed(player) != 0) {
        ObjModel* model = Obj_GetActiveModel(player);
        model->bufferFlags &= ~0x8;

        f32 x, y, z;
        ObjPath_GetPointWorldPosition(obj, state->pathPointIndex, &x, &y, &z, 0);
        objSetPos(player, x, y, z);
        playerRender(player, renderArg2, renderArg3, renderArg4, renderArg5, -1);
    }
}

static void warpstone_hitDetect(GameObject* obj) {
    WarpStoneState* state = obj->extra;
    PartFxSpawnParams lightParams;

    if (ObjHits_GetPriorityHitWithPosition(obj, 0, 0, 0, &lightParams.posX, &lightParams.posY, &lightParams.posZ) !=
        0) {
        lightParams.posX += playerMapOffsetX;
        lightParams.posZ += playerMapOffsetZ;
        objDoHitParticleFx((void*)obj, 0.01f, &lightParams, 1, 0);
        if (randomChanceOneIn(3) != 0) {
            Sfx_PlayFromObject(obj, SFXTRIG_swapstone_move_short_2bc);
        } else {
            Sfx_PlayFromObject(obj, SFXTRIG_swapstone_move_short_2bc);
        }
        objSoundStartTimed(obj, &state->soundState, 171, -1280, -1, 0);
    }
}

static void warpstone_update(GameObject* obj) {
    WarpStoneState* state = obj->extra;

    if (state->child != NULL) {
        ObjLink_DetachChild(obj, state->child);
        Obj_FreeObject(state->child);
        state->child = NULL;
    }

    int advanceResult = warpstone_advanceAnimEvents(obj, 0.0055555557f);
    if (obj->anim.currentMove == WARPSTONE_MOVE_IDLE) {
        if (randomChanceOneIn(100) != 0) {
            objSoundStartTimed(obj, &state->soundState, 0xAB, -0x100, -1, 0);
        }
        if (randomChanceOneIn(500) != 0) {
            objSoundStartTimed(obj, &state->soundState, 0x417, -0x500, -1, 0);
        }
    }

    if (mainGetBit(GAMEBIT_ITEM_RockCandy_Used) != 0) {
        if (randomChanceOneIn(sWarpStoneLookToggleChance) != 0) {
            state->behaviorFlags ^= WARPSTONE_LOOK_AT_PLAYER;
        }
        if ((state->behaviorFlags & WARPSTONE_LOOK_AT_PLAYER) == 0 &&
            mainGetBit(GAMEBIT_SH_WarpStoneLookAtPlayer) != 0) {
            state->behaviorFlags |= WARPSTONE_LOOK_AT_PLAYER;
        }
    }

    GameObject* target;
    if ((state->behaviorFlags & WARPSTONE_LOOK_AT_PLAYER) != 0) {
        target = Obj_GetPlayerObject();
    } else {
        target = objGetNearestTypeTo(WARPSTONE_TARGET_OBJECT_GROUP, obj, 0);
    }

    obj->anim.localPosY += sWarpStoneHeadAimHeightOffset;
    characterAimHeadAtTarget(obj, target, &state->headAimState, 0x23, 1, sWarpStoneHeadAimMode);
    s16* modelVec = objFindJointPoseVector(obj, 0);
    obj->anim.localPosY -= sWarpStoneHeadAimHeightOffset;

    if (modelVec != NULL) {
        modelVec[1] += sWarpStoneHeadPitchOffset;
        modelVec[0] = 0;
        modelVec[0] += sWarpStoneHeadYawOffset;
    }

    if (advanceResult != 0) {
        state->behaviorFlags &= ~WARPSTONE_SFX_FIRED;
        int yawDelta = Obj_GetYawDeltaToObject(obj, target, NULL);
        yawDelta = (s16)(yawDelta - sWarpStoneYawBias);
        {
            int mag = yawDelta - 0x8000;
            mag = (mag >= 0) ? mag : -mag;
            if (mag > 0x18e3) {
                int moveId;
                if (yawDelta > 0) {
                    if (yawDelta > 0xe38) {
                        moveId = WARPSTONE_MOVE_TURN_FAR_RIGHT;
                    } else {
                        moveId = WARPSTONE_MOVE_TURN_RIGHT;
                    }
                } else if (yawDelta < -0xe38) {
                    moveId = WARPSTONE_MOVE_TURN_FAR_LEFT;
                } else {
                    moveId = WARPSTONE_MOVE_TURN_LEFT;
                }
                if (obj->anim.currentMove != moveId) {
                    ObjAnim_SetCurrentMove(obj, moveId, 0.0f, 0);
                }
            } else if (obj->anim.currentMove != WARPSTONE_MOVE_IDLE) {
                ObjAnim_SetCurrentMove(obj, WARPSTONE_MOVE_IDLE, 0.0f, 0);
                Sfx_StopFromObject(obj, SFXTRIG_swapstone_move_long);
            } else if (randomChanceOneIn(sWarpStoneMumbleChance) != 0) {
                Sfx_PlayFromObject(obj, SFXTRIG_swapstone_mumble);
                ObjAnim_SetCurrentMove(obj, WARPSTONE_MOVE_MUMBLE, 0.0f, 0);
            } else if (randomChanceOneIn(sWarpStoneYawnChance) != 0) {
                Sfx_PlayFromObject(obj, SFXTRIG_swapstone_move_long);
                ObjAnim_SetCurrentMove(obj, WARPSTONE_MOVE_YAWN, 0.0f, 0);
            }
        }
    }

    objSoundUpdateMouth(obj, &state->soundState);
    characterDoEyeAnims(obj, &state->eyeAnimState);
    if (mainGetBit(GAMEBIT_SH_SawWarpStoneIntro) == 0) {
        state->activated = 0;
    }
    if ((state->behaviorFlags & WARPSTONE_SFX_FIRED) != 0) {
        return;
    }

    switch (obj->anim.currentMove) {
    case WARPSTONE_MOVE_TURN_FAR_RIGHT:
    case WARPSTONE_MOVE_TURN_FAR_LEFT:
        if (obj->anim.currentMoveProgress > 0.5f) {
            Sfx_PlayFromObject(obj, SFXTRIG_swapstone_move_long);
            state->behaviorFlags |= WARPSTONE_SFX_FIRED;
        }
        break;
    case WARPSTONE_MOVE_TURN_RIGHT:
    case WARPSTONE_MOVE_TURN_LEFT:
        if (obj->anim.currentMoveProgress > 0.5f) {
            Sfx_PlayFromObject(obj, SFXTRIG_swapstone_move_short_2bc);
            state->behaviorFlags |= WARPSTONE_SFX_FIRED;
        }
        break;
    case WARPSTONE_MOVE_YAWN:
        if (obj->anim.currentMoveProgress > 0.6f) {
            Sfx_PlayFromObject(obj, SFXTRIG_swapstone_yawn);
            state->behaviorFlags |= WARPSTONE_SFX_FIRED;
        }
        break;
    case WARPSTONE_MOVE_MUMBLE:
        if (obj->anim.currentMoveProgress > 0.25f) {
            Sfx_PlayFromObject(obj, SFXTRIG_swapstone_move_short);
            state->behaviorFlags |= WARPSTONE_SFX_FIRED;
        }
        break;
    }
}

static void warpstone_init(GameObject* obj, const WarpStonePlacement* placement) {
    WarpStoneState* state = obj->extra;

    obj->anim.rotX = (s16)((u16)placement->initialYaw * 0x100);
    obj->animEventCallback = warpstone_SeqFn;
    state->sequenceGameBit = GAMEBIT_SH_WarpStoneRelated015A;
    state->resetGameBit = GAMEBIT_ITEM_RockCandyRelated0886;
    ObjHits_EnableObject(obj);
    if (mainGetBit(GAMEBIT_SH_SawWarpStoneIntro) != 0 && mainGetBit(GAMEBIT_SH_WarpStoneRelated015A) != 0) {
        state->activated = 1;
    } else {
        state->activated = 0;
    }
    mainSetBits(state->resetGameBit, 0);
    state->child = NULL;
}

static void warpstone_release(void) {
}

static void warpstone_initialise(void) {
}

OBJECT_INIT_ADAPTER(gWarpStoneObjDescriptorInitAdapter, warpstone_init, obj, placement)
OBJECT_TYPE_ID_ADAPTER(gWarpStoneObjDescriptorTypeIdAdapter, warpstone_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gWarpStoneObjDescriptorExtraSizeAdapter, warpstone_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gWarpStoneObjDescriptorAcquire, warpstone_initialise)

ObjectDescriptor gWarpStoneObjDescriptor = {
    .header = {
        .metadata = { 0, 0, 0, OBJECT_DESCRIPTOR_FLAGS_10_SLOTS },
        .acquire = gWarpStoneObjDescriptorAcquire,
        .release = warpstone_release,
    },
    .init = gWarpStoneObjDescriptorInitAdapter,
    .update = warpstone_update,
    .hitDetect = warpstone_hitDetect,
    .render = warpstone_render,
    .free = warpstone_free,
    .getObjectTypeId = gWarpStoneObjDescriptorTypeIdAdapter,
    .getExtraSize = gWarpStoneObjDescriptorExtraSizeAdapter,
};;
