#include "dlls/objects/371_LINK_levcon.h"

#include "main/audio/music_api.h"
#include "main/audio/music_trigger_ids.h"
#include "main/dll/savegame_load_api.h"
#include "main/gamebits_api.h"
#include "main/lightmap_api.h"
#include "main/render_envfx_api.h"
#include "main/sky_api.h"
#include "main/sky_interface.h"
#include "sys/objects.h"

#define LINK_LEVEL_CONTROL_ENVFX_A 0x13E
#define LINK_LEVEL_CONTROL_ENVFX_B 0x140
#define LINK_LEVEL_CONTROL_ENVFX_C 0x13F

#define LINK_LEVEL_CONTROL_ENVFX_LOADED_VALUE  0x3F
#define LINK_LEVEL_CONTROL_ENVFX_LOADING_VALUE 0x1F

enum {
    LINK_LEVEL_CONTROL_AREA_CELL_45 = 0x45,
    LINK_LEVEL_CONTROL_AREA_CELL_46 = 0x46,
    LINK_LEVEL_CONTROL_AREA_CELL_47 = 0x47,
    LINK_LEVEL_CONTROL_AREA_CELL_48 = 0x48,
    LINK_LEVEL_CONTROL_AREA_CELL_49 = 0x49,
};

enum {
    LINK_LEVEL_CONTROL_SAVE_STATUS_FRESH = 1,
    LINK_LEVEL_CONTROL_SAVE_STATUS_LOADED = 2,
};

enum {
    LINK_GAMEBIT_AREA_48_DISABLED = 0xE1E,
    LINK_GAMEBIT_AREA_48_ALIEN_MUSIC = 0xB72,
};

SkyEnvFxRampTables gLINKLevelControlEnvFxRampTables = {
    {
        0x01B3, 0x0061, 0x0061, 0x0061, 0x01B3, 0x01B6, 0x01B9, 0x01B9, 0x01B3, 0x0061, 0x0061, 0x0061, 0x0061, 0x0061,
        0x01B6, 0x01B9, 0x01B9, 0x01B9, 0x01B3, 0x0061, 0x0061, 0x0061, 0x0061, 0x0061, 0x01B3, 0x0061, 0x0061, 0x0061,
    },
    {
        0x01B2, 0x005F, 0x005F, 0x005F, 0x01B2, 0x01B5, 0x01B8, 0x01B8, 0x01B2, 0x005F, 0x005F, 0x005F, 0x005F, 0x005F,
        0x01B5, 0x01B8, 0x01B8, 0x01B8, 0x01B2, 0x005F, 0x005F, 0x005F, 0x005F, 0x005F, 0x01B2, 0x005F, 0x005F, 0x005F,
    },
    {
        0x01B4, 0x0060, 0x0060, 0x0060, 0x01B4, 0x01B7, 0x01BA, 0x01BA, 0x01B4, 0x0060, 0x0060, 0x0060, 0x0060, 0x0060,
        0x01B7, 0x01BA, 0x01BA, 0x01BA, 0x01B4, 0x0060, 0x0060, 0x0060, 0x0060, 0x0060, 0x01B4, 0x0060, 0x0060, 0x0060,
    },
    {
        -1, -1,     -1, -1,     -1, -1, 0x01A8, -1, -1, -1, -1, -1, -1, -1,
        -1, 0x01A8, -1, 0x01A8, -1, -1, -1,     -1, -1, -1, -1, -1, -1, -1,
    },
};

void linkLevelControl_updateAreaMusic(GameObject* obj) {
    LINKLevelControlState* state = obj->extra;

    switch (obj->anim.mapEventSlot) {
    case LINK_LEVEL_CONTROL_AREA_CELL_47:
        if ((*gSkyInterface)->getSunPosition(NULL) != 0) {
            if (state->musicTriggerId != MUSICTRIG_PU1_Mysterious) {
                state->musicTriggerId = MUSICTRIG_PU1_Mysterious;
                Music_Trigger(MUSICTRIG_PU1_Mysterious, 1);
            }
        } else if (state->musicTriggerId != MUSICTRIG_KP_Text) {
            state->musicTriggerId = MUSICTRIG_KP_Text;
            Music_Trigger(MUSICTRIG_KP_Text, 1);
        }
        break;
    case LINK_LEVEL_CONTROL_AREA_CELL_48:
        if (mainGetBit(LINK_GAMEBIT_AREA_48_DISABLED) == 0) {
            if (mainGetBit(LINK_GAMEBIT_AREA_48_ALIEN_MUSIC) != 0) {
                if (state->musicTriggerId != MUSICTRIG_mmpassalien) {
                    state->musicTriggerId = MUSICTRIG_mmpassalien;
                    Music_Trigger(MUSICTRIG_mmpassalien, 1);
                }
            } else if ((*gSkyInterface)->getSunPosition(NULL) != 0) {
                if (state->musicTriggerId != MUSICTRIG_PU1_Mysterious) {
                    state->musicTriggerId = MUSICTRIG_PU1_Mysterious;
                    Music_Trigger(MUSICTRIG_PU1_Mysterious, 1);
                }
            } else if (state->musicTriggerId != MUSICTRIG_KP_Text) {
                state->musicTriggerId = MUSICTRIG_KP_Text;
                Music_Trigger(MUSICTRIG_KP_Text, 1);
            }
        }
        GameBitLatch_Update(&state->musicLatch, 1, -1, -1, LINK_GAMEBIT_AREA_48_DISABLED, MUSICTRIG_Teleport);
        break;
    }
}

void linkLevelControl_applyEnterAreaEffects(GameObject* obj) {
    u8* envFxRampBase = (u8*)&gLINKLevelControlEnvFxRampTables;

    switch (obj->anim.mapEventSlot) {
    case LINK_LEVEL_CONTROL_AREA_CELL_47:
        skySetEnvFxRampTables(envFxRampBase + 0x38, envFxRampBase, envFxRampBase + 0x70, envFxRampBase + 0xA8);
        if (obj->userData1 == LINK_LEVEL_CONTROL_SAVE_STATUS_LOADED) {
            skySetEnvFxFlags(LINK_LEVEL_CONTROL_ENVFX_LOADED_VALUE);
        } else {
            skySetEnvFxFlags(LINK_LEVEL_CONTROL_ENVFX_LOADING_VALUE);
        }
        Music_Trigger(MUSICTRIG_cldrnr_walkabout, 0);
        Music_Trigger(MUSICTRIG_CRF_Swim, 0);
        Music_Trigger(MUSICTRIG_wind_ambi, 0);
        Music_Trigger(MUSICTRIG_mammoth_walk_db, 0);
        Music_Trigger(MUSICTRIG_LVF_Tracking_f2, 0);
        break;
    case LINK_LEVEL_CONTROL_AREA_CELL_45:
        skySetSlotFlag80(7, 0);
        skySetEnvFxFlags(0);
        getEnvfxAct(NULL, NULL, LINK_LEVEL_CONTROL_ENVFX_A, 0);
        getEnvfxAct(NULL, NULL, LINK_LEVEL_CONTROL_ENVFX_B, 0);
        getEnvfxAct(NULL, NULL, LINK_LEVEL_CONTROL_ENVFX_C, 0);
        Music_Trigger(MUSICTRIG_underwater, 1);
        break;
    case LINK_LEVEL_CONTROL_AREA_CELL_49:
        Music_Trigger(MUSICTRIG_Teleport, 1);
        break;
    case LINK_LEVEL_CONTROL_AREA_CELL_48:
        Music_Trigger(MUSICTRIG_Arwing_Crash, 0);
        break;
    case LINK_LEVEL_CONTROL_AREA_CELL_46:
        Music_Trigger(MUSICTRIG_ice_race, 0);
        Music_Trigger(MUSICTRIG_citytombs, 1);
        break;
    }
}

int linkLevelControl_getExtraSize(void) {
    return sizeof(LINKLevelControlState);
}

void linkLevelControl_free(GameObject* obj) {
    switch ((s32)obj->anim.mapEventSlot) {
    case LINK_LEVEL_CONTROL_AREA_CELL_45:
        Music_Trigger(MUSICTRIG_underwater, 0);
        break;
    case LINK_LEVEL_CONTROL_AREA_CELL_48:
    case LINK_LEVEL_CONTROL_AREA_CELL_49:
        Music_Trigger(MUSICTRIG_Teleport, 0);
        break;
    }
}

void linkLevelControl_update(GameObject* obj) {
    LINKLevelControlState* state = obj->extra;
    GameObject* player = Obj_GetPlayerObject();

    if (player == NULL) {
        return;
    }

    if ((s32)state->previousPlayerAreaCell != (s32)obj->anim.mapEventSlot) {
        if ((s32)obj->anim.mapEventSlot == coordsToMapCell(player->anim.localPosX, player->anim.localPosZ)) {
            linkLevelControl_applyEnterAreaEffects(obj);
        } else {
            return;
        }
    }
    if ((s32)obj->anim.mapEventSlot == coordsToMapCell(player->anim.localPosX, player->anim.localPosZ)) {
        linkLevelControl_updateAreaMusic(obj);
    }
    state->previousPlayerAreaCell = coordsToMapCell(player->anim.localPosX, player->anim.localPosZ);
}

void linkLevelControl_init(GameObject* obj) {
    LINKLevelControlState* state = obj->extra;

    state->previousPlayerAreaCell = -1;
    state->unknown04 = -1;
    state->musicTriggerId = -1;
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN;
    if (getSaveGameLoadStatus() != 0) {
        obj->userData1 = LINK_LEVEL_CONTROL_SAVE_STATUS_LOADED;
    } else {
        obj->userData1 = LINK_LEVEL_CONTROL_SAVE_STATUS_FRESH;
    }
}

ObjectDescriptor gLINKLevelControlObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    0,
    0,
    0,
    (ObjectDescriptorCallback)linkLevelControl_init,
    (ObjectDescriptorCallback)linkLevelControl_update,
    0,
    0,
    (ObjectDescriptorCallback)linkLevelControl_free,
    0,
    linkLevelControl_getExtraSize,
};
