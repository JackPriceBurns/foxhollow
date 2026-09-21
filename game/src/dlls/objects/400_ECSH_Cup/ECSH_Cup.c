/*
 * ECSH_Cup (DLL 0x190) drives one cup in the Krazoa Test of Observation.
 *
 * The cup follows shuffle positions supplied by ECSH_Shrine, animates the
 * puzzle's rise and sink transitions, and reports player picks to the shrine.
 */
#include "dlls/objects/400_ECSH_Cup.h"

#include "dlls/objects/399_ECSH_Shrine.h"
#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/dll/expgfx_interface.h"
#include "main/dll/partfx_interface.h"
#include "main/frame_timing.h"
#include "main/object_render.h"
#include "main/objseq.h"
#include "main/objtype.h"
#include "main/vecmath_distance.h"
#include "sys/objects.h"
#include "main/objhits.h"
#include "main/vecmath.h"

typedef struct ECSHCupPlacement {
    ObjPlacement base;
    u8 unk18[2];
    s16 cupIndex;
} ECSHCupPlacement;

typedef struct ECSHCupState {
    Vec3f startPos;
    Vec3f velocity;
    f32 transitionHeight;
    f32 particleTimer;
    f32 bobTimer;
    s32 currentAnimState;
    s32 cupIndex;
    s16 spinRate;
    s8 bobDirection;
    u8 unk2F;
} ECSHCupState;

typedef enum ECSHCupAnimState {
    ECSH_CUP_ANIM_STATE_STATIONARY = 0,
    ECSH_CUP_ANIM_STATE_MOVE_TO_SLOT = 1,
    ECSH_CUP_ANIM_STATE_STORE_SLOT_POSITION = 2,
    ECSH_CUP_ANIM_STATE_HOLD = 3,
    ECSH_CUP_ANIM_STATE_SNAP_TO_SLOT = 4,
    ECSH_CUP_ANIM_STATE_CHECK_PICK = 5,
    ECSH_CUP_ANIM_STATE_RISE = 6,
    ECSH_CUP_ANIM_STATE_SINK = 7,
    ECSH_CUP_ANIM_STATE_RUN_ACTIVE_SEQUENCE = 8,
} ECSHCupAnimState;

typedef enum ECSHCupPartFxId {
    ECSH_CUP_PARTFX_IDLE = 0x270,
    ECSH_CUP_PARTFX_TRANSITION = 0x271,
} ECSHCupPartFxId;

typedef enum ECSHCupSequenceSlot {
    ECSH_CUP_SEQUENCE_ACTIVE = 0,
    ECSH_CUP_SEQUENCE_PICKED = 1,
} ECSHCupSequenceSlot;

STATIC_ASSERT(sizeof(ECSHCupPlacement) == 0x1C);
STATIC_ASSERT(offsetof(ECSHCupPlacement, cupIndex) == 0x1A);
STATIC_ASSERT(sizeof(ECSHCupState) == 0x30);
STATIC_ASSERT(offsetof(ECSHCupState, velocity) == 0x0C);
STATIC_ASSERT(offsetof(ECSHCupState, transitionHeight) == 0x18);
STATIC_ASSERT(offsetof(ECSHCupState, currentAnimState) == 0x24);
STATIC_ASSERT(offsetof(ECSHCupState, cupIndex) == 0x28);
STATIC_ASSERT(offsetof(ECSHCupState, bobDirection) == 0x2E);

static GameObject* gECSHCupShrineObject;

static int ecshCup_getExtraSize(void) {
    return sizeof(ECSHCupState);
}

static int ecshCup_getObjectTypeId(void) {
    return 0;
}

static void ecshCup_free(GameObject* obj) {
    (*gExpgfxInterface)->freeSource2((uintptr_t)obj);
}

static void ecshCup_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                           s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

static void ecshCup_hitDetect(void) {
}

static void ecshCup_update(GameObject* obj) {
    f32 searchDistance = 500.0f;
    int mode = -1;
    u8 spiritCup = 0;
    Vec3f slotPosition = {0.0f, 0.0f, 0.0f};
    GameObject* player = Obj_GetPlayerObject();
    ECSHCupState* state = obj->extra;

    if (gECSHCupShrineObject == NULL) {
        gECSHCupShrineObject = objGetNearestTypeTo(OBJECT_CLASS_KRAZOA_SHRINE, obj, &searchDistance);
    }
    if (gECSHCupShrineObject != NULL && gECSHCupShrineObject->anim.classId != 0) {
        ECSHShrineInterface* shrineInterface = (ECSHShrineInterface*)*gECSHCupShrineObject->anim.dll;

        shrineInterface->getPhaseAndSpiritCup(&mode, &spiritCup);
        obj->anim.rotX += state->spinRate;
        if (mode != ECSH_CUP_ANIM_STATE_RISE) {
            state->particleTimer -= timeDelta;
            if (state->particleTimer <= 0.0f) {
                state->particleTimer = 10.0f;
                if (mode != ECSH_CUP_ANIM_STATE_HOLD && mode != ECSH_CUP_ANIM_STATE_RISE &&
                    mode != ECSH_CUP_ANIM_STATE_SINK) {
                    (*gPartfxInterface)->spawnObject(obj, ECSH_CUP_PARTFX_IDLE, NULL, 0, -1, NULL);
                }
            }
        }
        state->bobTimer -= timeDelta;
        if (state->bobTimer <= 0.0f) {
            state->bobDirection = -state->bobDirection;
            state->bobTimer = 100.0f;
        }
        obj->anim.localPosY += 0.02f * state->bobDirection;
        if (mode == ECSH_CUP_ANIM_STATE_MOVE_TO_SLOT && state->currentAnimState == ECSH_CUP_ANIM_STATE_MOVE_TO_SLOT) {
            obj->anim.localPosX += state->velocity.x * timeDelta;
            obj->anim.localPosZ += state->velocity.z * timeDelta;
            ObjHits_EnableObject(obj);
            ObjHits_SetHitVolumeSlot(&obj->anim, 10, 1, 0);
            ObjHits_SyncObjectPositionIfDirty(obj);
        } else {
            ObjHits_EnableObject(obj);
            ObjHits_SetHitVolumeSlot(&obj->anim, 0, 0, 0);
            ObjHits_SyncObjectPositionIfDirty(obj);
        }
        if (mode == ECSH_CUP_ANIM_STATE_RISE) {
            if (obj->anim.localPosY < state->transitionHeight) {
                obj->anim.localPosY += 0.5f * timeDelta;
            }
            if (obj->anim.renderAlpha != 0xFF) {
                f32 fade = (f32)(u32)obj->anim.renderAlpha;

                fade += 2.0f * timeDelta;
                if (fade >= 255.0f) {
                    fade = 255.0f;
                }
                obj->anim.renderAlpha = (u8)fade;
            }
            state->particleTimer -= timeDelta;
            if (state->particleTimer <= 0.0f) {
                state->particleTimer = 10.0f;
                (*gPartfxInterface)->spawnObject(obj, ECSH_CUP_PARTFX_TRANSITION, NULL, 0, -1, NULL);
            }
        } else if (mode == ECSH_CUP_ANIM_STATE_SINK) {
            if (obj->anim.localPosY > state->transitionHeight - 50.0f) {
                obj->anim.localPosY -= 0.5f * timeDelta;
                state->particleTimer -= timeDelta;
                if (state->particleTimer <= 0.0f) {
                    state->particleTimer = 10.0f;
                    if (mode != ECSH_CUP_ANIM_STATE_HOLD) {
                        (*gPartfxInterface)->spawnObject(obj, ECSH_CUP_PARTFX_TRANSITION, NULL, 0, -1, NULL);
                    }
                }
            }
            if (obj->anim.renderAlpha != 0) {
                f32 fade = (f32)(u32)obj->anim.renderAlpha;

                fade -= 2.0f * timeDelta;
                if (fade <= 0.0f) {
                    fade = 0.0f;
                }
                obj->anim.renderAlpha = (u8)fade;
            }
        } else if (mode == ECSH_CUP_ANIM_STATE_RUN_ACTIVE_SEQUENCE && mode != state->currentAnimState) {
            if (state->cupIndex == spiritCup) {
                (*gObjectTriggerInterface)->runSequence(ECSH_CUP_SEQUENCE_ACTIVE, obj, -1);
            }
            state->currentAnimState = mode;
        } else if (mode == ECSH_CUP_ANIM_STATE_MOVE_TO_SLOT && mode != state->currentAnimState) {
            shrineInterface->getCupPosition((u8)state->cupIndex, &slotPosition.x, &slotPosition.z);
            state->velocity.x = (slotPosition.x - obj->anim.localPosX) / 100.0f;
            state->velocity.z = (slotPosition.z - obj->anim.localPosZ) / 100.0f;
            state->startPos.x = obj->anim.localPosX;
            state->startPos.z = obj->anim.localPosZ;
            state->currentAnimState = mode;
        } else if (mode == ECSH_CUP_ANIM_STATE_STATIONARY && mode != state->currentAnimState) {
            state->velocity.x = 0.0f;
            state->velocity.z = 0.0f;
            state->currentAnimState = mode;
        } else if (mode == ECSH_CUP_ANIM_STATE_STORE_SLOT_POSITION && mode != state->currentAnimState) {
            state->velocity.x = 0.0f;
            state->velocity.z = 0.0f;
            shrineInterface->setCupPosition((u8)state->cupIndex, obj->anim.localPosX, obj->anim.localPosZ);
            state->currentAnimState = mode;
        } else if (mode == ECSH_CUP_ANIM_STATE_HOLD && mode != state->currentAnimState) {
            state->currentAnimState = mode;
        } else if (mode == ECSH_CUP_ANIM_STATE_SNAP_TO_SLOT && mode != state->currentAnimState) {
            shrineInterface->getCupPosition((u8)state->cupIndex, &slotPosition.x, &slotPosition.z);
            obj->anim.localPosX = slotPosition.x;
            obj->anim.localPosZ = slotPosition.z;
            state->currentAnimState = mode;
        } else if (mode == ECSH_CUP_ANIM_STATE_CHECK_PICK) {
            if (player != NULL) {
                if (Vec_distance(&obj->anim.worldPosX, &player->anim.worldPosX) < 30.0f) {
                    shrineInterface->checkCupPick((u8)state->cupIndex);
                    if (state->cupIndex == spiritCup) {
                        (*gObjectTriggerInterface)->runSequence(ECSH_CUP_SEQUENCE_PICKED, obj, -1);
                    }
                }
            }
        }
    }
}

static void ecshCup_init(GameObject* obj, const ECSHCupPlacement* placement) {
    ECSHCupState* state = obj->extra;
    f32 searchDistance = 500.0f;

    gECSHCupShrineObject = NULL;
    state->startPos.x = obj->anim.localPosX;
    state->startPos.y = obj->anim.localPosY;
    state->startPos.z = obj->anim.localPosZ;
    state->transitionHeight = obj->anim.localPosY;
    obj->anim.localPosY -= 50.0f;
    state->velocity = (Vec3f){0.0f, 0.0f, 0.0f};
    state->currentAnimState = ECSH_CUP_ANIM_STATE_STATIONARY;
    state->cupIndex = ObjAnim_ReadPlacementS16(&obj->anim, &placement->cupIndex);
    state->bobTimer = randomGetRange(0, 0x258);
    state->spinRate = randomGetRange(-0x320, 0x320);
    state->bobDirection = 1;
    obj->anim.renderAlpha = 0;
    state->particleTimer = 0.0f;
    if (gECSHCupShrineObject == NULL) {
        gECSHCupShrineObject = objGetNearestTypeTo(OBJECT_CLASS_KRAZOA_SHRINE, obj, &searchDistance);
    }
    ObjHits_EnableObject(obj);
    ObjHits_SetHitVolumeSlot(&obj->anim, 0, 0, 0);
    ObjHits_SyncObjectPositionIfDirty(obj);
}

static void ecshCup_release(void) {
}

static void ecshCup_initialise(void) {
}

OBJECT_INIT_ADAPTER(gECSHCupObjDescriptorInitAdapter, ecshCup_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gECSHCupObjDescriptorHitDetectAdapter, ecshCup_hitDetect)
OBJECT_FREE_ADAPTER(gECSHCupObjDescriptorFreeAdapter, ecshCup_free, obj)
OBJECT_TYPE_ID_ADAPTER(gECSHCupObjDescriptorTypeIdAdapter, ecshCup_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gECSHCupObjDescriptorExtraSizeAdapter, ecshCup_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gECSHCupObjDescriptorAcquire, ecshCup_initialise)

ObjectDescriptor gECSHCupObjDescriptor = {
    .header = {
        .metadata = { 0, 0, 0, OBJECT_DESCRIPTOR_FLAGS_10_SLOTS },
        .acquire = gECSHCupObjDescriptorAcquire,
        .release = ecshCup_release,
    },
    .init = gECSHCupObjDescriptorInitAdapter,
    .update = ecshCup_update,
    .hitDetect = gECSHCupObjDescriptorHitDetectAdapter,
    .render = ecshCup_render,
    .free = gECSHCupObjDescriptorFreeAdapter,
    .getObjectTypeId = gECSHCupObjDescriptorTypeIdAdapter,
    .getExtraSize = gECSHCupObjDescriptorExtraSizeAdapter,
};;
