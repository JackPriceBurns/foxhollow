#include "dlls/objects/425_BombPlant.h"

#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/audio/sfx_keep_alive_api.h"
#include "main/audio/sfx_play_api.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/dll/dll_80136a40.h"
#include "main/dll/partfx_interface.h"
#include "main/frame_timing.h"
#include "main/gamebit_ids.h"
#include "main/gamebits_api.h"
#include "main/object_render.h"
#include "main/objfx.h"
#include "main/objhits.h"
#include "main/objseq.h"
#include "main/shader_api.h"
#include "main/vecmath.h"
#include "sys/objects.h"
#include "sys/objects/lifecycle.h"

enum BombPlantStateId {
    BOMB_PLANT_STATE_ACTIVE,
    BOMB_PLANT_STATE_DORMANT,
    BOMB_PLANT_STATE_GROWING,
    BOMB_PLANT_STATE_UNUSED_3,
    BOMB_PLANT_STATE_EXPLODING,
};

enum BombPlantStateFlag {
    BOMB_PLANT_ANIMATION_DONE = 1 << 0,
    BOMB_PLANT_JUST_ENTERED = 1 << 1,
};

enum BombPlantConfigFlag {
    BOMB_PLANT_CHECK_HITS = 1 << 0,
    BOMB_PLANT_ENABLE_INTERACTION = 1 << 1,
    BOMB_PLANT_HIDDEN = 1 << 2,
    BOMB_PLANT_ENABLE_HITS = 1 << 3,
    BOMB_PLANT_USE_HIT_VOLUME = 1 << 4,
};

enum BombPlantParticleEffect {
    BOMB_PLANT_SPARK_EFFECT = 0x7F1,
};

enum BombPlantObjectId {
    BOMB_PLANT_SPORE_OBJECT_ID = 0x198,
};

enum BombPlantHitType {
    BOMB_PLANT_EXPLOSIVE_HIT_MIN = 0xE,
    BOMB_PLANT_EXPLOSIVE_HIT_MAX = 0xF,
    BOMB_PLANT_EXPLOSIVE_HIT_ALT = 0x11,
};

enum BombPlantHitVolume {
    BOMB_PLANT_HIT_VOLUME_SLOT = 5,
};

typedef struct BombPlantPlacement {
    ObjPlacement base;
    s16 regrowTimer;
    s16 activeTimerBase;
    s16 gameBitId;
    s8 sporeAngleSpreadByte;
    s8 initialRotXByte;
} BombPlantPlacement;

typedef struct BombPlantSporeSpawnPlacement {
    ObjPlacement base;
    u8 unused18[2];
    s16 angleSpread;
    s16 baseAngle;
    u8 unused1E[6];
} BombPlantSporeSpawnPlacement;

typedef struct BombPlantState {
    f32 timer;
    f32 growStartScale;
    f32 growDuration;
    f32 growTargetScale;
    f32 growRate;
    u8 stateId;
    u8 flags;
    u8 unused16[2];
} BombPlantState;

typedef struct BombPlantStateConfig {
    s16 moveId;
    f32 moveStepScale;
    u8 flags;
} BombPlantStateConfig;

STATIC_ASSERT(sizeof(BombPlantPlacement) == 0x20);
STATIC_ASSERT(offsetof(BombPlantPlacement, regrowTimer) == 0x18);
STATIC_ASSERT(offsetof(BombPlantPlacement, activeTimerBase) == 0x1A);
STATIC_ASSERT(offsetof(BombPlantPlacement, gameBitId) == 0x1C);
STATIC_ASSERT(offsetof(BombPlantPlacement, sporeAngleSpreadByte) == 0x1E);
STATIC_ASSERT(offsetof(BombPlantPlacement, initialRotXByte) == 0x1F);

STATIC_ASSERT(sizeof(BombPlantSporeSpawnPlacement) == 0x24);
STATIC_ASSERT(offsetof(BombPlantSporeSpawnPlacement, angleSpread) == 0x1A);
STATIC_ASSERT(offsetof(BombPlantSporeSpawnPlacement, baseAngle) == 0x1C);

STATIC_ASSERT(sizeof(BombPlantState) == 0x18);
STATIC_ASSERT(offsetof(BombPlantState, timer) == 0x00);
STATIC_ASSERT(offsetof(BombPlantState, growStartScale) == 0x04);
STATIC_ASSERT(offsetof(BombPlantState, growDuration) == 0x08);
STATIC_ASSERT(offsetof(BombPlantState, growTargetScale) == 0x0C);
STATIC_ASSERT(offsetof(BombPlantState, growRate) == 0x10);
STATIC_ASSERT(offsetof(BombPlantState, stateId) == 0x14);
STATIC_ASSERT(offsetof(BombPlantState, flags) == 0x15);

STATIC_ASSERT(sizeof(BombPlantStateConfig) == 0x0C);
STATIC_ASSERT(offsetof(BombPlantStateConfig, moveId) == 0x00);
STATIC_ASSERT(offsetof(BombPlantStateConfig, moveStepScale) == 0x04);
STATIC_ASSERT(offsetof(BombPlantStateConfig, flags) == 0x08);

static const BombPlantStateConfig sBombPlantStateConfigs[] = {
    [BOMB_PLANT_STATE_ACTIVE] =
        {
            0,
            0.005f,
            BOMB_PLANT_CHECK_HITS | BOMB_PLANT_ENABLE_INTERACTION | BOMB_PLANT_ENABLE_HITS,
        },
    [BOMB_PLANT_STATE_DORMANT] = {0, 0.0f, BOMB_PLANT_HIDDEN},
    [BOMB_PLANT_STATE_GROWING] = {0, 0.0f, BOMB_PLANT_ENABLE_HITS},
    [BOMB_PLANT_STATE_UNUSED_3] =
        {
            2,
            0.01f,
            BOMB_PLANT_CHECK_HITS | BOMB_PLANT_ENABLE_INTERACTION | BOMB_PLANT_ENABLE_HITS,
        },
    [BOMB_PLANT_STATE_EXPLODING] = {1, 0.008f, BOMB_PLANT_ENABLE_HITS | BOMB_PLANT_USE_HIT_VOLUME},
};

static void bombPlant_restorePosition(GameObject* obj, const BombPlantPlacement* placement) {
    obj->anim.alpha = 0xFF;
    obj->anim.flags &= ~OBJANIM_FLAG_HIDDEN;
    obj->anim.localPos.x = placement->base.posX;
    obj->anim.localPos.y = placement->base.posY;
    obj->anim.localPos.z = placement->base.posZ;
}

static void bombPlant_beginGrowth(GameObject* obj, BombPlantState* state) {
    const BombPlantPlacement* placement = (BombPlantPlacement*)obj->anim.placementData;

    bombPlant_restorePosition(obj, placement);
    obj->anim.rootMotionScale = 0.00001f;
    state->growDuration = 135.0f;
    state->growStartScale = state->growTargetScale;
    state->growRate = state->growStartScale / state->growDuration;
    state->timer = state->growDuration;
    ObjHits_RefreshObjectState(obj);
}

static void bombPlant_updateActiveEffects(GameObject* obj, BombPlantState* state) {
    const BombPlantPlacement* placement = (BombPlantPlacement*)obj->anim.placementData;

    if (state->flags & BOMB_PLANT_JUST_ENTERED) {
        state->flags &= ~BOMB_PLANT_JUST_ENTERED;
        state->timer =
            (f32)(int)(ObjAnim_ReadPlacementS16(&obj->anim, &placement->activeTimerBase) + randomGetRange(-0x32, 0x32));
    }
    if (obj->objectFlags & OBJECT_OBJFLAG_RENDERED) {
        (*gPartfxInterface)->spawnObject(obj, BOMB_PLANT_SPARK_EFFECT, NULL, PARTFXFLAG_2, -1, NULL);
    }
}

static int bombPlant_animEventCallback(GameObject* obj) {
    BombPlantState* state = obj->extra;

    if (state->stateId != BOMB_PLANT_STATE_ACTIVE) {
        bombPlant_beginGrowth(obj, state);
        state->stateId = BOMB_PLANT_STATE_ACTIVE;
        state->flags |= BOMB_PLANT_JUST_ENTERED;
    } else {
        Sfx_KeepAliveLoopedObjectSound(obj, SFXTRIG_baddie_eggsnatch_sniff2);
        bombPlant_updateActiveEffects(obj, state);
    }

    return 0;
}

static void bombPlant_tryBeginGrow(GameObject* obj, BombPlantState* state) {
    GameObject* player = Obj_GetPlayerObject();

    if (vec3f_distanceSquared(&obj->anim.worldPos.x, &player->anim.worldPos.x) > 6400.0f) {
        state->stateId = BOMB_PLANT_STATE_GROWING;
        state->flags |= BOMB_PLANT_JUST_ENTERED;
    }
}

static void bombPlant_spawnSpore(GameObject* obj) {
    const BombPlantPlacement* placement = (BombPlantPlacement*)obj->anim.placementData;

    if (!Obj_IsLoadingLocked()) {
        return;
    }

    BombPlantSporeSpawnPlacement* spore = (BombPlantSporeSpawnPlacement*)Obj_AllocObjectSetup(
        sizeof(BombPlantSporeSpawnPlacement), BOMB_PLANT_SPORE_OBJECT_ID);
    MatrixTransform transform = {
        .rotX = obj->anim.rotX,
        .rotY = obj->anim.rotY,
        .rotZ = obj->anim.rotZ,
        .scale = 1.0f,
        .x = 0.0f,
        .y = 0.0f,
        .z = 0.0f,
    };
    f32 matrix[16];
    Vec3f offset;

    setMatrixFromObjectPos(matrix, &transform);
    Matrix_TransformPoint(matrix, 0.0f, 1.0f, 0.0f, &offset.x, &offset.y, &offset.z);
    transform.x = 26.0f * offset.x;
    transform.y = 26.0f * offset.y;
    transform.z = 26.0f * offset.z;
    spore->base.posX = obj->anim.localPos.x + transform.x;
    spore->base.posY = obj->anim.localPos.y + transform.y;
    spore->base.posZ = obj->anim.localPos.z + transform.z;
    spore->base.color[1] = 1;
    spore->base.color[0] = 2;
    spore->angleSpread = (s16)((s32)placement->sporeAngleSpreadByte * 0x100);
    spore->baseAngle = obj->anim.rotX;
    objSetupObject(&spore->base, 5, -1, -1, NULL);
}

static void bombPlant_explode(GameObject* obj, BombPlantState* state) {
    const BombPlantPlacement* placement = (BombPlantPlacement*)obj->anim.placementData;
    GameObject* tricky = getTrickyObject();

    if (tricky != NULL) {
        trickyImpress(tricky);
    }
    Sfx_PlayFromObject(obj, SFXTRIG_bombplant_woompf);
    ObjAnim_GetPriorityHitState(&obj->anim)->flags |= OBJHITS_PRIORITY_STATE_POSITION_DIRTY;
    spawnExplosion(obj, 100.0f, 0, 1, 1, 1, 0, 1, 0);
    state->stateId = BOMB_PLANT_STATE_DORMANT;
    state->flags |= BOMB_PLANT_JUST_ENTERED;

    s16 gameBitId = ObjAnim_ReadPlacementS16(&obj->anim, &placement->gameBitId);
    if (gameBitId != -1) {
        mainSetBits(gameBitId, 0);
    } else {
        for (int i = 0; i < 3; i++) {
            bombPlant_spawnSpore(obj);
        }
    }
}

static void bombPlant_updateDormant(GameObject* obj, BombPlantState* state, const BombPlantPlacement* placement) {
    if (state->flags & BOMB_PLANT_JUST_ENTERED) {
        state->flags &= ~BOMB_PLANT_JUST_ENTERED;
        state->timer = (f32)(int)ObjAnim_ReadPlacementS16(&obj->anim, &placement->regrowTimer);
    }

    s16 gameBitId = ObjAnim_ReadPlacementS16(&obj->anim, &placement->gameBitId);
    if (gameBitId != -1) {
        if (mainGetBit(gameBitId) != 0) {
            bombPlant_tryBeginGrow(obj, state);
        }
        return;
    }

    state->timer -= timeDelta;
    if (state->timer <= 0.0f) {
        bombPlant_tryBeginGrow(obj, state);
        state->timer = 0.0f;
    }
}

static void bombPlant_updateGrowing(GameObject* obj, BombPlantState* state) {
    if (state->flags & BOMB_PLANT_JUST_ENTERED) {
        Sfx_PlayFromObject(obj, SFXTRIG_bombplant_grows);
        state->flags &= ~BOMB_PLANT_JUST_ENTERED;
        bombPlant_beginGrowth(obj, state);
    }

    if (obj->anim.rootMotionScale > state->growStartScale) {
        state->growRate /= 1.1f;
    }
    if (state->growRate < 0.00001f) {
        state->growRate = 0.0f;
    }
    obj->anim.rootMotionScale += state->growRate * timeDelta;
    state->timer -= timeDelta;
    if (state->timer < 0.0f) {
        state->stateId = BOMB_PLANT_STATE_ACTIVE;
        state->flags |= BOMB_PLANT_JUST_ENTERED;
    }
}

static void bombPlant_checkHit(GameObject* obj, BombPlantState* state) {
    GameObject* hitObject;
    int hitSphereIndex;
    u32 hitVolume;
    PartFxSpawnParams effectParams;
    int hitType = ObjHits_GetPriorityHitWithPosition(obj, &hitObject, &hitSphereIndex, &hitVolume, &effectParams.pos.x,
                                                     &effectParams.pos.y, &effectParams.pos.z);

    if (hitType == 0 || hitVolume == 0) {
        return;
    }
    if (hitType == OBJHITS_SHAPE_MODEL_HIT_VOLUMES) {
        Obj_StartModelFadeIn(obj, 0x12C);
        return;
    }
    if ((u32)(hitType - BOMB_PLANT_EXPLOSIVE_HIT_MIN) > BOMB_PLANT_EXPLOSIVE_HIT_MAX - BOMB_PLANT_EXPLOSIVE_HIT_MIN &&
        hitType != BOMB_PLANT_EXPLOSIVE_HIT_ALT) {
        return;
    }

    Sfx_PlayFromObject(obj, SFXTRIG_mv_ladderslide16);
    effectParams.pos.x += playerMapOffsetX;
    effectParams.pos.z += playerMapOffsetZ;
    objDoHitParticleFx(obj, 0.014f, &effectParams, 1, NULL);
    Obj_SetModelColorFadeRecursive(obj, 0xF, 0xC8, 0, 0, 1);
    state->stateId = BOMB_PLANT_STATE_EXPLODING;
    state->flags |= BOMB_PLANT_JUST_ENTERED;

    ObjDef* model = obj->anim.modelInstance;
    ObjHitbox_SetCapsuleBounds(&obj->anim, (s16)(model->primaryHitboxRadius + 0x50),
                               (s16)(model->primaryCapsuleOffsetA - 0x50), (s16)(model->primaryCapsuleOffsetB + 0x50));
    ObjHits_MarkObjectPositionDirty(&obj->anim);
}

static void bombPlant_applyConfig(GameObject* obj, BombPlantState* state, const BombPlantStateConfig* config) {
    if (config->flags & BOMB_PLANT_ENABLE_HITS) {
        ObjHits_EnableObject(obj);
    } else {
        ObjHits_DisableObject(obj);
    }

    if (config->flags & BOMB_PLANT_USE_HIT_VOLUME) {
        ObjHits_SetHitVolumeSlot(&obj->anim, BOMB_PLANT_HIT_VOLUME_SLOT, 1, 0);
    } else {
        ObjHits_ClearHitVolumes(&obj->anim);
    }

    if (config->flags & BOMB_PLANT_ENABLE_INTERACTION) {
        obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_DISABLED;
        if (obj->anim.resetHitboxFlags & INTERACT_FLAG_IN_RANGE && mainGetBit(GAMEBIT_SawBombPlant) == 0) {
            (*gObjectTriggerInterface)->runSequence(0, obj, -1);
            mainSetBits(GAMEBIT_SawBombPlant, 1);
        }
    } else {
        obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
    }

    if (config->flags & BOMB_PLANT_HIDDEN) {
        obj->anim.flags |= OBJANIM_FLAG_HIDDEN;
    } else {
        obj->anim.flags &= ~OBJANIM_FLAG_HIDDEN;
    }

    if (obj->anim.currentMove != config->moveId) {
        ObjAnim_SetCurrentMove(obj, config->moveId, 0.0f, 0);
    }
    if (ObjAnim_AdvanceCurrentMove(obj, config->moveStepScale, timeDelta, NULL) != 0) {
        state->flags |= BOMB_PLANT_ANIMATION_DONE;
    } else {
        state->flags &= ~BOMB_PLANT_ANIMATION_DONE;
    }
}

static int bombPlant_getExtraSize(void) {
    return sizeof(BombPlantState);
}

static int bombPlant_getObjectTypeId(void) {
    return 0;
}

static void bombPlant_free(void) {
}

static void bombPlant_render(GameObject* obj, int flags, int texData, int colorTable, int modelState, s8 visible) {
    (void)visible;
    objRenderModelAndHitVolumes(obj, flags, texData, colorTable, modelState, 1.0f);
}

static void bombPlant_hitDetect(void) {
}

static void bombPlant_update(GameObject* obj) {
    (void)Obj_GetPlayerObject();
    if (objIsFrozen(obj) != 0) {
        return;
    }

    BombPlantState* state = obj->extra;
    const BombPlantPlacement* placement = (BombPlantPlacement*)obj->anim.placementData;
    const BombPlantStateConfig* config = &sBombPlantStateConfigs[state->stateId];

    switch (state->stateId) {
    case BOMB_PLANT_STATE_DORMANT:
        bombPlant_updateDormant(obj, state, placement);
        break;
    case BOMB_PLANT_STATE_GROWING:
        bombPlant_updateGrowing(obj, state);
        break;
    case BOMB_PLANT_STATE_EXPLODING:
        bombPlant_explode(obj, state);
        break;
    case BOMB_PLANT_STATE_ACTIVE:
        Sfx_KeepAliveLoopedObjectSound(obj, SFXTRIG_baddie_eggsnatch_sniff2);
        bombPlant_updateActiveEffects(obj, state);
        break;
    case BOMB_PLANT_STATE_UNUSED_3:
    default:
        bombPlant_updateActiveEffects(obj, state);
        break;
    }

    if (config->flags & BOMB_PLANT_CHECK_HITS) {
        bombPlant_checkHit(obj, state);
    }
    bombPlant_applyConfig(obj, state, config);
}

static void bombPlant_init(GameObject* obj, BombPlantPlacement* placement, int isReload) {
    BombPlantState* state = obj->extra;

    obj->anim.rotX = (s16)((s32)placement->initialRotXByte * 0x100);
    obj->objectFlags |= OBJECT_OBJFLAG_HITDETECT_DISABLED;
    obj->animEventCallback = bombPlant_animEventCallback;
    state->growTargetScale = obj->anim.rootMotionScale;
    if (isReload != 0) {
        return;
    }

    s16 gameBitId = ObjAnim_ReadPlacementS16(&obj->anim, &placement->gameBitId);
    if (gameBitId != -1 && mainGetBit(gameBitId) == 0) {
        bombPlant_beginGrowth(obj, state);
        state->stateId = BOMB_PLANT_STATE_DORMANT;
    } else {
        bombPlant_restorePosition(obj, placement);
        ObjHits_RefreshObjectState(obj);
    }
}

ObjectDescriptor10WithPadding gBombPlantObjDescriptor = {
    .descriptor =
        {
            .reserved0 = 0,
            .reserved1 = 0,
            .reserved2 = 0,
            .slotCountAndFlags = OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
            .initialise = NULL,
            .release = NULL,
            .slot02 = NULL,
            .init = (ObjectDescriptorCallback)bombPlant_init,
            .update = (ObjectDescriptorCallback)bombPlant_update,
            .hitDetect = bombPlant_hitDetect,
            .render = (ObjectDescriptorCallback)bombPlant_render,
            .free = bombPlant_free,
            .getObjectTypeId = (ObjectDescriptorCallback)bombPlant_getObjectTypeId,
            .getExtraSize = bombPlant_getExtraSize,
        },
    .padding = 0,
};
