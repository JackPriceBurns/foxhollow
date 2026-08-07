/*
 * CClevcontro (DLL 0x18B) - Cape Claw level controller.
 *
 * Drives the area's environment effects, day/night music, gamebit-controlled
 * music latches and object groups, a triggered camera action, and the
 * four-gold-bar completion sound.
 */
#include "dlls/objects/395_CClevcontro.h"

#include "main/audio/music_api.h"
#include "main/audio/music_trigger_ids.h"
#include "main/audio/sfx_play_api.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/camera_interface.h"
#include "main/dll/savegame_load_api.h"
#include "main/frame_timing.h"
#include "main/gamebit_ids.h"
#include "main/gamebits_api.h"
#include "main/gametext_show_api.h"
#include "main/mapEventTypes.h"
#include "main/object_render.h"
#include "main/objfx.h"
#include "main/render_envfx_api.h"
#include "main/sky.h"
#include "main/sky_api.h"
#include "main/sky_interface.h"
#include "sys/objects/lifecycle.h"

#define CC_LEVEL_CONTROL_TEXT_ID       0x34C
#define CC_LEVEL_CONTROL_TEXT_DURATION 300.0f

#define CC_LEVEL_CONTROL_EXPLOSION_SCALE 50.0f
#define CC_LEVEL_CONTROL_RENDER_SCALE    1.0f

#define CC_LEVEL_CONTROL_ENVFX_ID            0x242
#define CC_LEVEL_CONTROL_ENVFX_FLAGS_LOADING 0x1F
#define CC_LEVEL_CONTROL_ENVFX_FLAGS_LOADED  0x3F

#define CC_LEVEL_CONTROL_ALIEN_MUSIC_GAMEBIT     0xB72
#define CC_LEVEL_CONTROL_DAY_NIGHT_MUSIC_GAMEBIT 0xC47
#define CC_LEVEL_CONTROL_BLIZZARD_MUSIC_GAMEBIT  0xB45
#define CC_LEVEL_CONTROL_MUSIC_BF_GAMEBIT        0xB73
#define CC_LEVEL_CONTROL_MUSIC_C0_GAMEBIT        0xB24
#define CC_LEVEL_CONTROL_MUSIC_CD_GAMEBIT        0x19E
#define CC_LEVEL_CONTROL_MUSIC_EA_GAMEBIT        0x24

#define CC_LEVEL_CONTROL_MUSIC_TRIGGER_BF 0xBF
#define CC_LEVEL_CONTROL_MUSIC_TRIGGER_C0 0xC0
#define CC_LEVEL_CONTROL_MUSIC_TRIGGER_CD 0xCD
#define CC_LEVEL_CONTROL_MUSIC_TRIGGER_EA 0xEA

#define CC_LEVEL_CONTROL_LATCH_FLAG_CAMERA_ACTION 0x01
#define CC_LEVEL_CONTROL_LATCH_FLAG_ALIEN_MUSIC   0x02
#define CC_LEVEL_CONTROL_LATCH_FLAG_BLIZZARD      0x04
#define CC_LEVEL_CONTROL_LATCH_FLAG_MUSIC_BF      0x08
#define CC_LEVEL_CONTROL_LATCH_FLAG_MUSIC_C0      0x10
#define CC_LEVEL_CONTROL_LATCH_FLAG_DAY_NIGHT     0x20
#define CC_LEVEL_CONTROL_LATCH_FLAG_MUSIC_CD      0x40
#define CC_LEVEL_CONTROL_LATCH_FLAG_MUSIC_EA      0x80

#define CC_LEVEL_CONTROL_MAP_ACT_MUSIC_EA 2

#define CC_LEVEL_CONTROL_GROUP_1F_DISABLED_GAMEBIT 0x3D6
#define CC_LEVEL_CONTROL_GROUP_1E_ENABLED_GAMEBIT  0x161
#define CC_LEVEL_CONTROL_GROUP_1D_ENABLED_GAMEBIT  0x3D7
#define CC_LEVEL_CONTROL_OBJECT_GROUP_1F           0x1F
#define CC_LEVEL_CONTROL_OBJECT_GROUP_1E           0x1E
#define CC_LEVEL_CONTROL_OBJECT_GROUP_1D           0x1D

#define CC_LEVEL_CONTROL_CAMERA_STOP_GAMEBIT    0x22D
#define CC_LEVEL_CONTROL_CAMERA_READY_GAMEBIT   0x22A
#define CC_LEVEL_CONTROL_CAMERA_GATE_GAMEBIT    0x22E
#define CC_LEVEL_CONTROL_CAMERA_BLOCKED_GAMEBIT 0x160
#define CC_LEVEL_CONTROL_CAMERA_ACTION_STOP     0
#define CC_LEVEL_CONTROL_CAMERA_ACTION_START    1

#define CC_LEVEL_CONTROL_GOLD_BAR_TARGET                 4
#define CC_LEVEL_CONTROL_GOLD_BAR_COMPLETION_SFX_GAMEBIT 0xF26

SkyEnvFxRampTables gCCLevelControlEnvFxRampTables = {
    {0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241,
     0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241, 0x0241},
    {0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F,
     0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F, 0x023F},
    {0x0240, 0x0240, 0x0240, 0x0243, 0x0240, 0x0243, 0x0243, 0x0240, 0x0240, 0x0240, 0x0240, 0x0240, 0x0240, 0x0240,
     0x0240, 0x0240, 0x0243, 0x0243, 0x0243, 0x0240, 0x0240, 0x0240, 0x0240, 0x0240, 0x0240, 0x0240, 0x0240, 0x0240},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
};

int ccLevelControl_animationEventCallback(GameObject* obj, int unusedArg, ObjSeqState* animUpdate) {
    if (animUpdate->eventCount != 0) {
        spawnExplosion(obj, CC_LEVEL_CONTROL_EXPLOSION_SCALE, 1, 1, 0, 1, 1, 1, 0);
    }
    return 0;
}

int ccLevelControl_getExtraSize(void) {
    return sizeof(CCLevelControlState);
}

void ccLevelControl_free(void) {
    skySetEnvFxFlags(0);
    Music_Trigger(MUSICTRIG_Arwing_Crash, 0);
}

void ccLevelControl_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                           s8 unusedVisible) {
    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, CC_LEVEL_CONTROL_RENDER_SCALE);
}

void ccLevelControl_update(GameObject* obj) {
    CCLevelControlState* state = obj->extra;
    GameObject* tricky;
    u32 usedGoldBarCount;
    u32 heldGoldBarCount;

    if (state->textTimer > 0.0f) {
        gameTextShow(CC_LEVEL_CONTROL_TEXT_ID);
        state->textTimer -= timeDelta;
        if (state->textTimer < 0.0f) {
            state->textTimer = 0.0f;
        }
    }
    if ((*gSkyInterface)->getSunPosition(NULL) != 0) {
        if (state->musicTriggerId != -1) {
            state->musicTriggerId = -1;
            if (state->gameBitLatch.activeMask & CC_LEVEL_CONTROL_LATCH_FLAG_DAY_NIGHT) {
                Music_Trigger(MUSICTRIG_Arwing_Crash, 0);
            }
        }
    } else {
        if (state->musicTriggerId != MUSICTRIG_Arwing_Crash) {
            state->musicTriggerId = MUSICTRIG_Arwing_Crash;
            if (state->gameBitLatch.activeMask & CC_LEVEL_CONTROL_LATCH_FLAG_DAY_NIGHT) {
                Music_Trigger(MUSICTRIG_Arwing_Crash, 1);
            }
        }
    }
    GameBitLatch_Update(&state->gameBitLatch, CC_LEVEL_CONTROL_LATCH_FLAG_ALIEN_MUSIC, -1, -1,
                          CC_LEVEL_CONTROL_ALIEN_MUSIC_GAMEBIT, MUSICTRIG_mmpassalien);
    GameBitLatch_Update(&state->gameBitLatch, CC_LEVEL_CONTROL_LATCH_FLAG_DAY_NIGHT, -1, -1,
                          CC_LEVEL_CONTROL_DAY_NIGHT_MUSIC_GAMEBIT, state->musicTriggerId);
    GameBitLatch_Update(&state->gameBitLatch, CC_LEVEL_CONTROL_LATCH_FLAG_BLIZZARD, -1, -1,
                          CC_LEVEL_CONTROL_BLIZZARD_MUSIC_GAMEBIT, MUSICTRIG_blizzard);
    GameBitLatch_Update(&state->gameBitLatch, CC_LEVEL_CONTROL_LATCH_FLAG_MUSIC_BF, -1, -1,
                          CC_LEVEL_CONTROL_MUSIC_BF_GAMEBIT, CC_LEVEL_CONTROL_MUSIC_TRIGGER_BF);
    GameBitLatch_Update(&state->gameBitLatch, CC_LEVEL_CONTROL_LATCH_FLAG_MUSIC_C0, -1, -1,
                          CC_LEVEL_CONTROL_MUSIC_C0_GAMEBIT, CC_LEVEL_CONTROL_MUSIC_TRIGGER_C0);
    GameBitLatch_Update(&state->gameBitLatch, CC_LEVEL_CONTROL_LATCH_FLAG_MUSIC_CD, -1, -1,
                          CC_LEVEL_CONTROL_MUSIC_CD_GAMEBIT, CC_LEVEL_CONTROL_MUSIC_TRIGGER_CD);
    if (state->mapAct == CC_LEVEL_CONTROL_MAP_ACT_MUSIC_EA) {
        GameBitLatch_UpdateInverted(&state->gameBitLatch, CC_LEVEL_CONTROL_LATCH_FLAG_MUSIC_EA, -1, -1,
                                      CC_LEVEL_CONTROL_MUSIC_EA_GAMEBIT, CC_LEVEL_CONTROL_MUSIC_TRIGGER_EA);
    }
    if (mainGetBit(CC_LEVEL_CONTROL_GROUP_1F_DISABLED_GAMEBIT) != 0 &&
        (u8)(*gMapEventInterface)->getObjGroupStatus(obj->anim.mapEventSlot, CC_LEVEL_CONTROL_OBJECT_GROUP_1F) != 0) {
        (*gMapEventInterface)->setObjGroupStatus(obj->anim.mapEventSlot, CC_LEVEL_CONTROL_OBJECT_GROUP_1F, 0);
    }
    if (mainGetBit(CC_LEVEL_CONTROL_GROUP_1E_ENABLED_GAMEBIT) != 0 &&
        (u8)(*gMapEventInterface)->getObjGroupStatus(obj->anim.mapEventSlot, CC_LEVEL_CONTROL_OBJECT_GROUP_1E) == 0) {
        (*gMapEventInterface)->setObjGroupStatus(obj->anim.mapEventSlot, CC_LEVEL_CONTROL_OBJECT_GROUP_1E, 1);
    }
    if (mainGetBit(CC_LEVEL_CONTROL_GROUP_1D_ENABLED_GAMEBIT) != 0 &&
        (u8)(*gMapEventInterface)->getObjGroupStatus(obj->anim.mapEventSlot, CC_LEVEL_CONTROL_OBJECT_GROUP_1D) == 0) {
        (*gMapEventInterface)->setObjGroupStatus(obj->anim.mapEventSlot, CC_LEVEL_CONTROL_OBJECT_GROUP_1D, 1);
    }
    tricky = getTrickyObject();
    if (state->gameBitLatch.activeMask & CC_LEVEL_CONTROL_LATCH_FLAG_CAMERA_ACTION) {
        if (mainGetBit(CC_LEVEL_CONTROL_CAMERA_STOP_GAMEBIT) != 0 ||
            mainGetBit(CC_LEVEL_CONTROL_CAMERA_GATE_GAMEBIT) == 0 ||
            (tricky->objectFlags & OBJECT_OBJFLAG_PARENT_SLACK) != 0) {
            state->gameBitLatch.activeMask &= ~CC_LEVEL_CONTROL_LATCH_FLAG_CAMERA_ACTION;
            (*gCameraInterface)->loadTriggeredCamAction(CC_LEVEL_CONTROL_CAMERA_ACTION_STOP, 1, 0);
        }
    } else {
        if (mainGetBit(CC_LEVEL_CONTROL_CAMERA_STOP_GAMEBIT) == 0 &&
            mainGetBit(CC_LEVEL_CONTROL_CAMERA_READY_GAMEBIT) != 0 &&
            mainGetBit(CC_LEVEL_CONTROL_CAMERA_GATE_GAMEBIT) != 0 &&
            mainGetBit(CC_LEVEL_CONTROL_CAMERA_BLOCKED_GAMEBIT) == 0) {
            state->gameBitLatch.activeMask |= CC_LEVEL_CONTROL_LATCH_FLAG_CAMERA_ACTION;
            (*gCameraInterface)->loadTriggeredCamAction(CC_LEVEL_CONTROL_CAMERA_ACTION_START, 1, 0);
        }
    }
    usedGoldBarCount = mainGetBit(GAMEBIT_ITEM_CCGoldBar_Used);
    heldGoldBarCount = mainGetBit(GAMEBIT_ITEM_CCGoldBar_Count);
    if (heldGoldBarCount + usedGoldBarCount == CC_LEVEL_CONTROL_GOLD_BAR_TARGET &&
        mainGetBit(CC_LEVEL_CONTROL_GOLD_BAR_COMPLETION_SFX_GAMEBIT) == 0) {
        Sfx_PlayFromObject(obj, SFXTRIG_mpick1_b);
        mainSetBits(CC_LEVEL_CONTROL_GOLD_BAR_COMPLETION_SFX_GAMEBIT, 1);
    }
}

void ccLevelControl_init(GameObject* obj) {
    void* envFxRampTables;
    CCLevelControlState* state;

    envFxRampTables = &gCCLevelControlEnvFxRampTables;
    state = obj->extra;
    obj->animEventCallback = ccLevelControl_animationEventCallback;
    skySetEnvFxRampTables((u8*)envFxRampTables + offsetof(SkyEnvFxRampTables, groupB), envFxRampTables,
                          (u8*)envFxRampTables + offsetof(SkyEnvFxRampTables, groupC),
                          (u8*)envFxRampTables + offsetof(SkyEnvFxRampTables, groupD));
    if (getSaveGameLoadStatus() != 0) {
        skySetEnvFxFlags(CC_LEVEL_CONTROL_ENVFX_FLAGS_LOADED);
        getEnvfxActImmediately(NULL, NULL, CC_LEVEL_CONTROL_ENVFX_ID, 0);
    } else {
        skySetEnvFxFlags(CC_LEVEL_CONTROL_ENVFX_FLAGS_LOADING);
        getEnvfxAct(NULL, NULL, CC_LEVEL_CONTROL_ENVFX_ID, 0);
    }
    state->textTimer = CC_LEVEL_CONTROL_TEXT_DURATION;
    state->musicTriggerId = -1;
    state->mapAct = (u32)(u8)(*gMapEventInterface)->getMapAct(obj->anim.mapEventSlot);
}

ObjectDescriptor gCCLevelControlObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    0,
    0,
    0,
    (ObjectDescriptorCallback)ccLevelControl_init,
    (ObjectDescriptorCallback)ccLevelControl_update,
    0,
    (ObjectDescriptorCallback)ccLevelControl_render,
    (ObjectDescriptorCallback)ccLevelControl_free,
    0,
    ccLevelControl_getExtraSize,
};
