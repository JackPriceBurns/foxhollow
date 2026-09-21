#include "dlls/objects/443_SC_totembon.h"

#include "dlls/objects/437.h"
#include "dlls/objects/438_SC_levelcon.h"
#include "dolphin/math.h"
#include "main/audio/music.h"
#include "main/audio/music_trigger_ids.h"
#include "main/audio/sfx.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/camera_interface.h"
#include "main/dll/CAM/dll_0001_camcontrol.h"
#include "main/dll/dll_0044_cameramodeviewfinder.h"
#include "main/dll/player.h"
#include "main/dll/tricky.h"
#include "main/frame_timing.h"
#include "main/game_ui_interface.h"
#include "main/gamebit_ids.h"
#include "main/gamebits.h"
#include "main/mapEventTypes.h"
#include "main/obj_list.h"
#include "main/object_render.h"
#include "main/objhits.h"
#include "main/objseq.h"
#include "main/screen_transition.h"
#include "main/vecmath.h"
#include "sys/objects.h"
#include "sys/objects/lifecycle.h"

enum ScTotemBondRing {
    SC_TOTEM_BOND_RING_COUNT = 8,
    SC_TOTEM_BOND_RING_ANGLE_STEP = 0x2000,
};

enum ScTotemBondEventFlag {
    SC_TOTEM_BOND_START_ORBS = 1 << 0,
    SC_TOTEM_BOND_ORBS_ACTIVE = 1 << 1,
    SC_TOTEM_BOND_SET_MAP_MODE = 1 << 4,
};

enum ScTotemBondSequenceEvent {
    SC_TOTEM_BOND_SEQUENCE_START_ORBS = 1,
    SC_TOTEM_BOND_SEQUENCE_CLEAR_LEVEL_CONTROL = 2,
    SC_TOTEM_BOND_SEQUENCE_RESET_LEVEL_CONTROL = 3,
};

typedef struct ScTotemBondLightfootSetup {
    LightfootPlacement placement;
    u8 unused34[4];
} ScTotemBondLightfootSetup;

typedef struct ScTotemBondState {
    CameraModeViewfinderPose cameraPose;
    f32 spawnTimer;
    f32 completionTimer;
    u32 orbGameStarted;
    s16 ringIndex;
    u8 eventFlags;
    u8 unused27;
} ScTotemBondState;

STATIC_ASSERT(sizeof(ScTotemBondLightfootSetup) == 0x38);
STATIC_ASSERT(offsetof(ScTotemBondLightfootSetup, placement) == 0x00);
STATIC_ASSERT(offsetof(ScTotemBondLightfootSetup, unused34) == 0x34);

STATIC_ASSERT(sizeof(ScTotemBondState) == 0x28);
STATIC_ASSERT(offsetof(ScTotemBondState, cameraPose) == 0x00);
STATIC_ASSERT(offsetof(ScTotemBondState, spawnTimer) == 0x18);
STATIC_ASSERT(offsetof(ScTotemBondState, completionTimer) == 0x1C);
STATIC_ASSERT(offsetof(ScTotemBondState, orbGameStarted) == 0x20);
STATIC_ASSERT(offsetof(ScTotemBondState, ringIndex) == 0x24);
STATIC_ASSERT(offsetof(ScTotemBondState, eventFlags) == 0x26);
STATIC_ASSERT(offsetof(ScTotemBondState, unused27) == 0x27);

static const u16 sTotemBondRingGameBits[SC_TOTEM_BOND_RING_COUNT] = {
    GAMEBIT_SC_TotemBondRing0Active, GAMEBIT_SC_TotemBondRing1Active, GAMEBIT_SC_TotemBondRing2Active,
    GAMEBIT_SC_TotemBondRing3Active, GAMEBIT_SC_TotemBondRing4Active, GAMEBIT_SC_TotemBondRing5Active,
    GAMEBIT_SC_TotemBondRing6Active, GAMEBIT_SC_TotemBondRing7Active,
};

static const u16 sTotemBondOrbGameBits[SC_TOTEM_BOND_RING_COUNT] = {
    GAMEBIT_SC_LightfootChallengeDifficulty0, GAMEBIT_SC_LightfootChallengeDifficulty1,
    GAMEBIT_SC_LightfootChallengeDifficulty2, GAMEBIT_SC_LightfootChallengeDifficulty3,
    GAMEBIT_SC_LightfootChallengeDifficulty4, GAMEBIT_SC_LightfootChallengeDifficulty5,
    GAMEBIT_SC_LightfootChallengeDifficulty6, GAMEBIT_SC_LightfootChallengeDifficulty7,
};

static void sc_totembond_spawnGameBitOrbs(GameObject* obj, f32 radius) {
    if (Obj_IsLoadingLocked() == 0) {
        return;
    }

    const ObjPlacement* sourcePlacement = obj->anim.placement;
    for (int spawnIndex = 0; spawnIndex < SC_TOTEM_BOND_RING_COUNT; spawnIndex++) {
        int ringIndex = (spawnIndex + 1) % SC_TOTEM_BOND_RING_COUNT;
        int angleOffset = spawnIndex * SC_TOTEM_BOND_RING_ANGLE_STEP;
        ScTotemBondLightfootSetup* setup =
            (ScTotemBondLightfootSetup*)Obj_AllocObjectSetup(sizeof(*setup), LIGHTFOOT_OBJECT_NPC);
        LightfootPlacement* placement = &setup->placement;

        placement->base.posX =
            radius * mathSinf((3.1415927f * (f32)(obj->anim.rotX + angleOffset)) / 32768.0f) + obj->anim.localPosX;
        placement->base.posY = obj->anim.localPosY;
        placement->base.posZ =
            radius * mathCosf((3.1415927f * (f32)(obj->anim.rotX + angleOffset)) / 32768.0f) + obj->anim.localPosZ;
        placement->base.color[0] = sourcePlacement->color[0];
        placement->base.color[1] = (sourcePlacement->color[1] & ~1) | 4;
        placement->base.color[2] = sourcePlacement->color[2];
        placement->base.color[3] = 30;
        placement->presenceGameBit = -1;
        placement->completionGameBit = GAMEBIT_SC_TotemBondOrbCollected;
        placement->eventGameBit = sTotemBondOrbGameBits[ringIndex];
        placement->activeGameBit = sTotemBondRingGameBits[ringIndex];
        placement->rotation = (s8)((obj->anim.rotX + 0x8000 + angleOffset) >> 8);
        placement->hitPoints = 1;
        objSetupObject(&placement->base, 5, -1, -1, 0);
    }
}

static void sc_totembond_beginOrbGame(GameObject* obj, ScTotemBondState* state) {
    state->orbGameStarted = 1;
    obj->anim.rotX = 0x3FFF;
    state->ringIndex = obj->anim.rotX / SC_TOTEM_BOND_RING_ANGLE_STEP;
    ObjHits_DisableObject(obj);
    sc_totembond_spawnGameBitOrbs(obj, -130.0f);
    mainSetBits(sTotemBondRingGameBits[state->ringIndex], 1);
    obj->anim.alpha = 0;
    state->eventFlags &= ~SC_TOTEM_BOND_START_ORBS;
    state->eventFlags |= SC_TOTEM_BOND_ORBS_ACTIVE;
    (*gGameUIInterface)->setCMenuShouldClose(1);
    setHudForceShowMask(1);
    (*gScreenTransitionInterface)->step(30, SCREEN_TRANSITION_BLACK);
    state->spawnTimer = 30.0f;
    Music_Trigger(MUSICTRIG_WLC_Puzzle_f0, 1);
}

static GameObject* sc_totembond_findLevelControl(GameObject* obj) {
    int firstObjectIndex;
    int objectCount;
    GameObject** objects = ObjList_GetObjects(&firstObjectIndex, &objectCount);

    for (int objectIndex = firstObjectIndex; objectIndex < objectCount; objectIndex++) {
        GameObject* candidate = objects[objectIndex];
        if (candidate != obj && candidate->anim.romDefNo == SC_LEVEL_CONTROL_ROM_DEF_NO) {
            return candidate;
        }
    }
    return NULL;
}

static u32 sc_totembond_animEventCallback(GameObject* obj, u32 unused, ObjSeqState* animUpdate) {
    ScTotemBondState* state = obj->extra;

    (void)unused;

    animUpdate->movementState = 0;
    for (int eventIndex = 0; eventIndex < animUpdate->eventCount; eventIndex++) {
        switch (animUpdate->eventIds[eventIndex]) {
        case SC_TOTEM_BOND_SEQUENCE_START_ORBS:
            state->eventFlags |= SC_TOTEM_BOND_START_ORBS;
            (*gObjectTriggerInterface)->setCamVars(CAMERA_MODE_VIEWFINDER_RESOURCE_ID, 1, 0, 0);
            break;
        case SC_TOTEM_BOND_SEQUENCE_CLEAR_LEVEL_CONTROL: {
            GameObject* levelControl = sc_totembond_findLevelControl(obj);
            if (levelControl != NULL) {
                sc_levelcontrol_setAnimEventState(levelControl, SC_LEVEL_CONTROL_ANIM_STATE_CLEAR);
            }
            state->eventFlags |= SC_TOTEM_BOND_SET_MAP_MODE;
            break;
        }
        case SC_TOTEM_BOND_SEQUENCE_RESET_LEVEL_CONTROL: {
            GameObject* levelControl = sc_totembond_findLevelControl(obj);
            if (levelControl != NULL) {
                sc_levelcontrol_setAnimEventState(levelControl, SC_LEVEL_CONTROL_ANIM_STATE_1);
            }
            break;
        }
        }
    }
    return 0;
}

static int sc_totembond_getExtraSize(void) {
    return sizeof(ScTotemBondState);
}

static int sc_totembond_getObjectTypeId(void) {
    return 0;
}

static void sc_totembond_free(GameObject* obj) {
    (void)obj;

    Music_Trigger(MUSICTRIG_WLC_Puzzle_f0, 0);
    fearTestMeterSetFadeIn(0);
}

static void sc_totembond_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                                s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

static void sc_totembond_hitDetect(void) {
}

static void sc_totembond_finishOrbGame(GameObject* obj, ScTotemBondState* state) {
    GameObject* player;

    state->completionTimer = 0.0f;
    player = Obj_GetPlayerObject();
    (*gMapEventInterface)->clearRestartPoint();
    (*gCameraInterface)->setMode(CAMCONTROL_ACTION_DEFAULT, 0, 3, 0, NULL, 0, 0);
    obj->anim.alpha = 0xFF;
    playerTeleport(player, NULL, NULL, 0);
    ObjHits_EnableObject(obj);
    setHudForceShowMask(0);
    mainSetBits(GAMEBIT_SC_TotemBondComplete, 1);
    state->eventFlags = 0;
    Music_Trigger(MUSICTRIG_WLC_Puzzle_f0, 0);
}

static u8 sc_totembond_gatherAvailableOrbs(u8 availableOrbs[SC_TOTEM_BOND_RING_COUNT]) {
    u8 availableCount = 0;

    for (u8 orbIndex = 0; orbIndex < SC_TOTEM_BOND_RING_COUNT; orbIndex++) {
        if (mainGetBit(sTotemBondOrbGameBits[orbIndex]) == 0) {
            availableOrbs[availableCount++] = orbIndex;
        }
    }
    return availableCount;
}

static void sc_totembond_update(GameObject* obj) {
    ScTotemBondState* state = obj->extra;
    GameObject* player = Obj_GetPlayerObject();

    if ((state->eventFlags & SC_TOTEM_BOND_START_ORBS) != 0) {
        sc_totembond_beginOrbGame(obj, state);
    }

    if ((state->eventFlags & SC_TOTEM_BOND_ORBS_ACTIVE) != 0) {
        if (state->spawnTimer != 0.0f) {
            state->spawnTimer -= timeDelta;
            if (state->spawnTimer < 0.0f) {
                state->spawnTimer = 0.0f;
            }
        } else if (state->completionTimer != 0.0f) {
            state->completionTimer -= timeDelta;
            if (state->completionTimer <= 0.0f) {
                sc_totembond_finishOrbGame(obj, state);
                return;
            }
        } else {
            if (mainGetBit(GAMEBIT_SC_TotemBondOrbCollected) != 0) {
                u8 availableOrbs[SC_TOTEM_BOND_RING_COUNT];
                u8 availableCount;

                mainSetBits(GAMEBIT_SC_TotemBondOrbCollected, 0);
                availableCount = sc_totembond_gatherAvailableOrbs(availableOrbs);
                if (availableCount == 0) {
                    state->completionTimer = 35.0f;
                    fearTestMeterSetFadeIn(0);
                    (*gScreenTransitionInterface)->start(30, SCREEN_TRANSITION_BLACK);
                } else {
                    u8 nextRing = availableOrbs[randomGetRange(0, availableCount - 1)];
                    if (state->ringIndex == nextRing) {
                        mainSetBits(sTotemBondRingGameBits[state->ringIndex], 1);
                    } else {
                        state->ringIndex = nextRing;
                        Sfx_PlayFromObject(obj, SFXTRIG_mv_cagerat01);
                    }
                }
            }
            if (((u16)obj->anim.rotX >> 13) != state->ringIndex) {
                obj->anim.rotX = (s16) - ((512.0f * timeDelta) - (f32)obj->anim.rotX);
                if (((u16)obj->anim.rotX >> 13) == state->ringIndex) {
                    mainSetBits(sTotemBondRingGameBits[state->ringIndex], 1);
                }
            }
        }

        playerTeleport(player, (Vec3f*)&obj->anim.localPosX, (Vec3s*)&obj->anim.rotX, 0);
        state->cameraPose.positionX = obj->anim.localPosX;
        state->cameraPose.positionY = obj->anim.localPosY + 30.0f;
        state->cameraPose.positionZ = obj->anim.localPosZ;
        state->cameraPose.rotationX = (s16)(0x8000 - obj->anim.rotX);
        state->cameraPose.rotationY = obj->anim.rotY;
        state->cameraPose.rotationZ = obj->anim.rotZ;
        state->cameraPose.fov = 72.0f;
        (*gCameraInterface)->releaseAction(&state->cameraPose, sizeof(state->cameraPose));
    }

    if ((state->eventFlags & SC_TOTEM_BOND_SET_MAP_MODE) != 0) {
        (*gMapEventInterface)->setMapAct(SC_LEVEL_CONTROL_MAP_SWAPCIRCLE, SC_LEVEL_CONTROL_MAP_ACT_ESCAPE_COMPLETE);
        state->eventFlags &= ~SC_TOTEM_BOND_SET_MAP_MODE;
    }
}

static void sc_totembond_init(GameObject* obj, const void* placement) {
    ScTotemBondState* state = obj->extra;

    (void)placement;

    state->ringIndex = obj->anim.rotX / SC_TOTEM_BOND_RING_ANGLE_STEP;
    obj->animEventCallback = sc_totembond_animEventCallback;
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN | OBJECT_OBJFLAG_HITDETECT_DISABLED;
}

static void sc_totembond_release(void) {
}

static void sc_totembond_initialise(void) {
}

int sc_totembond_insertOrderedGameBit(const u16 gameBitIds[3], u16 newValue) {
    u16 values[4];
    int changed = 0;

    for (int index = 0; index < 3; index++) {
        values[index] = mainGetBit(gameBitIds[index]);
    }
    values[3] = newValue;

    for (int pass = 0; pass < 3; pass++) {
        for (int index = 0; index < 3; index++) {
            if (values[index + 1] != 0 && (values[index + 1] < values[index] || values[index] == 0)) {
                u16 value = values[index];
                values[index] = values[index + 1];
                values[index + 1] = value;
                changed = 1;
            }
        }
    }

    for (int index = 0; index < 3; index++) {
        mainSetBits(gameBitIds[index], values[index]);
    }
    return changed;
}

OBJECT_INIT_ADAPTER(gSC_totembondObjDescriptorInitAdapter, sc_totembond_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gSC_totembondObjDescriptorHitDetectAdapter, sc_totembond_hitDetect)
OBJECT_FREE_ADAPTER(gSC_totembondObjDescriptorFreeAdapter, sc_totembond_free, obj)
OBJECT_TYPE_ID_ADAPTER(gSC_totembondObjDescriptorTypeIdAdapter, sc_totembond_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gSC_totembondObjDescriptorExtraSizeAdapter, sc_totembond_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gSC_totembondObjDescriptorAcquire, sc_totembond_initialise)

ObjectDescriptor gSC_totembondObjDescriptor = {
    .header =
        {
            .metadata = {0, 0, 0, OBJECT_DESCRIPTOR_FLAGS_10_SLOTS},
            .acquire = gSC_totembondObjDescriptorAcquire,
            .release = sc_totembond_release,
        },
    .init = gSC_totembondObjDescriptorInitAdapter,
    .update = sc_totembond_update,
    .hitDetect = gSC_totembondObjDescriptorHitDetectAdapter,
    .render = sc_totembond_render,
    .free = gSC_totembondObjDescriptorFreeAdapter,
    .getObjectTypeId = gSC_totembondObjDescriptorTypeIdAdapter,
    .getExtraSize = gSC_totembondObjDescriptorExtraSizeAdapter,
};
;
