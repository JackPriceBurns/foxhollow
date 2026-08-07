/*
 * NWSH_levcon (DLL 0x198) - SnowHorn Wastes shrine level controller.
 *
 * The sole retail instance is in nwshrine. It restores the shrine's sky and
 * environment effects after loading, then grants the sixth spirit and advances
 * Krazoa Palace when its animation event fires.
 */
#include "dlls/objects/408_NWSH_levcon.h"

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

#define NWSH_LEVEL_CONTROL_KRAZOA_PALACE_MAP_ID 0xB
#define NWSH_LEVEL_CONTROL_BOSS_SCALES_MAP_ID   0x28

#define NWSH_LEVEL_CONTROL_OBJ_GROUP_04          0x04
#define NWSH_LEVEL_CONTROL_OBJ_GROUP_1D          0x1D
#define NWSH_LEVEL_CONTROL_OBJ_GROUP_1E          0x1E
#define NWSH_LEVEL_CONTROL_OBJ_GROUP_1F          0x1F
#define NWSH_LEVEL_CONTROL_OBJ_GROUP_ACTIVE      1
#define NWSH_LEVEL_CONTROL_KRAZOA_PALACE_MAP_ACT 6

#define NWSH_LEVEL_CONTROL_ENVFX_A     0xD1
#define NWSH_LEVEL_CONTROL_ENVFX_B     0xD6
#define NWSH_LEVEL_CONTROL_ENVFX_C     0x222
#define NWSH_LEVEL_CONTROL_ENVFX_FLAGS 0

#define NWSH_LEVEL_CONTROL_PLAYER_ANIM_STATE_FLAG 0x10
#define NWSH_LEVEL_CONTROL_ANIM_EVENT_COMPLETE    1
#define NWSH_LEVEL_CONTROL_LOAD_DELAY_FRAMES      1
#define NWSH_LEVEL_CONTROL_SKY_FLAGS              7
#define NWSH_LEVEL_CONTROL_SKY_ENABLED            1
#define NWSH_LEVEL_CONTROL_RENDER_SCALE           1.0f

int nwshLevelControl_processAnimEvents(GameObject* unusedObj, int unusedArg, ObjSeqState* animUpdate) {
    GameObject* player;
    int eventIndex;

    (void)unusedObj;
    (void)unusedArg;
    player = Obj_GetPlayerObject();
    if (player != NULL) {
        for (eventIndex = 0; eventIndex < animUpdate->eventCount; eventIndex++) {
            switch (animUpdate->eventIds[eventIndex]) {
            case NWSH_LEVEL_CONTROL_ANIM_EVENT_COMPLETE:
                objSetAnimStateFlags(player, NWSH_LEVEL_CONTROL_PLAYER_ANIM_STATE_FLAG, 1);
                mainSetBits(GAMEBIT_ITEM_Spirit6_Got, 1);
                (*gMapEventInterface)
                    ->setObjGroupStatus(NWSH_LEVEL_CONTROL_KRAZOA_PALACE_MAP_ID, NWSH_LEVEL_CONTROL_OBJ_GROUP_04,
                                        NWSH_LEVEL_CONTROL_OBJ_GROUP_ACTIVE);
                (*gMapEventInterface)
                    ->setObjGroupStatus(NWSH_LEVEL_CONTROL_KRAZOA_PALACE_MAP_ID, NWSH_LEVEL_CONTROL_OBJ_GROUP_1D,
                                        NWSH_LEVEL_CONTROL_OBJ_GROUP_ACTIVE);
                (*gMapEventInterface)
                    ->setObjGroupStatus(NWSH_LEVEL_CONTROL_KRAZOA_PALACE_MAP_ID, NWSH_LEVEL_CONTROL_OBJ_GROUP_1E,
                                        NWSH_LEVEL_CONTROL_OBJ_GROUP_ACTIVE);
                (*gMapEventInterface)
                    ->setObjGroupStatus(NWSH_LEVEL_CONTROL_KRAZOA_PALACE_MAP_ID, NWSH_LEVEL_CONTROL_OBJ_GROUP_1F,
                                        NWSH_LEVEL_CONTROL_OBJ_GROUP_ACTIVE);
                (*gMapEventInterface)
                    ->setMapAct(NWSH_LEVEL_CONTROL_KRAZOA_PALACE_MAP_ID, NWSH_LEVEL_CONTROL_KRAZOA_PALACE_MAP_ACT);
                break;
            default:
                break;
            }
        }
    }
    return 0;
}

int nwshLevelControl_getExtraSize(void) {
    return 0;
}

int nwshLevelControl_getObjectTypeId(void) {
    return 0;
}

void nwshLevelControl_free(GameObject* unusedObj) {
    (void)unusedObj;
    Music_Trigger(MUSICTRIG_ewt_chase, 0);
    mainSetBits(GAMEBIT_SETPIECE_ACTIVE, 0);
}

void nwshLevelControl_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                             s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5,
                                    NWSH_LEVEL_CONTROL_RENDER_SCALE);
    }
}

void nwshLevelControl_hitDetect(void) {
}

void nwshLevelControl_update(GameObject* obj) {
    if (obj->userData1 != 0) {
        obj->userData1--;
        if (obj->userData1 == 0) {
            skySetSlotFlag80(NWSH_LEVEL_CONTROL_SKY_FLAGS, NWSH_LEVEL_CONTROL_SKY_ENABLED);
            getEnvfxAct(NULL, NULL, NWSH_LEVEL_CONTROL_ENVFX_A, NWSH_LEVEL_CONTROL_ENVFX_FLAGS);
            getEnvfxAct(NULL, NULL, NWSH_LEVEL_CONTROL_ENVFX_B, NWSH_LEVEL_CONTROL_ENVFX_FLAGS);
            getEnvfxAct(NULL, NULL, NWSH_LEVEL_CONTROL_ENVFX_C, NWSH_LEVEL_CONTROL_ENVFX_FLAGS);
        }
    }
}

void nwshLevelControl_init(GameObject* obj, const void* placement) {
    (void)placement;
    obj->animEventCallback = nwshLevelControl_processAnimEvents;
    unlockLevel(mapGetDirIdx(NWSH_LEVEL_CONTROL_BOSS_SCALES_MAP_ID), 1, 0);
    Music_Trigger(MUSICTRIG_ewt_chase, 1);
    obj->userData1 = NWSH_LEVEL_CONTROL_LOAD_DELAY_FRAMES;
    mainSetBits(GAMEBIT_K6_Entered, 1);
    mainSetBits(GAMEBIT_SETPIECE_ACTIVE, 1);
}

void nwshLevelControl_release(void) {
}

void nwshLevelControl_initialise(void) {
}

ObjectDescriptor gNWSHLevelControlObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    (ObjectDescriptorCallback)nwshLevelControl_initialise,
    (ObjectDescriptorCallback)nwshLevelControl_release,
    0,
    (ObjectDescriptorCallback)nwshLevelControl_init,
    (ObjectDescriptorCallback)nwshLevelControl_update,
    (ObjectDescriptorCallback)nwshLevelControl_hitDetect,
    (ObjectDescriptorCallback)nwshLevelControl_render,
    (ObjectDescriptorCallback)nwshLevelControl_free,
    (ObjectDescriptorCallback)nwshLevelControl_getObjectTypeId,
    nwshLevelControl_getExtraSize,
};
