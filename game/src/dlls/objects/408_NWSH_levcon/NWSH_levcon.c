#include "dlls/objects/408_NWSH_levcon.h"

#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/audio/music_api.h"
#include "main/audio/music_trigger_ids.h"
#include "main/dll/player_api.h"
#include "main/gamebit_ids.h"
#include "main/gamebits_api.h"
#include "main/mapEventTypes.h"
#include "main/map_load.h"
#include "main/object_render.h"
#include "main/objseq.h"
#include "main/pi_dolphin_api.h"
#include "main/render_envfx_api.h"
#include "main/sky_api.h"
#include "sys/objects.h"

enum NwshLevelControlMapId {
    NWSH_LEVEL_CONTROL_MAP_KRAZOA_PALACE = 0xB,
    NWSH_LEVEL_CONTROL_MAP_BOSS_SCALES = 0x28,
};

enum NwshLevelControlObjectGroup {
    NWSH_LEVEL_CONTROL_OBJECT_GROUP_04 = 0x04,
    NWSH_LEVEL_CONTROL_OBJECT_GROUP_1D = 0x1D,
    NWSH_LEVEL_CONTROL_OBJECT_GROUP_1E = 0x1E,
    NWSH_LEVEL_CONTROL_OBJECT_GROUP_1F = 0x1F,
};

enum NwshLevelControlEnvFxId {
    NWSH_LEVEL_CONTROL_ENVFX_D1 = 0xD1,
    NWSH_LEVEL_CONTROL_ENVFX_D6 = 0xD6,
    NWSH_LEVEL_CONTROL_ENVFX_222 = 0x222,
};

enum NwshLevelControlAnimEvent {
    NWSH_LEVEL_CONTROL_ANIM_EVENT_COMPLETE = 1,
};

enum NwshLevelControlMapAct {
    NWSH_LEVEL_CONTROL_MAP_ACT_SPIRIT_SIX = 6,
};

static int nwshLevelControl_processAnimEvents(GameObject* obj, int arg1, ObjSeqState* sequence) {
    GameObject* player = Obj_GetPlayerObject();

    (void)obj;
    (void)arg1;

    if (player == NULL) {
        return 0;
    }

    for (int eventIndex = 0; eventIndex < sequence->eventCount; eventIndex++) {
        if (sequence->eventIds[eventIndex] != NWSH_LEVEL_CONTROL_ANIM_EVENT_COMPLETE) {
            continue;
        }

        objSetAnimStateFlags(player, 0x10, 1);
        mainSetBits(GAMEBIT_ITEM_Spirit6_Got, 1);
        (*gMapEventInterface)
            ->setObjGroupStatus(NWSH_LEVEL_CONTROL_MAP_KRAZOA_PALACE, NWSH_LEVEL_CONTROL_OBJECT_GROUP_04, 1);
        (*gMapEventInterface)
            ->setObjGroupStatus(NWSH_LEVEL_CONTROL_MAP_KRAZOA_PALACE, NWSH_LEVEL_CONTROL_OBJECT_GROUP_1D, 1);
        (*gMapEventInterface)
            ->setObjGroupStatus(NWSH_LEVEL_CONTROL_MAP_KRAZOA_PALACE, NWSH_LEVEL_CONTROL_OBJECT_GROUP_1E, 1);
        (*gMapEventInterface)
            ->setObjGroupStatus(NWSH_LEVEL_CONTROL_MAP_KRAZOA_PALACE, NWSH_LEVEL_CONTROL_OBJECT_GROUP_1F, 1);
        (*gMapEventInterface)->setMapAct(NWSH_LEVEL_CONTROL_MAP_KRAZOA_PALACE, NWSH_LEVEL_CONTROL_MAP_ACT_SPIRIT_SIX);
    }
    return 0;
}

static int nwshLevelControl_getExtraSize(void) {
    return 0;
}

static int nwshLevelControl_getObjectTypeId(void) {
    return 0;
}

static void nwshLevelControl_free(GameObject* obj) {
    (void)obj;
    Music_Trigger(MUSICTRIG_ewt_chase, 0);
    mainSetBits(GAMEBIT_SETPIECE_ACTIVE, 0);
}

static void nwshLevelControl_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                                    s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

static void nwshLevelControl_hitDetect(void) {
}

static void nwshLevelControl_update(GameObject* obj) {
    if (obj->userData1 == 0) {
        return;
    }

    obj->userData1--;
    if (obj->userData1 == 0) {
        skySetSlotFlag80(7, 1);
        getEnvfxAct(NULL, NULL, NWSH_LEVEL_CONTROL_ENVFX_D1, 0);
        getEnvfxAct(NULL, NULL, NWSH_LEVEL_CONTROL_ENVFX_D6, 0);
        getEnvfxAct(NULL, NULL, NWSH_LEVEL_CONTROL_ENVFX_222, 0);
    }
}

static void nwshLevelControl_init(GameObject* obj, const ObjPlacement* placement) {
    (void)placement;
    obj->animEventCallback = nwshLevelControl_processAnimEvents;
    unlockLevel(mapGetDirIdx(NWSH_LEVEL_CONTROL_MAP_BOSS_SCALES), 1, 0);
    Music_Trigger(MUSICTRIG_ewt_chase, 1);
    obj->userData1 = 1;
    mainSetBits(GAMEBIT_K6_Entered, 1);
    mainSetBits(GAMEBIT_SETPIECE_ACTIVE, 1);
}

static void nwshLevelControl_release(void) {
}

static void nwshLevelControl_initialise(void) {
}

ObjectDescriptor gNWSHLevelControlObjDescriptor = {
    .slotCountAndFlags = OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    .initialise = (ObjectDescriptorCallback)nwshLevelControl_initialise,
    .release = (ObjectDescriptorCallback)nwshLevelControl_release,
    .init = (ObjectDescriptorCallback)nwshLevelControl_init,
    .update = (ObjectDescriptorCallback)nwshLevelControl_update,
    .hitDetect = (ObjectDescriptorCallback)nwshLevelControl_hitDetect,
    .render = (ObjectDescriptorCallback)nwshLevelControl_render,
    .free = (ObjectDescriptorCallback)nwshLevelControl_free,
    .getObjectTypeId = (ObjectDescriptorCallback)nwshLevelControl_getObjectTypeId,
    .getExtraSize = nwshLevelControl_getExtraSize,
};
