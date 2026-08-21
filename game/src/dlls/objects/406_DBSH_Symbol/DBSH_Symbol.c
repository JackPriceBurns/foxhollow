/*
 * DBSH_Symbol (DLL 0x196) - Dark Ice Mines shrine spinning symbol.
 *
 * Trigger sequence 0 lets the player rotate this symbol and its paired
 * symbol. The shrine's shared game bits report whether the spin completed.
 */
#include "dlls/objects/406_DBSH_Symbol.h"

#include "dolphin/pad.h"
#include "game/objects/object.h"
#include "main/audio/sfx_keep_alive_api.h"
#include "main/audio/sfx_object_volume_api.h"
#include "main/audio/sfx_play_api.h"
#include "main/audio/sfx_stop_channel_api.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/frame_timing.h"
#include "main/game_timer_control_api.h"
#include "main/gamebit_ids.h"
#include "main/gamebits_api.h"
#include "main/object_render.h"
#include "main/obj_list.h"
#include "main/objseq.h"
#include "main/pad.h"
#include "main/vecmath.h"
#include "sys/objects.h"

typedef struct DBSHSymbolState {
    GameObject* partnerSymbol;
    f32 spinSpeed;
    f32 objectSfxTimer;
    f32 playerSfxTimer;
    s32 spinProgress;
    s32 previousSpinProgress;
    s32 sequenceHandle;
    u8 unk20[2];
    s16 phase;
    u8 flags;
    u8 unk25[3];
} DBSHSymbolState;

typedef enum DBSHSymbolPhase {
    DBSH_SYMBOL_PHASE_HIDE = 0,
    DBSH_SYMBOL_PHASE_PLAY_SCUFF = 1,
    DBSH_SYMBOL_PHASE_START_SEQUENCE = 2,
    DBSH_SYMBOL_PHASE_RESOLVE = 3,
} DBSHSymbolPhase;

typedef enum DBSHSymbolFlags {
    DBSH_SYMBOL_SPIN_COMPLETED = 1 << 0,
    DBSH_SYMBOL_SEQUENCE_INACTIVE = 1 << 1,
} DBSHSymbolFlags;

typedef enum DBSHSymbolAnimEvent {
    DBSH_SYMBOL_ANIM_EVENT_START = 1,
} DBSHSymbolAnimEvent;

STATIC_ASSERT(sizeof(DBSHSymbolState) == 0x28);
STATIC_ASSERT(offsetof(DBSHSymbolState, partnerSymbol) == 0x00);
STATIC_ASSERT(offsetof(DBSHSymbolState, spinSpeed) == 0x08);
STATIC_ASSERT(offsetof(DBSHSymbolState, spinProgress) == 0x14);
STATIC_ASSERT(offsetof(DBSHSymbolState, sequenceHandle) == 0x1C);
STATIC_ASSERT(offsetof(DBSHSymbolState, phase) == 0x22);
STATIC_ASSERT(offsetof(DBSHSymbolState, flags) == 0x24);

static u8 gDBSHSymbolScuffSfxEnabled = 1;

static int dbshSymbol_processAnimEvents(GameObject* obj, int unused, ObjSeqState* animUpdate) {
    int volume;
    GameObject** objectList;
    int objectIndex;
    int objectCount;
    int i;
    int buttons;
    DBSHSymbolState* state;
    GameObject* player;

    (void)unused;
    state = obj->extra;
    player = Obj_GetPlayerObject();
    Sfx_SetObjectSfxVolume(obj, SFXTRIG_blockscrape_lp, 10, 127.0f);
    Sfx_KeepAliveLoopedObjectSound(obj, SFXTRIG_blockscrape_lp);
    animUpdate->movementState = 0;
    for (i = 0; i < animUpdate->eventCount; i++) {
        if (animUpdate->eventIds[i] == DBSH_SYMBOL_ANIM_EVENT_START) {
            gameTimerInit(0x1D, 0x3C);
            timerSetToCountUp();
            state->flags &= ~DBSH_SYMBOL_SEQUENCE_INACTIVE;
            obj->anim.modelState->flags |= OBJ_MODEL_STATE_SHADOW_VISIBLE;
        }
    }
    if ((state->flags & DBSH_SYMBOL_SEQUENCE_INACTIVE) != 0) {
        return 0;
    }
    if (state->partnerSymbol == NULL) {
        objectList = ObjList_GetObjects(&objectIndex, &objectCount);
        while (objectIndex < objectCount) {
            state->partnerSymbol = objectList[objectIndex];
            if (state->partnerSymbol->anim.romDefNo == 0x20F) {
                break;
            }
            objectIndex++;
        }
    }
    if (state->partnerSymbol == NULL) {
        return 0;
    }
    for (i = 0; i < framesThisStep; i++) {
        if (isGameTimerDisabled() != 0) {
            Sfx_PlayFromObject(obj, SFXTRIG_wp_iceywindlp16);
            state->flags &= ~DBSH_SYMBOL_SPIN_COMPLETED;
            state->flags |= DBSH_SYMBOL_SEQUENCE_INACTIVE;
            (*gObjectTriggerInterface)->yield(animUpdate, 0xBD);
        }
        buttons = getButtonsJustPressedIfNotBusy(0);
        if ((buttons & PAD_BUTTON_A) != 0) {
            state->spinSpeed += 14.8f;
        }
        if (state->spinSpeed > 80.0f) {
            state->spinSpeed = 80.0f;
        }
        state->spinProgress = (int)((f32)state->spinProgress + state->spinSpeed);
        if (state->spinProgress >= 0x7EF4) {
            gameTimerStop();
            Sfx_PlayFromObject(obj, SFXTRIG_wp_iceywindlp16);
            ObjAnim_SetCurrentMove(player, 0, 0.0f, 0);
            state->flags |= DBSH_SYMBOL_SPIN_COMPLETED | DBSH_SYMBOL_SEQUENCE_INACTIVE;
            state->spinProgress = 0x7EF4;
            (*gObjectTriggerInterface)->yield(animUpdate, 0xBD);
            return 0;
        }
        (*gObjectTriggerInterface)->setXrot(state->sequenceHandle, state->spinProgress);
        if (state->spinProgress < 0) {
            state->spinProgress = 0;
            if (state->spinSpeed < 0.0f) {
                state->spinSpeed = 0.0f;
            }
            state->previousSpinProgress = state->spinProgress;
            if (state->spinSpeed > -300.0f) {
                state->spinSpeed -= 10.1f;
            }
            return 0;
        }
        if (state->spinSpeed > -80.0f) {
            state->spinSpeed -= 1.6f;
        }
        if (ObjAnim_AdvanceCurrentMove(
                player, ((f32)state->spinProgress - state->previousSpinProgress) / 7500.0f, timeDelta, NULL) != 0) {
            if (player->anim.currentMoveProgress < 0.0f) {
                player->anim.currentMoveProgress =
                    1.0f + player->anim.currentMoveProgress;
            }
        }
        if (state->partnerSymbol != NULL) {
            if (ObjAnim_AdvanceCurrentMove(state->partnerSymbol,
                                           -((f32)state->spinProgress - state->previousSpinProgress) / 7500.0f,
                                           timeDelta, NULL) != 0) {
                f32 partnerProgress = state->partnerSymbol->anim.currentMoveProgress;
                if (partnerProgress < 0.0f) {
                    state->partnerSymbol->anim.currentMoveProgress = 1.0f + partnerProgress;
                }
            }
        }
        state->previousSpinProgress = state->spinProgress;
    }
    state->playerSfxTimer -= timeDelta;
    if (state->playerSfxTimer < 0.0f) {
        if (state->spinSpeed < 0.0f) {
            state->playerSfxTimer =
                (f32)randomGetRange(0x28, 0x64);
        } else {
            state->playerSfxTimer =
                (f32)randomGetRange(0x78, 0xF0);
        }
        Sfx_PlayFromObject(player, SFXTRIG_literun116_var);
    }
    state->objectSfxTimer -= timeDelta;
    if (state->objectSfxTimer < 0.0f) {
        if (state->spinSpeed > 0.0f) {
            state->objectSfxTimer =
                (f32)randomGetRange(0x28, 0x64);
        } else {
            state->objectSfxTimer =
                (f32)randomGetRange(0x78, 0xF0);
        }
        Sfx_PlayFromObject(obj, SFXTRIG_spotfox03);
    }
    {
        f32 absoluteSpeed =
            (4.0f * state->spinSpeed >= 0.0f) ? 4.0f * state->spinSpeed : -(4.0f * state->spinSpeed);

        volume = (int)absoluteSpeed;
        if (volume > 100) {
            volume = 100;
        }
        Sfx_SetObjectSfxVolume(obj, SFXTRIG_blockscrape_lp, volume, 127.0f);
    }
    return 0;
}

static int dbshSymbol_getExtraSize(void) {
    return sizeof(DBSHSymbolState);
}

static void dbshSymbol_free(void) {
    gameTimerStop();
}

static void dbshSymbol_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                              s8 visible) {
    (void)visible;
    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
}

static void dbshSymbol_update(GameObject* obj) {
    DBSHSymbolState* state = obj->extra;
    u32 symbolsRaised = mainGetBit(GAMEBIT_DBSH_SymbolRiseComplete);

    if (symbolsRaised == 0) {
        state->phase = DBSH_SYMBOL_PHASE_HIDE;
        state->partnerSymbol = NULL;
        mainSetBits(GAMEBIT_DBSH_SymbolSpinFailed, 0);
    } else {
        s16 phase = state->phase;

        if (phase == DBSH_SYMBOL_PHASE_HIDE) {
            obj->anim.modelState->flags &= ~(u64)OBJ_MODEL_STATE_SHADOW_VISIBLE;
            state->phase = DBSH_SYMBOL_PHASE_PLAY_SCUFF;
        } else if (phase == DBSH_SYMBOL_PHASE_START_SEQUENCE) {
            state->phase = DBSH_SYMBOL_PHASE_RESOLVE;
            state->sequenceHandle = (*gObjectTriggerInterface)->runSequence(0, obj, -1);
        } else if (phase == DBSH_SYMBOL_PHASE_PLAY_SCUFF) {
            if (gDBSHSymbolScuffSfxEnabled != 0) {
                gDBSHSymbolScuffSfxEnabled = 0;
                Sfx_PlayFromObject(obj, SFXTRIG_wp_iceywindlp16);
            }
            state->phase = DBSH_SYMBOL_PHASE_START_SEQUENCE;
            gDBSHSymbolScuffSfxEnabled = 1;
        } else if (phase == DBSH_SYMBOL_PHASE_RESOLVE) {
            obj->anim.modelState->flags &= ~(u64)OBJ_MODEL_STATE_SHADOW_VISIBLE;
            if ((state->flags & DBSH_SYMBOL_SPIN_COMPLETED) != 0) {
                mainSetBits(GAMEBIT_DBSH_SymbolSpinSucceeded, 1);
            } else {
                mainSetBits(GAMEBIT_DBSH_SymbolSpinFailed, 1);
            }
            Sfx_StopObjectChannel(obj, 0x7F);
            state->flags |= DBSH_SYMBOL_SEQUENCE_INACTIVE;
        }
    }
}

static void dbshSymbol_init(GameObject* obj) {
    DBSHSymbolState* state = obj->extra;

    state->spinSpeed = 0.0f;
    state->spinProgress = 0;
    state->previousSpinProgress = 0;
    state->phase = DBSH_SYMBOL_PHASE_HIDE;
    state->partnerSymbol = NULL;
    state->flags &= ~DBSH_SYMBOL_SPIN_COMPLETED;
    state->flags |= DBSH_SYMBOL_SEQUENCE_INACTIVE;

    obj->anim.localPosY -= 50.0f;
    obj->animEventCallback = dbshSymbol_processAnimEvents;

    obj->anim.modelState->flags &= ~(u64)OBJ_MODEL_STATE_SHADOW_VISIBLE;
}

ObjectDescriptor gDBSHSymbolObjDescriptor = {
    .slotCountAndFlags = OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    .init = (ObjectDescriptorCallback)dbshSymbol_init,
    .update = (ObjectDescriptorCallback)dbshSymbol_update,
    .render = (ObjectDescriptorCallback)dbshSymbol_render,
    .free = (ObjectDescriptorCallback)dbshSymbol_free,
    .getExtraSize = dbshSymbol_getExtraSize,
};
