#include "dlls/objects/405_DBSH_Shrine.h"

#include "main/gamebit_latch.h"
#include "dolphin/math.h"
#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/audio/audio_control.h"
#include "main/audio/music.h"
#include "main/audio/music_trigger_ids.h"
#include "main/audio/sfx.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/dll/objfx.h"
#include "main/dll/player.h"
#include "main/frame_timing.h"
#include "main/game_timer_control.h"
#include "main/gamebit_ids.h"
#include "main/gamebits.h"
#include "main/mapEventTypes.h"
#include "main/map_load.h"
#include "main/model_light.h"
#include "main/object_render.h"
#include "main/objtype.h"
#include "main/obj_message.h"
#include "main/objseq.h"
#include "main/pi_dolphin.h"
#include "main/render_envfx.h"
#include "main/sky.h"
#include "main/vecmath.h"
#include "sys/objects.h"

enum DBSHShrineAnimEvent {
    DBSH_SHRINE_ANIM_EVENT_ACTIVATE = 3,
    DBSH_SHRINE_ANIM_EVENT_GRANT_SPIRIT = 7,
    DBSH_SHRINE_ANIM_EVENT_LOCK_POSE = 14,
    DBSH_SHRINE_ANIM_EVENT_UNLOCK_POSE = 15,
};

enum DBSHShrineSequence {
    DBSH_SHRINE_SEQUENCE_ACTIVATE,
    DBSH_SHRINE_SEQUENCE_CLOSE,
};

enum DBSHShrinePhase {
    DBSH_SHRINE_PHASE_WAITING = 0,
    DBSH_SHRINE_PHASE_RISING = 1,
    DBSH_SHRINE_PHASE_ACTIVE = 2,
    DBSH_SHRINE_PHASE_CLOSING = 4,
    DBSH_SHRINE_PHASE_RESET = 5,
};

enum DBSHShrineStateFlag {
    DBSH_SHRINE_STATE_RISE_SEQUENCE_READY = 1 << 0,
};

enum DBSHShrineLatchFlag {
    DBSH_SHRINE_LATCH_AMBIENT_MUSIC = 1 << 0,
    DBSH_SHRINE_LATCH_TEST_MUSIC = 1 << 1,
    DBSH_SHRINE_LATCH_SHRINE_MUSIC = 1 << 2,
};

typedef struct DBSHShrineState {
    ModelLightStruct* light;
    int gameBitLatch;
    f32 idleSfxTimer;
    s16 unknown10;
    s16 orbitPhaseA;
    s16 orbitPhaseB;
    s16 orbitPhaseC;
    u8 phase;
    u8 flags;
} DBSHShrineState;

STATIC_ASSERT(sizeof(DBSHShrineState) == 0x20);
STATIC_ASSERT(offsetof(DBSHShrineState, gameBitLatch) == 0x08);
STATIC_ASSERT(offsetof(DBSHShrineState, idleSfxTimer) == 0x0C);
STATIC_ASSERT(offsetof(DBSHShrineState, unknown10) == 0x10);
STATIC_ASSERT(offsetof(DBSHShrineState, orbitPhaseA) == 0x12);
STATIC_ASSERT(offsetof(DBSHShrineState, orbitPhaseB) == 0x14);
STATIC_ASSERT(offsetof(DBSHShrineState, orbitPhaseC) == 0x16);
STATIC_ASSERT(offsetof(DBSHShrineState, phase) == 0x18);
STATIC_ASSERT(offsetof(DBSHShrineState, flags) == 0x19);

static void dbshShrine_updateHoverMotion(GameObject* obj) {
    const ObjPlacement* placement = (const ObjPlacement*)obj->anim.placementData;
    DBSHShrineState* state = obj->extra;
    GameObject* player = Obj_GetPlayerObject();
    f32 trigA;
    f32 trigB;
    f32 distance;
    s32 angleDelta;
    ObjAnimEventList animEvents;

    if ((obj->anim.flags & OBJANIM_FLAG_HIDDEN) != 0) {
        obj->anim.rotX = 0;
        obj->anim.localPosY = placement->posY;
        return;
    }

    state->orbitPhaseA += (s32)(512.0f * timeDelta);
    state->orbitPhaseB += (s32)(128.0f * timeDelta);
    state->orbitPhaseC += (s32)(192.0f * timeDelta);

    obj->anim.localPosY =
        20.0f + (placement->posY + mathSinf((3.1415927f * state->orbitPhaseA) / 32768.0f));

    trigA = mathSinf((3.1415927f * state->orbitPhaseB) / 32768.0f);
    trigB = mathSinf((3.1415927f * state->orbitPhaseA) / 32768.0f);
    trigB += trigA;
    obj->anim.rotZ = (s16)(600.0f * trigB);

    trigA = mathSinf((3.1415927f * state->orbitPhaseC) / 32768.0f);
    trigB = mathSinf((3.1415927f * state->orbitPhaseA) / 32768.0f);
    trigB += trigA;
    obj->anim.rotY = (s16)(600.0f * trigB);

    ObjAnim_AdvanceCurrentMove(obj, 0.005f, timeDelta, &animEvents);

    if (player == NULL) {
        return;
    }

    angleDelta =
        (u16)getAngle(obj->anim.worldPosX - player->anim.worldPosX, obj->anim.worldPosZ - player->anim.worldPosZ) -
        (u16)obj->anim.rotX;
    if (angleDelta > 0x8000) {
        angleDelta -= 0xFFFF;
    }
    if (angleDelta < -0x8000) {
        angleDelta += 0xFFFF;
    }
    obj->anim.rotX = (s16)(obj->anim.rotX + (s32)(((f32)angleDelta * timeDelta) / 12.0f));

    distance = Vec_xzDistance(&obj->anim.worldPosX, &player->anim.worldPosX);
    if (distance <= 30.0f) {
        obj->anim.alpha = (u8)(s32)(255.0f * (distance / 30.0f));
    } else {
        obj->anim.alpha = 0xFF;
    }
}

static int dbshShrine_processAnimEvents(GameObject* obj, int unused, ObjSeqState* animUpdate) {
    DBSHShrineState* state = obj->extra;
    GameObject* player = Obj_GetPlayerObject();

    (void)unused;
    animUpdate->savedFlags = -1;
    animUpdate->movementState = 0;

    for (int i = 0; i < animUpdate->eventCount; i++) {
        u8 event = animUpdate->eventIds[i];

        if (event != 0) {
            switch (event) {
            case DBSH_SHRINE_ANIM_EVENT_ACTIVATE:
                state->flags |= DBSH_SHRINE_STATE_RISE_SEQUENCE_READY;
                break;
            case DBSH_SHRINE_ANIM_EVENT_GRANT_SPIRIT:
                objSetAnimStateFlags(player, 2, 1);
                mainSetBits(GAMEBIT_DBSH_SpiritGrantTriggered, 1);
                mainSetBits(GAMEBIT_ITEM_SpiritTestStrength_Got, 1);
                (*gMapEventInterface)->setMapAct(0xB, 3);
                unlockLevel(0, 0, 1);
                lockLevel(mapGetDirIdx(10), 0);
                break;
            case DBSH_SHRINE_ANIM_EVENT_LOCK_POSE:
                obj->anim.flags |= OBJANIM_FLAG_HIDDEN;
                if (state->light != NULL) {
                    modelLightStruct_setEnabled(state->light, 0, 1.0f);
                }
                break;
            case DBSH_SHRINE_ANIM_EVENT_UNLOCK_POSE:
                obj->anim.flags &= ~OBJANIM_FLAG_HIDDEN;
                if (state->light != NULL) {
                    modelLightStruct_setEnabled(state->light, 0, 1.0f);
                }
                break;
            }
        }
        animUpdate->eventIds[i] = 0;
    }

    return 0;
}

static int dbshShrine_getExtraSize(void) {
    return sizeof(DBSHShrineState);
}

static int dbshShrine_getObjectTypeId(void) {
    return 0;
}

static void dbshShrine_free(GameObject* obj) {
    DBSHShrineState* state = obj->extra;

    if (state->light != NULL) {
        ModelLightStruct_free(state->light);
        state->light = NULL;
    }
    gameTimerStop();
    objFreeObjectType(obj, OBJECT_CLASS_KRAZOA_SHRINE);
    Music_Trigger(MUSICTRIG_DIM_Snow, 0);
    Music_Trigger(MUSICTRIG_CC_Visit1, 0);
    Music_Trigger(MUSICTRIG_vfp_walkabout, 0);
    Music_Trigger(MUSICTRIG_test_of_fear, 0);
    mainSetBits(GAMEBIT_IN_KRAZOA_SHRINE, 0);
    mainSetBits(GAMEBIT_SHRINE_MUSIC_LOCK, 1);
}

static void dbshShrine_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                              s8 visible) {
    DBSHShrineState* state = obj->extra;

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

static void dbshShrine_hitDetect(void) {
}

static void dbshShrine_update(GameObject* obj) {
    DBSHShrineState* state = obj->extra;
    GameObject* player = Obj_GetPlayerObject();

    if (player == NULL) {
        return;
    }

    if (obj->userData1 != 0) {
        obj->userData1--;
        if (obj->userData1 == 0) {
            skySetSlotFlag80(7, 1);
            getEnvfxAct(obj, player, 0xD4, 0);
            getEnvfxAct(obj, player, 0xD5, 0);
            getEnvfxAct(obj, player, 0x222, 0);
        }
    }

    dbshShrine_updateHoverMotion(obj);
    GameBitLatch_Update(&state->gameBitLatch, DBSH_SHRINE_LATCH_TEST_MUSIC, -1, -1,
                        GAMEBIT_DBSH_TestStrengthRunning, MUSICTRIG_test_of_fear);
    GameBitLatch_UpdateInverted(&state->gameBitLatch, DBSH_SHRINE_LATCH_AMBIENT_MUSIC, -1, -1,
                                GAMEBIT_SHRINE_MUSIC_LOCK, MUSICTRIG_vfp_walkabout);
    GameBitLatch_Update(&state->gameBitLatch, DBSH_SHRINE_LATCH_SHRINE_MUSIC, -1, -1,
                        GAMEBIT_SHRINE_MUSIC_LOCK, MUSICTRIG_PU3_Adventure_c4);

    switch (state->phase) {
    case DBSH_SHRINE_PHASE_WAITING: {
        obj->anim.flags &= ~OBJANIM_FLAG_HIDDEN;
        f32 idleSfxTimer = state->idleSfxTimer - timeDelta;

        state->idleSfxTimer = idleSfxTimer;
        if (idleSfxTimer <= 0.0f) {
            Sfx_PlayFromObject(obj, SFXTRIG_spirit_voice);
            state->idleSfxTimer = (f32)randomGetRange(500, 1000);
        }
        if ((obj->anim.resetHitboxFlags & INTERACT_FLAG_ACTIVATED) != 0) {
            u8 groupActive = (*gMapEventInterface)->getObjGroupStatus(obj->anim.mapEventSlot, 1);

            if (groupActive != 0) {
                (*gMapEventInterface)->setObjGroupStatus(obj->anim.mapEventSlot, 1, 0);
            }
            state->phase = DBSH_SHRINE_PHASE_RISING;
            mainSetBits(GAMEBIT_DBSH_TestStrengthRunning, 1);
            obj->anim.rotX = 0x7FFF;
            (*gObjectTriggerInterface)->runSequence(DBSH_SHRINE_SEQUENCE_ACTIVATE, obj, -1);
            Music_Trigger(MUSICTRIG_DIM_Snow, 1);
        }
        break;
    }
    case DBSH_SHRINE_PHASE_RISING:
        obj->anim.flags |= OBJANIM_FLAG_HIDDEN;
        if ((state->flags & DBSH_SHRINE_STATE_RISE_SEQUENCE_READY) != 0) {
            state->phase = DBSH_SHRINE_PHASE_ACTIVE;
            mainSetBits(GAMEBIT_DBSH_SymbolRiseComplete, 1);
        }
        break;
    case DBSH_SHRINE_PHASE_ACTIVE:
        if (mainGetBit(GAMEBIT_DBSH_SymbolSpinSucceeded) != 0) {
            state->phase = DBSH_SHRINE_PHASE_CLOSING;
            state->unknown10 = 0;
        } else if (mainGetBit(GAMEBIT_DBSH_SymbolSpinFailed) != 0) {
            state->phase = DBSH_SHRINE_PHASE_RESET;
            mainSetBits(GAMEBIT_DBSH_ShrineRelated0C72, 1);
            state->unknown10 = 10;
        }
        break;
    case DBSH_SHRINE_PHASE_CLOSING:
        state->phase = DBSH_SHRINE_PHASE_RESET;
        audioStopByMask(3);
        (*gObjectTriggerInterface)->runSequence(DBSH_SHRINE_SEQUENCE_CLOSE, obj, -1);
        mainSetBits(GAMEBIT_DBSH_TestStrengthRunning, 0);
        break;
    case DBSH_SHRINE_PHASE_RESET:
        state->phase = DBSH_SHRINE_PHASE_WAITING;
        state->flags &= ~DBSH_SHRINE_STATE_RISE_SEQUENCE_READY;
        state->unknown10 = 0;
        mainSetBits(GAMEBIT_DBSH_TestStrengthRunning, 0);
        mainSetBits(GAMEBIT_DBSH_SpiritGrantTriggered, 0);
        mainSetBits(GAMEBIT_DBSH_SymbolRiseComplete, 0);
        mainSetBits(GAMEBIT_DBSH_SymbolSpinSucceeded, 0);
        mainSetBits(GAMEBIT_DBSH_SymbolSpinFailed, 0);
        mainSetBits(GAMEBIT_DBSH_ShrineRelated0C72, 0);
        mainSetBits(GAMEBIT_DBSH_ShrineRelated0C73, 0);
        break;
    }
}

static void dbshShrine_init(GameObject* obj) {
    DBSHShrineState* state = obj->extra;

    obj->animEventCallback = dbshShrine_processAnimEvents;
    obj->anim.rotX = 0;
    state->phase = DBSH_SHRINE_PHASE_WAITING;
    state->flags &= ~DBSH_SHRINE_STATE_RISE_SEQUENCE_READY;
    state->unknown10 = 0;

    ObjMsg_AllocQueue(obj, 4);
    mainSetBits(GAMEBIT_DBSH_SpiritGrantTriggered, 0);

    if ((*gMapEventInterface)->getObjGroupStatus(obj->anim.mapEventSlot, 1) == 0) {
        (*gMapEventInterface)->setObjGroupStatus(obj->anim.mapEventSlot, 1, 1);
    }

    obj->anim.worldPosX = obj->anim.localPosX;
    obj->anim.worldPosY = obj->anim.localPosY;
    obj->anim.worldPosZ = obj->anim.localPosZ;
    obj->userData1 = 1;

    if (state->light == NULL) {
        state->light = objCreateLight(NULL, 1);
    }

    mainSetBits(GAMEBIT_IN_KRAZOA_SHRINE, 1);
    mainSetBits(GAMEBIT_DBSH_Entered, 1);
}

static void dbshShrine_release(void) {
}

static void dbshShrine_initialise(void) {
}

OBJECT_INIT_ADAPTER(gDBSHShrineObjDescriptorInitAdapter, dbshShrine_init, obj)
OBJECT_HIT_DETECT_ADAPTER(gDBSHShrineObjDescriptorHitDetectAdapter, dbshShrine_hitDetect)
OBJECT_FREE_ADAPTER(gDBSHShrineObjDescriptorFreeAdapter, dbshShrine_free, obj)
OBJECT_TYPE_ID_ADAPTER(gDBSHShrineObjDescriptorTypeIdAdapter, dbshShrine_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gDBSHShrineObjDescriptorExtraSizeAdapter, dbshShrine_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gDBSHShrineObjDescriptorAcquire, dbshShrine_initialise)

ObjectDescriptor gDBSHShrineObjDescriptor = {
    .header = {
        .metadata = { 0, 0, 0, OBJECT_DESCRIPTOR_FLAGS_10_SLOTS },
        .acquire = gDBSHShrineObjDescriptorAcquire,
        .release = dbshShrine_release,
    },
    .init = gDBSHShrineObjDescriptorInitAdapter,
    .update = dbshShrine_update,
    .hitDetect = gDBSHShrineObjDescriptorHitDetectAdapter,
    .render = dbshShrine_render,
    .free = gDBSHShrineObjDescriptorFreeAdapter,
    .getObjectTypeId = gDBSHShrineObjDescriptorTypeIdAdapter,
    .getExtraSize = gDBSHShrineObjDescriptorExtraSizeAdapter,
};;
