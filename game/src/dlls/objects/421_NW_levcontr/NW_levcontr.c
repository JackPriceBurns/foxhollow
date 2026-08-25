#include "dlls/objects/421_NW_levcontr.h"

#include "main/gamebit_latch.h"
#include "game/objects/object.h"
#include "main/audio/music.h"
#include "main/audio/music_trigger_ids.h"
#include "main/audio/sfx.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/dll/savegame_load.h"
#include "main/frame_timing.h"
#include "main/game_timer_control.h"
#include "main/gamebit_ids.h"
#include "main/gamebits.h"
#include "main/gametext_show.h"
#include "main/mapEventTypes.h"
#include "main/model_engine.h"
#include "main/obj_trigger.h"
#include "main/objseq.h"
#include "main/render_envfx.h"
#include "main/sky.h"
#include "main/sky_interface.h"
#include "sys/objects.h"

enum NwLevelControlMode {
    NW_LEVEL_CONTROL_MODE_WAIT_START,
    NW_LEVEL_CONTROL_MODE_INIT_START,
    NW_LEVEL_CONTROL_MODE_WALK_TABLE,
    NW_LEVEL_CONTROL_MODE_WALK_STAGE_3,
    NW_LEVEL_CONTROL_MODE_WALK_STAGE_4,
    NW_LEVEL_CONTROL_MODE_WALK_STAGE_5,
    NW_LEVEL_CONTROL_MODE_WALK_STAGE_6,
    NW_LEVEL_CONTROL_MODE_WALK_STAGE_7,
    NW_LEVEL_CONTROL_MODE_WALK_FINAL,
    NW_LEVEL_CONTROL_MODE_WAIT_PARENT_SLACK,
    NW_LEVEL_CONTROL_MODE_TIMER_STEP,
    NW_LEVEL_CONTROL_MODE_CLEANUP,
    NW_LEVEL_CONTROL_MODE_RESCUE_RETRIGGER,
};

enum NwLevelControlStateFlag {
    NW_LEVEL_CONTROL_TIMER_START_PENDING = 1 << 0,
    NW_LEVEL_CONTROL_TIMER_RUNNING = 1 << 1,
    NW_LEVEL_CONTROL_TIMER_COMPLETE = 1 << 2,
    NW_LEVEL_CONTROL_WATER_MUSIC_ACTIVE = 1 << 3,
    NW_LEVEL_CONTROL_DAY_NIGHT_MUSIC_ACTIVE = 1 << 4,
    NW_LEVEL_CONTROL_TELEPORT_MUSIC_ACTIVE = 1 << 5,
    NW_LEVEL_CONTROL_SHRINE_MUSIC_ACTIVE = 1 << 6,
    NW_LEVEL_CONTROL_TIMER_MUSIC_ACTIVE = 1 << 7,
};

enum NwLevelControlTargetId {
    NW_LEVEL_CONTROL_TARGET_A = 0x447D5,
    NW_LEVEL_CONTROL_TARGET_B = 0x447D6,
};

enum NwLevelControlTriggerId {
    NW_LEVEL_CONTROL_RESCUE_RETRIGGER = 0x5A,
    NW_LEVEL_CONTROL_START_TRIGGER = 0x64A,
    NW_LEVEL_CONTROL_TARGET_TRIGGER = 0x1EE,
};

enum NwLevelControlMusicId {
    NW_LEVEL_CONTROL_WATER_MUSIC = 0x35,
    NW_LEVEL_CONTROL_TIMER_END_MUSIC = 0xAF,
};

enum NwLevelControlMapEvent {
    NW_LEVEL_CONTROL_MAP_EVENT_SLOT = 7,
    NW_LEVEL_CONTROL_ACTIVE_OBJECT_GROUP = 9,
    NW_LEVEL_CONTROL_GEYSER_OBJECT_GROUP = 0x1F,
};

enum NwLevelControlTimerId {
    NW_LEVEL_CONTROL_TIMER = 0x15,
};

enum NwLevelControlTextId {
    NW_LEVEL_CONTROL_HINT_TEXT = 0x435,
};

enum NwLevelControlEnvironmentEffect {
    NW_LEVEL_CONTROL_ENVIRONMENT_EFFECT = 0x23C,
};

typedef struct NwLevelControlSequenceEntry {
    s32 targetObjectId;
    u8 sequenceId;
    u8 nextMode;
} NwLevelControlSequenceEntry;

typedef struct NwLevelControlState {
    f32 hintCountdown;
    u8 mode;
    u8 timerMinutes;
    u8 unused06[2];
    int musicLatch;
    u8 sequenceId;
    u8 nextMode;
    u8 tableIndex;
    u8 unused0F;
    s16 dayNightMusicId;
    u8 unused12[2];
} NwLevelControlState;

STATIC_ASSERT(sizeof(NwLevelControlState) == 0x14);
STATIC_ASSERT(offsetof(NwLevelControlState, hintCountdown) == 0x00);
STATIC_ASSERT(offsetof(NwLevelControlState, mode) == 0x04);
STATIC_ASSERT(offsetof(NwLevelControlState, timerMinutes) == 0x05);
STATIC_ASSERT(offsetof(NwLevelControlState, musicLatch) == 0x08);
STATIC_ASSERT(offsetof(NwLevelControlState, sequenceId) == 0x0C);
STATIC_ASSERT(offsetof(NwLevelControlState, nextMode) == 0x0D);
STATIC_ASSERT(offsetof(NwLevelControlState, tableIndex) == 0x0E);
STATIC_ASSERT(offsetof(NwLevelControlState, dayNightMusicId) == 0x10);

static const NwLevelControlSequenceEntry sNwLevelControlSequenceEntries[] = {
    {NW_LEVEL_CONTROL_TARGET_A, 2, NW_LEVEL_CONTROL_MODE_WALK_STAGE_3},
    {NW_LEVEL_CONTROL_TARGET_B, 3, NW_LEVEL_CONTROL_MODE_WALK_STAGE_4},
    {NW_LEVEL_CONTROL_TARGET_A, 4, NW_LEVEL_CONTROL_MODE_WALK_STAGE_5},
    {NW_LEVEL_CONTROL_TARGET_B, 5, NW_LEVEL_CONTROL_MODE_WALK_STAGE_6},
    {NW_LEVEL_CONTROL_TARGET_A, 6, NW_LEVEL_CONTROL_MODE_WALK_STAGE_7},
    {NW_LEVEL_CONTROL_TARGET_B, 7, NW_LEVEL_CONTROL_MODE_WALK_FINAL},
    {NW_LEVEL_CONTROL_TARGET_A, 1, NW_LEVEL_CONTROL_MODE_CLEANUP},
};

static s16 sNwLevelControlSkyRamp180[28] = {
    180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180,
    180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180,
};

static s16 sNwLevelControlSkyRamp182[28] = {
    182, 182, 182, 182, 182, 182, 182, 182, 182, 182, 182, 182, 182, 182,
    182, 182, 182, 182, 182, 182, 182, 182, 182, 182, 182, 182, 182, 182,
};

static s16 sNwLevelControlSkyRamp181[28] = {
    181, 181, 181, 181, 181, 181, 181, 181, 181, 181, 181, 181, 181, 181,
    181, 181, 181, 181, 181, 181, 181, 181, 181, 181, 181, 181, 181, 181,
};

static s16 sNwLevelControlSkyRamp183[28] = {
    183, 183, 183, 183, 183, 183, 183, 183, 183, 183, 183, 183, 183, 183,
    183, 183, 183, 183, 183, 183, 183, 183, 183, 183, 183, 183, 183, 183,
};

static const u8 sNwLevelControlResetObjectGroups[] = {
    0, 2, 5, 10, 0x1C,
};

static int nwLevelControl_advanceSequenceTable(NwLevelControlState* state) {
    const NwLevelControlSequenceEntry* entry = &sNwLevelControlSequenceEntries[state->tableIndex];
    GameObject* target = ObjList_FindObjectById(entry->targetObjectId);

    if (ObjTrigger_IsSetById(target, NW_LEVEL_CONTROL_TARGET_TRIGGER) != 0) {
        (*gObjectTriggerInterface)->runSequence(0, target, -1);
        state->mode = NW_LEVEL_CONTROL_MODE_WAIT_PARENT_SLACK;
        state->sequenceId = entry->sequenceId;
        state->nextMode = entry->nextMode;
        state->tableIndex++;
        state->timerMinutes = 30;
        return 1;
    }

    if (state->tableIndex != 0) {
        entry = &sNwLevelControlSequenceEntries[state->tableIndex - 1];
        target = ObjList_FindObjectById(entry->targetObjectId);
        if (ObjTrigger_IsSetById(target, NW_LEVEL_CONTROL_TARGET_TRIGGER) != 0) {
            (*gObjectTriggerInterface)->runSequence(0, target, -1);
            state->mode = NW_LEVEL_CONTROL_MODE_WAIT_PARENT_SLACK;
            state->sequenceId = entry->sequenceId;
            state->timerMinutes = 0;
            return 2;
        }
    }

    return 0;
}

static int nwLevelControl_getExtraSize(void) {
    return sizeof(NwLevelControlState);
}

static void nwLevelControl_free(GameObject* obj) {
    s8 slot = obj->anim.mapEventSlot;
    int groupStatus = (*gMapEventInterface)->getObjGroupStatus(slot, 0);

    if ((u8)groupStatus == 0) {
        skySetEnvFxFlags(0);
    }
    gameTimerStop();
}

static void nwLevelControl_updateHint(NwLevelControlState* state) {
    if (state->hintCountdown <= 0.0f) {
        return;
    }

    gameTextShow(NW_LEVEL_CONTROL_HINT_TEXT);
    state->hintCountdown -= timeDelta;
    if (state->hintCountdown < 0.0f) {
        state->hintCountdown = 0.0f;
    }
}

static void nwLevelControl_updateMapState(GameObject* obj) {
    int slot = obj->anim.mapEventSlot;

    if ((*gMapEventInterface)->getMapAct(slot) != 1) {
        (*gMapEventInterface)->setMapAct(slot, 1);
    }

    if ((*gMapEventInterface)->getMapAct(NW_LEVEL_CONTROL_MAP_EVENT_SLOT) == 1) {
        static const s16 bushGameBits[] = {
            GAMEBIT_NW_MammothBush1,
            GAMEBIT_NW_MammothBush2,
            GAMEBIT_NW_MammothBush3,
            GAMEBIT_NW_MammothBush4,
        };

        (*gMapEventInterface)->setMapAct(NW_LEVEL_CONTROL_MAP_EVENT_SLOT, 2);
        for (int i = 0; i < ARRAY_COUNT(bushGameBits); i++) {
            mainSetBits(bushGameBits[i], 1);
        }
    }
}

static void nwLevelControl_updateMusic(NwLevelControlState* state) {
    if ((*gSkyInterface)->getSunPosition(0) != 0) {
        if (state->dayNightMusicId != -1) {
            state->dayNightMusicId = -1;
            if ((state->musicLatch & NW_LEVEL_CONTROL_DAY_NIGHT_MUSIC_ACTIVE) != 0) {
                Music_Trigger(MUSICTRIG_galleon_docks, 0);
            }
        }
    } else if (state->dayNightMusicId != MUSICTRIG_galleon_docks) {
        state->dayNightMusicId = MUSICTRIG_galleon_docks;
        if ((state->musicLatch & NW_LEVEL_CONTROL_DAY_NIGHT_MUSIC_ACTIVE) != 0) {
            Music_Trigger(MUSICTRIG_galleon_docks, 1);
        }
    }

    GameBitLatch_Update(&state->musicLatch, NW_LEVEL_CONTROL_WATER_MUSIC_ACTIVE, -1, -1, GAMEBIT_IM_WaterRelated03A0,
                        NW_LEVEL_CONTROL_WATER_MUSIC);
    GameBitLatch_Update(&state->musicLatch, NW_LEVEL_CONTROL_DAY_NIGHT_MUSIC_ACTIVE, -1, -1, GAMEBIT_IM_Done,
                        state->dayNightMusicId);
    GameBitLatch_Update(&state->musicLatch, NW_LEVEL_CONTROL_TELEPORT_MUSIC_ACTIVE, -1, -1, GAMEBIT_TELEPORT_MUSIC_LOCK,
                        MUSICTRIG_Teleport);
    GameBitLatch_Update(&state->musicLatch, NW_LEVEL_CONTROL_SHRINE_MUSIC_ACTIVE, -1, -1, GAMEBIT_SHRINE_MUSIC_LOCK,
                        MUSICTRIG_PU3_Adventure_c4);
}

static void nwLevelControl_updateTimerMusic(NwLevelControlState* state) {
    u32 challengeComplete = mainGetBit(GAMEBIT_SnowHornArtifact19F);
    u32 challengeStarted = mainGetBit(GAMEBIT_SnowHornArtifact19D);
    u32 timerActive = 0;

    if ((challengeStarted ^ challengeComplete) != 0 && gameTimerIsRunning() != 0) {
        timerActive = 1;
    }

    mainSetBits(GAMEBIT_TIMER_MUSIC_LOCK, timerActive);
    GameBitLatch_Update(&state->musicLatch, NW_LEVEL_CONTROL_TIMER_MUSIC_ACTIVE, -1, -1, GAMEBIT_TIMER_MUSIC_LOCK,
                        NW_LEVEL_CONTROL_TIMER_END_MUSIC);
}

static void nwLevelControl_updateGeyser(GameObject* obj) {
    int slot = obj->anim.mapEventSlot;

    if (mainGetBit(GAMEBIT_NW_GeyserComplete) != 0 &&
        (*gMapEventInterface)->getObjGroupStatus(slot, NW_LEVEL_CONTROL_GEYSER_OBJECT_GROUP) == 0) {
        (*gMapEventInterface)->setObjGroupStatus(slot, NW_LEVEL_CONTROL_GEYSER_OBJECT_GROUP, 1);
    }
}

static void nwLevelControl_updateTimerStep(NwLevelControlState* state, GameObject* obj, GameObject* player) {
    if ((player->objectFlags & OBJECT_OBJFLAG_PARENT_SLACK) != 0) {
        return;
    }

    int activeMask = state->musicLatch;
    if ((activeMask & NW_LEVEL_CONTROL_TIMER_START_PENDING) != 0) {
        state->musicLatch = activeMask & ~NW_LEVEL_CONTROL_TIMER_START_PENDING;
        state->musicLatch |= NW_LEVEL_CONTROL_TIMER_RUNNING;
        gameTimerInit(NW_LEVEL_CONTROL_TIMER, state->timerMinutes);
        timerSetToCountUp();
        (*gMapEventInterface)->savePoint(&player->anim.localPosX, player->anim.rotX, 0, 0);
    } else if ((activeMask & NW_LEVEL_CONTROL_TIMER_COMPLETE) != 0) {
        state->musicLatch = activeMask & ~NW_LEVEL_CONTROL_TIMER_RUNNING;
        state->musicLatch &= ~NW_LEVEL_CONTROL_TIMER_COMPLETE;
        gameTimerStop();
        Music_Trigger(NW_LEVEL_CONTROL_TIMER_END_MUSIC, 0);
        mainSetBits(GAMEBIT_SnowHornArtifact19F, 1);
    } else {
        int extraMinutes = (int)(gameTimerGetValue() / 60.0f);

        gameTimerStop();
        gameTimerInit(NW_LEVEL_CONTROL_TIMER, (u32)state->timerMinutes + extraMinutes);
        timerSetToCountUp();
    }

    (*gObjectTriggerInterface)->runSequence(state->sequenceId, obj, -1);
    state->mode = state->nextMode;
}

static void nwLevelControl_updateState(GameObject* obj, GameObject* player, NwLevelControlState* state) {
    switch (state->mode) {
    case NW_LEVEL_CONTROL_MODE_WAIT_START:
        if (mainGetBit(GAMEBIT_SnowHornArtifact19D) != 0) {
            (*gObjectTriggerInterface)->runSequence(0, obj, -1);
            state->mode = NW_LEVEL_CONTROL_MODE_WALK_TABLE;
            mainSetBits(GAMEBIT_NW_RescueSequenceActive, 1);
        }
        break;
    case NW_LEVEL_CONTROL_MODE_INIT_START:
        (*gObjectTriggerInterface)->preempt((uintptr_t)obj, NW_LEVEL_CONTROL_START_TRIGGER);
        (*gObjectTriggerInterface)->runSequence(0, obj, 0x20);
        state->mode = NW_LEVEL_CONTROL_MODE_WALK_TABLE;
        mainSetBits(GAMEBIT_NW_RescueSequenceActive, 1);
        break;
    case NW_LEVEL_CONTROL_MODE_WALK_TABLE:
        if (nwLevelControl_advanceSequenceTable(state) != 0) {
            state->timerMinutes = 50;
            state->musicLatch |= NW_LEVEL_CONTROL_TIMER_START_PENDING;
        }
        break;
    case NW_LEVEL_CONTROL_MODE_WALK_STAGE_3:
    case NW_LEVEL_CONTROL_MODE_WALK_STAGE_4:
    case NW_LEVEL_CONTROL_MODE_WALK_STAGE_5:
    case NW_LEVEL_CONTROL_MODE_WALK_STAGE_6:
    case NW_LEVEL_CONTROL_MODE_WALK_STAGE_7:
        nwLevelControl_advanceSequenceTable(state);
        break;
    case NW_LEVEL_CONTROL_MODE_WALK_FINAL:
        if (nwLevelControl_advanceSequenceTable(state) == 1) {
            state->musicLatch |= NW_LEVEL_CONTROL_TIMER_COMPLETE;
        }
        break;
    case NW_LEVEL_CONTROL_MODE_WAIT_PARENT_SLACK:
        if ((player->objectFlags & OBJECT_OBJFLAG_PARENT_SLACK) != 0) {
            state->mode = NW_LEVEL_CONTROL_MODE_TIMER_STEP;
        }
        break;
    case NW_LEVEL_CONTROL_MODE_TIMER_STEP:
        nwLevelControl_updateTimerStep(state, obj, player);
        break;
    case NW_LEVEL_CONTROL_MODE_CLEANUP:
        if (mainGetBit(GAMEBIT_NW_RescueSequenceActive) != 0) {
            mainSetBits(GAMEBIT_NW_RescueSequenceActive, 0);
        }
        break;
    case NW_LEVEL_CONTROL_MODE_RESCUE_RETRIGGER:
        (*gObjectTriggerInterface)->preempt((uintptr_t)obj, NW_LEVEL_CONTROL_RESCUE_RETRIGGER);
        (*gObjectTriggerInterface)->runSequence(1, obj, 8);
        state->mode = NW_LEVEL_CONTROL_MODE_CLEANUP;
        break;
    }
}

static void nwLevelControl_update(GameObject* obj) {
    NwLevelControlState* state = obj->extra;
    GameObject* player = Obj_GetPlayerObject();

    nwLevelControl_updateHint(state);
    nwLevelControl_updateMapState(obj);
    nwLevelControl_updateMusic(state);
    nwLevelControl_updateTimerMusic(state);
    nwLevelControl_updateGeyser(obj);

    if ((state->musicLatch & NW_LEVEL_CONTROL_TIMER_RUNNING) != 0 && isGameTimerDisabled() != 0) {
        Sfx_PlayFromObject(NULL, SFXTRIG_sc_lockon22);
        (*gMapEventInterface)->gotoRestartPoint();
        return;
    }

    nwLevelControl_updateState(obj, player, state);
}

static void nwLevelControl_init(GameObject* obj) {
    NwLevelControlState* state = obj->extra;

    Obj_GetPlayerObject();
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN | OBJECT_OBJFLAG_HITDETECT_DISABLED;

    if (mainGetBit(GAMEBIT_SnowHornArtifact19F) != 0) {
        state->mode = NW_LEVEL_CONTROL_MODE_RESCUE_RETRIGGER;
    } else if (mainGetBit(GAMEBIT_SnowHornArtifact19D) != 0) {
        state->mode = NW_LEVEL_CONTROL_MODE_INIT_START;
    } else {
        state->mode = NW_LEVEL_CONTROL_MODE_WAIT_START;
    }

    state->hintCountdown = 300.0f;
    skySetEnvFxRampTables(sNwLevelControlSkyRamp182, sNwLevelControlSkyRamp180, sNwLevelControlSkyRamp181,
                          sNwLevelControlSkyRamp183);

    if (getSaveGameLoadStatus() != 0) {
        skySetEnvFxFlags(0x3F);
        getEnvfxActImmediately(0, 0, NW_LEVEL_CONTROL_ENVIRONMENT_EFFECT, 0);
    } else {
        skySetEnvFxFlags(0x1F);
        getEnvfxAct(0, 0, NW_LEVEL_CONTROL_ENVIRONMENT_EFFECT, 0);
    }

    for (int i = 0; i < ARRAY_COUNT(sNwLevelControlResetObjectGroups); i++) {
        (*gMapEventInterface)
            ->setObjGroupStatus(NW_LEVEL_CONTROL_MAP_EVENT_SLOT, sNwLevelControlResetObjectGroups[i], 0);
    }
    (*gMapEventInterface)->setObjGroupStatus(NW_LEVEL_CONTROL_MAP_EVENT_SLOT, NW_LEVEL_CONTROL_ACTIVE_OBJECT_GROUP, 1);
}

OBJECT_INIT_ADAPTER(gNWLevelControlObjDescriptorInitAdapter, nwLevelControl_init, obj)
OBJECT_FREE_ADAPTER(gNWLevelControlObjDescriptorFreeAdapter, nwLevelControl_free, obj)
OBJECT_EXTRA_SIZE_ADAPTER(gNWLevelControlObjDescriptorExtraSizeAdapter, nwLevelControl_getExtraSize)

ObjectDescriptor gNWLevelControlObjDescriptor = {
    .header = {
        .metadata = { 0, 0, 0, OBJECT_DESCRIPTOR_FLAGS_10_SLOTS },
        .acquire = NULL,
        .release = NULL,
    },
    .slot02 = NULL,
    .init = gNWLevelControlObjDescriptorInitAdapter,
    .update = nwLevelControl_update,
    .hitDetect = NULL,
    .render = NULL,
    .free = gNWLevelControlObjDescriptorFreeAdapter,
    .getObjectTypeId = NULL,
    .getExtraSize = gNWLevelControlObjDescriptorExtraSizeAdapter,
};;
