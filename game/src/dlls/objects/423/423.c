#include "dlls/objects/423.h"

#include "dolphin/math.h"
#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/audio/sfx.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/curve.h"
#include "main/dll/curve_walker.h"
#include "main/dll/dll_00C4_tricky.h"
#include "main/dll/partfx_interface.h"
#include "main/dll/rom_curve_interface.h"
#include "main/dll_000A_expgfx.h"
#include "main/frame_timing.h"
#include "main/gamebit_ids.h"
#include "main/gamebits.h"
#include "main/gameloop_gamebit.h"
#include "main/obj_message.h"
#include "main/objfx.h"
#include "main/objhits.h"
#include "main/objtype.h"
#include "main/sky_interface.h"
#include "main/track_bbox.h"
#include "main/track_dolphin.h"
#include "main/vecmath.h"
#include "main/vecmath_distance.h"
#include "sys/objects.h"
#include "sys/objects/lifecycle.h"

enum EdibleMushroomAnimationState {
    EDIBLE_MUSHROOM_STATE_IDLE,
    EDIBLE_MUSHROOM_STATE_LUNGING,
    EDIBLE_MUSHROOM_STATE_UNUSED_2,
    EDIBLE_MUSHROOM_STATE_NOTICE_PLAYER,
    EDIBLE_MUSHROOM_STATE_TRACK_PLAYER,
    EDIBLE_MUSHROOM_STATE_RETREATING,
    EDIBLE_MUSHROOM_STATE_GROUNDED,
    EDIBLE_MUSHROOM_STATE_RETURN_TO_IDLE,
    EDIBLE_MUSHROOM_STATE_COLLECTED,
    EDIBLE_MUSHROOM_STATE_STRUCK,
    EDIBLE_MUSHROOM_STATE_UNUSED_10,
};

enum EdibleMushroomStateFlag {
    EDIBLE_MUSHROOM_ANIMATION_DONE = 1 << 0,
    EDIBLE_MUSHROOM_ON_CURVE = 1 << 1,
    EDIBLE_MUSHROOM_GROUNDED = 1 << 2,
    EDIBLE_MUSHROOM_MOVING = 1 << 3,
    EDIBLE_MUSHROOM_STRUCK = 1 << 4,
};

enum EdibleMushroomPlacementType {
    EDIBLE_MUSHROOM_TYPE_CURVE_A = 4,
    EDIBLE_MUSHROOM_TYPE_CURVE_B = 5,
};

enum EdibleMushroomObjectId {
    EDIBLE_MUSHROOM_EARTH_WARRIOR_ALIAS = 0x416,
    EDIBLE_MUSHROOM_WHITE_ALIAS = 0x658,
};

enum EdibleMushroomObjectGroup {
    EDIBLE_MUSHROOM_SECONDARY_GROUP = 0x31,
    EDIBLE_MUSHROOM_GROUP = 0x47,
};

enum EdibleMushroomParticleEffect {
    EDIBLE_MUSHROOM_SPORE_PUFF_EFFECT = 0x51D,
    EDIBLE_MUSHROOM_TAIL_SWING_EFFECT = 0x7F0,
};

enum EdibleMushroomHitKind {
    EDIBLE_MUSHROOM_FADE_IN_HIT = 0x10,
};

enum EdibleMushroomSurfaceType {
    EDIBLE_MUSHROOM_GROUND_SURFACE = 13,
};

typedef struct EdibleMushroomAnimation {
    s16 moveId;
    f32 stepScale;
} EdibleMushroomAnimation;

typedef struct EdibleMushroomPlacement {
    ObjPlacement base;
    u8 objectType;
    u8 lungeTriggerDistance;
    s16 gameBitId;
    u8 scaleParam;
    u8 unused1D[2];
    u8 retreatTriggerDistance;
} EdibleMushroomPlacement;

typedef struct EdibleMushroomState {
    RomCurveWalker curveWalker;
    f32 currentTargetDistance;
    f32 previousTargetDistance;
    f32 lungeRootSpeedScale;
    f32 mapParamScale;
    f32 lungeRange;
    f32 retreatRange;
    f32 curveAdvanceStep;
    f32 burrowAttackTimer;
    f32 sporePuffTimer;
    f32 tailSwingFxTimer;
    s16 moveAngle;
    u8 unused132[2];
    s16 collectedGameBitId;
    u8 animationState;
    u8 flags;
    u8 unused138;
    u8 sequenceResetPending;
    u8 unused13A[2];
    ObjPickupOffer pickupOffer;
    f32 pickupMessageDelay;
} EdibleMushroomState;

STATIC_ASSERT(sizeof(EdibleMushroomPlacement) == 0x20);
STATIC_ASSERT(offsetof(EdibleMushroomPlacement, objectType) == 0x18);
STATIC_ASSERT(offsetof(EdibleMushroomPlacement, lungeTriggerDistance) == 0x19);
STATIC_ASSERT(offsetof(EdibleMushroomPlacement, gameBitId) == 0x1A);
STATIC_ASSERT(offsetof(EdibleMushroomPlacement, scaleParam) == 0x1C);
STATIC_ASSERT(offsetof(EdibleMushroomPlacement, retreatTriggerDistance) == 0x1F);

STATIC_ASSERT(sizeof(EdibleMushroomState) == 0x144);
STATIC_ASSERT(offsetof(EdibleMushroomState, curveWalker) == 0x000);
STATIC_ASSERT(offsetof(EdibleMushroomState, currentTargetDistance) == 0x108);
STATIC_ASSERT(offsetof(EdibleMushroomState, previousTargetDistance) == 0x10C);
STATIC_ASSERT(offsetof(EdibleMushroomState, lungeRootSpeedScale) == 0x110);
STATIC_ASSERT(offsetof(EdibleMushroomState, mapParamScale) == 0x114);
STATIC_ASSERT(offsetof(EdibleMushroomState, lungeRange) == 0x118);
STATIC_ASSERT(offsetof(EdibleMushroomState, retreatRange) == 0x11C);
STATIC_ASSERT(offsetof(EdibleMushroomState, curveAdvanceStep) == 0x120);
STATIC_ASSERT(offsetof(EdibleMushroomState, burrowAttackTimer) == 0x124);
STATIC_ASSERT(offsetof(EdibleMushroomState, sporePuffTimer) == 0x128);
STATIC_ASSERT(offsetof(EdibleMushroomState, tailSwingFxTimer) == 0x12C);
STATIC_ASSERT(offsetof(EdibleMushroomState, moveAngle) == 0x130);
STATIC_ASSERT(offsetof(EdibleMushroomState, collectedGameBitId) == 0x134);
STATIC_ASSERT(offsetof(EdibleMushroomState, animationState) == 0x136);
STATIC_ASSERT(offsetof(EdibleMushroomState, flags) == 0x137);
STATIC_ASSERT(offsetof(EdibleMushroomState, sequenceResetPending) == 0x139);
STATIC_ASSERT(offsetof(EdibleMushroomState, pickupOffer) == 0x13C);
STATIC_ASSERT(offsetof(EdibleMushroomState, pickupMessageDelay) == 0x140);

static const EdibleMushroomAnimation sEdibleMushroomAnimations[] = {
    [EDIBLE_MUSHROOM_STATE_IDLE] = {0, 0.005f},        [EDIBLE_MUSHROOM_STATE_LUNGING] = {1, 0.01f},
    [EDIBLE_MUSHROOM_STATE_UNUSED_2] = {6, 0.005f},    [EDIBLE_MUSHROOM_STATE_NOTICE_PLAYER] = {2, 0.01f},
    [EDIBLE_MUSHROOM_STATE_TRACK_PLAYER] = {3, 0.01f}, [EDIBLE_MUSHROOM_STATE_RETREATING] = {4, 0.015f},
    [EDIBLE_MUSHROOM_STATE_GROUNDED] = {0, 0.005f},    [EDIBLE_MUSHROOM_STATE_RETURN_TO_IDLE] = {5, 0.01f},
    [EDIBLE_MUSHROOM_STATE_COLLECTED] = {6, 0.005f},   [EDIBLE_MUSHROOM_STATE_STRUCK] = {7, 0.012f},
    [EDIBLE_MUSHROOM_STATE_UNUSED_10] = {-1, 0.0f},
};

static int edibleMushroom_animEventCallback(GameObject* obj) {
    EdibleMushroomState* state = obj->extra;

    state->sequenceResetPending = 1;
    return 0;
}

static s16 edibleMushroom_findClearApproachAngle(GameObject* obj, GameObject* player, f32 distance) {
    s16 angle =
        getAngle(-(obj->anim.localPosX - player->anim.localPosX), -(obj->anim.localPosZ - player->anim.localPosZ));
    f32 radians = (3.1415927f * angle) / 32768.0f;
    f32 sinAngle = mathSinf(radians);
    f32 cosAngle = mathCosf(radians);
    Vec3f target = {
        obj->anim.localPosX - distance * sinAngle,
        obj->anim.localPosY,
        obj->anim.localPosZ - distance * cosAngle,
    };

    if (trackGetLineIntersect(&obj->anim.localPosX, &target.x, 0.1f, 3, NULL, obj, 8, -1, 0xFF, 0) == 0) {
        return angle;
    }

    s16 anglePlus = angle;
    s16 angleMinus = angle;
    f32 sinMinus = sinAngle;
    f32 sinPlus = sinAngle;
    f32 sinStepPlus = mathSinf(0.34898064f);
    f32 sinStepMinus = mathSinf(-0.34898064f);
    f32 cosPlus = cosAngle;
    f32 cosMinus = cosAngle;
    f32 cosStepPlus = mathCosf(0.34898064f);
    f32 cosStepMinus = mathCosf(-0.34898064f);

    for (int i = 0; i < 8; i++) {
        anglePlus = (s16)(anglePlus + 0xE38);
        f32 sinNext = sinPlus * cosStepPlus + cosPlus * sinStepPlus;
        cosPlus = cosPlus * cosStepPlus - sinPlus * sinStepPlus;
        sinPlus = sinNext;
        target.x = obj->anim.localPosX - distance * sinNext;
        target.z = obj->anim.localPosZ - distance * cosPlus;
        if (trackGetLineIntersect(&obj->anim.localPosX, &target.x, 0.1f, 1, NULL, obj, 8, -1, 0xFF, 0) == 0) {
            return anglePlus;
        }

        angleMinus = (s16)(angleMinus - 0xE38);
        sinNext = sinMinus * cosStepMinus + cosMinus * sinStepMinus;
        cosMinus = cosMinus * cosStepMinus - sinMinus * sinStepMinus;
        sinMinus = sinNext;
        target.x = obj->anim.localPosX - distance * sinNext;
        target.z = obj->anim.localPosZ - distance * cosMinus;
        if (trackGetLineIntersect(&obj->anim.localPosX, &target.x, 0.1f, 1, NULL, obj, 8, -1, 0xFF, 0) == 0) {
            return angleMinus;
        }
    }

    return angle;
}

static s16 edibleMushroom_chooseMovementAngle(GameObject* obj, GameObject* player, EdibleMushroomState* state,
                                              f32 distance) {
    if ((state->flags & EDIBLE_MUSHROOM_ON_CURVE) == 0) {
        return edibleMushroom_findClearApproachAngle(obj, player, distance);
    }

    f32 rangeSquared = distance * distance;
    f32 deltaX;
    f32 deltaZ;

    while (true) {
        deltaX = state->curveWalker.curve.sample[0] - obj->anim.localPosX;
        deltaZ = state->curveWalker.curve.sample[2] - obj->anim.localPosZ;
        if (!(deltaX * deltaX + deltaZ * deltaZ < rangeSquared)) {
            break;
        }
        if (Curve_AdvanceAlongPath(&state->curveWalker.curve, state->curveAdvanceStep) != 0 ||
            state->curveWalker.curve.idx != 0) {
            (*gRomCurveInterface)->goNextPoint(&state->curveWalker);
        }
    }

    return getAngle(-deltaX, -deltaZ);
}

static void edibleMushroom_spawnPickupEffect(GameObject* obj) {
    int effectId = obj->anim.romDefNo == EDIBLE_MUSHROOM_WHITE_ALIAS ? 0xFF : 6;

    itemPickupDoParticleFx(obj, 1.0f, effectId, 0x28);
}

static void edibleMushroom_updateIdle(GameObject* obj, GameObject* player, EdibleMushroomState* state,
                                      const EdibleMushroomPlacement* placement) {
    if ((state->flags & EDIBLE_MUSHROOM_STRUCK) != 0) {
        state->animationState = EDIBLE_MUSHROOM_STATE_STRUCK;
        return;
    }

    f32 sunTime;
    if ((*gSkyInterface)->getSunPosition(&sunTime) == 0) {
        if (state->currentTargetDistance < placement->lungeTriggerDistance) {
            state->moveAngle = edibleMushroom_chooseMovementAngle(obj, player, state, state->lungeRange);
            state->animationState = EDIBLE_MUSHROOM_STATE_LUNGING;
            Sfx_PlayFromObject(obj, SFXTRIG_mushrele16);
            obj->anim.rotX = (s16)(state->moveAngle - 0x4000);
        } else if (state->currentTargetDistance < placement->retreatTriggerDistance) {
            state->animationState = EDIBLE_MUSHROOM_STATE_NOTICE_PLAYER;
        }
        return;
    }

    state->tailSwingFxTimer -= timeDelta;
    if (state->tailSwingFxTimer <= 0.0f) {
        if ((obj->objectFlags & OBJECT_OBJFLAG_RENDERED) != 0) {
            PartFxSpawnParams spawnParams;

            spawnParams.pos.x = obj->anim.worldPosX;
            spawnParams.pos.y = obj->anim.worldPosY + 18.0f;
            spawnParams.pos.z = obj->anim.worldPosZ;
            (*gPartfxInterface)->spawnObject(obj, EDIBLE_MUSHROOM_TAIL_SWING_EFFECT, &spawnParams, 0x200001, -1, NULL);
        }
        state->tailSwingFxTimer = 30.0f;
    }
}

static void edibleMushroom_updateTracking(GameObject* obj, GameObject* player, EdibleMushroomState* state,
                                          const EdibleMushroomPlacement* placement, f32 targetApproachSpeed) {
    if ((state->flags & EDIBLE_MUSHROOM_STRUCK) != 0) {
        state->animationState = EDIBLE_MUSHROOM_STATE_STRUCK;
        return;
    }

    obj->anim.rotX =
        getAngle(-(obj->anim.localPosX - player->anim.localPosX), -(obj->anim.localPosZ - player->anim.localPosZ));
    if (state->currentTargetDistance > placement->retreatTriggerDistance + 10.0f) {
        state->animationState = EDIBLE_MUSHROOM_STATE_RETURN_TO_IDLE;
    } else if (state->currentTargetDistance < placement->lungeTriggerDistance) {
        Sfx_PlayFromObject(obj, SFXTRIG_mushrele16);
        if (targetApproachSpeed >= 0.54f) {
            state->moveAngle = edibleMushroom_chooseMovementAngle(obj, player, state, state->lungeRange);
            state->animationState = EDIBLE_MUSHROOM_STATE_LUNGING;
            obj->anim.rotX = (s16)(state->moveAngle - 0x4000);
        } else {
            state->moveAngle = edibleMushroom_chooseMovementAngle(obj, player, state, state->retreatRange);
            state->animationState = EDIBLE_MUSHROOM_STATE_RETREATING;
            obj->anim.rotX = state->moveAngle;
        }
    }
}

static void edibleMushroom_updateRetreat(GameObject* obj, GameObject* player, EdibleMushroomState* state,
                                         const EdibleMushroomPlacement* placement, f32 targetApproachSpeed) {
    if ((state->flags & (EDIBLE_MUSHROOM_STRUCK | EDIBLE_MUSHROOM_ANIMATION_DONE)) ==
        (EDIBLE_MUSHROOM_STRUCK | EDIBLE_MUSHROOM_ANIMATION_DONE)) {
        state->animationState = EDIBLE_MUSHROOM_STATE_STRUCK;
    }

    if (state->currentTargetDistance > placement->lungeTriggerDistance + 10.0f &&
        (state->flags & EDIBLE_MUSHROOM_ANIMATION_DONE) != 0) {
        state->animationState = EDIBLE_MUSHROOM_STATE_TRACK_PLAYER;
    } else if (targetApproachSpeed >= 0.54f) {
        state->moveAngle = edibleMushroom_chooseMovementAngle(obj, player, state, state->lungeRange);
        state->animationState = EDIBLE_MUSHROOM_STATE_LUNGING;
        Sfx_PlayFromObject(obj, SFXTRIG_mushrele16);
        obj->anim.rotX = (s16)(state->moveAngle - 0x4000);
    }
}

static void edibleMushroom_offerPickup(GameObject* obj, GameObject* player, EdibleMushroomState* state,
                                       const EdibleMushroomPlacement* placement) {
    if (mainGetBit(GAMEBIT_ITEM_TrickyFood_GrabInProgress) != 0 ||
        (player->objectFlags & OBJECT_OBJFLAG_PARENT_SLACK) != 0 ||
        Vec_xzDistance(&player->anim.worldPosX, &obj->anim.worldPosX) >= 25.0f) {
        return;
    }

    (*gExpgfxInterface)->freeSource((uintptr_t)obj);
    state->pickupOffer.gameBitId =
        obj->anim.romDefNo == EDIBLE_MUSHROOM_WHITE_ALIAS ? GAMEBIT_SawWhiteShroom : GAMEBIT_SawTrickyFood;
    edibleMushroom_spawnPickupEffect(obj);
    state->pickupOffer.value = 0;
    state->pickupMessageDelay = 0.4f;
    ObjMsg_SendToObject(player, OBJ_MESSAGE_PICKUP_IN_RANGE, obj, (uintptr_t)&state->pickupOffer);

    s16 placementGameBit = ObjAnim_ReadPlacementS16(&obj->anim, &placement->gameBitId);
    if (placementGameBit != -1) {
        mainSetBits(placementGameBit, 1);
    }
    state->animationState = EDIBLE_MUSHROOM_STATE_COLLECTED;
    mainSetBits(GAMEBIT_ITEM_TrickyFood_GrabInProgress, 1);
}

static void edibleMushroom_updateStruck(GameObject* obj, GameObject* player, EdibleMushroomState* state,
                                        const EdibleMushroomPlacement* placement) {
    ObjHits_ClearSourceMask(&obj->anim, 1);
    Sfx_KeepAliveLoopedObjectSound(obj, SFXTRIG_cagelp_c);

    if (state->burrowAttackTimer <= 0.0f) {
        state->burrowAttackTimer = (f32)randomGetRange(0xF0, 0x12C);
    }
    state->burrowAttackTimer -= timeDelta;
    if (state->burrowAttackTimer <= 0.0f) {
        ObjHits_SetSourceMask(&obj->anim, 1);
        (*gExpgfxInterface)->freeSource((uintptr_t)obj);
        state->animationState = EDIBLE_MUSHROOM_STATE_IDLE;
        state->flags &= ~EDIBLE_MUSHROOM_STRUCK;
        return;
    }

    state->sporePuffTimer -= timeDelta;
    if (state->sporePuffTimer <= 0.0f) {
        PartFxSpawnParams spawnParams;

        spawnParams.pos.x = 10.0f;
        spawnParams.pos.y = 12.0f;
        if ((obj->objectFlags & OBJECT_OBJFLAG_RENDERED) != 0) {
            (*gPartfxInterface)->spawnObject(obj, EDIBLE_MUSHROOM_SPORE_PUFF_EFFECT, &spawnParams, 2, -1, NULL);
        }
        state->sporePuffTimer = 20.0f;
    }

    edibleMushroom_offerPickup(obj, player, state, placement);
}

static void edibleMushroom_updateAnimation(GameObject* obj, EdibleMushroomState* state) {
    const EdibleMushroomAnimation* animation = &sEdibleMushroomAnimations[state->animationState];
    ObjAnimEventList animEvents;

    if (obj->anim.currentMove != animation->moveId && animation->moveId != -1) {
        ObjAnim_SetCurrentMove(obj, animation->moveId, 0.25f, 0);
    }

    if (ObjAnim_AdvanceCurrentMove(obj, animation->stepScale, timeDelta, &animEvents) != 0) {
        state->flags |= EDIBLE_MUSHROOM_ANIMATION_DONE;
    } else {
        state->flags &= ~EDIBLE_MUSHROOM_ANIMATION_DONE;
    }

    f32 movementSpeed;
    if (state->animationState == EDIBLE_MUSHROOM_STATE_LUNGING) {
        movementSpeed = state->lungeRootSpeedScale * (animEvents.rootDeltaX * oneOverTimeDelta);
    } else if (state->animationState == EDIBLE_MUSHROOM_STATE_RETREATING) {
        movementSpeed = animEvents.rootDeltaZ * oneOverTimeDelta;
    } else {
        movementSpeed = 0.0f;
    }

    if (movementSpeed != 0.0f) {
        state->flags |= EDIBLE_MUSHROOM_MOVING;
    } else {
        state->flags &= ~EDIBLE_MUSHROOM_MOVING;
    }

    f32 radians = (3.1415927f * state->moveAngle) / 32768.0f;
    obj->anim.velocityX = movementSpeed * mathSinf(radians);
    obj->anim.velocityZ = movementSpeed * mathCosf(radians);
    objMove(obj, obj->anim.velocityX * timeDelta, 0.0f, obj->anim.velocityZ * timeDelta);
}

static void edibleMushroom_updateBehavior(GameObject* obj, EdibleMushroomState* state,
                                          const EdibleMushroomPlacement* placement) {
    GameObject* player = Obj_GetPlayerObject();

    if ((state->flags & EDIBLE_MUSHROOM_GROUNDED) != 0) {
        state->animationState = EDIBLE_MUSHROOM_STATE_GROUNDED;
    }

    f32 targetApproachSpeed = oneOverTimeDelta * (state->previousTargetDistance - state->currentTargetDistance);
    int animationState = state->animationState;

    switch (animationState) {
    case EDIBLE_MUSHROOM_STATE_IDLE:
        edibleMushroom_updateIdle(obj, player, state, placement);
        break;
    case EDIBLE_MUSHROOM_STATE_LUNGING:
        if ((state->flags & EDIBLE_MUSHROOM_STRUCK) != 0) {
            state->animationState = EDIBLE_MUSHROOM_STATE_STRUCK;
        } else if ((state->flags & EDIBLE_MUSHROOM_ANIMATION_DONE) != 0) {
            state->animationState = EDIBLE_MUSHROOM_STATE_IDLE;
        }
        break;
    case EDIBLE_MUSHROOM_STATE_NOTICE_PLAYER:
    case EDIBLE_MUSHROOM_STATE_RETURN_TO_IDLE:
        if ((state->flags & EDIBLE_MUSHROOM_STRUCK) != 0) {
            state->animationState = EDIBLE_MUSHROOM_STATE_STRUCK;
        } else if ((state->flags & EDIBLE_MUSHROOM_ANIMATION_DONE) != 0) {
            state->animationState = animationState == EDIBLE_MUSHROOM_STATE_NOTICE_PLAYER
                                        ? EDIBLE_MUSHROOM_STATE_TRACK_PLAYER
                                        : EDIBLE_MUSHROOM_STATE_IDLE;
        } else {
            edibleMushroom_updateTracking(obj, player, state, placement, targetApproachSpeed);
        }
        break;
    case EDIBLE_MUSHROOM_STATE_TRACK_PLAYER:
        edibleMushroom_updateTracking(obj, player, state, placement, targetApproachSpeed);
        break;
    case EDIBLE_MUSHROOM_STATE_RETREATING:
        edibleMushroom_updateRetreat(obj, player, state, placement, targetApproachSpeed);
        break;
    case EDIBLE_MUSHROOM_STATE_STRUCK:
        edibleMushroom_updateStruck(obj, player, state, placement);
        break;
    case EDIBLE_MUSHROOM_STATE_GROUNDED:
        if ((state->flags & EDIBLE_MUSHROOM_STRUCK) != 0) {
            state->animationState = EDIBLE_MUSHROOM_STATE_STRUCK;
        }
        break;
    case EDIBLE_MUSHROOM_STATE_UNUSED_2:
    case EDIBLE_MUSHROOM_STATE_COLLECTED:
    case EDIBLE_MUSHROOM_STATE_UNUSED_10:
        break;
    }

    edibleMushroom_updateAnimation(obj, state);
}

static int edibleMushroom_getExtraSize(void) {
    return sizeof(EdibleMushroomState);
}

static void edibleMushroom_free(GameObject* obj) {
    objFreeObjectType(obj, EDIBLE_MUSHROOM_GROUP);
    objFreeObjectType(obj, EDIBLE_MUSHROOM_SECONDARY_GROUP);
}

static void edibleMushroom_hitDetect(GameObject* obj) {
    EdibleMushroomState* state = obj->extra;
    const EdibleMushroomPlacement* placement = (EdibleMushroomPlacement*)obj->anim.placementData;
    ObjHitsPriorityState* hitState = (ObjHitsPriorityState*)obj->anim.hitReactState;

    if ((obj->objectFlags & OBJECT_OBJFLAG_PARENT_SLACK) != 0 ||
        ((state->flags & EDIBLE_MUSHROOM_MOVING) == 0 &&
         (hitState->flags & OBJHITS_PRIORITY_STATE_PAIR_RESPONSE_APPLIED) == 0)) {
        return;
    }

    TrackGroundHit** hits;
    int hitCount = trackGetHeight(obj, obj->anim.localPosX, obj->anim.localPosY, obj->anim.localPosZ, &hits, 0, 0);
    for (int i = 0; i < hitCount; i++) {
        if (hits[i]->height < obj->anim.localPosY + 10.0f) {
            obj->anim.localPosY = hits[i]->height;
            break;
        }
    }

    TrackBBoxHit bboxHit;
    hitCount = trackGetLineIntersect(&obj->anim.previousLocalPosX, &obj->anim.localPosX, 6.0f, 2, &bboxHit, obj, 8, -1,
                                     0xFF, 0x14);
    if (placement->objectType == EDIBLE_MUSHROOM_TYPE_CURVE_A && hitCount != 0 &&
        bboxHit.surfaceType == EDIBLE_MUSHROOM_GROUND_SURFACE) {
        state->flags |= EDIBLE_MUSHROOM_GROUNDED;
    }
}

static void edibleMushroom_processPickupMessages(GameObject* obj, EdibleMushroomState* state) {
    u32 message;

    while (ObjMsg_Pop(obj, &message, NULL, NULL) != 0) {
        if (message != OBJ_MESSAGE_PICKUP_COMPLETE) {
            continue;
        }

        obj->anim.flags |= OBJANIM_FLAG_HIDDEN;
        ObjHits_DisableObject(obj);
        gameBitIncrement(state->collectedGameBitId);
        mainSetBits(GAMEBIT_ITEM_TrickyFood_GrabInProgress, 0);
        edibleMushroom_spawnPickupEffect(obj);
        Sfx_PlayFromObject(obj, SFXTRIG_cam90_c);
    }
}

static void edibleMushroom_updateTargetDistance(GameObject* obj, GameObject* player, GameObject* tricky,
                                                EdibleMushroomState* state, const EdibleMushroomPlacement* placement) {
    state->previousTargetDistance = state->currentTargetDistance;
    f32 playerDistanceSquared = vec3f_distanceSquared(&player->anim.worldPosX, &obj->anim.worldPosX);

    if (tricky == NULL) {
        state->currentTargetDistance = sqrtf(playerDistanceSquared);
        return;
    }

    f32 trickyDistanceSquared = vec3f_distanceSquared(&tricky->anim.worldPosX, &obj->anim.worldPosX);
    state->currentTargetDistance =
        sqrtf(playerDistanceSquared < trickyDistanceSquared ? playerDistanceSquared : trickyDistanceSquared);
    if (state->currentTargetDistance < placement->retreatTriggerDistance) {
        TrickyCompanionInterface* trickyInterface = (TrickyCompanionInterface*)*tricky->anim.dll;

        trickyInterface->sideCommandEnable(tricky, obj, 0, 1);
    }
}

static void edibleMushroom_update(GameObject* obj) {
    EdibleMushroomState* state = obj->extra;
    const EdibleMushroomPlacement* placement = (EdibleMushroomPlacement*)obj->anim.placementData;
    GameObject* player = Obj_GetPlayerObject();
    GameObject* tricky = getTrickyObject();

    if (objIsFrozen(obj) != 0) {
        return;
    }

    if (state->animationState == EDIBLE_MUSHROOM_STATE_COLLECTED) {
        edibleMushroom_processPickupMessages(obj, state);
        return;
    }

    if (state->sequenceResetPending != 0) {
        obj->anim.localPosX = placement->base.posX;
        obj->anim.localPosY = placement->base.posY;
        obj->anim.localPosZ = placement->base.posZ;
        obj->anim.alpha = 0xFF;
        state->sequenceResetPending = 0;
    }

    edibleMushroom_updateTargetDistance(obj, player, tricky, state, placement);

    GameObject* hitObject;
    int hitKind = ObjHits_GetPriorityHit(obj, &hitObject, NULL, NULL);
    if (hitKind == EDIBLE_MUSHROOM_FADE_IN_HIT) {
        Obj_StartModelFadeIn(obj, 0x12C);
    } else if (hitKind != 0) {
        Obj_SetModelColorFadeRecursive(obj, 0xF, 0xC8, 0, 0, 1);
        if (hitObject->anim.romDefNo != EDIBLE_MUSHROOM_EARTH_WARRIOR_ALIAS) {
            if ((state->flags & EDIBLE_MUSHROOM_STRUCK) == 0) {
                Sfx_PlayFromObject(obj, SFXTRIG_mv_ladderslide16);
            }
            state->flags |= EDIBLE_MUSHROOM_STRUCK;
        }
    }

    edibleMushroom_updateBehavior(obj, state, placement);
}

static void edibleMushroom_init(GameObject* obj, const EdibleMushroomPlacement* placement) {
    EdibleMushroomState* state = obj->extra;
    GameObject* player = Obj_GetPlayerObject();
    int curveInitParam = 0x19;
    ObjAnimEventList animEvents;

    obj->animEventCallback = edibleMushroom_animEventCallback;
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN;

    if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &placement->gameBitId)) != 0) {
        state->animationState = EDIBLE_MUSHROOM_STATE_COLLECTED;
        ObjHits_DisableObject(obj);
        obj->anim.flags |= OBJANIM_FLAG_HIDDEN;
    }

    obj->anim.modelState->flags |= OBJ_MODEL_STATE_UNREAD_0800 | OBJ_MODEL_STATE_UNREAD_0010;
    state->lungeRootSpeedScale = 0.5f;
    state->mapParamScale = 0.2f * ((f32)placement->scaleParam / 255.0f);

    ObjAnim_SetCurrentMove(obj, 1, 0.0f, 0);
    ObjAnim_AdvanceCurrentMove(obj, 1.0f, 1.0f, &animEvents);
    state->lungeRange = animEvents.rootDeltaX;
    if (state->lungeRange < 0.0f) {
        state->lungeRange = -state->lungeRange;
    }
    state->lungeRange = state->lungeRange * state->lungeRootSpeedScale;
    state->lungeRange += 20.0f;

    ObjAnim_SetCurrentMove(obj, 4, 0.0f, 0);
    ObjAnim_AdvanceCurrentMove(obj, 1.0f, 1.0f, &animEvents);
    state->retreatRange = animEvents.rootDeltaZ;
    if (state->retreatRange < 0.0f) {
        state->retreatRange = -state->retreatRange;
    }
    state->retreatRange += 20.0f;

    ObjMsg_AllocQueue(obj, 1);

    if (placement->objectType == EDIBLE_MUSHROOM_TYPE_CURVE_A ||
        placement->objectType == EDIBLE_MUSHROOM_TYPE_CURVE_B) {
        state->flags |= EDIBLE_MUSHROOM_ON_CURVE;
        (*gRomCurveInterface)->initCurve(&state->curveWalker, obj, 1000.0f, &curveInitParam, -1);
        obj->anim.localPosX = state->curveWalker.curve.sample[0];
        obj->anim.localPosZ = state->curveWalker.curve.sample[2];
    }

    state->curveAdvanceStep = 5.0f;

    if (player != NULL) {
        f32 distance = Vec_distance(&player->anim.worldPosX, &obj->anim.worldPosX);

        state->currentTargetDistance = distance;
        state->previousTargetDistance = distance;
    } else {
        state->currentTargetDistance = 200.0f;
        state->previousTargetDistance = 200.0f;
    }

    objAddObjectType(obj, EDIBLE_MUSHROOM_SECONDARY_GROUP);
    objAddObjectType(obj, EDIBLE_MUSHROOM_GROUP);

    state->collectedGameBitId = obj->anim.romDefNo == EDIBLE_MUSHROOM_WHITE_ALIAS ? GAMEBIT_ITEM_WhiteShroom_Count
                                                                                  : GAMEBIT_ITEM_TrickyFood_Count;
}

OBJECT_INIT_ADAPTER(gEdibleMushroomObjDescriptorInitAdapter, edibleMushroom_init, obj, placement)
OBJECT_FREE_ADAPTER(gEdibleMushroomObjDescriptorFreeAdapter, edibleMushroom_free, obj)
OBJECT_EXTRA_SIZE_ADAPTER(gEdibleMushroomObjDescriptorExtraSizeAdapter, edibleMushroom_getExtraSize)

ObjectDescriptor gEdibleMushroomObjDescriptor = {
    .header = {
        .metadata = { 0, 0, 0, OBJECT_DESCRIPTOR_FLAGS_10_SLOTS },
        .acquire = NULL,
        .release = NULL,
    },
    .slot02 = NULL,
    .init = gEdibleMushroomObjDescriptorInitAdapter,
    .update = edibleMushroom_update,
    .hitDetect = edibleMushroom_hitDetect,
    .render = NULL,
    .free = gEdibleMushroomObjDescriptorFreeAdapter,
    .getObjectTypeId = NULL,
    .getExtraSize = gEdibleMushroomObjDescriptorExtraSizeAdapter,
};;
