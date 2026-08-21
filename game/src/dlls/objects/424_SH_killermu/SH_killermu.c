#include "dlls/objects/424_SH_killermu.h"

#include "dolphin/MSL_C/PPCEABI/bare/H/math_api.h"
#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/audio/sfx_keep_alive_api.h"
#include "main/audio/sfx_play_api.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/dll/player_api.h"
#include "main/dll/partfx_interface.h"
#include "main/dll_000A_expgfx.h"
#include "main/frame_timing.h"
#include "main/gamebits_api.h"
#include "main/object_render.h"
#include "main/objfx.h"
#include "main/objhits.h"
#include "main/obj_path.h"
#include "main/objtype.h"
#include "main/shader_api.h"
#include "main/vecmath.h"
#include "sys/objects.h"

enum EnemyMushroomStateId {
    ENEMY_MUSHROOM_STATE_IDLE,
    ENEMY_MUSHROOM_STATE_REGROWING,
    ENEMY_MUSHROOM_STATE_FADING,
    ENEMY_MUSHROOM_STATE_STARTLED,
    ENEMY_MUSHROOM_STATE_POISONING,
    ENEMY_MUSHROOM_STATE_SETTLING,
    ENEMY_MUSHROOM_STATE_DEFLATING,
    ENEMY_MUSHROOM_STATE_UNUSED_7,
    ENEMY_MUSHROOM_STATE_UNUSED_8,
    ENEMY_MUSHROOM_STATE_STUNNED,
    ENEMY_MUSHROOM_STATE_RESPAWN_WAIT,
};

enum EnemyMushroomStateFlag {
    ENEMY_MUSHROOM_HIT_PLAYER = 1 << 0,
    ENEMY_MUSHROOM_ANIMATION_DONE = 1 << 1,
    ENEMY_MUSHROOM_ACTIVE = 1 << 2,
};

enum EnemyMushroomObjectGroup {
    ENEMY_MUSHROOM_OBJECT_GROUP = 3,
};

enum EnemyMushroomParticleEffect {
    ENEMY_MUSHROOM_HIT_EFFECT = 0x3EB,
    ENEMY_MUSHROOM_STUN_EFFECT = 0x51D,
};

enum EnemyMushroomHitType {
    ENEMY_MUSHROOM_CONTACT_HIT = 0x16,
};

typedef struct EnemyMushroomAnimation {
    s16 moveId;
    f32 stepScale;
} EnemyMushroomAnimation;

typedef struct EnemyMushroomPlacement {
    ObjPlacement base;
    u16 regrowDelayFrames;
    u16 respawnFrameLimit;
    s16 popGameBitId;
    u8 detectRange;
    u8 objectType;
    u8 unused20[4];
} EnemyMushroomPlacement;

typedef struct EnemyMushroomState {
    f32 timer;
    f32 heightTarget;
    f32 riseDuration;
    f32 baseScale;
    f32 riseStep;
    u8 unused14[12];
    Vec3f hitEffectPosition;
    f32 hitRadius;
    f32 effectTimer;
    s16 respawnFrameLimit;
    u8 stateId;
    u8 flags;
    u8 unused38[4];
} EnemyMushroomState;

STATIC_ASSERT(sizeof(EnemyMushroomPlacement) == 0x24);
STATIC_ASSERT(offsetof(EnemyMushroomPlacement, regrowDelayFrames) == 0x18);
STATIC_ASSERT(offsetof(EnemyMushroomPlacement, respawnFrameLimit) == 0x1A);
STATIC_ASSERT(offsetof(EnemyMushroomPlacement, popGameBitId) == 0x1C);
STATIC_ASSERT(offsetof(EnemyMushroomPlacement, detectRange) == 0x1E);
STATIC_ASSERT(offsetof(EnemyMushroomPlacement, objectType) == 0x1F);

STATIC_ASSERT(sizeof(EnemyMushroomState) == 0x3C);
STATIC_ASSERT(offsetof(EnemyMushroomState, timer) == 0x00);
STATIC_ASSERT(offsetof(EnemyMushroomState, heightTarget) == 0x04);
STATIC_ASSERT(offsetof(EnemyMushroomState, riseDuration) == 0x08);
STATIC_ASSERT(offsetof(EnemyMushroomState, baseScale) == 0x0C);
STATIC_ASSERT(offsetof(EnemyMushroomState, riseStep) == 0x10);
STATIC_ASSERT(offsetof(EnemyMushroomState, hitEffectPosition) == 0x20);
STATIC_ASSERT(offsetof(EnemyMushroomState, hitRadius) == 0x2C);
STATIC_ASSERT(offsetof(EnemyMushroomState, effectTimer) == 0x30);
STATIC_ASSERT(offsetof(EnemyMushroomState, respawnFrameLimit) == 0x34);
STATIC_ASSERT(offsetof(EnemyMushroomState, stateId) == 0x36);
STATIC_ASSERT(offsetof(EnemyMushroomState, flags) == 0x37);

static const EnemyMushroomAnimation sEnemyMushroomAnimations[] = {
    [ENEMY_MUSHROOM_STATE_IDLE] = {0, 0.0f},           [ENEMY_MUSHROOM_STATE_REGROWING] = {0, 0.0f},
    [ENEMY_MUSHROOM_STATE_FADING] = {4, 0.008f},       [ENEMY_MUSHROOM_STATE_STARTLED] = {1, 0.025f},
    [ENEMY_MUSHROOM_STATE_POISONING] = {2, 0.018f},    [ENEMY_MUSHROOM_STATE_SETTLING] = {3, 0.015f},
    [ENEMY_MUSHROOM_STATE_DEFLATING] = {5, 0.006f},    [ENEMY_MUSHROOM_STATE_UNUSED_7] = {6, 0.008f},
    [ENEMY_MUSHROOM_STATE_UNUSED_8] = {6, 0.005f},     [ENEMY_MUSHROOM_STATE_STUNNED] = {6, 0.005f},
    [ENEMY_MUSHROOM_STATE_RESPAWN_WAIT] = {0, 0.005f},
};

static void enemyMushroom_resetToSpawn(GameObject* obj, EnemyMushroomState* state, int enableTimer) {
    const EnemyMushroomPlacement* placement = (EnemyMushroomPlacement*)obj->anim.placementData;

    obj->anim.rotZ = randomGetRange(-0x5DC, 0x5DC);
    obj->anim.rotY = randomGetRange(-0x5DC, 0x5DC);
    obj->anim.rotX = randomGetRange(-0x5DC, 0x5DC);
    obj->anim.alpha = 0xFF;
    obj->anim.flags &= ~OBJANIM_FLAG_HIDDEN;
    obj->anim.localPos.x = placement->base.posX;
    obj->anim.localPos.y = placement->base.posY;
    obj->anim.localPos.z = placement->base.posZ;

    if (enableTimer != 0) {
        obj->anim.rootMotionScale = 0.00001f;
        state->timer = 0.0f;
        state->riseDuration = 200.0f + (f32)(s32)randomGetRange(0, 100);
        state->heightTarget = 0.001f * (f32)(s32)randomGetRange(-100, 100) + state->baseScale;
        state->riseStep = state->heightTarget / state->riseDuration;
    }

    ObjHits_EnableObject(obj);
    ObjHits_RefreshObjectState(obj);
}

static void enemyMushroom_tryHitPlayer(GameObject* obj, EnemyMushroomState* state, GameObject* player) {
    if (state->flags & ENEMY_MUSHROOM_HIT_PLAYER) {
        return;
    }

    if (Vec_distance(&obj->anim.worldPos.x, &player->anim.worldPos.x) <= state->hitRadius &&
        !EmissionController_IsLingering(player) && !playerGetFlags3F0Bit5(player) &&
        !(player->objectFlags & OBJECT_OBJFLAG_PARENT_SLACK)) {
        ObjHits_RecordObjectHit(player, obj, ENEMY_MUSHROOM_CONTACT_HIT, 1, 0);
        state->flags |= ENEMY_MUSHROOM_HIT_PLAYER;
    }
}

static void enemyMushroom_spawnHitEffect(GameObject* obj, const EnemyMushroomState* state,
                                         PartFxSpawnParams* effectParams) {
    effectParams->pos = state->hitEffectPosition;
    (*gPartfxInterface)
        ->spawnObject(obj, ENEMY_MUSHROOM_HIT_EFFECT, effectParams, PARTFXFLAG_200000 | PARTFXFLAG_1, -1, NULL);
}

static int enemyMushroom_updateFrozen(GameObject* obj, PartFxSpawnParams* effectParams) {
    GameObject* hitObject;
    int hitSphereIndex;
    u32 hitVolume;
    int hitType;

    if (!objIsFrozen(obj)) {
        return 0;
    }

    hitType = ObjHits_GetPriorityHitWithPosition(obj, &hitObject, &hitSphereIndex, &hitVolume, &effectParams->pos.x,
                                                 &effectParams->pos.y, &effectParams->pos.z);
    if (hitType != 0 && hitType != OBJHITS_SHAPE_MODEL_HIT_VOLUMES) {
        effectParams->pos.x += playerMapOffsetX;
        effectParams->pos.z += playerMapOffsetZ;
        objDoHitParticleFx(obj, 0.014f, effectParams, 1, NULL);
        Sfx_PlayFromObject(obj, SFXTRIG_barrel_bounce1);
        Obj_Shatter(obj);
    }

    return 1;
}

static void enemyMushroom_updateDeflating(GameObject* obj, EnemyMushroomState* state, GameObject* player,
                                          PartFxSpawnParams* effectParams) {
    Sfx_KeepAliveLoopedObjectSound(obj, SFXTRIG_diallp_c);
    state->flags &= ~ENEMY_MUSHROOM_ACTIVE;
    state->hitRadius += 3.5f * timeDelta;
    if (state->hitRadius > 80.0f) {
        state->hitRadius = 80.0f;
    }

    enemyMushroom_tryHitPlayer(obj, state, player);

    if (state->flags & ENEMY_MUSHROOM_ANIMATION_DONE) {
        state->timer = 0.0f;
        state->stateId = ENEMY_MUSHROOM_STATE_FADING;
    }

    enemyMushroom_spawnHitEffect(obj, state, effectParams);
}

static void enemyMushroom_updateFading(GameObject* obj, EnemyMushroomState* state) {
    state->flags &= ~ENEMY_MUSHROOM_ACTIVE;
    if (!(state->flags & ENEMY_MUSHROOM_ANIMATION_DONE)) {
        return;
    }

    int newAlpha = obj->anim.alpha - framesThisStep * 4;
    if (newAlpha < 0) {
        newAlpha = 0;
    }
    obj->anim.alpha = newAlpha;
    state->timer += timeDelta;
    if (state->timer > (f32)state->respawnFrameLimit) {
        enemyMushroom_resetToSpawn(obj, state, 1);
        state->stateId = ENEMY_MUSHROOM_STATE_REGROWING;
    }
}

static void enemyMushroom_updateStartled(GameObject* obj, EnemyMushroomState* state) {
    obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_DISABLED;
    Sfx_KeepAliveLoopedObjectSound(obj, SFXTRIG_id_9c);
    if (state->flags & ENEMY_MUSHROOM_ANIMATION_DONE) {
        state->stateId = ENEMY_MUSHROOM_STATE_POISONING;
    }
}

static void enemyMushroom_updatePoisoning(GameObject* obj, EnemyMushroomState* state, GameObject* player,
                                          PartFxSpawnParams* effectParams) {
    obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_DISABLED;
    state->hitRadius += 2.5f * timeDelta;
    Sfx_KeepAliveLoopedObjectSound(obj, SFXTRIG_diallp_c);
    enemyMushroom_tryHitPlayer(obj, state, player);

    if (state->hitRadius > 80.0f) {
        state->hitRadius = 80.0f;
    }

    state->timer += timeDelta;
    if (state->timer > 120.0f) {
        state->timer = 0.0f;
        state->stateId = ENEMY_MUSHROOM_STATE_SETTLING;
    }

    enemyMushroom_spawnHitEffect(obj, state, effectParams);
}

static void enemyMushroom_updateSettling(GameObject* obj, EnemyMushroomState* state,
                                         const EnemyMushroomPlacement* placement) {
    obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_DISABLED;
    state->timer += timeDelta;
    if (state->timer > (f32)ObjAnim_ReadPlacementU16(&obj->anim, &placement->regrowDelayFrames) &&
        state->flags & ENEMY_MUSHROOM_ANIMATION_DONE) {
        state->stateId = ENEMY_MUSHROOM_STATE_IDLE;
        state->hitRadius = 0.0f;
        state->flags &= ~ENEMY_MUSHROOM_HIT_PLAYER;
    }
}

static void enemyMushroom_updateRegrowing(GameObject* obj, EnemyMushroomState* state) {
    state->flags &= ~ENEMY_MUSHROOM_ACTIVE;
    if (obj->anim.rootMotionScale > state->heightTarget) {
        state->riseStep /= 1.1f;
    }
    if (state->riseStep < 0.00001f) {
        state->riseStep = 0.0f;
    }

    state->timer += timeDelta;
    obj->anim.rootMotionScale += state->riseStep * timeDelta;
    if (state->timer > state->riseDuration) {
        state->stateId = ENEMY_MUSHROOM_STATE_IDLE;
    }
}

static void enemyMushroom_updateStunned(GameObject* obj, EnemyMushroomState* state, PartFxSpawnParams* effectParams) {
    if (state->timer <= 0.0f) {
        state->timer = (f32)(int)randomGetRange(0xF0, 0x12C);
    }
    if (state->flags & ENEMY_MUSHROOM_ANIMATION_DONE) {
        state->timer = 0.0f;
    }

    Sfx_KeepAliveLoopedObjectSound(obj, SFXTRIG_cagelp_c);
    state->timer -= timeDelta;
    if (state->timer <= 0.0f) {
        (*gExpgfxInterface)->freeSource((uintptr_t)obj);
        state->stateId = ENEMY_MUSHROOM_STATE_IDLE;
        Obj_ResetActiveHitVolumeBounds(obj);
        return;
    }

    state->effectTimer -= timeDelta;
    if (state->effectTimer <= 0.0f) {
        effectParams->pos.x = 14.0f;
        effectParams->pos.y = 25.0f;
        (*gPartfxInterface)->spawnObject(obj, ENEMY_MUSHROOM_STUN_EFFECT, effectParams, PARTFXFLAG_2, -1, NULL);
        state->effectTimer = 20.0f;
    }
    obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_DISABLED;
}

static void enemyMushroom_updateRespawnWait(GameObject* obj, EnemyMushroomState* state) {
    ObjHits_DisableObject(obj);
    state->timer += timeDelta;
    if (state->timer > (f32)state->respawnFrameLimit) {
        enemyMushroom_resetToSpawn(obj, state, 1);
        state->stateId = ENEMY_MUSHROOM_STATE_REGROWING;
        Obj_ResetActiveHitVolumeBounds(obj);
    }
}

static void enemyMushroom_updateIdle(GameObject* obj, EnemyMushroomState* state, GameObject* player,
                                     const EnemyMushroomPlacement* placement) {
    obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_DISABLED;

    f32 dx = player->anim.localPos.x - obj->anim.localPos.x;
    f32 dy = player->anim.localPos.y - obj->anim.localPos.y;
    f32 dz = player->anim.localPos.z - obj->anim.localPos.z;
    if ((u16)(int)sqrtf(dx * dx + dy * dy + dz * dz) >= (u16)(int)(1.5f * (f32)placement->detectRange)) {
        return;
    }
    if (playerGetAnimSpeed(player) < 0.54f) {
        return;
    }

    state->flags &= ~ENEMY_MUSHROOM_HIT_PLAYER;
    state->stateId = ENEMY_MUSHROOM_STATE_STARTLED;
    state->timer = 0.0f;
    Sfx_PlayFromObject(obj, SFXTRIG_baddie_haga_talk3);
}

static void enemyMushroom_handleHit(GameObject* obj, EnemyMushroomState* state, const EnemyMushroomPlacement* placement,
                                    PartFxSpawnParams* effectParams) {
    GameObject* hitObject;
    int hitSphereIndex;
    u32 hitVolume;
    int hitType = ObjHits_GetPriorityHitWithPosition(obj, &hitObject, &hitSphereIndex, &hitVolume, &effectParams->pos.x,
                                                     &effectParams->pos.y, &effectParams->pos.z);

    effectParams->pos.x += playerMapOffsetX;
    effectParams->pos.z += playerMapOffsetZ;
    if (hitType == 0 || !(state->flags & ENEMY_MUSHROOM_ACTIVE)) {
        return;
    }

    if (hitType == OBJHITS_SHAPE_MODEL_HIT_VOLUMES) {
        Obj_StartModelFadeIn(obj, 0x12C);
    } else {
        if (state->stateId != ENEMY_MUSHROOM_STATE_STUNNED) {
            Sfx_PlayFromObject(obj, SFXTRIG_mv_ladderslide16);
        }
        state->flags &= ~ENEMY_MUSHROOM_HIT_PLAYER;

        s16 popGameBitId = ObjAnim_ReadPlacementS16(&obj->anim, &placement->popGameBitId);
        if (popGameBitId != -1) {
            mainSetBits(popGameBitId, 1);
        }

        state->stateId = ENEMY_MUSHROOM_STATE_STUNNED;
        state->timer = 0.0f;
        obj->anim.currentMoveProgress = (f32)(int)randomGetRange(0, 0x28) / 100.0f;
    }

    objDoHitParticleFx(obj, 0.014f, effectParams, 1, NULL);
}

static void enemyMushroom_updateAnimation(GameObject* obj, EnemyMushroomState* state) {
    const EnemyMushroomAnimation* animation = &sEnemyMushroomAnimations[state->stateId];

    if (obj->anim.currentMove != animation->moveId) {
        ObjAnim_SetCurrentMove(obj, animation->moveId, 0.0f, 0);
    }
    if (ObjAnim_AdvanceCurrentMove(obj, animation->stepScale, timeDelta, NULL) != 0) {
        state->flags |= ENEMY_MUSHROOM_ANIMATION_DONE;
    } else {
        state->flags &= ~ENEMY_MUSHROOM_ANIMATION_DONE;
    }
}

static int enemyMushroom_getExtraSize(void) {
    return sizeof(EnemyMushroomState);
}

static int enemyMushroom_getObjectTypeId(GameObject* obj) {
    const EnemyMushroomPlacement* placement = (EnemyMushroomPlacement*)obj->anim.placementData;

    return (placement->objectType << 11) | 0x400;
}

static void enemyMushroom_free(GameObject* obj) {
    (*gExpgfxInterface)->freeSource((uintptr_t)obj);
    objFreeObjectType(obj, ENEMY_MUSHROOM_OBJECT_GROUP);
}

static void enemyMushroom_render(GameObject* obj, u32 flags, u32 texData, u32 colorTable, u32 modelState,
                                 char visible) {
    EnemyMushroomState* state = obj->extra;

    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, flags, texData, colorTable, modelState, 1.0f);
        ObjPath_GetPointWorldPosition(obj, 0, &state->hitEffectPosition.x, &state->hitEffectPosition.y,
                                      &state->hitEffectPosition.z, 0);
    }
}

static void enemyMushroom_hitDetect(void) {
}

static void enemyMushroom_update(GameObject* obj) {
    EnemyMushroomState* state = obj->extra;
    GameObject* player = Obj_GetPlayerObject();
    const EnemyMushroomPlacement* placement = (EnemyMushroomPlacement*)obj->anim.placementData;
    PartFxSpawnParams effectParams;

    ObjHits_ClearHitVolumes(&obj->anim);
    obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
    state->flags |= ENEMY_MUSHROOM_ACTIVE;

    if (enemyMushroom_updateFrozen(obj, &effectParams)) {
        return;
    }
    if (player->objectFlags & OBJECT_OBJFLAG_PARENT_SLACK) {
        return;
    }

    switch (state->stateId) {
    case ENEMY_MUSHROOM_STATE_DEFLATING:
        enemyMushroom_updateDeflating(obj, state, player, &effectParams);
        break;
    case ENEMY_MUSHROOM_STATE_FADING:
        enemyMushroom_updateFading(obj, state);
        break;
    case ENEMY_MUSHROOM_STATE_STARTLED:
        enemyMushroom_updateStartled(obj, state);
        break;
    case ENEMY_MUSHROOM_STATE_POISONING:
        enemyMushroom_updatePoisoning(obj, state, player, &effectParams);
        break;
    case ENEMY_MUSHROOM_STATE_SETTLING:
        enemyMushroom_updateSettling(obj, state, placement);
        break;
    case ENEMY_MUSHROOM_STATE_REGROWING:
        enemyMushroom_updateRegrowing(obj, state);
        break;
    case ENEMY_MUSHROOM_STATE_STUNNED:
        enemyMushroom_updateStunned(obj, state, &effectParams);
        break;
    case ENEMY_MUSHROOM_STATE_RESPAWN_WAIT:
        enemyMushroom_updateRespawnWait(obj, state);
        break;
    case ENEMY_MUSHROOM_STATE_IDLE:
    case ENEMY_MUSHROOM_STATE_UNUSED_7:
    case ENEMY_MUSHROOM_STATE_UNUSED_8:
    default:
        enemyMushroom_updateIdle(obj, state, player, placement);
        break;
    }

    enemyMushroom_handleHit(obj, state, placement, &effectParams);
    enemyMushroom_updateAnimation(obj, state);
}

static void enemyMushroom_init(GameObject* obj, EnemyMushroomPlacement* placement, int flags) {
    EnemyMushroomState* state = obj->extra;

    state->timer = 0.0f;
    state->hitRadius = 0.0f;
    state->baseScale = obj->anim.rootMotionScale;
    state->respawnFrameLimit = (s16)ObjAnim_ReadPlacementU16(&obj->anim, &placement->respawnFrameLimit);
    if (state->respawnFrameLimit < 0x708) {
        state->respawnFrameLimit = 0x708;
    }
    obj->anim.localPos.y = placement->base.posY - 2.0f;
    if (obj->anim.modelState != NULL) {
        obj->anim.modelState->flags |= OBJ_MODEL_STATE_UNREAD_0800 | OBJ_MODEL_STATE_UNREAD_0010;
    }
    if (flags == 0) {
        enemyMushroom_resetToSpawn(obj, state, 0);
    }
    objAddObjectType(obj, ENEMY_MUSHROOM_OBJECT_GROUP);
}

static void enemyMushroom_release(void) {
}

static void enemyMushroom_initialise(void) {
}

ObjectDescriptor gEnemyMushroomObjDescriptor = {
    .reserved0 = 0,
    .reserved1 = 0,
    .reserved2 = 0,
    .slotCountAndFlags = OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    .initialise = enemyMushroom_initialise,
    .release = enemyMushroom_release,
    .slot02 = NULL,
    .init = (ObjectDescriptorCallback)enemyMushroom_init,
    .update = (ObjectDescriptorCallback)enemyMushroom_update,
    .hitDetect = enemyMushroom_hitDetect,
    .render = (ObjectDescriptorCallback)enemyMushroom_render,
    .free = (ObjectDescriptorCallback)enemyMushroom_free,
    .getObjectTypeId = (ObjectDescriptorCallback)enemyMushroom_getObjectTypeId,
    .getExtraSize = enemyMushroom_getExtraSize,
};
