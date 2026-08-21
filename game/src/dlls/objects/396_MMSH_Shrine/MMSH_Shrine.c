/*
 * MMSH_Shrine (DLL 0x18C) - Moon Mountain Pass Test of Fear shrine.
 *
 * The shrine bobs and turns toward the player, interprets animation commands, and
 * drives the fear-test sway meter and its object-trigger sequences.
 */
#include "dlls/objects/396_MMSH_Shrine.h"

#include "dlls/objects/430_SH_LevelCon.h"
#include "dolphin/MSL_C/PPCEABI/bare/H/math_trig_api.h"
#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/audio/audio_control_api.h"
#include "main/audio/music_api.h"
#include "main/audio/music_trigger_ids.h"
#include "main/audio/sfx_play_api.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/dll/dll_0000_gameui.h"
#include "main/dll/objfx_api.h"
#include "main/dll/player_api.h"
#include "main/dll/tricky_api.h"
#include "main/frame_timing.h"
#include "main/gamebit_ids.h"
#include "main/gamebits_api.h"
#include "main/map_load.h"
#include "main/mapEventTypes.h"
#include "main/model_light.h"
#include "main/object_render.h"
#include "main/objanim.h"
#include "main/objseq.h"
#include "main/pad.h"
#include "main/pi_dolphin_api.h"
#include "main/render_envfx_api.h"
#include "main/sky_api.h"
#include "main/vecmath.h"
#include "main/vecmath_distance_api.h"
#include "sys/objects.h"

enum MmshShrineStateFlag {
    MMSH_SHRINE_STATE_SEQUENCE_READY = 0x01,
    MMSH_SHRINE_STATE_SWAY_ACTIVE = 0x02,
    MMSH_SHRINE_STATE_MUSIC_LATCH_AMBIENT = 0x04,
    MMSH_SHRINE_STATE_MUSIC_LATCH_TEST = 0x08,
    MMSH_SHRINE_STATE_MUSIC_LATCH_SHRINE = 0x10,
    MMSH_SHRINE_STATE_FEAR_METER_ACTIVE = 0x20,
};

enum MmshShrineSequence {
    MMSH_SHRINE_SEQUENCE_ACTIVATE,
    MMSH_SHRINE_SEQUENCE_PLAYER_INACTIVE,
    MMSH_SHRINE_SEQUENCE_READY,
    MMSH_SHRINE_SEQUENCE_SWAY_LIMIT,
};

enum MmshShrinePhase {
    MMSH_SHRINE_PHASE_IDLE = 0,
    MMSH_SHRINE_PHASE_WAIT_FOR_SEQUENCE = 1,
    MMSH_SHRINE_PHASE_WAIT_FOR_PLAYER = 2,
    MMSH_SHRINE_PHASE_SWAY_LIMIT = 3,
    MMSH_SHRINE_PHASE_SET_COMPLETE = 4,
    MMSH_SHRINE_PHASE_RESET = 5,
};

enum MmshShrineAnimCommand {
    MMSH_SHRINE_ANIM_COMMAND_ENABLE_SWAY = 1,
    MMSH_SHRINE_ANIM_COMMAND_DISABLE_SWAY = 2,
    MMSH_SHRINE_ANIM_COMMAND_TARGET_LEFT = 3,
    MMSH_SHRINE_ANIM_COMMAND_TARGET_RIGHT = 4,
    MMSH_SHRINE_ANIM_COMMAND_REVERSE_TARGET = 5,
    MMSH_SHRINE_ANIM_COMMAND_DOUBLE_TARGET = 6,
    MMSH_SHRINE_ANIM_COMMAND_GRANT_SPIRIT = 7,
    MMSH_SHRINE_ANIM_COMMAND_HALVE_TARGET = 8,
    MMSH_SHRINE_ANIM_COMMAND_HIDE_MODEL = 0xE,
    MMSH_SHRINE_ANIM_COMMAND_SHOW_MODEL = 0xF,
};

typedef struct MmshShrinePlacement {
    ObjPlacement base;
    u8 pad18[2];
    s16 initialValue;
    u8 pad1C[8];
} MmshShrinePlacement;

STATIC_ASSERT(sizeof(MmshShrinePlacement) == 0x24);
STATIC_ASSERT(offsetof(MmshShrinePlacement, initialValue) == 0x1A);

typedef struct MmshShrineState {
    ModelLightStruct* light;
    f32 swayPhase;
    f32 stickVelocity;
    f32 targetVelocity;
    f32 swayTarget;
    f32 idleSfxTimer;
    GameBitLatchState latch;
    s16 initialValue;
    s16 orbitPhaseA;
    s16 orbitPhaseB;
    s16 orbitPhaseC;
    u8 phase;
} MmshShrineState;

STATIC_ASSERT(sizeof(MmshShrineState) == 0x30);
STATIC_ASSERT(offsetof(MmshShrineState, swayPhase) == 0x08);
STATIC_ASSERT(offsetof(MmshShrineState, latch) == 0x1C);
STATIC_ASSERT(offsetof(MmshShrineState, initialValue) == 0x20);
STATIC_ASSERT(offsetof(MmshShrineState, orbitPhaseA) == 0x22);
STATIC_ASSERT(offsetof(MmshShrineState, phase) == 0x28);

static void mmshShrine_updateHoverMotion(GameObject* obj) {
    const MmshShrinePlacement* placement;
    MmshShrineState* state;
    GameObject* player;
    f32 trigA;
    f32 trigB;
    s32 angleDelta;
    f32 distance;
    ObjAnimEventList animEvents;

    placement = (const MmshShrinePlacement*)obj->anim.placementData;
    state = obj->extra;
    player = Obj_GetPlayerObject();

    if ((obj->anim.flags & OBJANIM_FLAG_HIDDEN) != 0) {
        obj->anim.rotX = 0;
        obj->anim.localPosY = placement->base.posY;
        return;
    }

    state->orbitPhaseA += (int)(512.0f * timeDelta);
    state->orbitPhaseB += (int)(128.0f * timeDelta);
    state->orbitPhaseC += (int)(192.0f * timeDelta);

    obj->anim.localPosY =
        20.0f + (placement->base.posY + mathSinf((3.1415927f * state->orbitPhaseA) / 32768.0f));

    trigA = mathSinf((3.1415927f * state->orbitPhaseB) / 32768.0f);
    trigB = mathSinf((3.1415927f * state->orbitPhaseA) / 32768.0f);
    trigB = trigB + trigA;
    obj->anim.rotZ = (s16)(600.0f * trigB);

    trigA = mathSinf((3.1415927f * state->orbitPhaseC) / 32768.0f);
    trigB = mathSinf((3.1415927f * state->orbitPhaseA) / 32768.0f);
    trigB = trigB + trigA;
    obj->anim.rotY = (s16)(600.0f * trigB);

    ObjAnim_AdvanceCurrentMove(obj, 0.005f, timeDelta, &animEvents);
    if (player == NULL) {
        return;
    }

    {
        f32 dx = obj->anim.worldPosX - player->anim.worldPosX;
        f32 dz = obj->anim.worldPosZ - player->anim.worldPosZ;
        int targetAngle = (u16)getAngle(dx, dz);

        angleDelta = targetAngle - (int)(u16)obj->anim.rotX;
        if (angleDelta > 0x8000) {
            angleDelta -= 0xFFFF;
        }
        if (angleDelta < -0x8000) {
            angleDelta += 0xFFFF;
        }
        obj->anim.rotX = (s16)(obj->anim.rotX + (int)(((f32)angleDelta * timeDelta) / 12.0f));
    }
    distance = Vec_xzDistance(&obj->anim.worldPosX, &player->anim.worldPosX);
    if (distance <= 30.0f) {
        obj->anim.alpha = (u8)(int)(255.0f * (distance / 30.0f));
    } else {
        obj->anim.alpha = 0xFF;
    }
}

static int mmshShrine_updateFearSway(GameObject* obj) {
    MmshShrineState* state;
    f32 stickAccel;
    f32 target;
    int swayValue;

    state = obj->extra;
    if ((state->latch.activeMask & MMSH_SHRINE_STATE_FEAR_METER_ACTIVE) == 0) {
        fearTestMeterSetFadeIn(1);
        state->latch.activeMask |= MMSH_SHRINE_STATE_FEAR_METER_ACTIVE;
        state->swayPhase = 0.0f;
        state->stickVelocity = 0.0f;
        state->targetVelocity = 0.0f;
    }

    stickAccel = (f32)padGetStickX(0) / 72.0f;
    stickAccel *= 0.0010416667209938169f;
    state->stickVelocity += stickAccel * timeDelta;

    target = state->swayTarget;
    if (target < 0.0f && state->targetVelocity > target) {
        state->targetVelocity -= 0.0010416667209938169f * timeDelta;
    } else if (target > 0.0f) {
        if (state->targetVelocity < target) {
            state->targetVelocity += 0.0010416667209938169f * timeDelta;
        }
    }

    state->swayPhase += timeDelta * (state->stickVelocity + state->targetVelocity);
    swayValue = (int)(96.0f * state->swayPhase);
    fearTestMeterSetRange(0x60, 0x39, (s16)swayValue);
    if (swayValue > 0x39 || swayValue < -0x39) {
        return 1;
    }
    return 0;
}

static void mmshShrine_clearFearTestBits(void) {
    mainSetBits(GAMEBIT_MMSH_FearTestRelated0E82, 0);
    mainSetBits(GAMEBIT_MMSH_FearTestRelated0E83, 0);
    mainSetBits(GAMEBIT_MMSH_FearTestRelated0E84, 0);
    mainSetBits(GAMEBIT_MMSH_FearTestRelated0E85, 0);
}

static int mmshShrine_processAnimEvents(GameObject* obj, int unusedArg, ObjSeqState* animUpdate) {
    MmshShrineState* state;
    u8 command;
    GameObject* player;

    state = obj->extra;
    player = Obj_GetPlayerObject();
    animUpdate->savedFlags = -1;
    animUpdate->movementState = 0;

    for (int i = 0; i < animUpdate->eventCount; i++) {
        command = animUpdate->eventIds[i];
        if (command != 0) {
            switch (command) {
            case MMSH_SHRINE_ANIM_COMMAND_GRANT_SPIRIT:
                objSetAnimStateFlags(player, 4, 1);
                mainSetBits(GAMEBIT_MMSH_SpiritGrantTriggered, 1);
                mainSetBits(GAMEBIT_ITEM_SpiritTestFear_Got, 1);
                (*gMapEventInterface)->setMapAct(0xB, 3);
                break;
            case MMSH_SHRINE_ANIM_COMMAND_HIDE_MODEL:
                obj->anim.flags |= OBJANIM_FLAG_HIDDEN;
                if (state->light != NULL) {
                    modelLightStruct_setEnabled(state->light, 0, 1.0f);
                }
                break;
            case MMSH_SHRINE_ANIM_COMMAND_SHOW_MODEL:
                obj->anim.flags &= ~OBJANIM_FLAG_HIDDEN;
                if (state->light != NULL) {
                    modelLightStruct_setEnabled(state->light, 0, 1.0f);
                }
                break;
            case MMSH_SHRINE_ANIM_COMMAND_ENABLE_SWAY:
                state->latch.activeMask |= MMSH_SHRINE_STATE_SWAY_ACTIVE;
                break;
            case MMSH_SHRINE_ANIM_COMMAND_DISABLE_SWAY:
                state->latch.activeMask &= ~MMSH_SHRINE_STATE_SWAY_ACTIVE;
                if ((state->latch.activeMask & MMSH_SHRINE_STATE_FEAR_METER_ACTIVE) != 0) {
                    fearTestMeterSetFadeIn(0);
                    state->latch.activeMask &= ~MMSH_SHRINE_STATE_FEAR_METER_ACTIVE;
                }
                break;
            case MMSH_SHRINE_ANIM_COMMAND_TARGET_LEFT:
                state->swayTarget = -0.0026041667442768812f;
                break;
            case MMSH_SHRINE_ANIM_COMMAND_TARGET_RIGHT:
                state->swayTarget = 0.0026041667442768812f;
                break;
            case MMSH_SHRINE_ANIM_COMMAND_REVERSE_TARGET:
                state->swayTarget = -state->swayTarget;
                state->targetVelocity = -state->swayTarget;
                break;
            case MMSH_SHRINE_ANIM_COMMAND_DOUBLE_TARGET:
                state->swayTarget *= 2.0f;
                break;
            case MMSH_SHRINE_ANIM_COMMAND_HALVE_TARGET:
                state->swayTarget *= 0.5f;
                break;
            }
        }
        animUpdate->eventIds[i] = 0;
    }

    if (((state->latch.activeMask & MMSH_SHRINE_STATE_SWAY_ACTIVE) != 0) &&
        ((u8)mmshShrine_updateFearSway(obj) != 0)) {
        fearTestMeterSetFadeIn(0);
        state->latch.activeMask &= ~(MMSH_SHRINE_STATE_SWAY_ACTIVE | MMSH_SHRINE_STATE_FEAR_METER_ACTIVE);
        state->phase = MMSH_SHRINE_PHASE_SWAY_LIMIT;
        mmshShrine_clearFearTestBits();
        return 4;
    }
    state->latch.activeMask |= MMSH_SHRINE_STATE_SEQUENCE_READY;
    return 0;
}

static int mmshShrine_getExtraSize(void) {
    return sizeof(MmshShrineState);
}

static int mmshShrine_getObjectTypeId(void) {
    return 0;
}

static void mmshShrine_free(GameObject* obj) {
    MmshShrineState* state = obj->extra;

    if ((state->latch.activeMask & MMSH_SHRINE_STATE_FEAR_METER_ACTIVE) != 0) {
        fearTestMeterSetFadeIn(0);
        state->latch.activeMask &= ~MMSH_SHRINE_STATE_FEAR_METER_ACTIVE;
    }
    if (state->light != NULL) {
        ModelLightStruct_free(state->light);
        state->light = NULL;
    }
    Music_Trigger(MUSICTRIG_DIM_Snow, 0);
    Music_Trigger(MUSICTRIG_CC_Visit1, 0);
    Music_Trigger(MUSICTRIG_vfp_walkabout, 0);
    Music_Trigger(0xA, 0);
    mainSetBits(GAMEBIT_IN_KRAZOA_SHRINE, 0);
    mainSetBits(GAMEBIT_SHRINE_MUSIC_LOCK, 1);
    mmshShrine_clearFearTestBits();
}

static void mmshShrine_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                              s8 visible) {
    MmshShrineState* state = obj->extra;

    if (visible == 0) {
        if (state->light != NULL) {
            modelLightStruct_setEnabled(state->light, 0, 1.0f);
        }
    } else {
        if (state->light != NULL) {
            modelLightStruct_setEnabled(state->light, 1, 1.0f);
        }
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
        objDoParticleFx(obj, 1.0f, 7, 1.0f, state->light);
    }
}

static void mmshShrine_hitDetect(void) {
}

static void mmshShrine_update(GameObject* obj) {
    MmshShrineState* state;
    GameObject* player;

    state = obj->extra;
    player = Obj_GetPlayerObject();

    if (obj->userData1 != 0) {
        obj->userData1--;
        if (obj->userData1 == 0) {
            skySetSlotFlag80(7, 1);
            getEnvfxAct(obj, player, 0x20D, 0);
            getEnvfxAct(obj, player, 0x20E, 0);
            getEnvfxAct(obj, player, 0x222, 0);
            obj->anim.worldPos = obj->anim.localPos;
        }
    }
    unlockLevel(mapGetDirIdx(0x20), 1, 0);
    mmshShrine_updateHoverMotion(obj);
    GameBitLatch_Update(&state->latch, MMSH_SHRINE_STATE_MUSIC_LATCH_TEST, -1, -1,
                        GAMEBIT_MMSH_TestMusicActive, 0xA);
    GameBitLatch_UpdateInverted(&state->latch, MMSH_SHRINE_STATE_MUSIC_LATCH_AMBIENT, -1, -1,
                                GAMEBIT_SHRINE_MUSIC_LOCK, MUSICTRIG_vfp_walkabout);
    GameBitLatch_Update(&state->latch, MMSH_SHRINE_STATE_MUSIC_LATCH_SHRINE, -1, -1,
                        GAMEBIT_SHRINE_MUSIC_LOCK, MUSICTRIG_PU3_Adventure_c4);

    switch (state->phase) {
    case MMSH_SHRINE_PHASE_IDLE: {
        f32 idleSfxTimer = state->idleSfxTimer - timeDelta;

        state->idleSfxTimer = idleSfxTimer;
        if (idleSfxTimer <= 0.0f) {
            Sfx_PlayFromObject(obj, SFXTRIG_spirit_voice);
            state->idleSfxTimer = (f32)(s32)randomGetRange(500, 1000);
        }
    }
        if ((obj->anim.resetHitboxFlags & INTERACT_FLAG_ACTIVATED) == 0) {
            break;
        }
        state->phase = MMSH_SHRINE_PHASE_WAIT_FOR_SEQUENCE;
        (*gObjectTriggerInterface)->setCamVars(0x4C, 0, 0, 0);
        (*gObjectTriggerInterface)->runSequence(MMSH_SHRINE_SEQUENCE_ACTIVATE, obj, -1);
        Music_Trigger(MUSICTRIG_DIM_Snow, 1);
        break;
    case MMSH_SHRINE_PHASE_WAIT_FOR_SEQUENCE:
        if ((state->latch.activeMask & MMSH_SHRINE_STATE_SEQUENCE_READY) == 0) {
            break;
        }
        obj->anim.flags |= OBJANIM_FLAG_HIDDEN;
        obj->anim.rotX = 0;
        state->phase = MMSH_SHRINE_PHASE_WAIT_FOR_PLAYER;
        state->latch.activeMask &= ~MMSH_SHRINE_STATE_SEQUENCE_READY;
        mainSetBits(GAMEBIT_MMSH_TestMusicActive, 1);
        (*gObjectTriggerInterface)->runSequence(MMSH_SHRINE_SEQUENCE_READY, obj, -1);
        break;
    case MMSH_SHRINE_PHASE_SWAY_LIMIT:
        (*gObjectTriggerInterface)->endSequence(obj->seqIndex);
        (*gObjectTriggerInterface)->runSequence(MMSH_SHRINE_SEQUENCE_SWAY_LIMIT, obj, -1);
        state->phase = MMSH_SHRINE_PHASE_SET_COMPLETE;
        mainSetBits(GAMEBIT_MMSH_TestMusicActive, 0);
        break;
    case MMSH_SHRINE_PHASE_SET_COMPLETE:
        state->phase = MMSH_SHRINE_PHASE_RESET;
        mainSetBits(GAMEBIT_MMSH_TestMusicActive, 0);
        mainSetBits(GAMEBIT_MMSH_ShrineRelated0AE4, 1);
        break;
    case MMSH_SHRINE_PHASE_WAIT_FOR_PLAYER:
        if (objGetAnimStateFlags(player, 4) == 0) {
            audioStopByMask(3);
            (*gObjectTriggerInterface)->runSequence(MMSH_SHRINE_SEQUENCE_PLAYER_INACTIVE, obj, -1);
        }
        state->phase = MMSH_SHRINE_PHASE_RESET;
        mainSetBits(GAMEBIT_MMSH_TestMusicActive, 0);
        break;
    case MMSH_SHRINE_PHASE_RESET:
        state->phase = MMSH_SHRINE_PHASE_IDLE;
        state->latch.activeMask &= ~MMSH_SHRINE_STATE_SEQUENCE_READY;
        obj->anim.flags &= ~OBJANIM_FLAG_HIDDEN;
        mainSetBits(GAMEBIT_ShrineRelated012B, 0);
        mainSetBits(GAMEBIT_MMSH_ShrineRelated0AE4, 0);
        mainSetBits(GAMEBIT_MMSH_ShrineRelated0AE5, 0);
        mainSetBits(GAMEBIT_MMSH_TestMusicActive, 0);
        break;
    }
}

static void mmshShrine_init(GameObject* obj, const MmshShrinePlacement* placement) {
    MmshShrineState* state;
    s16 initialValue;

    state = obj->extra;
    obj->anim.rotX = 0;
    obj->animEventCallback = mmshShrine_processAnimEvents;
    state->initialValue = 10;
    state->phase = MMSH_SHRINE_PHASE_IDLE;
    initialValue = ObjAnim_ReadPlacementS16(&obj->anim, &placement->initialValue);
    if (initialValue > 0) {
        state->initialValue = initialValue >> 8;
    }
    mainSetBits(GAMEBIT_ShrineRelated012B, 0);
    mainSetBits(GAMEBIT_MMSH_ShrineRelated012D, 0);
    obj->userData1 = 1;
    if (state->light == NULL) {
        state->light = objCreateLight(NULL, 1);
    }
    mainSetBits(GAMEBIT_LV_LocatedKrazoaShrine, 1);
    mainSetBits(GAMEBIT_IN_KRAZOA_SHRINE, 1);
}

static void mmshShrine_release(void) {
}

static void mmshShrine_initialise(void) {
}

ObjectDescriptor gMMSHShrineObjDescriptor = {
    .slotCountAndFlags = OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    .initialise = (ObjectDescriptorCallback)mmshShrine_initialise,
    .release = (ObjectDescriptorCallback)mmshShrine_release,
    .init = (ObjectDescriptorCallback)mmshShrine_init,
    .update = (ObjectDescriptorCallback)mmshShrine_update,
    .hitDetect = (ObjectDescriptorCallback)mmshShrine_hitDetect,
    .render = (ObjectDescriptorCallback)mmshShrine_render,
    .free = (ObjectDescriptorCallback)mmshShrine_free,
    .getObjectTypeId = (ObjectDescriptorCallback)mmshShrine_getObjectTypeId,
    .getExtraSize = mmshShrine_getExtraSize,
};
