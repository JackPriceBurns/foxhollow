#include "dlls/objects/390_CCgasventCo.h"
#include "dlls/objects/389_CCgasvent.h"

#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/audio/sfx.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/camera_interface.h"
#include "main/frame_timing.h"
#include "main/gamebit_ids.h"
#include "main/game_ui_interface.h"
#include "main/gamebits.h"
#include "main/mapEventTypes.h"
#include "main/objseq.h"
#include "main/object_render.h"
#include "main/objtype.h"
#include "main/pi_dolphin.h"
#include "main/vecmath.h"
#include "sys/objects.h"

enum CcGasVentPopulation {
    CC_GAS_VENT_COUNT = 4,
};

enum CcGasVentControlSequence {
    CC_GAS_VENT_SEQUENCE_INTRO,
    CC_GAS_VENT_SEQUENCE_RESTART,
};

enum CcGasVentControlPhase {
    CC_GAS_VENT_CONTROL_PHASE_WAIT_FOR_VENTS,
    CC_GAS_VENT_CONTROL_PHASE_WAIT_FOR_INTRO,
    CC_GAS_VENT_CONTROL_PHASE_INIT_METER,
    CC_GAS_VENT_CONTROL_PHASE_ACTIVE,
    CC_GAS_VENT_CONTROL_PHASE_RESTART,
    CC_GAS_VENT_CONTROL_PHASE_SAVE_POINT,
    CC_GAS_VENT_CONTROL_PHASE_WAIT_FOR_CLEAR,
    CC_GAS_VENT_CONTROL_PHASE_COMPLETE,
};

typedef struct CcGasVentControlPlacement {
    ObjPlacement base;
    u8 pad18[2];
    u8 rotXByte;
    u8 pad1B[5];
} CcGasVentControlPlacement;

STATIC_ASSERT(sizeof(CcGasVentControlPlacement) == 0x20);
STATIC_ASSERT(offsetof(CcGasVentControlPlacement, rotXByte) == 0x1A);

typedef struct CcGasVentControlState {
    u8 phase;
    u8 loopedSoundActive;
    u8 pad02[2];
    f32 airRemaining;
    f32 gasHeightOffset;
    u8 previousUnblockedVentCount;
    u8 pad0D[3];
} CcGasVentControlState;

STATIC_ASSERT(sizeof(CcGasVentControlState) == 0x10);
STATIC_ASSERT(offsetof(CcGasVentControlState, airRemaining) == 0x04);
STATIC_ASSERT(offsetof(CcGasVentControlState, gasHeightOffset) == 0x08);
STATIC_ASSERT(offsetof(CcGasVentControlState, previousUnblockedVentCount) == 0x0C);

static u8 ccGasVentControl_countUnblockedVents(GameObject* obj, CcGasVentControlState* state);

static int ccGasVentControl_sequenceCallback(GameObject* obj) {
    ccGasVentControl_countUnblockedVents(obj, obj->extra);
    return 0;
}

static u8 ccGasVentControl_countUnblockedVents(GameObject* obj, CcGasVentControlState* state) {
    u8 unblockedVentCount = 0;

    if (mainGetBit(GAMEBIT_CC_GasVentActive) != 0) {
        int ventCount;
        GameObject** vents = objGetAllOfType(CC_GAS_VENT_OBJECT_GROUP, &ventCount);

        for (u8 i = 0; i < CC_GAS_VENT_COUNT; i++) {
            GameObject* nearestBlocker =
                objGetNearestTypeTo(CC_GAS_VENT_BLOCKER_OBJECT_GROUP, vents[i], 0);
            if (getXZDistanceSquared(&vents[i]->anim.worldPosX, &nearestBlocker->anim.worldPosX) > 100.0f) {
                unblockedVentCount++;
            }
        }
    }
    if (unblockedVentCount != 0) {
        if (state->loopedSoundActive == 0) {
            Sfx_AddLoopedObjectSound(obj, SFXTRIG_en_diallp_c_223);
            state->loopedSoundActive = 1;
        }
        Sfx_SetObjectSfxVolume(obj, SFXTRIG_en_diallp_c_223,
                               unblockedVentCount * 0x0F + 0x28, 127.0f);
    } else {
        if (state->loopedSoundActive != 0) {
            Sfx_RemoveLoopedObjectSound(obj, SFXTRIG_en_diallp_c_223);
            state->loopedSoundActive = 0;
        }
    }
    return unblockedVentCount;
}

static int ccGasVentControl_getExtraSize(void) {
    return sizeof(CcGasVentControlState);
}

static void ccGasVentControl_free(GameObject* obj) {
    CcGasVentControlState* state = obj->extra;
    u8 phase = state->phase;

    if (phase == CC_GAS_VENT_CONTROL_PHASE_ACTIVE || phase == CC_GAS_VENT_CONTROL_PHASE_RESTART) {
        disableHeavyFog();
    }
    (*gGameUIInterface)->airMeterShutdown();
}

static void ccGasVentControl_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                                    s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

static void ccGasVentControl_update(GameObject* obj) {
    CcGasVentControlState* state = obj->extra;
    u8 unblockedVentCount = ccGasVentControl_countUnblockedVents(obj, state);

    switch (state->phase) {
    case CC_GAS_VENT_CONTROL_PHASE_WAIT_FOR_VENTS: {
        int ventCount;

        objGetAllOfType(CC_GAS_VENT_OBJECT_GROUP, &ventCount);
        if (ventCount == CC_GAS_VENT_COUNT) {
            state->phase = CC_GAS_VENT_CONTROL_PHASE_WAIT_FOR_INTRO;
        }
        break;
    }
    case CC_GAS_VENT_CONTROL_PHASE_WAIT_FOR_INTRO:
        if (mainGetBit(GAMEBIT_CC_GasVentIntroTriggered) != 0) {
            (*gObjectTriggerInterface)->runSequence(CC_GAS_VENT_SEQUENCE_INTRO, obj, -1);
            state->phase = CC_GAS_VENT_CONTROL_PHASE_INIT_METER;
        }
        break;
    case CC_GAS_VENT_CONTROL_PHASE_INIT_METER:
        (*gGameUIInterface)->initAirMeter(6000, 0x603);
        state->airRemaining = 6000.0f;
        state->phase = CC_GAS_VENT_CONTROL_PHASE_ACTIVE;
        state->previousUnblockedVentCount = unblockedVentCount;
        break;
    case CC_GAS_VENT_CONTROL_PHASE_ACTIVE:
        if (unblockedVentCount != 0) {
            GameObject* player = Obj_GetPlayerObject();

            state->gasHeightOffset += timeDelta / 100.0f;
            if (state->gasHeightOffset > 50.0f) {
                state->gasHeightOffset = 50.0f;
            }
            if (player->anim.localPosY <= obj->anim.localPosY + state->gasHeightOffset) {
                state->airRemaining = -(timeDelta * unblockedVentCount - state->airRemaining);
            } else {
                state->airRemaining = 16.0f * timeDelta + state->airRemaining;
                if (state->airRemaining > 6000.0f) {
                    state->airRemaining = 6000.0f;
                }
            }
            enableHeavyFog(obj->anim.localPosY + state->gasHeightOffset,
                           obj->anim.localPosY - 15.0f, 800.0f, 0.1f, 0.0005f, 0);
            if (state->airRemaining >= 0.0f) {
                (*gGameUIInterface)->runAirMeter((int)state->airRemaining);
            } else {
                (*gGameUIInterface)->airMeterShutdown();
                obj->anim.localPosX = player->anim.localPosX;
                obj->anim.localPosY = player->anim.localPosY;
                obj->anim.localPosZ = player->anim.localPosZ;
                (*gObjectTriggerInterface)->runSequence(CC_GAS_VENT_SEQUENCE_RESTART, obj, -1);
                (*gCameraInterface)->setMode(0x42, 0, 1, 0, NULL, 0x1E, 0xFF);
                state->phase = CC_GAS_VENT_CONTROL_PHASE_RESTART;
            }
            if (unblockedVentCount != state->previousUnblockedVentCount) {
                Sfx_PlayFromObject(0, SFXTRIG_sc_menuups16k_409);
                state->previousUnblockedVentCount = unblockedVentCount;
            }
        } else {
            Sfx_PlayFromObject(0, SFXTRIG_mpick1_b);
            (*gGameUIInterface)->airMeterShutdown();
            mainSetBits(GAMEBIT_CC_GasVentPuzzleComplete, 1);
            mainSetBits(GAMEBIT_CC_GasVentPuzzleState, 0);
            state->phase = CC_GAS_VENT_CONTROL_PHASE_SAVE_POINT;
        }
        break;
    case CC_GAS_VENT_CONTROL_PHASE_RESTART:
        (*gMapEventInterface)->gotoRestartPoint();
        break;
    case CC_GAS_VENT_CONTROL_PHASE_SAVE_POINT: {
        GameObject* player = Obj_GetPlayerObject();

        (*gMapEventInterface)->savePoint(&player->anim.localPosX, player->anim.rotX, 1, 0);
        state->phase = CC_GAS_VENT_CONTROL_PHASE_WAIT_FOR_CLEAR;
        break;
    }
    case CC_GAS_VENT_CONTROL_PHASE_WAIT_FOR_CLEAR:
        if (mainGetBit(GAMEBIT_CC_GasVentActive) == 0) {
            disableHeavyFog();
            state->phase = CC_GAS_VENT_CONTROL_PHASE_COMPLETE;
        }
        break;
    }
}

static void ccGasVentControl_init(GameObject* obj, const CcGasVentControlPlacement* placement) {
    CcGasVentControlState* state = obj->extra;

    obj->animEventCallback = ccGasVentControl_sequenceCallback;
    obj->anim.rotX = (s16)(placement->rotXByte << 8);
    if (mainGetBit(GAMEBIT_CC_GasVentPuzzleComplete) != 0) {
        state->phase = CC_GAS_VENT_CONTROL_PHASE_COMPLETE;
    }
}

OBJECT_INIT_ADAPTER(gCCGasVentControlObjDescriptorInitAdapter, ccGasVentControl_init, obj, placement)
OBJECT_FREE_ADAPTER(gCCGasVentControlObjDescriptorFreeAdapter, ccGasVentControl_free, obj)
OBJECT_EXTRA_SIZE_ADAPTER(gCCGasVentControlObjDescriptorExtraSizeAdapter, ccGasVentControl_getExtraSize)

ObjectDescriptor gCCGasVentControlObjDescriptor = {
    .header = {
        .metadata = { 0, 0, 0, OBJECT_DESCRIPTOR_FLAGS_10_SLOTS },
        .acquire = NULL,
        .release = NULL,
    },
    .init = gCCGasVentControlObjDescriptorInitAdapter,
    .update = ccGasVentControl_update,
    .render = ccGasVentControl_render,
    .free = gCCGasVentControlObjDescriptorFreeAdapter,
    .getExtraSize = gCCGasVentControlObjDescriptorExtraSizeAdapter,
};;
