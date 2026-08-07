#include "dlls/objects/282.h"

#include "dolphin/mtx/vec.h"
#include "main/model.h"
#include "main/object_render.h"
#include "main/vecmath.h"
#include "main/object_transform.h"
#include "main/objtype.h"

#define DECORATION11A_COLLISION_OBJECT_GROUP 2
#define DECORATION11A_ROTATION_SHIFT         8
#define DECORATION11A_SCALE_DIVISOR          255.0f

enum {
    DECORATION11A_MODEL_A = 0x7A1,
    DECORATION11A_MODEL_B = 0x7A2,
    DECORATION11A_MODEL_C = 0x7A3,
};

int decoration11a_getExtraSize(void) {
    return DECORATION11A_STATE_SIZE;
}

void decoration11a_free(void) {
}

void decoration11a_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

void decoration11a_hitDetect(GameObject* obj) {
    s16 modelId;
    Decoration11AState* state;
    int objectCount;
    GameObject** objectCursor;
    f32 candidateRadius;
    Vec3f localPosition;
    f32 distanceSquared;
    f32 axisDelta;
    f32 axisDistanceSquared;

    modelId = obj->anim.romDefNo;
    if (modelId != DECORATION11A_MODEL_A && modelId != DECORATION11A_MODEL_B && modelId != DECORATION11A_MODEL_C) {
        return;
    }

    state = obj->extra;
    objectCursor = (GameObject**)objGetAllOfType(DECORATION11A_COLLISION_OBJECT_GROUP, &objectCount);
    while (objectCount != 0) {
        if (Vec_distance(&(*objectCursor)->anim.worldPosX, &obj->anim.worldPosX) < state->boundingRadius) {
            if ((*objectCursor)->anim.hitReactState != NULL) {
                candidateRadius = (f32)((ObjHitsPriorityState*)(*objectCursor)->anim.hitReactState)->primaryRadius;
                objWorldToLocalPos(&localPosition.x, (MatrixTransform*)obj, &(*objectCursor)->anim.localPosX);

                distanceSquared = 0.0f;

                {
                    f32 bMax;
                    f32 bMin;
                    f32 px;
                    bMin = state->boundsMin.x;
                    bMax = state->boundsMax.x;
                    distanceSquared += ((px = localPosition.x) < bMin) ? (px - bMin) * (px - bMin)
                                       : (px > bMax)                   ? (px - bMax) * (px - bMax)
                                                                       : 0.0f;
                }

                {
                    f32 bMax;
                    f32 bMin;
                    bMin = state->boundsMin.y;
                    bMax = state->boundsMax.y;
                    if (localPosition.y < bMin) {
                        axisDelta = localPosition.y - bMin;
                        axisDistanceSquared = axisDelta * axisDelta;
                    } else if (localPosition.y > bMax) {
                        axisDelta = localPosition.y - bMax;
                        axisDistanceSquared = axisDelta * axisDelta;
                    } else {
                        axisDistanceSquared = 0.0f;
                    }
                    distanceSquared += axisDistanceSquared;
                }

                {
                    f32 bMax;
                    f32 bMin;
                    bMin = state->boundsMin.z;
                    bMax = state->boundsMax.z;
                    if (localPosition.z < bMin) {
                        axisDelta = localPosition.z - bMin;
                        axisDistanceSquared = axisDelta * axisDelta;
                    } else if (localPosition.z > bMax) {
                        axisDelta = localPosition.z - bMax;
                        axisDistanceSquared = axisDelta * axisDelta;
                    } else {
                        axisDistanceSquared = 0.0f;
                    }
                    distanceSquared += axisDistanceSquared;
                }

                if (distanceSquared < candidateRadius * candidateRadius) {
                    ((ObjHitsPriorityState*)(*objectCursor)->anim.hitReactState)->lastHitObject = (u32)obj;
                    ((ObjHitsPriorityState*)(*objectCursor)->anim.hitReactState)->contactFlags =
                        OBJHITS_CONTACT_FLAG_KIND0;
                }
            }
        }
        objectCount--;
        objectCursor++;
    }
}

void decoration11a_update(void) {
}

void decoration11a_expandBoundsWithVertex(const Vec3f* vertex, Vec3f* boundsMax, Vec3f* boundsMin) {
    f32 component;

    component = vertex->x;
    if (component > boundsMax->x) {
        boundsMax->x = component;
    } else if (component < boundsMin->x) {
        boundsMin->x = component;
    }
    component = vertex->y;
    if (component > boundsMax->y) {
        boundsMax->y = component;
    } else if (component < boundsMin->y) {
        boundsMin->y = component;
    }
    component = vertex->z;
    if (component > boundsMax->z) {
        boundsMax->z = component;
    } else if (component < boundsMin->z) {
        boundsMin->z = component;
    }
}

void decoration11a_init(GameObject* obj, Decoration11APlacement* placement) {
    obj->anim.rotZ = (s16)((s32)placement->rotationZ << DECORATION11A_ROTATION_SHIFT);
    obj->anim.rotY = (s16)((s32)placement->rotationY << DECORATION11A_ROTATION_SHIFT);
    obj->anim.rotX = (s16)((s32)placement->rotationX << DECORATION11A_ROTATION_SHIFT);
    if (placement->scale != 0) {
        obj->anim.rootMotionScale = (f32)(u32)placement->scale / DECORATION11A_SCALE_DIVISOR;
        if (!obj->anim.rootMotionScale) {
            obj->anim.rootMotionScale = 1.0f;
        }
        obj->anim.rootMotionScale *= obj->anim.modelInstance->rootMotionScaleBase;
    }
    {
        s16 modelId = obj->anim.romDefNo;
        if (modelId != DECORATION11A_MODEL_A && modelId != DECORATION11A_MODEL_B && modelId != DECORATION11A_MODEL_C) {
            return;
        }
        {
            int vertexIndex;
            ModelFileHeader* model;
            Decoration11AState* state;
            Vec3f vertexPosition;
            f32 minMagnitude;
            f32 boundingRadius;

            state = obj->extra;
            model = obj->anim.modelBanks[0]->file;
            Model_GetVertexPosition(model, 0, &state->boundsMax.x);
            Model_GetVertexPosition(model, 0, &state->boundsMin.x);
            for (vertexIndex = 1; vertexIndex < model->vertexCount; vertexIndex++) {
                Model_GetVertexPosition(model, vertexIndex, &vertexPosition.x);
                decoration11a_expandBoundsWithVertex(&vertexPosition, &state->boundsMax, &state->boundsMin);
            }
            PSVECScale(&state->boundsMax, &state->boundsMax, obj->anim.rootMotionScale);
            PSVECScale(&state->boundsMin, &state->boundsMin, obj->anim.rootMotionScale);
            minMagnitude = PSVECMag(&state->boundsMin);
            if (PSVECMag(&state->boundsMax) > minMagnitude) {
                boundingRadius = PSVECMag(&state->boundsMax);
            } else {
                boundingRadius = PSVECMag(&state->boundsMin);
            }
            state->boundingRadius = boundingRadius;
        }
    }
}

ObjectDescriptor gDecoration11AObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    0,
    0,
    0,
    (ObjectDescriptorCallback)decoration11a_init,
    (ObjectDescriptorCallback)decoration11a_update,
    (ObjectDescriptorCallback)decoration11a_hitDetect,
    (ObjectDescriptorCallback)decoration11a_render,
    (ObjectDescriptorCallback)decoration11a_free,
    0,
    decoration11a_getExtraSize,
};
