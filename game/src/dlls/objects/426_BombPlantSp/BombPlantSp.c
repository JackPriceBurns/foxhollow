#include "dlls/objects/426_BombPlantSp.h"

#include "dolphin/math.h"
#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/audio/sfx.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/dll/curves_collision_state.h"
#include "main/dll/partfx_interface.h"
#include "main/dll/path_control_interface.h"
#include "main/dll_000A_expgfx.h"
#include "main/frame_timing.h"
#include "main/gamebit_ids.h"
#include "main/gameloop_gamebit.h"
#include "main/model_light.h"
#include "main/obj_message.h"
#include "main/objfx.h"
#include "main/objhits.h"
#include "main/vecmath.h"
#include "sys/objects.h"
#include "sys/objects/lifecycle.h"

enum BombPlantSporeFlag {
    BOMB_PLANT_SPORE_HIT_SURFACE = 1 << 0,
    BOMB_PLANT_SPORE_WAITING_FOR_PICKUP = 1 << 1,
};

enum BombPlantSporePathConfig {
    BOMB_PLANT_SPORE_PATH_CONTACT_MASK = 0x11,
    BOMB_PLANT_SPORE_PATH_FLAGS = 0x40002,
    BOMB_PLANT_SPORE_PATH_PARAM = 5,
};

enum BombPlantSporeParticleEffect {
    BOMB_PLANT_SPORE_SPAWN_EFFECT = 0x3F1,
    BOMB_PLANT_SPORE_EXPLOSION_EFFECT = 0x3F3,
};

enum BombPlantSporeObjectId {
    BOMB_PLANT_SPORE_OBJECT_ID = 0x198,
    BOMB_PLANT_ALIAS_ID = 0x36D,
    GROUND_QUAKE_ALIAS_ID = 0x63C,
};

typedef struct BombPlantSporePlacement {
    ObjPlacement base;
    u8 unused18[2];
    s16 angleSpread;
    s16 baseAngle;
    u8 unused1E[6];
} BombPlantSporePlacement;

typedef struct BombPlantSporeState {
    ObjPickupOffer pickupOffer;
    f32 pickupMessageDelay;
    CurvesCollisionState path;
    ModelLightStruct* light;
    f32 fuseTimer;
    f32 driftAmplitude;
    f32 driftSpeed;
    f32 driftAmplitudeTarget;
    f32 driftTimer;
    f32 driftBaseX;
    f32 driftBaseZ;
    f32 driftSin;
    f32 driftCos;
    f32 spinTimer;
    f32 driftSpeedTarget;
    f32 spinChangeTimer;
    f32 detonateTimer;
    s16 currentSpinAngle;
    s16 burstDriftAngle;
    s16 spinAngle;
    s16 yawStep;
    u8 flags;
    u8 unusedTail[3];
} BombPlantSporeState;

STATIC_ASSERT(sizeof(BombPlantSporePlacement) == 0x24);
STATIC_ASSERT(offsetof(BombPlantSporePlacement, angleSpread) == 0x1A);
STATIC_ASSERT(offsetof(BombPlantSporePlacement, baseAngle) == 0x1C);

STATIC_ASSERT(offsetof(BombPlantSporeState, pickupOffer) == 0x00);
STATIC_ASSERT(offsetof(BombPlantSporeState, pickupMessageDelay) == 0x04);
STATIC_ASSERT(offsetof(BombPlantSporeState, path) == 0x08);
STATIC_ASSERT(offsetof(BombPlantSporeState, light) == 0x08 + sizeof(CurvesCollisionState));
STATIC_ASSERT(offsetof(BombPlantSporeState, fuseTimer) == offsetof(BombPlantSporeState, light) + sizeof(void*));
STATIC_ASSERT(offsetof(BombPlantSporeState, flags) == offsetof(BombPlantSporeState, currentSpinAngle) + 8);

static f32 sBombPlantSporePathPoint[] = {0.0f, 0.0f, 0.0f};
static u8 sBombPlantSporePathSetup[] = {0x40, 0xA0, 0, 0, 0, 0, 0, 0};

extern f32 gBombPlantSporeLightAttenuationNear;
extern const f32 gBombPlantSporeLightAttenuationFar;

static s16 bombPlantSpore_clampAngle(s16 angle, s16 baseAngle, s16 angleSpread) {
    s32 angleDelta = (s32)angle - (u16)baseAngle;

    if (angleDelta > 0x8000) {
        angleDelta -= 0xFFFF;
    }
    if (angleDelta < -0x8000) {
        angleDelta += 0xFFFF;
    }
    if (angleDelta > angleSpread) {
        angle = (s16)(baseAngle + angleSpread);
    }
    if (angleDelta < -(s32)angleSpread) {
        angle = (s16)(baseAngle - angleSpread);
    }

    return angle;
}

static void bombPlantSpore_startDriftBurst(GameObject* obj, BombPlantSporeState* state) {
    const BombPlantSporePlacement* placement = (BombPlantSporePlacement*)obj->anim.placementData;
    s16 baseAngle = ObjAnim_ReadPlacementS16(&obj->anim, &placement->baseAngle);
    s16 angleSpread = ObjAnim_ReadPlacementS16(&obj->anim, &placement->angleSpread);

    state->spinTimer = (f32)randomGetRange(0x1E, 0x2D);
    state->driftTimer = state->spinTimer + (f32)randomGetRange(0x78, 0xB4);
    state->burstDriftAngle =
        bombPlantSpore_clampAngle((s16)(state->currentSpinAngle + randomGetRange(-2000, 2000)), baseAngle, angleSpread);
    state->driftSpeedTarget = (f32)randomGetRange(900, 0x514) / 1000.0f;
    state->driftSpeed = 0.0f;
    state->driftSin = mathSinf((3.1415927f * (f32)state->burstDriftAngle) / 32768.0f);
    state->driftCos = mathCosf((3.1415927f * (f32)state->burstDriftAngle) / 32768.0f);
}

static void bombPlantSpore_updateDrift(GameObject* obj, BombPlantSporeState* state) {
    const BombPlantSporePlacement* placement = (BombPlantSporePlacement*)obj->anim.placementData;
    s16 baseAngle = ObjAnim_ReadPlacementS16(&obj->anim, &placement->baseAngle);
    s16 angleSpread = ObjAnim_ReadPlacementS16(&obj->anim, &placement->angleSpread);

    if (randomGetRange(0, 100) < 10 && state->spinChangeTimer <= 0.0f) {
        state->spinAngle = (s16)randomGetRange(2000, 4000);
        if (randomGetRange(0, 1) != 0) {
            state->spinAngle = -state->spinAngle;
        }
        state->spinAngle =
            bombPlantSpore_clampAngle((s16)(state->spinAngle + state->currentSpinAngle), baseAngle, angleSpread);
        state->spinChangeTimer = 150.0f;
    }

    if (randomGetRange(0, 100) < 10 && state->spinChangeTimer <= 0.0f) {
        state->driftAmplitudeTarget = state->driftAmplitude + (f32)randomGetRange(-200, 200) / 1000.0f;
        if (state->driftAmplitudeTarget < 0.5f) {
            state->driftAmplitudeTarget = 0.5f;
        } else if (state->driftAmplitudeTarget > 1.0f) {
            state->driftAmplitudeTarget = 1.0f;
        }
    }

    s32 angleDelta = (s32)state->spinAngle - (u16)state->currentSpinAngle;
    if (angleDelta > 0x8000) {
        angleDelta -= 0xFFFF;
    }
    if (angleDelta < -0x8000) {
        angleDelta += 0xFFFF;
    }
    state->currentSpinAngle += (angleDelta * framesThisStep) >> 4;

    f32 amplitude = state->driftAmplitude;
    f32 amplitudeStep = (state->driftAmplitudeTarget - amplitude) * 0.006f;
    state->driftAmplitude = amplitudeStep * timeDelta + amplitude;
    state->driftBaseX = state->driftAmplitude * mathSinf((3.1415927f * (f32)state->currentSpinAngle) / 32768.0f);
    state->driftBaseZ = state->driftAmplitude * mathCosf((3.1415927f * (f32)state->currentSpinAngle) / 32768.0f);
}

static void bombPlantSpore_beginDetonation(GameObject* obj, BombPlantSporeState* state) {
    (*gExpgfxInterface)->freeSource((uintptr_t)obj);
    for (int i = 0; i < 10; i++) {
        objfx_spawnDirectionalBurst(obj, 5, 1.0f, 7, 1, 0x3C, 1.5f, NULL, 0);
        (*gPartfxInterface)->spawnObject(obj, BOMB_PLANT_SPORE_EXPLOSION_EFFECT, NULL, PARTFXFLAG_4, -1, NULL);
    }
    modelLightStruct_setEnabled(state->light, 0, 0.5f);
    state->detonateTimer = 200.0f;
    obj->anim.flags |= OBJANIM_FLAG_HIDDEN;
    ObjHits_DisableObject(obj);
}

static int bombPlantSpore_processPickup(GameObject* obj, BombPlantSporeState* state) {
    if (!(state->flags & BOMB_PLANT_SPORE_WAITING_FOR_PICKUP)) {
        return 0;
    }

    u32 message;
    uintptr_t sender;
    while (ObjMsg_Pop(obj, &message, &sender, NULL) != 0) {
        if (message == OBJ_MESSAGE_PICKUP_COMPLETE) {
            gameBitIncrement(GAMEBIT_ITEM_BombSpore_Count);
            Sfx_PlayFromObject(obj, SFXTRIG_sc_gemrun0122);
            bombPlantSpore_beginDetonation(obj, state);
            state->flags &= ~BOMB_PLANT_SPORE_WAITING_FOR_PICKUP;
        }
    }

    return (state->flags & BOMB_PLANT_SPORE_WAITING_FOR_PICKUP) != 0;
}

static int bombPlantSpore_updateDetonation(GameObject* obj, BombPlantSporeState* state) {
    if (state->detonateTimer == 0.0f) {
        return 0;
    }

    obj->anim.rotX += framesThisStep * 0x40;
    state->detonateTimer -= timeDelta;
    if (state->detonateTimer <= 0.0f) {
        Obj_FreeObject(obj);
    }
    return 1;
}

static void bombPlantSpore_updateFuseEffect(GameObject* obj, const BombPlantSporeState* state) {
    if (state->fuseTimer < 120.0f) {
        s32 particleAlpha = (s32)(30.0f - 0.25f * state->fuseTimer);
        f32 particleScale = (f32)(0.041 * (double)(120.0f - state->fuseTimer) + 1.5);
        objfx_spawnDirectionalBurst(obj, 5, 1.0f, 7, 1, particleAlpha & 0xFF, particleScale, NULL, 0);
    }
}

static void bombPlantSpore_markSurfaceHit(BombPlantSporeState* state) {
    state->flags |= BOMB_PLANT_SPORE_HIT_SURFACE;
    if (state->fuseTimer > 120.0f) {
        state->fuseTimer = 120.0f;
    }
}

static void bombPlantSpore_updateMotion(GameObject* obj, BombPlantSporeState* state, GameObject* contactObj) {
    state->driftTimer -= timeDelta;
    if (state->driftTimer < 0.0f) {
        state->driftTimer = 0.0f;
    }
    state->spinChangeTimer -= timeDelta;
    if (state->spinChangeTimer < 0.0f) {
        state->spinChangeTimer = 0.0f;
    }

    obj->anim.rotX += state->yawStep;
    obj->anim.velocityY += -0.009f * timeDelta;
    if (obj->anim.velocityY < -0.2f) {
        obj->anim.velocityY = -0.2f;
    }
    if (obj->anim.velocityY > 0.0f) {
        obj->anim.velocityY *= 0.97f;
    }
    if (obj->anim.velocityY < 0.0f) {
        ObjHits_EnableObject(obj);
    }

    bombPlantSpore_updateDrift(obj, state);
    if (randomGetRange(0, 100) < 5 && state->driftTimer <= 0.0f) {
        bombPlantSpore_startDriftBurst(obj, state);
    }

    state->spinTimer -= timeDelta;
    if (state->spinTimer <= 0.0f) {
        state->driftSin *= 0.97f;
        state->driftCos *= 0.97f;
        state->spinTimer = 0.0f;
    } else {
        f32 driftSpeed = state->driftSpeed;
        f32 driftStep = (state->driftSpeedTarget - driftSpeed) * 0.01f;
        state->driftSpeed = driftStep * timeDelta + driftSpeed;
    }

    obj->anim.velocityX = state->driftSin * state->driftSpeed + state->driftBaseX;
    obj->anim.velocityZ = state->driftCos * state->driftSpeed + state->driftBaseZ;
    objMove(obj, obj->anim.velocityX * timeDelta, obj->anim.velocityY * timeDelta, obj->anim.velocityZ * timeDelta);
    (*gPathControlInterface)->update(obj, &state->path, timeDelta);
    (*gPathControlInterface)->apply(obj, &state->path);
    (*gPathControlInterface)->advance(obj, &state->path, timeDelta);

    if (contactObj != NULL && contactObj->anim.romDefNo != BOMB_PLANT_ALIAS_ID &&
        contactObj->anim.romDefNo != BOMB_PLANT_SPORE_OBJECT_ID && contactObj->anim.romDefNo != GROUND_QUAKE_ALIAS_ID) {
        Sfx_PlayFromObject(obj, SFXTRIG_sc_eatthefood16);
        bombPlantSpore_markSurfaceHit(state);
    }
    if (state->path.surfaceFlags & BOMB_PLANT_SPORE_PATH_CONTACT_MASK) {
        bombPlantSpore_markSurfaceHit(state);
    }
}

static int bombPlantSpore_getExtraSize(void) {
    return sizeof(BombPlantSporeState);
}

static void bombPlantSpore_free(GameObject* obj) {
    BombPlantSporeState* state = obj->extra;

    (*gExpgfxInterface)->freeSource((uintptr_t)obj);
    if (state->light != NULL) {
        ModelLightStruct_free(state->light);
        state->light = NULL;
    }
}

static void bombPlantSpore_update(GameObject* obj) {
    BombPlantSporeState* state = obj->extra;

    if (bombPlantSpore_processPickup(obj, state) || bombPlantSpore_updateDetonation(obj, state)) {
        return;
    }

    bombPlantSpore_updateFuseEffect(obj, state);

    GameObject* hitObject;
    ObjHits_GetPriorityHit(obj, &hitObject, NULL, NULL);
    GameObject* contactObj = (GameObject*)ObjAnim_GetPriorityHitState(&obj->anim)->activeHit;

    if (!(state->flags & BOMB_PLANT_SPORE_HIT_SURFACE)) {
        bombPlantSpore_updateMotion(obj, state, contactObj);
    }

    GameObject* player = Obj_GetPlayerObject();
    if (contactObj == player) {
        state->pickupOffer.gameBitId = GAMEBIT_SawBombSpore;
        ObjMsg_SendToObject(contactObj, OBJ_MESSAGE_PICKUP_IN_RANGE, obj, (uintptr_t)state);
        state->flags |= BOMB_PLANT_SPORE_WAITING_FOR_PICKUP;
        return;
    }

    state->fuseTimer -= timeDelta;
    if (state->fuseTimer <= 0.0f) {
        Sfx_PlayFromObject(obj, SFXTRIG_en_majring2);
        bombPlantSpore_beginDetonation(obj, state);
    }
}

static void bombPlantSpore_init(GameObject* obj, const BombPlantSporePlacement* placement) {
    (void)placement;

    BombPlantSporeState* state = obj->extra;
    u8 pathParam[8];

    pathParam[0] = BOMB_PLANT_SPORE_PATH_PARAM;
    state->fuseTimer = 1500.0f;
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN | OBJECT_OBJFLAG_HITDETECT_DISABLED;
    obj->anim.velocityY = 2.0f;
    ObjHits_DisableObject(obj);
    state->spinAngle = (s16)randomGetRange(0, 0xFFFF);
    state->driftAmplitudeTarget = (f32)randomGetRange(0, 1000) / 1000.0f;

    (*gPathControlInterface)->init(&state->path, 0, BOMB_PLANT_SPORE_PATH_FLAGS, 1);
    (*gPathControlInterface)->setup(&state->path, 1, sBombPlantSporePathPoint, sBombPlantSporePathSetup, pathParam);
    (*gPathControlInterface)->attachObject(obj, &state->path);
    (*gPartfxInterface)->spawnObject(obj, BOMB_PLANT_SPORE_SPAWN_EFFECT, NULL, PARTFXFLAG_4, -1, NULL);

    ModelLightStruct* light = objCreateLight(obj, 1);
    if (light != NULL) {
        modelLightStruct_setLightKind(light, MODEL_LIGHT_KIND_POINT);
        modelLightStruct_setDiffuseColor(light, 0xFF, 0, 0xFF, 0);
        modelLightStruct_setFieldBC(light, 1);
        modelLightStruct_setDistanceAttenuation(light, gBombPlantSporeLightAttenuationNear,
                                                gBombPlantSporeLightAttenuationFar);
    }
    state->light = light;
    ObjMsg_AllocQueue(obj, 2);
    state->yawStep = (s16)randomGetRange(-0x200, 0x200);
}

OBJECT_INIT_ADAPTER(gBombPlantSporeObjDescriptorInitAdapter, bombPlantSpore_init, obj, placement)
OBJECT_FREE_ADAPTER(gBombPlantSporeObjDescriptorFreeAdapter, bombPlantSpore_free, obj)
OBJECT_EXTRA_SIZE_ADAPTER(gBombPlantSporeObjDescriptorExtraSizeAdapter, bombPlantSpore_getExtraSize)

ObjectDescriptor10WithPadding gBombPlantSporeObjDescriptor = {
    .descriptor =
        {
            .header.metadata[0] = 0,
            .header.metadata[1] = 0,
            .header.metadata[2] = 0,
            .header.metadata[3] = OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
            .header.acquire = NULL,
            .header.release = NULL,
            .slot02 = NULL,
            .init = gBombPlantSporeObjDescriptorInitAdapter,
            .update = bombPlantSpore_update,
            .hitDetect = NULL,
            .render = NULL,
            .free = gBombPlantSporeObjDescriptorFreeAdapter,
            .getObjectTypeId = NULL,
            .getExtraSize = gBombPlantSporeObjDescriptorExtraSizeAdapter,
        },
    .padding = 0,
};
