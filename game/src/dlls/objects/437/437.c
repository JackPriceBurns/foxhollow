#include "main/render_envfx_api.h"
#include "main/objprint_character_api.h"
#include "dlls/objects/229_Shield.h"
#include "dlls/objects/284.h"
#include "dlls/objects/315_WallAnimato.h"
#include "dlls/objects/437.h"
#include "main/dll/dll_000D_playershadow.h"
#include "main/sky_api.h"
#include "main/object_render.h"
#include "main/dll/dll_0015_curves.h"
#include "track/intersect_api.h"
#include "main/track_dolphin_api.h"
#include "main/vecmath_distance_api.h"

#include "sys/objects.h"
#include "main/curve_eval.h"
#include "main/objhits.h"
#include "main/audio/sfx_keep_alive_api.h"
#include "main/audio/sfx_object_query_api.h"
#include "main/audio/sfx_position_api.h"
#include "main/audio/sfx_stop_object_api.h"
#include "main/gameloop_api.h"
#include "main/dll/dll_0000_gameui_api.h"
#include "main/lightmap_api.h"
#include "main/objfx.h"
#include "main/screen_transition.h"
#include "dolphin/MSL_C/PPCEABI/bare/H/math_api.h"
#include "main/shader_api.h"
#include "main/pi_dolphin_api.h"
#include "main/dll/player_state.h"
#include "main/dll/baddie_control_interface.h"
#include "main/dll/boneparticleeffect_interface.h"
#include "main/camera_interface.h"
#include "main/camera.h"
#include "main/dll/rom_curve_interface.h"
#include "main/dll/waterfx_interface.h"
#include "dolphin/pad.h"

#include "main/game_ui_interface.h"
#include "main/mapEventTypes.h"
#include "main/mm.h"
#include "main/objanim.h"
#include "main/objseq.h"
#include "main/resource.h"
#include "main/sky_interface.h"
#include "main/vecmath.h"
#include "main/dll/path_control_interface.h"
#include "main/frame_timing.h"
#include "main/pad.h"
#include "dolphin/gx/GXPixel.h"
#include "dolphin/gx/GXTransform.h"
#include "dlls/objects/260_SmallBasket.h"
#include "main/dll/dll_0000_gameui.h"
#include "main/dll/dll_000F_unk.h"
#include "main/dll/dll_00C9_enemy.h"
#include "main/dll/partfx_interface.h"
#include "main/objtype.h"
#include "main/obj_link.h"
#include "main/dll/dll_029B_arwingandrossstuff.h"
#include "main/dll/tricky_api.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/gamebit_ids.h"
#include "main/player_control_interface.h"
#include "main/sky.h"

#include "game/objects/object.h"
#include "main/audio/sfx_play_api.h"
#include "main/dll/player_api.h"
#include "main/gamebits_api.h"
#include "sys/objects/lifecycle.h"

enum LightfootStateId {
    LIGHTFOOT_STATE_ANIMATION_CYCLE,
    LIGHTFOOT_STATE_BUTTON_TIMING_CHALLENGE,
    LIGHTFOOT_STATE_TARGET_ANIMATION_CYCLE,
    LIGHTFOOT_STATE_RANDOM_TURN,
    LIGHTFOOT_STATE_WANDER,
    LIGHTFOOT_STATE_COUNT,
};

enum LightfootSubstateId {
    LIGHTFOOT_SUBSTATE_CHALLENGE_GATE,
    LIGHTFOOT_SUBSTATE_COMPLETION,
    LIGHTFOOT_SUBSTATE_PROXIMITY,
    LIGHTFOOT_SUBSTATE_COUNT,
};

enum LightfootInteractionState {
    LIGHTFOOT_INTERACTION_ACTIVE,
    LIGHTFOOT_INTERACTION_WAITING,
    LIGHTFOOT_INTERACTION_SUCCESS,
    LIGHTFOOT_INTERACTION_FAILURE,
};

enum LightfootMapObjectId {
    LIGHTFOOT_MAP_OBJECT_SCRIPTED_0 = 0x33E34,
    LIGHTFOOT_MAP_OBJECT_SCRIPTED_1 = 0x33E3C,
    LIGHTFOOT_MAP_OBJECT_SCRIPTED_2 = 0x34316,
    LIGHTFOOT_MAP_OBJECT_HEAVY_SCUFF = 0x3433F,
    LIGHTFOOT_MAP_OBJECT_SCRIPTED_3 = 0x45C47,
    LIGHTFOOT_MAP_OBJECT_SCRIPTED_4 = 0x460B6,
    LIGHTFOOT_MAP_OBJECT_CHALLENGE_GATE_1 = 0x46A51,
    LIGHTFOOT_MAP_OBJECT_CHALLENGE_GATE_2 = 0x46A55,
    LIGHTFOOT_MAP_OBJECT_CHALLENGE_GATE_3 = 0x49928,
    LIGHTFOOT_MAP_OBJECT_BABY_STATIC_0 = 0x4993F,
    LIGHTFOOT_MAP_OBJECT_BABY_STATIC_1 = 0x49940,
    LIGHTFOOT_MAP_OBJECT_BABY_STATIC_2 = 0x49941,
    LIGHTFOOT_MAP_OBJECT_BABY_STATIC_END = 0x49942,
    LIGHTFOOT_MAP_OBJECT_BABY_CHALLENGE_2_0 = 0x499AC,
    LIGHTFOOT_MAP_OBJECT_BABY_CHALLENGE_2_1 = 0x499AE,
    LIGHTFOOT_MAP_OBJECT_BABY_CHALLENGE_2_2 = 0x499AF,
    LIGHTFOOT_MAP_OBJECT_BABY_CHALLENGE_3_0 = 0x499B0,
    LIGHTFOOT_MAP_OBJECT_BABY_CHALLENGE_3_1 = 0x499B1,
    LIGHTFOOT_MAP_OBJECT_BABY_CHALLENGE_3_2 = 0x499B2,
    LIGHTFOOT_MAP_OBJECT_CHALLENGE_2_TARGET = 0x499B5,
    LIGHTFOOT_MAP_OBJECT_CHALLENGE_3_TARGET = 0x499B6,
};

enum LightfootWeaponObjectId {
    LIGHTFOOT_OBJECT_SWORD = 0x6F1,
    LIGHTFOOT_OBJECT_ALT_WEAPON = 0x6F2,
};

enum LightfootMoveCycleId {
    LIGHTFOOT_MOVE_CYCLE_TOTEM_BOND,
    LIGHTFOOT_MOVE_CYCLE_SCRIPTED_0,
    LIGHTFOOT_MOVE_CYCLE_SCRIPTED_3,
    LIGHTFOOT_MOVE_CYCLE_SCRIPTED_2,
    LIGHTFOOT_MOVE_CYCLE_SCRIPTED_4,
    LIGHTFOOT_MOVE_CYCLE_COUNT,
};

enum LightfootFlag {
    LIGHTFOOT_FLAG_SEQUENCE_TRIGGERED = 1 << 0,
};

enum LightfootRouteFlag {
    LIGHTFOOT_ROUTE_CHALLENGE_HIT = 1 << 1,
};

enum LightfootEventFlag {
    LIGHTFOOT_EVENT_FOOTSTEP = 1 << 0,
    LIGHTFOOT_EVENT_SCUFF = 1 << 1,
    LIGHTFOOT_EVENT_SCUFF_ALT = 1 << 2,
    LIGHTFOOT_EVENT_WEAPON_SWING_ALT = 1 << 6,
    LIGHTFOOT_EVENT_WEAPON_SWING = 1 << 7,
    LIGHTFOOT_EVENT_LANDING = 1 << 9,
    LIGHTFOOT_EVENT_HEAVY_HIT = 1 << 11,
};

enum LightfootSequenceEvent {
    LIGHTFOOT_SEQUENCE_EVENT_COMPLETE = 1,
};

typedef struct LightfootWeaponPlacement {
    ObjPlacement base;
    u8 unused18[8];
} LightfootWeaponPlacement;

typedef struct LightfootControlState {
    const s16* moveIds;
    const f32* moveSpeeds;
    f32 completionTimer;
    f32 pulseTimer;
    f32 lifeTimer;
    f32 wanderTimer;
    u16 meterPhase;
    u16 meterPhaseTwoFramesAgo;
    u16 previousMeterPhase;
    u16 targetSector;
    u16 targetYawDelta;
    u16 targetDistance;
    u16 moveIndex;
    s16 currentWeaponObjectId;
    s16 requestedWeaponObjectId;
    u16 movementSfxId;
    u8 completionCountdown;
    u8 challengeDifficulty;
    u8 challengeCompletePending;
    u8 unused37;
} LightfootControlState;

typedef struct LightfootState {
    GroundBaddieState groundBaddie;
    LightfootControlState control;
} LightfootState;

typedef struct LightfootScuffPattern {
    s16 moveIds[8];
    f32 moveSpeeds[8];
} LightfootScuffPattern;

typedef struct LightfootAnimationData {
    LightfootScuffPattern lightScuff;
    LightfootScuffPattern heavyScuff;
    s16 challengeMoveIds[14];
    f32 challengeMoveSpeeds[14];
} LightfootAnimationData;

typedef struct LightfootChallengeData {
    f32 targetMoveSpeeds[7];
    u16 difficultyGameBits[8];
    f32 meterScales[9];
} LightfootChallengeData;

typedef struct LightfootMoveCycle {
    s16 moveIds[2];
    f32 moveSpeeds[2];
} LightfootMoveCycle;

STATIC_ASSERT(sizeof(LightfootWeaponPlacement) == 0x20);
STATIC_ASSERT(offsetof(LightfootControlState, moveSpeeds) == sizeof(void*));
STATIC_ASSERT(offsetof(LightfootControlState, completionTimer) == sizeof(void*) * 2);
STATIC_ASSERT(offsetof(LightfootControlState, meterPhase) == sizeof(void*) * 2 + sizeof(f32) * 4);
STATIC_ASSERT(offsetof(LightfootControlState, moveIndex) == 0x2C);
STATIC_ASSERT(offsetof(LightfootControlState, challengeDifficulty) == 0x35);
STATIC_ASSERT(sizeof(LightfootControlState) == 0x38);
STATIC_ASSERT(offsetof(LightfootState, control) == sizeof(GroundBaddieState));
STATIC_ASSERT(sizeof(LightfootState) == 0x4B8);
STATIC_ASSERT(sizeof(LightfootScuffPattern) == 0x30);
STATIC_ASSERT(sizeof(LightfootAnimationData) == 0xB4);
STATIC_ASSERT(sizeof(LightfootChallengeData) == 0x50);

static PlayerStateFn sLightfootStateHandlers[LIGHTFOOT_STATE_COUNT];
static PlayerSubstateFn sLightfootSubstateHandlers[LIGHTFOOT_SUBSTATE_COUNT];

static LightfootControlState* Lightfoot_GetControl(GameObject* obj) {
    return &((LightfootState*)obj->extra)->control;
}

static int Lightfoot_UpdateProximityInteractionState(GameObject* obj, BaddieState* state, f32 timeDelta) {
    (void)timeDelta;
    GroundBaddieState* inner = obj->extra;
    if (state->targetObj != NULL) {
        if (Lightfoot_GetControl(obj)->targetDistance < inner->aggroRange) {
            if (state->moveJustStartedB != 0 || state->moveDone != 0 ||
                state->controlMode == LIGHTFOOT_STATE_ANIMATION_CYCLE) {
                (*gPlayerInterface)->setState(obj, state, LIGHTFOOT_STATE_WANDER);
            }
        } else if (state->moveJustStartedB != 0 || state->moveDone != 0) {
            (*gPlayerInterface)->setState(obj, state, LIGHTFOOT_STATE_ANIMATION_CYCLE);
        }
    }
    return 0;
}

static int Lightfoot_UpdateCompletionInteraction(GameObject* obj, BaddieState* state, f32 timeDelta) {
    (void)timeDelta;
    const LightfootPlacement* placement = (const LightfootPlacement*)obj->anim.placement;
    GroundBaddieState* inner = obj->extra;
    LightfootControlState* control = Lightfoot_GetControl(obj);
    if (state->moveJustStartedB != 0 || state->moveDone != 0) {
        if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->eventGameBit)) != 0) {
            inner->configFlags |= LIGHTFOOT_FLAG_SEQUENCE_TRIGGERED;
        }
        if ((inner->configFlags & LIGHTFOOT_FLAG_SEQUENCE_TRIGGERED) != 0) {
            if (state->controlMode != LIGHTFOOT_STATE_RANDOM_TURN) {
                control->completionCountdown = 4;
                (*gPlayerInterface)->setState(obj, state, LIGHTFOOT_STATE_RANDOM_TURN);
            }
            if (control->completionCountdown != 0) {
                control->completionCountdown -= 1;
                if (control->completionCountdown == 0) {
                    mainSetBits(ObjAnim_ReadPlacementS16(&obj->anim, &placement->completionGameBit), 1);
                    mainSetBits(ObjAnim_ReadPlacementS16(&obj->anim, &placement->activeGameBit), 0);
                    obj->anim.alpha = 0;
                    obj->anim.flags |= OBJANIM_FLAG_HIDDEN;
                    control->completionTimer = 120.0f;
                    control->lifeTimer = 100.0f;
                }
            }
        } else {
            if (state->controlMode != LIGHTFOOT_STATE_BUTTON_TIMING_CHALLENGE) {
                if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->activeGameBit)) != 0) {
                    (*gPlayerInterface)->setState(obj, state, LIGHTFOOT_STATE_BUTTON_TIMING_CHALLENGE);
                }
            }
        }
    }
    return 0;
}

static int Lightfoot_UpdateChallengeGateInteraction(GameObject* obj, BaddieState* state, f32 timeDelta) {
    (void)timeDelta;

    if (state->targetObj != NULL) {
        LightfootControlState* control = Lightfoot_GetControl(obj);
        int targetYawDelta = (s16)control->targetYawDelta;
        if (targetYawDelta < 0) {
            targetYawDelta = -targetYawDelta;
        }
        if ((u16)targetYawDelta < 0x1770) {
            const LightfootPlacement* placement = (const LightfootPlacement*)obj->anim.placement;
            obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_DISABLED;
            switch (placement->base.ident) {
            case LIGHTFOOT_MAP_OBJECT_CHALLENGE_GATE_1:
                if (mainGetBit(GAMEBIT_LV_ChallengeGate1Complete)) {
                    obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
                }
                break;
            case LIGHTFOOT_MAP_OBJECT_CHALLENGE_GATE_2:
                if (mainGetBit(GAMEBIT_LV_ChallengeGate2Complete)) {
                    obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
                }
                break;
            case LIGHTFOOT_MAP_OBJECT_CHALLENGE_GATE_3:
                if (mainGetBit(GAMEBIT_SC_ChallengeGate3Complete)) {
                    obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
                }
                break;
            }
            if ((obj->anim.resetHitboxFlags & INTERACT_FLAG_ACTIVATED) != 0) {
                buttonDisable(0, PAD_BUTTON_A);
                switch (placement->base.ident) {
                case LIGHTFOOT_MAP_OBJECT_CHALLENGE_GATE_1:
                    if (mainGetBit(GAMEBIT_LV_ChallengeGate1Target0) != 0 &&
                        mainGetBit(GAMEBIT_LV_ChallengeGate1Target1) != 0 &&
                        mainGetBit(GAMEBIT_LV_ChallengeGate1Target2) != 0) {
                        if (mainGetBit(GAMEBIT_LV_ChallengeGate1Complete) == 0) {
                            mainSetBits(GAMEBIT_LV_ChallengeGate1Complete, 1);
                            (*gObjectTriggerInterface)->runSequence(3, obj, -1);
                            control->challengeCompletePending = 1;
                            obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
                        }
                    } else {
                        (*gObjectTriggerInterface)->runSequence(2, obj, -1);
                    }
                    break;
                case LIGHTFOOT_MAP_OBJECT_CHALLENGE_GATE_2:
                    if (mainGetBit(GAMEBIT_LV_ChallengeGate2Target0) != 0 &&
                        mainGetBit(GAMEBIT_LV_ChallengeGate2Target1) != 0 &&
                        mainGetBit(GAMEBIT_LV_ChallengeGate2Target2) != 0) {
                        if (mainGetBit(GAMEBIT_LV_ChallengeGate2Complete) == 0) {
                            mainSetBits(GAMEBIT_LV_ChallengeGate2Complete, 1);
                            (*gObjectTriggerInterface)->runSequence(5, obj, -1);
                            control->challengeCompletePending = 1;
                            obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
                        }
                    } else {
                        (*gObjectTriggerInterface)->runSequence(4, obj, -1);
                    }
                    break;
                case LIGHTFOOT_MAP_OBJECT_CHALLENGE_GATE_3:
                    if (mainGetBit(GAMEBIT_SC_ChallengeGate3Target0) != 0 &&
                        mainGetBit(GAMEBIT_SC_ChallengeGate3Target1) != 0 &&
                        mainGetBit(GAMEBIT_SC_ChallengeGate3Target2) != 0) {
                        if (mainGetBit(GAMEBIT_SC_ChallengeGate3Complete) == 0) {
                            mainSetBits(GAMEBIT_SC_ChallengeGate3Complete, 1);
                            (*gObjectTriggerInterface)->runSequence(7, obj, -1);
                            control->challengeCompletePending = 1;
                            obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
                        }
                    } else {
                        (*gObjectTriggerInterface)->runSequence(6, obj, -1);
                    }
                    break;
                }
            }
        } else {
            obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
        }
        if (state->moveJustStartedB != 0 || state->moveDone != 0) {
            (*gPlayerInterface)->setState(obj, state, LIGHTFOOT_STATE_ANIMATION_CYCLE);
        }
    }
    return 0;
}

static int Lightfoot_UpdateWanderSteering(GameObject* obj, BaddieState* state, f32 deltaTime) {
    GroundBaddieState* inner = obj->extra;
    LightfootControlState* control = Lightfoot_GetControl(obj);
    if (control->wanderTimer <= 0.0f) {
        Sfx_PlayFromObject(obj, SFXTRIG_htop_hurry1);
        control->wanderTimer = (f32)randomGetRange(0x78, 0xb4);
    }
    state->moveSpeed = 0.04f * (1.0f - (f32)control->targetDistance / (f32)inner->aggroRange);
    if (state->moveSpeed < 0.01f) {
        state->moveSpeed = 0.01f;
    }
    if (state->moveJustStartedA != 0 || state->moveDone != 0) {
        if (control->completionCountdown != 0) {
            control->completionCountdown -= 1;
        } else {
            u8 clearDirections = (*gBaddieControlInterface)->getClearDirectionMask(obj, state, 50.0f);
            if ((clearDirections & 1) == 0) {
                if ((clearDirections & 4) != 0) {
                    obj->anim.rotX += 0x7ff8;
                    control->completionCountdown = 3;
                } else if ((clearDirections & 2) != 0) {
                    obj->anim.rotX -= 0x3ffc;
                    control->completionCountdown = 3;
                } else if ((clearDirections & 8) != 0) {
                    obj->anim.rotX += 0x3ffc;
                    control->completionCountdown = 3;
                }
            }
        }
        ObjAnim_SetCurrentMove(obj, 0x14, 0.0f, 0);
    }
    if (control->completionCountdown == 0) {
        obj->anim.rotX += (s16)((f32)(s32)(control->targetYawDelta - 0x7fff) * timeDelta / 4.0f);
    }
    (*gPlayerInterface)->updateAnimRootMotion(obj, state, deltaTime, 1);
    return 0;
}

static int Lightfoot_UpdateRandomTurn(GameObject* obj, BaddieState* state, f32 deltaTime) {
    if (state->moveJustStartedA != 0) {
        Sfx_PlayFromObject(obj, Lightfoot_GetControl(obj)->movementSfxId);
        if (randomGetRange(0, 1) != 0) {
            obj->anim.rotX += 0x8AA9;
        } else {
            obj->anim.rotX -= 0x8AA9;
        }
        ObjAnim_SetCurrentMove(obj, 0x23, 0.0f, 0);
    }
    state->moveSpeed = 0.017f;
    (*gPlayerInterface)->updateAnimRootMotion(obj, state, deltaTime, 1);
    return 0;
}

static const LightfootAnimationData sLightfootAnimationData = {
    {{10, 11, 12, 0, 11, 10, 10, -1}, {0.01f, 0.01f, 0.005f, 0.003f, 0.01f, 0.01f, 0.01f, -1.0f}},
    {{7, 10, 8, 9, 11, 12, 7, -1}, {0.0115f, 0.0105f, 0.005f, 0.003f, 0.01f, 0.011f, 0.012f, -1.0f}},
    {43, 45, 45, 45, 45, 46, 47, 47, 47, 47, 47, 47, 53, -1},
    {0.02f, 0.021f, 0.022f, 0.023f, 0.024f, 0.015f, 0.08f, 0.07f, 0.06f, 0.05f, 0.04f, 0.03f, 0.01f, -1.0f},
};

static const s16 sLightfootTargetMoveIds[8] = {51, 50, 52, 50, 52, 51, -1, 0};

static const LightfootChallengeData sLightfootChallengeData = {
    {0.013f, 0.005f, 0.01f, 0.005f, 0.013f, 0.01f, -1.0f},
    {GAMEBIT_SC_LightfootChallengeDifficulty0, GAMEBIT_SC_LightfootChallengeDifficulty1,
     GAMEBIT_SC_LightfootChallengeDifficulty2, GAMEBIT_SC_LightfootChallengeDifficulty3,
     GAMEBIT_SC_LightfootChallengeDifficulty4, GAMEBIT_SC_LightfootChallengeDifficulty5,
     GAMEBIT_SC_LightfootChallengeDifficulty6, GAMEBIT_SC_LightfootChallengeDifficulty7},
    {0.708f, 0.629f, 0.551f, 0.472f, 0.393f, 0.314f, 0.236f, 0.157f, 0.0f},
};

static int Lightfoot_UpdateTargetAnimationCycle(GameObject* obj, BaddieState* state, f32 deltaTime) {
    GroundBaddieState* inner = obj->extra;
    LightfootControlState* control = Lightfoot_GetControl(obj);
    if (state->targetObj != NULL) {
        characterSetHeadYawToTarget(obj, state->targetObj, &inner->eyeAnimState, 0x19);
    }
    if (state->moveDone != 0 || state->moveJustStartedA != 0) {
        const ObjPlacement* placement = obj->anim.placement;
        obj->anim.localPosX = placement->posX;
        obj->anim.localPosZ = placement->posZ;
        control->moveIndex += 1;
        if (sLightfootTargetMoveIds[control->moveIndex] == -1) {
            control->moveIndex = 0;
        }
        ObjAnim_SetCurrentMove(obj, sLightfootTargetMoveIds[control->moveIndex], 0.0f, 0);
    }
    state->moveSpeed = sLightfootChallengeData.targetMoveSpeeds[control->moveIndex];
    (*gPlayerInterface)->updateAnimRootMotion(obj, state, deltaTime, 1);
    return 0;
}

static int Lightfoot_UpdateButtonTimingChallenge(GameObject* obj, BaddieState* state, f32 deltaTime) {
    const LightfootPlacement* placement;
    GroundBaddieState* actor = obj->extra;
    LightfootControlState* control = Lightfoot_GetControl(obj);
    GameObject* target = state->targetObj;
    if (target != NULL) {
        characterSetHeadYawToTarget(obj, target, &actor->eyeAnimState, 0x19);
    }
    if (obj->userData2 == LIGHTFOOT_INTERACTION_ACTIVE) {
        control->meterPhaseTwoFramesAgo = control->previousMeterPhase;
        control->previousMeterPhase = control->meterPhase;
        control->meterPhase += (u16)(1200.0f * timeDelta);
    }
    if (control->moveIndex < 4) {
        s16 meterPosition = (s16)(90.0f * mathSinf(3.1415927f * control->meterPhase / 32768.0f));
        u16 successRange = (u16)(90.0f * sLightfootChallengeData.meterScales[control->challengeDifficulty]);
        if (obj->userData2 == LIGHTFOOT_INTERACTION_ACTIVE) {
            if ((s16)control->meterPhase * (s16)control->previousMeterPhase < 0) {
                Sfx_PlayFromObject(0, SFXTRIG_lockon3_off);
            }
        }
        setAButtonIcon(6);
        fearTestMeterSetRange(0x60, (u8)successRange, meterPosition);
        if ((getButtonsJustPressed(0) & PAD_BUTTON_A) != 0 && obj->userData2 == LIGHTFOOT_INTERACTION_ACTIVE) {
            int distanceFromCenter = meterPosition < 0 ? -meterPosition : meterPosition;
            if (distanceFromCenter <= successRange) {
                Sfx_PlayFromObject(0, SFXTRIG_menuups16k);
                obj->userData2 = LIGHTFOOT_INTERACTION_SUCCESS;
            } else {
                Sfx_PlayFromObject(0, SFXTRIG_lowoxy_beep);
                obj->userData2 = LIGHTFOOT_INTERACTION_FAILURE;
            }
            fearTestMeterSetFadeIn(0);
        }
    } else {
        fearTestMeterSetFadeIn(0);
    }
    if (state->moveDone != 0 || state->moveJustStartedA != 0) {
        if (state->moveJustStartedA != 0) {
            control->challengeDifficulty = 0;
            for (int i = 0; i < 8; i++) {
                if (mainGetBit(sLightfootChallengeData.difficultyGameBits[i]) != 0) {
                    control->challengeDifficulty += 1;
                }
            }
            control->meterPhase = (u16)randomGetRange(0, 0xffff);
            control->previousMeterPhase = control->meterPhase;
            control->meterPhaseTwoFramesAgo = control->previousMeterPhase;
            fearTestMeterSetRange(0x60, (u8)(96.0f * sLightfootChallengeData.meterScales[control->challengeDifficulty]),
                                  (s16)(90.0f * mathSinf(3.1415927f * control->meterPhase / 32768.0f)));
            fearTestMeterSetFadeIn(1);
            setAButtonIcon(6);
        }
        placement = (const LightfootPlacement*)obj->anim.placement;
        if (state->moveJustStartedA != 0) {
            control->moveIndex = 0;
            obj->anim.localPosX = placement->base.posX;
            obj->anim.localPosZ = placement->base.posZ;
        } else {
            control->moveIndex += 1;
        }
        if (sLightfootAnimationData.challengeMoveIds[control->moveIndex] == -1) {
            control->moveIndex = 0;
            obj->anim.localPosX = placement->base.posX;
            obj->anim.localPosZ = placement->base.posZ;
            mainSetBits(ObjAnim_ReadPlacementS16(&obj->anim, &placement->completionGameBit), 1);
            mainSetBits(ObjAnim_ReadPlacementS16(&obj->anim, &placement->activeGameBit), 0);
            return LIGHTFOOT_STATE_RANDOM_TURN;
        }
        ObjAnim_SetCurrentMove(obj, sLightfootAnimationData.challengeMoveIds[control->moveIndex], 0.0f, 0);
    }
    state->moveSpeed = sLightfootAnimationData.challengeMoveSpeeds[control->moveIndex];
    (*gPlayerInterface)->updateAnimRootMotion(obj, state, deltaTime, 1);
    return 0;
}

static int Lightfoot_UpdateAnimationCycle(GameObject* obj, BaddieState* state, f32 deltaTime) {
    GroundBaddieState* inner = obj->extra;
    LightfootControlState* control;
    const s16* moves;
    const f32* blends;
    if (state->targetObj != NULL) {
        characterSetHeadYawToTarget(obj, state->targetObj, &inner->eyeAnimState, 0x19);
    }
    control = Lightfoot_GetControl(obj);
    moves = control->moveIds;
    blends = control->moveSpeeds;
    if (state->moveJustStartedA != 0 || state->moveDone != 0) {
        control->completionCountdown = 0;
        control->moveIndex += 1;
        if (moves[control->moveIndex] == -1) {
            control->moveIndex = 0;
        }
        if (state->moveJustStartedA != 0) {
            obj->anim.currentMoveProgress = (f32)randomGetRange(0, 0x63) / 100.0f;
            ObjAnim_SetCurrentMove(obj, moves[control->moveIndex], obj->anim.currentMoveProgress, 0);
        } else {
            ObjAnim_SetCurrentMove(obj, moves[control->moveIndex], 0.0f, 0);
        }
    }
    state->moveSpeed = blends[control->moveIndex];
    (*gPlayerInterface)->updateAnimRootMotion(obj, state, deltaTime, 0);
    return 0;
}

static void Lightfoot_RecordCompletedChallengeTargetHit(GameObject* obj, GroundBaddieState* inner,
                                                        LightfootControlState* control) {
    if (control->challengeCompletePending == 0) {
        return;
    }
    if ((inner->flags400 & LIGHTFOOT_ROUTE_CHALLENGE_HIT) == 0) {
        return;
    }

    switch (obj->anim.placement->ident) {
    case LIGHTFOOT_MAP_OBJECT_CHALLENGE_GATE_1:
        if (mainGetBit(GAMEBIT_LV_ChallengeGate1TargetHitRecorded) == 0) {
            mainSetBits(GAMEBIT_LV_ChallengeGate1TargetHitRecorded, 1);
        }
        break;
    case LIGHTFOOT_MAP_OBJECT_CHALLENGE_GATE_2:
        if (mainGetBit(GAMEBIT_LV_ChallengeGate2TargetHitRecorded) == 0) {
            mainSetBits(GAMEBIT_LV_ChallengeGate2TargetHitRecorded, 1);
        }
        break;
    case LIGHTFOOT_MAP_OBJECT_CHALLENGE_GATE_3:
        if (mainGetBit(GAMEBIT_SC_ChallengeGate3TargetHitRecorded) == 0) {
            mainSetBits(GAMEBIT_SC_ChallengeGate3TargetHitRecorded, 1);
        }
        break;
    }
    control->challengeCompletePending = 0;
}

static void Lightfoot_ProcessHitResponseFlags(GameObject* obj, BaddieState* inner) {
    if ((inner->eventFlags & LIGHTFOOT_EVENT_SCUFF_ALT) != 0) {
        inner->eventFlags &= ~LIGHTFOOT_EVENT_SCUFF_ALT;
        Sfx_PlayFromObject(obj, SFXTRIG_sc_spotfox02);
    }
    if ((inner->eventFlags & LIGHTFOOT_EVENT_SCUFF) != 0) {
        inner->eventFlags &= ~LIGHTFOOT_EVENT_SCUFF;
        Sfx_PlayFromObject(obj, SFXTRIG_sc_spotfox02);
    }
    if ((inner->eventFlags & LIGHTFOOT_EVENT_FOOTSTEP) != 0) {
        inner->eventFlags &= ~LIGHTFOOT_EVENT_FOOTSTEP;
        if (randomGetRange(0, 2) == 0) {
            Sfx_PlayFromObject(obj, SFXTRIG_skeep_mumb4);
        }
    }
    if ((inner->eventFlags & LIGHTFOOT_EVENT_WEAPON_SWING) != 0) {
        inner->eventFlags &= ~LIGHTFOOT_EVENT_WEAPON_SWING;
        Sfx_PlayFromObject(obj, SFXTRIG_wp_swdtest322);
    }
    if ((inner->eventFlags & LIGHTFOOT_EVENT_LANDING) != 0) {
        inner->eventFlags &= ~LIGHTFOOT_EVENT_LANDING;
        Sfx_PlayFromObject(obj, SFXTRIG_sk_trwhin3);
    }
    if ((inner->eventFlags & LIGHTFOOT_EVENT_WEAPON_SWING_ALT) != 0) {
        inner->eventFlags &= ~LIGHTFOOT_EVENT_WEAPON_SWING_ALT;
        Sfx_PlayFromObject(obj, SFXTRIG_wp_swdtest322_135);
    }
    if ((inner->eventFlags & LIGHTFOOT_EVENT_HEAVY_HIT) != 0) {
        inner->eventFlags &= ~LIGHTFOOT_EVENT_HEAVY_HIT;
        ObjHits_RecordObjectHit(Obj_GetPlayerObject(), obj, 0x19, 2, 1);
        Sfx_PlayFromObject(obj, SFXTRIG_wp_simp1_c);
        CameraShake_StartDampened(2.5f, 5.0f, 4.0f);
        doRumble(11.0f);
    }
}

static void Lightfoot_ResetScriptedPosition(GameObject* obj) {
    switch (obj->anim.placement->ident) {
    case LIGHTFOOT_MAP_OBJECT_SCRIPTED_2:
        obj->anim.worldPosX = -2692.46f;
        obj->anim.worldPosY = -981.0f;
        obj->anim.worldPosZ = 497.2f;
        obj->anim.rotX = 0x2565;
        break;
    case LIGHTFOOT_MAP_OBJECT_SCRIPTED_1:
        obj->anim.worldPosX = -2746.88f;
        obj->anim.worldPosY = -997.0f;
        obj->anim.worldPosZ = 407.7f;
        obj->anim.rotX = 0x1c42;
        break;
    case LIGHTFOOT_MAP_OBJECT_SCRIPTED_0:
        obj->anim.worldPosX = -2789.69f;
        obj->anim.worldPosY = -997.0f;
        obj->anim.worldPosZ = 457.71f;
        obj->anim.rotX = 0x1d00;
        break;
    case LIGHTFOOT_MAP_OBJECT_SCRIPTED_3:
        obj->anim.worldPosX = -2682.64f;
        obj->anim.worldPosY = -981.0f;
        obj->anim.worldPosZ = 450.29f;
        obj->anim.rotX = 0x32c1;
        break;
    case LIGHTFOOT_MAP_OBJECT_SCRIPTED_4:
        obj->anim.worldPosX = -2737.49f;
        obj->anim.worldPosY = -981.0f;
        obj->anim.worldPosZ = 529.58f;
        obj->anim.rotX = 0x119f;
        break;
    }
}

static void Lightfoot_UpdateAttachedChild(GameObject* obj, GroundBaddieState* inner) {
    LightfootControlState* control = Lightfoot_GetControl(obj);
    GameObject* child;

    if (control->currentWeaponObjectId == control->requestedWeaponObjectId) {
        return;
    }
    if (obj->anim.alpha == 0) {
        return;
    }

    child = obj->childObjs[0];
    if (child != NULL) {
        ObjLink_DetachChild(obj, child);
        Obj_FreeObject(child);
    }
    if (Obj_IsLoadingLocked()) {
        if (control->requestedWeaponObjectId > 0) {
            LightfootWeaponPlacement* placement = (LightfootWeaponPlacement*)Obj_AllocObjectSetup(
                sizeof(LightfootWeaponPlacement), control->requestedWeaponObjectId);
            child = objSetupObject(&placement->base, 4, obj->anim.mapEventSlot, -1, obj->anim.parent);
            ObjLink_AttachChild(obj, child, 0);
            control->currentWeaponObjectId = control->requestedWeaponObjectId;
        }
    } else {
        control->currentWeaponObjectId = 0;
    }
}

static void Lightfoot_UpdatePlayerInteraction(GameObject* obj, GroundBaddieState* inner, BaddieState* state) {
    LightfootControlState* control = Lightfoot_GetControl(obj);
    const ObjPlacement* placement = obj->anim.placement;

    (*gBaddieControlInterface)
        ->getTargetGeometry(obj, Obj_GetPlayerObject(), 0x10, &control->targetSector, &control->targetYawDelta,
                            &control->targetDistance);
    state->targetDistance = (f32)(u32)control->targetDistance;
    if (obj->userData2 == LIGHTFOOT_INTERACTION_SUCCESS) {
        (*gObjectTriggerInterface)->runSequence(0, obj, -1);
        obj->userData2 = LIGHTFOOT_INTERACTION_WAITING;
    } else if (obj->userData2 == LIGHTFOOT_INTERACTION_FAILURE) {
        (*gObjectTriggerInterface)->runSequence(1, obj, -1);
        obj->userData2 = LIGHTFOOT_INTERACTION_WAITING;
    } else {
        characterDoEyeAnims(obj, &inner->eyeAnimState);
        state->targetObj = Obj_GetPlayerObject();
        if (placement->ident >= LIGHTFOOT_MAP_OBJECT_BABY_STATIC_END ||
            placement->ident < LIGHTFOOT_MAP_OBJECT_BABY_STATIC_0) {
            (*gBaddieControlInterface)->updateGravity(obj, state, 0.17f, 1);
        }
        inner->savedPendingParentObj = obj->pendingParentObj;
        obj->pendingParentObj = 0;
        (*gPlayerInterface)
            ->update(obj, state, timeDelta, timeDelta, sLightfootStateHandlers, sLightfootSubstateHandlers);
        obj->pendingParentObj = inner->savedPendingParentObj;
        Lightfoot_ProcessHitResponseFlags(obj, &inner->baddie);
    }
}

static void Lightfoot_UpdateLifeTimer(GameObject* obj, LightfootControlState* control) {
    if (control->lifeTimer == 0.0f) {
        return;
    }

    control->lifeTimer -= timeDelta;
    if (control->lifeTimer <= 0.0f) {
        Obj_FreeObject(obj);
    }
}

static int Lightfoot_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate) {
    (void)unused;
    GroundBaddieState* inner = obj->extra;
    const LightfootPlacement* placement = (const LightfootPlacement*)obj->anim.placement;
    LightfootControlState* control = Lightfoot_GetControl(obj);
    int mode;
    Vec pulseOffset;
    Vec pulseOffsets[2];

    Lightfoot_UpdateLifeTimer(obj, control);
    for (u8 i = 0; i < animUpdate->eventCount; i++) {
        switch (animUpdate->eventIds[i]) {
        case LIGHTFOOT_SEQUENCE_EVENT_COMPLETE:
            inner->configFlags |= LIGHTFOOT_FLAG_SEQUENCE_TRIGGERED;
            mainSetBits(ObjAnim_ReadPlacementS16(&obj->anim, &placement->eventGameBit), 1);
            pulseOffsets[1].x = 0.0f;
            pulseOffsets[1].y = 35.0f;
            pulseOffsets[1].z = 0.0f;
            for (u8 i = 0x19; i != 0; i--) {
                objfx_spawnPulseBurst(obj, 0.8f * obj->anim.rootMotionScale, 3, 0, 0, pulseOffsets);
            }
            break;
        }
    }
    if (ObjAnim_ReadPlacementS16(&obj->anim, &placement->completionGameBit) == GAMEBIT_SC_TotemBondOrbCollected) {
        Lightfoot_UpdatePlayerInteraction(obj, inner, &inner->baddie);
        if ((inner->configFlags & LIGHTFOOT_FLAG_SEQUENCE_TRIGGERED) != 0 &&
            (obj->objectFlags & OBJECT_OBJFLAG_RENDERED) != 0) {
            control->pulseTimer -= timeDelta;
            if (control->pulseTimer <= 0.0f) {
                mode = 3;
                control->pulseTimer += 15.0f;
            } else {
                mode = 0;
            }
            pulseOffset.x = 0.0f;
            pulseOffset.y = 35.0f;
            pulseOffset.z = 0.0f;
            Sfx_KeepAliveLoopedObjectSound(obj, SFXTRIG_foot_metal_scuff_455);
            objfx_spawnPulseBurst(obj, 0.2f * obj->anim.rootMotionScale, 3, mode, 0, &pulseOffset);
        }
    }
    inner->flags400 |= LIGHTFOOT_ROUTE_CHALLENGE_HIT;
    return 0;
}

static const LightfootMoveCycle sLightfootMoveCycles[LIGHTFOOT_MOVE_CYCLE_COUNT] = {
    [LIGHTFOOT_MOVE_CYCLE_TOTEM_BOND] = {{0x33, -1}, {0.0009f, -1.0f}},
    [LIGHTFOOT_MOVE_CYCLE_SCRIPTED_0] = {{0x33, -1}, {0.001f, -1.0f}},
    [LIGHTFOOT_MOVE_CYCLE_SCRIPTED_3] = {{0x36, -1}, {0.003f, -1.0f}},
    [LIGHTFOOT_MOVE_CYCLE_SCRIPTED_2] = {{0x128, -1}, {0.01f, -1.0f}},
    [LIGHTFOOT_MOVE_CYCLE_SCRIPTED_4] = {{1, -1}, {0.01f, -1.0f}},
};

static int Lightfoot_getExtraSize(void) {
    return sizeof(LightfootState);
}

static int Lightfoot_getObjectTypeId(void) {
    return 0x14B;
}

static void Lightfoot_free(GameObject* obj, int preserveChildren) {
    objFreeObjectType(obj, 3);
    int childCount = obj->childCount;
    for (int i = 0; i < childCount; i++) {
        GameObject* child = obj->childObjs[0];
        if (child != NULL) {
            ObjLink_DetachChild(obj, child);
            if (preserveChildren == 0) {
                Obj_FreeObject(child);
            }
        }
    }
    (*gBaddieControlInterface)->releaseState(obj, obj->extra, 0x20);
}

static void Lightfoot_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                             s8 visible) {
    if (visible != 0 && obj->userData1 == 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

static void Lightfoot_hitDetect(void) {
}

static int Lightfoot_AreTargetsComplete(enum GameBitId target0, enum GameBitId target1, enum GameBitId target2) {
    return mainGetBit(target0) != 0 && mainGetBit(target1) != 0 && mainGetBit(target2) != 0;
}

static int Lightfoot_UpdateBabyDelivery(GameObject* obj, GroundBaddieState* state,
                                        enum GameBitId challengeActiveGameBit, s32 targetObjectId,
                                        enum GameBitId target0, enum GameBitId target1, enum GameBitId target2) {
    if (mainGetBit(challengeActiveGameBit) == 0 || mainGetBit(state->gameBitA) != 0) {
        return 1;
    }

    GameObject* target = ObjList_FindObjectById(targetObjectId);
    if (target != NULL && Vec_distance(&obj->anim.worldPosX, &target->anim.worldPosX) < 25.0f) {
        mainSetBits(state->gameBitA, 1);
        PartFxSpawnParams effectOrigin = {
            .pos = {0.0f, 10.0f, 0.0f},
        };
        for (u8 i = 0x14; i != 0; i--) {
            objfx_spawnDirectionalBurst(obj, 5, 5.0f, 5, 6, 0x64, 10.0f, &effectOrigin, 0);
        }

        if (Lightfoot_AreTargetsComplete(target0, target1, target2)) {
            Sfx_PlayFromObject(0, SFXTRIG_mpick1_b);
        } else {
            Sfx_PlayFromObject(0, SFXTRIG_sc_menuups16k_409);
        }
    }
    return mainGetBit(state->gameBitA);
}

static void Lightfoot_update(GameObject* obj) {
    LightfootState* state = obj->extra;
    GroundBaddieState* actor = &state->groundBaddie;
    LightfootControlState* control = &state->control;
    const LightfootPlacement* placement = (const LightfootPlacement*)obj->anim.placement;

    Lightfoot_UpdateLifeTimer(obj, control);

    if (obj->anim.romDefNo == LIGHTFOOT_OBJECT_BABY && actor->gameBitA != -1) {
        switch (placement->base.ident) {
        case LIGHTFOOT_MAP_OBJECT_BABY_STATIC_0:
        case LIGHTFOOT_MAP_OBJECT_BABY_STATIC_1:
        case LIGHTFOOT_MAP_OBJECT_BABY_STATIC_2:
            if (mainGetBit(GAMEBIT_SC_MusicTreeActive) != 0) {
                obj->userData1 = mainGetBit(actor->gameBitA);
            } else {
                obj->userData1 = 1;
            }
            break;
        case LIGHTFOOT_MAP_OBJECT_BABY_CHALLENGE_2_0:
        case LIGHTFOOT_MAP_OBJECT_BABY_CHALLENGE_2_1:
        case LIGHTFOOT_MAP_OBJECT_BABY_CHALLENGE_2_2:
            obj->userData1 = Lightfoot_UpdateBabyDelivery(
                obj, actor, GAMEBIT_LV_ChallengeGate2Active, LIGHTFOOT_MAP_OBJECT_CHALLENGE_2_TARGET,
                GAMEBIT_LV_ChallengeGate2Target0, GAMEBIT_LV_ChallengeGate2Target1, GAMEBIT_LV_ChallengeGate2Target2);
            break;
        case LIGHTFOOT_MAP_OBJECT_BABY_CHALLENGE_3_0:
        case LIGHTFOOT_MAP_OBJECT_BABY_CHALLENGE_3_1:
        case LIGHTFOOT_MAP_OBJECT_BABY_CHALLENGE_3_2:
            obj->userData1 = Lightfoot_UpdateBabyDelivery(
                obj, actor, GAMEBIT_SC_ChallengeGate3Active, LIGHTFOOT_MAP_OBJECT_CHALLENGE_3_TARGET,
                GAMEBIT_SC_ChallengeGate3Target0, GAMEBIT_SC_ChallengeGate3Target1, GAMEBIT_SC_ChallengeGate3Target2);
            break;
        default:
            obj->userData1 = mainGetBit(actor->gameBitA) == 0;
            break;
        }

        if (obj->userData1 != 0) {
            ObjHits_DisableObject(obj);
            obj->anim.flags |= OBJANIM_FLAG_HIDDEN;
        } else {
            ObjHits_EnableObject(obj);
            obj->anim.flags &= ~OBJANIM_FLAG_HIDDEN;
        }
    }

    if (obj->userData1 != 0) {
        int challengeTargetActive =
            (placement->base.ident == LIGHTFOOT_MAP_OBJECT_CHALLENGE_2_TARGET &&
             mainGetBit(GAMEBIT_LV_ChallengeGate2Active) != 0 &&
             !Lightfoot_AreTargetsComplete(GAMEBIT_LV_ChallengeGate2Target0, GAMEBIT_LV_ChallengeGate2Target1,
                                           GAMEBIT_LV_ChallengeGate2Target2)) ||
            (placement->base.ident == LIGHTFOOT_MAP_OBJECT_CHALLENGE_3_TARGET &&
             mainGetBit(GAMEBIT_SC_ChallengeGate3Active) != 0 &&
             !Lightfoot_AreTargetsComplete(GAMEBIT_SC_ChallengeGate3Target0, GAMEBIT_SC_ChallengeGate3Target1,
                                           GAMEBIT_SC_ChallengeGate3Target2));
        if (challengeTargetActive) {
            PartFxSpawnParams effectOrigin = {
                .pos = {0.0f, 24.0f, 0.0f},
            };
            objfx_spawnArcedBurst(obj, 5, 0.75f, 1, 6, 0x32, 25.0f, 25.0f, 48.0f, &effectOrigin, 0);
        }
        return;
    }

    Lightfoot_UpdateAttachedChild(obj, actor);
    if ((actor->flags400 & LIGHTFOOT_ROUTE_CHALLENGE_HIT) != 0) {
        Lightfoot_RecordCompletedChallengeTargetHit(obj, actor, control);
        Lightfoot_ResetScriptedPosition(obj);
        obj->userData2 = LIGHTFOOT_INTERACTION_ACTIVE;
        actor->flags400 &= ~LIGHTFOOT_ROUTE_CHALLENGE_HIT;
    }
    Lightfoot_UpdatePlayerInteraction(obj, actor, &actor->baddie);
    if ((actor->configFlags & LIGHTFOOT_FLAG_SEQUENCE_TRIGGERED) != 0 &&
        (obj->objectFlags & OBJECT_OBJFLAG_RENDERED) != 0) {
        int pulseMode;
        control->pulseTimer -= timeDelta;
        if (control->pulseTimer <= 0.0f) {
            pulseMode = 3;
            control->pulseTimer += 15.0f;
        } else {
            pulseMode = 0;
        }
        Vec pulseOffset = {0.0f, 35.0f, 0.0f};
        Sfx_KeepAliveLoopedObjectSound(obj, SFXTRIG_foot_metal_scuff_455);
        objfx_spawnPulseBurst(obj, 0.2f * obj->anim.rootMotionScale, 3, pulseMode, 0, &pulseOffset);
    }
    control->wanderTimer -= timeDelta;
}

static void Lightfoot_SetMoveCycle(LightfootControlState* control, enum LightfootMoveCycleId cycleId) {
    control->moveIds = sLightfootMoveCycles[cycleId].moveIds;
    control->moveSpeeds = sLightfootMoveCycles[cycleId].moveSpeeds;
}

static void Lightfoot_init(GameObject* obj, const LightfootPlacement* placement, int isReload) {
    LightfootState* state = obj->extra;
    GroundBaddieState* actor = &state->groundBaddie;
    u8 initFlags = 0x16;

    if (isReload != 0) {
        initFlags |= 1;
    }
    (*gBaddieControlInterface)->initGroundBaddie(obj, (u8*)placement, (u8*)state, 5, 3, 0x108, initFlags, 20.0f);
    obj->animEventCallback = Lightfoot_SeqFn;
    actor->baddie.controlMode = LIGHTFOOT_STATE_ANIMATION_CYCLE;
    actor->baddie.substate = LIGHTFOOT_SUBSTATE_CHALLENGE_GATE;
    obj->objectFlags |= OBJECT_OBJFLAG_HITDETECT_DISABLED;
    LightfootControlState* control = &state->control;
    control->currentWeaponObjectId = -1;
    control->requestedWeaponObjectId = control->currentWeaponObjectId;
    obj->objectFlags |= placement->objectFlags & 0x7;
    if (ObjAnim_ReadPlacementS16(&obj->anim, &placement->completionGameBit) == GAMEBIT_SC_TotemBondOrbCollected) {
        actor->baddie.controlMode = LIGHTFOOT_STATE_TARGET_ANIMATION_CYCLE;
        actor->baddie.substate = LIGHTFOOT_SUBSTATE_COMPLETION;
        ObjHits_DisableObject(obj);
        control->moveIndex = randomGetRange(0, 3);
        control->requestedWeaponObjectId = LIGHTFOOT_OBJECT_SWORD;
        Lightfoot_SetMoveCycle(control, LIGHTFOOT_MOVE_CYCLE_TOTEM_BOND);
        obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
        obj->userData2 = LIGHTFOOT_INTERACTION_ACTIVE;
    } else {
        switch (placement->base.ident) {
        case LIGHTFOOT_MAP_OBJECT_SCRIPTED_2:
            Lightfoot_SetMoveCycle(control, LIGHTFOOT_MOVE_CYCLE_SCRIPTED_2);
            ObjHits_DisableObject(obj);
            obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
            obj->anim.currentMoveProgress = (f32)(s32)randomGetRange(0, 0x63) / 100.0f;
            break;
        case LIGHTFOOT_MAP_OBJECT_SCRIPTED_1:
            Lightfoot_SetMoveCycle(control, LIGHTFOOT_MOVE_CYCLE_TOTEM_BOND);
            control->requestedWeaponObjectId = LIGHTFOOT_OBJECT_SWORD;
            obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
            obj->anim.currentMoveProgress = (f32)(s32)randomGetRange(0, 0x63) / 100.0f;
            break;
        case LIGHTFOOT_MAP_OBJECT_SCRIPTED_0:
            Lightfoot_SetMoveCycle(control, LIGHTFOOT_MOVE_CYCLE_SCRIPTED_0);
            control->requestedWeaponObjectId = LIGHTFOOT_OBJECT_SWORD;
            obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
            obj->anim.currentMoveProgress = (f32)(s32)randomGetRange(0, 0x63) / 100.0f;
            break;
        case LIGHTFOOT_MAP_OBJECT_SCRIPTED_3:
            Lightfoot_SetMoveCycle(control, LIGHTFOOT_MOVE_CYCLE_SCRIPTED_3);
            ObjHits_DisableObject(obj);
            control->requestedWeaponObjectId = LIGHTFOOT_OBJECT_ALT_WEAPON;
            obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
            obj->anim.currentMoveProgress = (f32)(s32)randomGetRange(0, 0x63) / 100.0f;
            break;
        case LIGHTFOOT_MAP_OBJECT_SCRIPTED_4:
            Lightfoot_SetMoveCycle(control, LIGHTFOOT_MOVE_CYCLE_SCRIPTED_4);
            ObjHits_DisableObject(obj);
            obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
            obj->anim.currentMoveProgress = (f32)(s32)randomGetRange(0, 0x63) / 100.0f;
            break;
        case LIGHTFOOT_MAP_OBJECT_HEAVY_SCUFF:
            control->moveIds = sLightfootAnimationData.heavyScuff.moveIds;
            control->moveSpeeds = sLightfootAnimationData.heavyScuff.moveSpeeds;
            obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
            obj->anim.currentMoveProgress = (f32)(s32)randomGetRange(0, 0x63) / 100.0f;
            break;
        case LIGHTFOOT_MAP_OBJECT_CHALLENGE_GATE_1:
            if (mainGetBit(GAMEBIT_LV_ChallengeGate1Complete)) {
                obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
            }
            control->moveIds = sLightfootAnimationData.lightScuff.moveIds;
            control->moveSpeeds = sLightfootAnimationData.lightScuff.moveSpeeds;
            break;
        case LIGHTFOOT_MAP_OBJECT_CHALLENGE_GATE_2:
            if (mainGetBit(GAMEBIT_LV_ChallengeGate2Complete)) {
                obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
            }
            control->moveIds = sLightfootAnimationData.lightScuff.moveIds;
            control->moveSpeeds = sLightfootAnimationData.lightScuff.moveSpeeds;
            break;
        case LIGHTFOOT_MAP_OBJECT_CHALLENGE_GATE_3:
            if (mainGetBit(GAMEBIT_SC_ChallengeGate3Complete)) {
                obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
            }
            control->moveIds = sLightfootAnimationData.lightScuff.moveIds;
            control->moveSpeeds = sLightfootAnimationData.lightScuff.moveSpeeds;
            break;
        case LIGHTFOOT_MAP_OBJECT_BABY_CHALLENGE_2_0:
        case LIGHTFOOT_MAP_OBJECT_BABY_CHALLENGE_2_1:
        case LIGHTFOOT_MAP_OBJECT_BABY_CHALLENGE_2_2:
        case LIGHTFOOT_MAP_OBJECT_BABY_CHALLENGE_3_0:
        case LIGHTFOOT_MAP_OBJECT_BABY_CHALLENGE_3_1:
        case LIGHTFOOT_MAP_OBJECT_BABY_CHALLENGE_3_2:
            actor->baddie.substate = LIGHTFOOT_SUBSTATE_PROXIMITY;
            control->moveIds = sLightfootAnimationData.heavyScuff.moveIds;
            control->moveSpeeds = sLightfootAnimationData.heavyScuff.moveSpeeds;
            control->wanderTimer = (f32)(s32)randomGetRange(0x78, 0xB4);
            obj->anim.currentMoveProgress = (f32)(s32)randomGetRange(0, 0x63) / 100.0f;
            break;
        case LIGHTFOOT_MAP_OBJECT_CHALLENGE_2_TARGET:
        case LIGHTFOOT_MAP_OBJECT_CHALLENGE_3_TARGET:
            obj->userData1 = 1;
            control->moveIds = sLightfootAnimationData.heavyScuff.moveIds;
            control->moveSpeeds = sLightfootAnimationData.heavyScuff.moveSpeeds;
            break;
        default:
            control->moveIds = sLightfootAnimationData.lightScuff.moveIds;
            control->moveSpeeds = sLightfootAnimationData.lightScuff.moveSpeeds;
            break;
        }
    }
    Lightfoot_ResetScriptedPosition(obj);
    ObjAnim_SetMoveProgress(&obj->anim, (f32)(s32)randomGetRange(0, 0x63) / 100.0f);
    control->movementSfxId = (u16)(randomGetRange(0, 1) != 0 ? SFXTRIG_sk_trwhin3 : SFXTRIG_trwhin4);
    control->pulseTimer = 15.0f;
    if (obj->userData1 != 0) {
        ObjHits_DisableObject(obj);
    }
}

static void Lightfoot_release(void) {
}

static void Lightfoot_initialise(void) {
    sLightfootStateHandlers[LIGHTFOOT_STATE_ANIMATION_CYCLE] = Lightfoot_UpdateAnimationCycle;
    sLightfootStateHandlers[LIGHTFOOT_STATE_BUTTON_TIMING_CHALLENGE] = Lightfoot_UpdateButtonTimingChallenge;
    sLightfootStateHandlers[LIGHTFOOT_STATE_TARGET_ANIMATION_CYCLE] = Lightfoot_UpdateTargetAnimationCycle;
    sLightfootStateHandlers[LIGHTFOOT_STATE_RANDOM_TURN] = Lightfoot_UpdateRandomTurn;
    sLightfootStateHandlers[LIGHTFOOT_STATE_WANDER] = Lightfoot_UpdateWanderSteering;
    sLightfootSubstateHandlers[LIGHTFOOT_SUBSTATE_CHALLENGE_GATE] = Lightfoot_UpdateChallengeGateInteraction;
    sLightfootSubstateHandlers[LIGHTFOOT_SUBSTATE_COMPLETION] = Lightfoot_UpdateCompletionInteraction;
    sLightfootSubstateHandlers[LIGHTFOOT_SUBSTATE_PROXIMITY] = Lightfoot_UpdateProximityInteractionState;
}

ObjectDescriptor gLightfootObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    (ObjectDescriptorCallback)Lightfoot_initialise,
    (ObjectDescriptorCallback)Lightfoot_release,
    0,
    (ObjectDescriptorCallback)Lightfoot_init,
    (ObjectDescriptorCallback)Lightfoot_update,
    (ObjectDescriptorCallback)Lightfoot_hitDetect,
    (ObjectDescriptorCallback)Lightfoot_render,
    (ObjectDescriptorCallback)Lightfoot_free,
    (ObjectDescriptorCallback)Lightfoot_getObjectTypeId,
    Lightfoot_getExtraSize,
};
