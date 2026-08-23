#include "dlls/objects/418_NW_tricky.h"

#include "game/objects/object.h"
#include "main/audio/sfx_ids.h"
#include "main/audio/sfx_stop_channel_api.h"
#include "main/dll/dll_00C4_tricky.h"
#include "main/dll/dll_00C9_enemy.h"
#include "main/dll/dll_80136a40.h"
#include "main/frame_timing.h"
#include "main/gamebit_ids.h"
#include "main/gamebits_api.h"
#include "main/mapEventTypes.h"
#include "main/objtype.h"
#include "main/vecmath_distance_api.h"
#include "sys/objects.h"
#include "sys/objects/lifecycle.h"

enum NwTrickyPhase {
    NW_TRICKY_PHASE_HERDING,
    NW_TRICKY_PHASE_ENERGY,
};

enum NwTrickyObjectGroup {
    NW_TRICKY_HERD_OBJECT_GROUP = 3,
};

enum NwTrickyObjectId {
    NW_TRICKY_OBJECT_SHARPCLAW_SNOW = 0x13A,
};

enum NwTrickyTalkState {
    NW_TRICKY_TALK_LOW_ENERGY = 1,
    NW_TRICKY_TALK_READY = 0xFF,
};

typedef struct NwTrickyState {
    u8 phase;
    u8 unused01[3];
    f32 phaseTimer;
} NwTrickyState;

STATIC_ASSERT(sizeof(NwTrickyState) == 0x08);
STATIC_ASSERT(offsetof(NwTrickyState, phase) == 0x00);
STATIC_ASSERT(offsetof(NwTrickyState, phaseTimer) == 0x04);

static const int sNwTrickyPlayBallTargetIds[] = {
    0xF5B,
    0x43EC9,
    0x43ED6,
};

static int nwTricky_processAnimEvents(GameObject* unusedObj, int unusedArg, ObjSeqState* unusedAnimUpdate) {
    (void)unusedObj;
    (void)unusedArg;
    (void)unusedAnimUpdate;

    Sfx_StopObjectChannel(getTrickyObject(), 16);
    return 0;
}

static int nwTricky_getExtraSize(void) {
    return sizeof(NwTrickyState);
}

static void nwTricky_free(GameObject* unusedObj) {
    (void)unusedObj;
    mainSetBits(GAMEBIT_Tricky_Usable, 1);
}

static void nwTricky_finishHerding(NwTrickyState* state, GameObject* player) {
    int herdObjectCount;
    GameObject** herdObjects = objGetAllOfType(NW_TRICKY_HERD_OBJECT_GROUP, &herdObjectCount);

    for (int i = 0; i < herdObjectCount; i++) {
        GameObject* herdObject = herdObjects[i];

        if (herdObject->anim.romDefNo == NW_TRICKY_OBJECT_SHARPCLAW_SNOW) {
            enemy_setTrackedObj(herdObject, player);
        }
    }
    mainSetBits(GAMEBIT_Tricky_Usable, 1);
    state->phase = NW_TRICKY_PHASE_ENERGY;
}

static void nwTricky_offerPlayBallTarget(GameObject* tricky, TrickyCompanionInterface* trickyInterface) {
    for (int i = 0; i < ARRAY_COUNT(sNwTrickyPlayBallTargetIds); i++) {
        GameObject* target = ObjList_FindObjectById(sNwTrickyPlayBallTargetIds[i]);

        if (target != NULL && enemy_getHealthFraction(target) > 0.0f) {
            trickyInterface->commandPlayBall(tricky, 1, target);
            break;
        }
    }
}

static void nwTricky_updateHerdTargets(GameObject* tricky, GameObject* player) {
    int herdObjectCount;
    GameObject** herdObjects = objGetAllOfType(NW_TRICKY_HERD_OBJECT_GROUP, &herdObjectCount);

    for (int i = 0; i < herdObjectCount; i++) {
        GameObject* herdObject = herdObjects[i];

        if (herdObject->anim.romDefNo == NW_TRICKY_OBJECT_SHARPCLAW_SNOW) {
            f32 playerDistanceSquared = vec3f_distanceSquared(&herdObject->anim.worldPos.x, &player->anim.worldPos.x);
            f32 trickyDistanceSquared = vec3f_distanceSquared(&herdObject->anim.worldPos.x, &tricky->anim.worldPos.x);

            enemy_setTrackedObj(herdObject, trickyDistanceSquared < playerDistanceSquared ? tricky : player);
        }
    }
}

static void nwTricky_updateHerding(NwTrickyState* state, GameObject* tricky, GameObject* player) {
    if (mainGetBit(GAMEBIT_NW_Tricky_HerdingComplete) != 0) {
        nwTricky_finishHerding(state, player);
        return;
    }

    if (mainGetBit(GAMEBIT_ITEM_TrickyStayFind_Got) != 0) {
        TrickyCompanionInterface* trickyInterface = (TrickyCompanionInterface*)*tricky->anim.dll;

        if (trickyInterface->isPlayingBall(tricky) == 0) {
            mainSetBits(GAMEBIT_Tricky_Usable, 0);
            state->phaseTimer = 0.0f;
        }

        nwTricky_offerPlayBallTarget(tricky, trickyInterface);

        state->phaseTimer += timeDelta;
        if (state->phaseTimer >= 600.0f) {
            state->phaseTimer -= 600.0f;
            trickyTryPlaySound(tricky, SFXwp_rolovr_6, 0x1000);
        }
    }

    nwTricky_updateHerdTargets(tricky, player);
}

static void nwTricky_updateEnergy(NwTrickyState* state, GameObject* tricky) {
    if ((tricky->objectFlags & OBJECT_OBJFLAG_PARENT_SLACK) == 0) {
        state->phaseTimer += timeDelta;
    }

    if (mainGetBit(GAMEBIT_TrickyTalk) == NW_TRICKY_TALK_LOW_ENERGY &&
        (*gMapEventInterface)->getTrickyStats()->energy >= 4) {
        mainSetBits(GAMEBIT_TrickyTalk, NW_TRICKY_TALK_READY);
    }

    if (state->phaseTimer >= 2000.0f) {
        state->phaseTimer -= 2000.0f;
        if (mainGetBit(GAMEBIT_TrickyTalk) == NW_TRICKY_TALK_READY && (*gMapEventInterface)->getTrickyStats()->energy < 4) {
            mainSetBits(GAMEBIT_TrickyTalk, NW_TRICKY_TALK_LOW_ENERGY);
        }
    }
}

static void nwTricky_update(GameObject* obj) {
    NwTrickyState* state = obj->extra;
    GameObject* tricky = getTrickyObject();
    GameObject* player = Obj_GetPlayerObject();

    if (tricky == NULL) {
        return;
    }

    switch (state->phase) {
    case NW_TRICKY_PHASE_HERDING:
        nwTricky_updateHerding(state, tricky, player);
        break;
    case NW_TRICKY_PHASE_ENERGY:
        nwTricky_updateEnergy(state, tricky);
        break;
    }
}

static void nwTricky_init(GameObject* obj) {
    obj->animEventCallback = nwTricky_processAnimEvents;
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN | OBJECT_OBJFLAG_HITDETECT_DISABLED;
}

ObjectDescriptor gNWTrickyObjDescriptor = {
    .reserved0 = 0,
    .reserved1 = 0,
    .reserved2 = 0,
    .slotCountAndFlags = OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    .initialise = NULL,
    .release = NULL,
    .slot02 = NULL,
    .init = (ObjectDescriptorCallback)nwTricky_init,
    .update = (ObjectDescriptorCallback)nwTricky_update,
    .hitDetect = NULL,
    .render = NULL,
    .free = (ObjectDescriptorCallback)nwTricky_free,
    .getObjectTypeId = NULL,
    .getExtraSize = nwTricky_getExtraSize,
};
