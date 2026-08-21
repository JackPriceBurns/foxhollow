/*
 * CClevcontro (DLL 0x18B) - Cape Claw level controller.
 *
 * Drives the area's environment effects, day/night music, gamebit-controlled
 * music latches and object groups, a triggered camera action, and the
 * four-gold-bar completion sound.
 */
#include "dlls/objects/395_CClevcontro.h"

#include "dlls/objects/430_SH_LevelCon.h"
#include "game/objects/object.h"
#include "main/audio/music_api.h"
#include "main/audio/music_trigger_ids.h"
#include "main/audio/sfx_play_api.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/camera_interface.h"
#include "main/dll/CAM/dll_0001_camcontrol.h"
#include "main/dll/savegame_load_api.h"
#include "main/frame_timing.h"
#include "main/gamebit_ids.h"
#include "main/gamebits_api.h"
#include "main/gametext_show_api.h"
#include "main/mapEventTypes.h"
#include "main/object_render.h"
#include "main/objfx.h"
#include "main/objseq.h"
#include "main/render_envfx_api.h"
#include "main/sky.h"
#include "main/sky_api.h"
#include "main/sky_interface.h"
#include "sys/objects/lifecycle.h"

enum CcLevelControlLatchFlag {
    CC_LEVEL_CONTROL_LATCH_CAMERA_ACTION = 0x01,
    CC_LEVEL_CONTROL_LATCH_ALIEN_MUSIC = 0x02,
    CC_LEVEL_CONTROL_LATCH_BLIZZARD = 0x04,
    CC_LEVEL_CONTROL_LATCH_MUSIC_BF = 0x08,
    CC_LEVEL_CONTROL_LATCH_MUSIC_C0 = 0x10,
    CC_LEVEL_CONTROL_LATCH_DAY_NIGHT = 0x20,
    CC_LEVEL_CONTROL_LATCH_MUSIC_CD = 0x40,
    CC_LEVEL_CONTROL_LATCH_MUSIC_EA = 0x80,
};

enum CcLevelControlObjectGroup {
    CC_LEVEL_CONTROL_OBJECT_GROUP_1D = 0x1D,
    CC_LEVEL_CONTROL_OBJECT_GROUP_1E = 0x1E,
    CC_LEVEL_CONTROL_OBJECT_GROUP_1F = 0x1F,
};

typedef struct CcLevelControlState {
    f32 textTimer;
    GameBitLatchState gameBitLatch;
    int musicTriggerId;
    u32 mapAct;
} CcLevelControlState;

STATIC_ASSERT(sizeof(CcLevelControlState) == 0x10);
STATIC_ASSERT(offsetof(CcLevelControlState, gameBitLatch) == 0x04);
STATIC_ASSERT(offsetof(CcLevelControlState, musicTriggerId) == 0x08);
STATIC_ASSERT(offsetof(CcLevelControlState, mapAct) == 0x0C);

static SkyEnvFxRampTables sCcLevelControlEnvFxRampTables = {
    {0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241,
     0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241},
    {0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F,
     0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F},
    {0x0240, 0x0240, 0x0240, 0x0243, 0x0240, 0x0243, 0x0243, 0x0240, 0x0240, 0x0240, 0x0240, 0x0240, 0x0240, 0x0240,
     0x0240, 0x0240, 0x0243, 0x0243, 0x0243, 0x0240, 0x0240, 0x0240, 0x0240, 0x0240, 0x0240, 0x0240, 0x0240, 0x0240},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
};

static int ccLevelControl_animationEventCallback(GameObject* obj, int unusedArg, ObjSeqState* animUpdate) {
    if (animUpdate->eventCount != 0) {
        spawnExplosion(obj, 50.0f, 1, 1, 0, 1, 1, 1, 0);
    }
    return 0;
}

static int ccLevelControl_getExtraSize(void) {
    return sizeof(CcLevelControlState);
}

static void ccLevelControl_free(void) {
    skySetEnvFxFlags(0);
    Music_Trigger(MUSICTRIG_Arwing_Crash, 0);
}

static void ccLevelControl_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                                  s8 unusedVisible) {
    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
}

static void ccLevelControl_update(GameObject* obj) {
    CcLevelControlState* state = obj->extra;
    GameObject* tricky;
    u32 usedGoldBarCount;
    u32 heldGoldBarCount;

    if (state->textTimer > 0.0f) {
        gameTextShow(0x34C);
        state->textTimer -= timeDelta;
        if (state->textTimer < 0.0f) {
            state->textTimer = 0.0f;
        }
    }
    if ((*gSkyInterface)->getSunPosition(NULL) != 0) {
        if (state->musicTriggerId != -1) {
            state->musicTriggerId = -1;
            if (state->gameBitLatch.activeMask & CC_LEVEL_CONTROL_LATCH_DAY_NIGHT) {
                Music_Trigger(MUSICTRIG_Arwing_Crash, 0);
            }
        }
    } else {
        if (state->musicTriggerId != MUSICTRIG_Arwing_Crash) {
            state->musicTriggerId = MUSICTRIG_Arwing_Crash;
            if (state->gameBitLatch.activeMask & CC_LEVEL_CONTROL_LATCH_DAY_NIGHT) {
                Music_Trigger(MUSICTRIG_Arwing_Crash, 1);
            }
        }
    }
    GameBitLatch_Update(&state->gameBitLatch, CC_LEVEL_CONTROL_LATCH_ALIEN_MUSIC, -1, -1,
                        GAMEBIT_CC_AlienMusic, MUSICTRIG_mmpassalien);
    GameBitLatch_Update(&state->gameBitLatch, CC_LEVEL_CONTROL_LATCH_DAY_NIGHT, -1, -1,
                        GAMEBIT_CC_DayNightMusic, state->musicTriggerId);
    GameBitLatch_Update(&state->gameBitLatch, CC_LEVEL_CONTROL_LATCH_BLIZZARD, -1, -1,
                        GAMEBIT_CC_BlizzardMusic, MUSICTRIG_blizzard);
    GameBitLatch_Update(&state->gameBitLatch, CC_LEVEL_CONTROL_LATCH_MUSIC_BF, -1, -1,
                        GAMEBIT_CC_MusicBF, 0xBF);
    GameBitLatch_Update(&state->gameBitLatch, CC_LEVEL_CONTROL_LATCH_MUSIC_C0, -1, -1,
                        GAMEBIT_CC_MusicC0, 0xC0);
    GameBitLatch_Update(&state->gameBitLatch, CC_LEVEL_CONTROL_LATCH_MUSIC_CD, -1, -1,
                        GAMEBIT_CC_MusicCD, 0xCD);
    if (state->mapAct == 2) {
        GameBitLatch_UpdateInverted(&state->gameBitLatch, CC_LEVEL_CONTROL_LATCH_MUSIC_EA, -1, -1,
                                    GAMEBIT_CC_MusicEA, 0xEA);
    }
    if (mainGetBit(GAMEBIT_CC_ObjGroup1FDisabled) != 0 &&
        (u8)(*gMapEventInterface)->getObjGroupStatus(obj->anim.mapEventSlot, CC_LEVEL_CONTROL_OBJECT_GROUP_1F) != 0) {
        (*gMapEventInterface)->setObjGroupStatus(obj->anim.mapEventSlot, CC_LEVEL_CONTROL_OBJECT_GROUP_1F, 0);
    }
    if (mainGetBit(GAMEBIT_CC_ObjGroup1EEnabled) != 0 &&
        (u8)(*gMapEventInterface)->getObjGroupStatus(obj->anim.mapEventSlot, CC_LEVEL_CONTROL_OBJECT_GROUP_1E) == 0) {
        (*gMapEventInterface)->setObjGroupStatus(obj->anim.mapEventSlot, CC_LEVEL_CONTROL_OBJECT_GROUP_1E, 1);
    }
    if (mainGetBit(GAMEBIT_CC_ObjGroup1DEnabled) != 0 &&
        (u8)(*gMapEventInterface)->getObjGroupStatus(obj->anim.mapEventSlot, CC_LEVEL_CONTROL_OBJECT_GROUP_1D) == 0) {
        (*gMapEventInterface)->setObjGroupStatus(obj->anim.mapEventSlot, CC_LEVEL_CONTROL_OBJECT_GROUP_1D, 1);
    }
    tricky = getTrickyObject();
    if (state->gameBitLatch.activeMask & CC_LEVEL_CONTROL_LATCH_CAMERA_ACTION) {
        if (mainGetBit(GAMEBIT_CC_CameraActionStop) != 0 || mainGetBit(GAMEBIT_CC_CameraActionGate) == 0 ||
            (tricky->objectFlags & OBJECT_OBJFLAG_PARENT_SLACK) != 0) {
            state->gameBitLatch.activeMask &= ~CC_LEVEL_CONTROL_LATCH_CAMERA_ACTION;
            (*gCameraInterface)->loadTriggeredCamAction(CAMCONTROL_TRIGGER_KIND_LOAD_ACTION, 1, 0);
        }
    } else {
        if (mainGetBit(GAMEBIT_CC_CameraActionStop) == 0 && mainGetBit(GAMEBIT_CC_CameraActionReady) != 0 &&
            mainGetBit(GAMEBIT_CC_CameraActionGate) != 0 && mainGetBit(GAMEBIT_CC_CameraActionBlocked) == 0) {
            state->gameBitLatch.activeMask |= CC_LEVEL_CONTROL_LATCH_CAMERA_ACTION;
            (*gCameraInterface)->loadTriggeredCamAction(CAMCONTROL_TRIGGER_KIND_QUEUE_TYPE1, 1, 0);
        }
    }
    usedGoldBarCount = mainGetBit(GAMEBIT_ITEM_CCGoldBar_Used);
    heldGoldBarCount = mainGetBit(GAMEBIT_ITEM_CCGoldBar_Count);
    if (heldGoldBarCount + usedGoldBarCount == 4 && mainGetBit(GAMEBIT_CC_GoldBarCompletionSfxPlayed) == 0) {
        Sfx_PlayFromObject(obj, SFXTRIG_mpick1_b);
        mainSetBits(GAMEBIT_CC_GoldBarCompletionSfxPlayed, 1);
    }
}

static void ccLevelControl_init(GameObject* obj) {
    CcLevelControlState* state = obj->extra;

    obj->animEventCallback = ccLevelControl_animationEventCallback;
    skySetEnvFxRampTables(sCcLevelControlEnvFxRampTables.groupB, sCcLevelControlEnvFxRampTables.groupA,
                          sCcLevelControlEnvFxRampTables.groupC, sCcLevelControlEnvFxRampTables.groupD);
    if (getSaveGameLoadStatus() != 0) {
        skySetEnvFxFlags(0x3F);
        getEnvfxActImmediately(NULL, NULL, 0x242, 0);
    } else {
        skySetEnvFxFlags(0x1F);
        getEnvfxAct(NULL, NULL, 0x242, 0);
    }
    state->textTimer = 300.0f;
    state->musicTriggerId = -1;
    state->mapAct = (u32)(u8)(*gMapEventInterface)->getMapAct(obj->anim.mapEventSlot);
}

ObjectDescriptor gCCLevelControlObjDescriptor = {
    .slotCountAndFlags = OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    .init = (ObjectDescriptorCallback)ccLevelControl_init,
    .update = (ObjectDescriptorCallback)ccLevelControl_update,
    .render = (ObjectDescriptorCallback)ccLevelControl_render,
    .free = (ObjectDescriptorCallback)ccLevelControl_free,
    .getExtraSize = ccLevelControl_getExtraSize,
};
