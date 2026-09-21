/* Destructible scenery that shatters into physics fragments. */

#include "dlls/objects/346.h"
#include "dlls/objects/358.h"

#include "dolphin/math.h"
#include "main/gamebits.h"
#include "main/model.h"
#include "sys/objects.h"
#include "sys/objects/lifecycle.h"
#include "main/audio/sfx.h"
#include "main/objtype.h"
#include "main/vecmath.h"
#include "track/intersect.h"

#define EXPLODABLE_FRAGMENT_SETUP_MODE           5
#define EXPLODABLE_RECIPE_FLAG_HIDE_OBJECT       0x01
#define EXPLODABLE_LAUNCH_FLAG_VELOCITY_X        0x01
#define EXPLODABLE_LAUNCH_FLAG_VELOCITY_Z        0x02
#define EXPLODABLE_LAUNCH_FLAG_SPIN_X            0x04
#define EXPLODABLE_LAUNCH_FLAG_SPIN_Y            0x08
#define EXPLODABLE_LAUNCH_FLAG_SPIN_Z            0x10
#define EXPLODABLE_FRAGMENT_FULL_ALPHA           0xFF
#define EXPLODABLE_DEFAULT_SCALE                 20

GameObject* explodable_spawnFragmentObject(GameObject* obj, int fragmentObjectId, ExplodableChunk* chunk,
                                           int fragmentIndex) {
    ExplodedPlacement* fragmentPlacement;
    f32 scale;

    if (Obj_IsLoadingLocked() == 0) {
        return 0;
    }
    fragmentPlacement = (ExplodedPlacement*)Obj_AllocObjectSetup(sizeof(ExplodedPlacement), fragmentObjectId);
    fragmentPlacement->base.objectId = fragmentObjectId;
    fragmentPlacement->base.color[0] = 2;
    fragmentPlacement->base.color[2] = EXPLODABLE_FRAGMENT_FULL_ALPHA;
    fragmentPlacement->base.color[1] = 1;
    fragmentPlacement->base.color[3] = EXPLODABLE_FRAGMENT_FULL_ALPHA;
    fragmentPlacement->base.posX = obj->anim.localPosX;
    fragmentPlacement->base.posY = obj->anim.localPosY;
    fragmentPlacement->base.posZ = obj->anim.localPosZ;
    scale = 100.0f;
    fragmentPlacement->initialVelocity.x = 100.0f * chunk->velocityX;
    fragmentPlacement->initialVelocity.y = scale * chunk->velocityY;
    fragmentPlacement->initialVelocity.z = scale * chunk->velocityZ;
    fragmentPlacement->initialRotation.x = chunk->rotX;
    fragmentPlacement->initialRotation.y = chunk->rotY;
    fragmentPlacement->initialRotation.z = chunk->rotZ;
    fragmentPlacement->spin.x = chunk->spinX * (f32)(u32)chunk->spinScale;
    fragmentPlacement->spin.y = chunk->spinY * (f32)(u32)chunk->spinScale;
    fragmentPlacement->spin.z = chunk->spinZ * (f32)(u32)chunk->spinScale;
    scale = 10.0f;
    fragmentPlacement->spinVelocity.x = 10.0f * chunk->secondarySpinX;
    fragmentPlacement->spinVelocity.z = scale * chunk->secondarySpinZ;
    fragmentPlacement->spinVelocity.y = scale * chunk->secondarySpinY;
    scale = 1000.0f;
    fragmentPlacement->acceleration.x = 1000.0f * chunk->secondaryVelocityX;
    fragmentPlacement->acceleration.y = scale * chunk->secondaryVelocityY;
    fragmentPlacement->acceleration.z = scale * chunk->secondaryVelocityZ;
    fragmentPlacement->modelBankIndex = fragmentIndex;
    fragmentPlacement->scaleByte =
        (s8)(int)(20.0f * (obj->anim.rootMotionScale / obj->anim.modelInstance->rootMotionScaleBase));
    fragmentPlacement->lifetimeFrames = chunk->launchDelayBase;
    fragmentPlacement->floorOffset = (int)chunk->height;
    return objSetupObject((ObjPlacement*)fragmentPlacement, EXPLODABLE_FRAGMENT_SETUP_MODE, obj->anim.mapEventSlot, -1,
                           NULL);
}

void explodable_buildFragments(GameObject* obj, ExplodablePlacement* placementAddress, int skipCentroid, ExplodableState* state) {
    ExplodableChunk* chunk;
    int fragmentIndex;
    int fragmentObjectId;
    u8 spinScale;
    int vertexIndex;
    ModelFileHeader* model;
    ExplodableBreakRecipe* recipes;
    f32 zero;
    struct {
        f32 vertex[3];
        f32 sum[3];
    } centroidScratch;

    recipes = gExplodableBreakRecipeTable;
    fragmentObjectId = recipes[state->recipeIndex].fragmentObjectId;
    state->breakSfxId = recipes[state->recipeIndex].breakSfxId;
    spinScale = recipes[state->recipeIndex].spinScale;
    if (fragmentObjectId != -1) {
        fragmentIndex = 0;
        chunk = (ExplodableChunk*)state;
        for (; fragmentIndex < state->fragmentCount; fragmentIndex++) {
            state->spawnedFlags[fragmentIndex] = 1;
            chunk->spinScale = spinScale;
            if (skipCentroid == 0) {
                zero = 0.0f;
                chunk->centroidX = zero;
                chunk->centroidY = zero;
                chunk->centroidZ = zero;
                model = obj->anim.modelBanks[fragmentIndex]->file;
                centroidScratch.sum[0] = zero;
                centroidScratch.sum[1] = zero;
                centroidScratch.sum[2] = zero;
                for (vertexIndex = 0; vertexIndex < model->vertexCount; vertexIndex++) {
                    Model_GetVertexPosition(model, vertexIndex, centroidScratch.vertex);
                    centroidScratch.sum[0] = centroidScratch.vertex[0] + centroidScratch.sum[0];
                    centroidScratch.sum[1] = centroidScratch.vertex[1] + centroidScratch.sum[1];
                    centroidScratch.sum[2] = centroidScratch.vertex[2] + centroidScratch.sum[2];
                }
                chunk->centroidX = centroidScratch.sum[0] * ((zero = 1.0f) / (f32)(u32)model->vertexCount);
                chunk->centroidY = centroidScratch.sum[1] * (zero / (f32)(u32)model->vertexCount);
                chunk->centroidZ = centroidScratch.sum[2] * (zero / (f32)(u32)model->vertexCount);
            }
            chunk->offsetX = chunk->centroidX;
            chunk->offsetY = chunk->centroidY;
            chunk->offsetZ = chunk->centroidZ;
            explodable_computeFragmentLaunch(obj, chunk, placementAddress);
            chunk->unknown6B = EXPLODABLE_FRAGMENT_FULL_ALPHA;
            chunk->gameBitMode = mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &(placementAddress->doneGameBit))) != 0 ? 2 : 0;
            state->children[fragmentIndex] =
                explodable_spawnFragmentObject(obj, fragmentObjectId, chunk, fragmentIndex);
            chunk++;
        }
        state->phase = (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &(placementAddress->doneGameBit))) != 0)
                           ? EXPLODABLE_PHASE_BREAKING
                           : EXPLODABLE_PHASE_WAIT;
    }
}

void explodable_computeFragmentLaunch(GameObject* obj, ExplodableChunk* chunk, ExplodablePlacement* placementAddress) {
    f32 dx;
    f32 dy;
    f32 dz;
    f32 magnitude;
    f32 scale;
    int secondaryRandomMaximum;
    int randomMaximum;
    s16 rotation[3];
    u16 launchDelayBase;
    ExplodablePlacement* placement = placementAddress;
    f32 zero = 0.0f;

    rotation[0] = ObjAnim_ReadPlacementS16(&obj->anim, &placement->rotX);
    rotation[1] = ObjAnim_ReadPlacementS16(&obj->anim, &placement->rotY);
    rotation[2] = ObjAnim_ReadPlacementS16(&obj->anim, &placement->rotZ);
    vecRotateZXY(rotation, &chunk->offsetX);
    chunk->positionX = chunk->offsetX * obj->anim.rootMotionScale + placement->base.posX;
    chunk->positionY = chunk->offsetY * obj->anim.rootMotionScale + placement->base.posY;
    chunk->positionZ = chunk->offsetZ * obj->anim.rootMotionScale + placement->base.posZ;
    chunk->rotX = rotation[0];
    chunk->rotY = rotation[1];
    chunk->rotZ = rotation[2];
    dx = chunk->offsetX - (f32)ObjAnim_ReadPlacementS16(&obj->anim, &placement->originX);
    dy = chunk->offsetY - (f32)ObjAnim_ReadPlacementS16(&obj->anim, &placement->originY);
    dz = chunk->offsetZ - (f32)ObjAnim_ReadPlacementS16(&obj->anim, &placement->originZ);
    magnitude = sqrtf(dz * dz + (dx * dx + dy * dy));
    if (magnitude != zero) {
        scale = (f32)ObjAnim_ReadPlacementS16(&obj->anim, &placement->launchForce) / (5.0f * magnitude);
        if (dx != zero || dy != zero || dz != zero) {
            normalize(&dx, &dy, &dz);
        }
        chunk->velocityX = dx * scale;
        chunk->velocityY = dy * scale;
        chunk->velocityZ = dz * scale;
        randomMaximum = (int)(200.0f * (0.5f + scale));
        chunk->spinX = (f32)randomGetRange(0, randomMaximum) / 50.0f;
        chunk->spinY = (f32)randomGetRange(0, randomMaximum) / 50.0f;
        chunk->spinZ = (f32)randomGetRange(0, randomMaximum) / 50.0f;
        scale = (f32)ObjAnim_ReadPlacementS16(&obj->anim, &placement->secondaryLaunchScale) / 1000.0f;
        if (obj->anim.velocityX > 0.0f) {
            chunk->launchFlags |= EXPLODABLE_LAUNCH_FLAG_VELOCITY_X;
        }
        if (obj->anim.velocityZ > 0.0f) {
            chunk->launchFlags |= EXPLODABLE_LAUNCH_FLAG_VELOCITY_Z;
        }
        if (chunk->spinX > 0.0f) {
            chunk->launchFlags |= EXPLODABLE_LAUNCH_FLAG_SPIN_X;
        }
        if (chunk->spinY > 0.0f) {
            chunk->launchFlags |= EXPLODABLE_LAUNCH_FLAG_SPIN_Y;
        }
        if (chunk->spinZ > 0.0f) {
            chunk->launchFlags |= EXPLODABLE_LAUNCH_FLAG_SPIN_Z;
        }
        secondaryRandomMaximum = (int)(200.0f * (0.5f + scale));
        chunk->secondarySpinX = (f32)randomGetRange(0, secondaryRandomMaximum) / 200.0f;
        chunk->secondarySpinY = (f32)randomGetRange(0, secondaryRandomMaximum) / 200.0f;
        chunk->secondarySpinZ = (f32)randomGetRange(0, secondaryRandomMaximum) / 200.0f;
        chunk->secondaryVelocityX = dx * scale;
        chunk->secondaryVelocityY = dy * scale - 0.07f;
        chunk->secondaryVelocityZ = dz * scale;
        {
            int height = ObjAnim_ReadPlacementS16(&obj->anim, &placement->fragmentHeight);
            if (height != 0) {
                chunk->height = height;
            }
        }
        launchDelayBase = ObjAnim_ReadPlacementU16(&obj->anim, &placement->launchDelayBase);
        chunk->launchDelayBase = launchDelayBase;
        if (launchDelayBase != 0) {
            chunk->launchDelay = (int)(launchDelayBase * (randomGetRange(0, 100) + 100)) / 200;
        } else {
            chunk->launchDelay = -1;
        }
    }
}

int explodable_getExtraSize(void) {
    return sizeof(ExplodableState);
}

void explodable_free(GameObject* obj, int keepChildren) {
    ExplodableState* state;
    int fragmentIndex = -1;
    GameObject* child;

    state = (ExplodableState*)obj->extra;
    objFreeObjectType(obj, EXPLODABLE_OBJECT_GROUP);
    if (keepChildren == 0) {
        while (++fragmentIndex < EXPLODABLE_FRAGMENT_COUNT) {
            child = state->children[fragmentIndex];
            if (child != NULL) {
                Obj_FreeObject(child);
            }
        }
    }
}

void explodable_render(void) {
}

void explodable_update(GameObject* obj) {
    int fragmentIndex;
    int fragmentStatus;
    GameObject* fragmentObject;
    ExplodableState* state;
    ExplodablePlacement* placement;

    state = (ExplodableState*)obj->extra;
    placement = (ExplodablePlacement*)obj->anim.placementData;
    if (state->phase != EXPLODABLE_PHASE_BROKEN) {
        if (state->phase == EXPLODABLE_PHASE_WAIT) {
            if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &(placement->activateGameBit))) != 0) {
                explodable_buildFragments(obj, placement, 0, state);
                if (state->breakSfxId != 0) {
                    Sfx_PlayFromObject(obj, state->breakSfxId & 0xffff);
                }
                state->phase = EXPLODABLE_PHASE_BREAKING;
                obj->anim.alpha = 0;
            } else {
                return;
            }
        } else {
            fragmentIndex = 0;
            do {
                fragmentObject = state->children[fragmentIndex];
                if (fragmentObject != NULL) {
                    fragmentStatus = EXPLODED_INTERFACE(fragmentObject)->getPhase(fragmentObject);
                    switch (fragmentStatus) {
                    case EXPLODED_PHASE_EXPIRED:
                        mainSetBits(ObjAnim_ReadPlacementS16(&obj->anim, &(placement->doneGameBit)), TRUE);
                        Obj_FreeObject(state->children[fragmentIndex]);
                        state->children[fragmentIndex] = NULL;
                        break;
                    case EXPLODED_PHASE_IDLE:
                        mainSetBits(ObjAnim_ReadPlacementS16(&obj->anim, &(placement->doneGameBit)), TRUE);
                        if ((state->completedFragmentMask & (1 << fragmentIndex)) == 0) {
                            state->completedFragmentMask |= 1 << fragmentIndex;
                        }
                        break;
                    }
                }
                fragmentIndex++;
            } while (fragmentIndex < EXPLODABLE_FRAGMENT_COUNT);
        }
    }
}

void explodable_init(GameObject* obj, ExplodablePlacement* placementAddress) {
    int recipeIndex;
    ExplodableBreakRecipe* recipes;
    u32 fragmentCount;
    ExplodableState* state;
    ExplodablePlacement* placement = placementAddress;

    objAddObjectType(obj, EXPLODABLE_OBJECT_GROUP);
    state = (ExplodableState*)obj->extra;
    fragmentCount = placement->fragmentCount;
    if (fragmentCount == 0) {
        fragmentCount = 1;
    }
    state->fragmentCount = fragmentCount;
    state->completedFragmentMask = 0;
    state->children[0] = 0;
    state->children[1] = 0;
    state->children[2] = 0;
    state->children[3] = 0;
    state->children[4] = 0;
    state->children[5] = 0;
    state->children[6] = 0;
    state->children[7] = 0;
    state->children[8] = 0;
    state->children[9] = 0;
    state->children[10] = 0;
    state->children[11] = 0;
    state->children[12] = 0;
    state->children[13] = 0;
    state->children[14] = 0;
    obj->anim.rotX = ObjAnim_ReadPlacementS16(&obj->anim, &(placement->rotX));
    obj->anim.rotY = ObjAnim_ReadPlacementS16(&obj->anim, &(placement->rotY));
    obj->anim.rotZ = ObjAnim_ReadPlacementS16(&obj->anim, &(placement->rotZ));
    if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &(placement->doneGameBit))) != 0) {
        state->phase = EXPLODABLE_PHASE_BROKEN;
    }
    for (recipeIndex = 0; recipeIndex < EXPLODABLE_RECIPE_COUNT; recipeIndex++) {
        if (obj->anim.romDefNo == gExplodableBreakRecipeTable[recipeIndex].sourceObjectId) {
            state->recipeIndex = recipeIndex;
            break;
        }
    }
    if (placement->scale == 0) {
        placement->scale = EXPLODABLE_DEFAULT_SCALE;
    }
    obj->anim.rootMotionScale = obj->anim.modelInstance->rootMotionScaleBase * (f32)(int)placement->scale / 20.0f;
    recipes = gExplodableBreakRecipeTable;
    if ((recipes[state->recipeIndex].flags & EXPLODABLE_RECIPE_FLAG_HIDE_OBJECT) != 0) {
        obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN;
    }
}

ExplodableBreakRecipe gExplodableBreakRecipeTable[EXPLODABLE_RECIPE_COUNT] = {
    {124, 876, 216, 1, 0, {0, 0}},    {2098, 2099, 705, 50, 0, {0, 0}}, {147, 1408, 0, 100, 0, {0, 0}},
    {176, 903, 0, 100, 0, {0, 0}},    {677, 933, 0, 20, 0, {0, 0}},     {1257, 1258, 216, 1, 0, {0, 0}},
    {1078, 1079, 705, 50, 0, {0, 0}}, {125, 126, 705, 50, 0, {0, 0}},   {127, 129, 0, 10, 0, {0, 0}},
    {1399, 1401, 216, 50, 0, {0, 0}}, {255, 254, 0, 10, 1, {0, 0}},     {1531, 1532, 705, 50, 0, {0, 0}},
    {1910, 1911, 705, 50, 0, {0, 0}}, {1938, 1937, 705, 50, 0, {0, 0}}, {1190, 1201, 705, 50, 0, {0, 0}},
    {2071, 2072, 705, 50, 0, {0, 0}},
};

OBJECT_INIT_ADAPTER(gExplodableObjDescriptorInitAdapter, explodable_init, obj, placement)
OBJECT_RENDER_ADAPTER(gExplodableObjDescriptorRenderAdapter, explodable_render)
OBJECT_EXTRA_SIZE_ADAPTER(gExplodableObjDescriptorExtraSizeAdapter, explodable_getExtraSize)

ObjectDescriptor gExplodableObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        0,
        0,
    },
    0,
    gExplodableObjDescriptorInitAdapter,
    explodable_update,
    0,
    gExplodableObjDescriptorRenderAdapter,
    explodable_free,
    0,
    gExplodableObjDescriptorExtraSizeAdapter,
};
