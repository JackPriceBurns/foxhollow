#include "dlls/objects/433_SH_staff.h"

#include "dolphin/mtx.h"

#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/dll/player_objects.h"
#include "main/frame_timing.h"
#include "main/game_ui_interface.h"
#include "main/gamebit_ids.h"
#include "main/gamebits_api.h"
#include "main/objseq.h"
#include "main/objtype.h"
#include "main/obj_path.h"
#include "main/objhits.h"
#include "main/object_render.h"
#include "main/objprint_render_api.h"
#include "main/obj_trigger.h"
#include "main/vecmath.h"
#include "sys/objects.h"
#include "sys/objects/lifecycle.h"
#include "main/audio/sfx_play_api.h"
#include "main/dll/player_staff_api.h"
#include "main/dll/tricky_api.h"
#include "main/map_load.h"

enum ShStaffPhase {
    SH_STAFF_PHASE_IDLE,
    SH_STAFF_PHASE_ARMED,
    SH_STAFF_PHASE_PICKUP,
    SH_STAFF_PHASE_ATTACH_WORLD,
    SH_STAFF_PHASE_ATTACH_LOCAL,
    SH_STAFF_PHASE_RENDER_ATTACHED,
    SH_STAFF_PHASE_DONE,
};

enum ShStaffHazeFlag {
    SH_STAFF_HAZE_SPAWN_EVEN = 1 << 0,
    SH_STAFF_HAZE_EVEN_COMPLETE = 1 << 1,
    SH_STAFF_HAZE_SPAWN_ODD = 1 << 2,
    SH_STAFF_HAZE_ODD_COMPLETE = 1 << 3,
    SH_STAFF_HAZE_FADE_OUT = 1 << 4,
    SH_STAFF_HAZE_CONVERGE = 1 << 5,
};

enum ShStaffSequenceEvent {
    SH_STAFF_EVENT_ATTACH_WORLD = 2,
    SH_STAFF_EVENT_SHOW_HELP_TEXT,
    SH_STAFF_EVENT_HIDE_HELP_TEXT,
    SH_STAFF_EVENT_DEACTIVATE,
    SH_STAFF_EVENT_ATTACH_LOCAL,
    SH_STAFF_EVENT_SHOW_STAFF_HUD,
    SH_STAFF_EVENT_SPAWN_EVEN_HAZE,
    SH_STAFF_EVENT_SPAWN_ODD_HAZE,
    SH_STAFF_EVENT_FADE_HAZE_OUT,
    SH_STAFF_EVENT_CONVERGE_HAZE,
    SH_STAFF_EVENT_FINISH_HAZE_EFFECT,
};

enum ShStaffObjectId {
    SH_STAFF_HAZE_OBJECT_ID = 0x659,
};

enum ShStaffObjectGroup {
    SH_STAFF_SEQUENCE_TARGET_GROUP = 0xF,
};

enum ShStaffHazeCount {
    SH_STAFF_HAZE_COUNT = 10,
};

typedef struct ShStaffPlacement {
    ObjPlacement base;
    u8 initialRotationZ;
    u8 initialRotationY;
} ShStaffPlacement;

typedef struct ShStaffHazePlacement {
    ObjPlacement base;
    u8 unused18[8];
} ShStaffHazePlacement;

typedef struct ShStaffState {
    u8 phase;
    u8 helpTextVisible;
    u8 hazeFlags;
    u8 pickupMapLoaded;
    f32 hazeFadeTimer;
    Mtx carryMatrix;
    GameObject* hazeChildren[SH_STAFF_HAZE_COUNT];
    u8 hazeSpawnPending[SH_STAFF_HAZE_COUNT];
    u8 unused92[2];
    f32 hazeClimbProgress;
    f32 fizzSfxTimer;
} ShStaffState;

STATIC_ASSERT(offsetof(ShStaffPlacement, initialRotationZ) == sizeof(ObjPlacement));
STATIC_ASSERT(offsetof(ShStaffPlacement, initialRotationY) == sizeof(ObjPlacement) + 1);
STATIC_ASSERT(sizeof(ShStaffHazePlacement) == 0x20);
STATIC_ASSERT(offsetof(ShStaffState, carryMatrix) == 8);
STATIC_ASSERT(offsetof(ShStaffState, hazeChildren) ==
              offsetof(ShStaffState, carryMatrix) + sizeof(((ShStaffState*)0)->carryMatrix));
STATIC_ASSERT(offsetof(ShStaffState, hazeSpawnPending) ==
              offsetof(ShStaffState, hazeChildren) + sizeof(((ShStaffState*)0)->hazeChildren));
STATIC_ASSERT(offsetof(ShStaffState, hazeClimbProgress) ==
              offsetof(ShStaffState, hazeSpawnPending) + sizeof(((ShStaffState*)0)->hazeSpawnPending) + 2);
STATIC_ASSERT(sizeof(ShStaffState) == 0xA0);

static void sh_staff_deactivate(GameObject* obj, ShStaffState* state, int clearChildren);

static GameObject* shStaff_spawnHaze(GameObject* obj) {
    if (Obj_IsLoadingLocked() == 0) {
        return NULL;
    }

    ShStaffHazePlacement* placement =
        (ShStaffHazePlacement*)Obj_AllocObjectSetup(sizeof(ShStaffHazePlacement), SH_STAFF_HAZE_OBJECT_ID);
    placement->base.color[0] = 2;
    placement->base.color[3] = 0xFF;
    return loadObjectAtObject(obj, &placement->base);
}

static int sh_staff_getExtraSize(void) {
    return sizeof(ShStaffState);
}

static void sh_staff_free(GameObject* obj, int freeArg) {
    ShStaffState* state = obj->extra;

    if (freeArg != 0) {
        return;
    }

    for (int i = 0; i < SH_STAFF_HAZE_COUNT; i++) {
        if (state->hazeChildren[i] != NULL) {
            state->hazeChildren[i]->anim.flags |= OBJANIM_FLAG_HIDDEN;
        }
    }
}

static void sh_staff_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                            s8 visible) {
    if (visible == 0) {
        return;
    }

    ShStaffState* state = obj->extra;
    GameObject* player = Obj_GetPlayerObject();
    Mtx worldMatrix;
    Mtx inverseHandMatrix;

    if (state->phase == SH_STAFF_PHASE_ATTACH_WORLD) {
        Obj_BuildWorldTransformMatrix(obj, &worldMatrix[0][0], 0);
        MtxPtr handMatrix = (MtxPtr)ObjPath_GetPointModelMtx(player, 0);
        PSMTXInverse(handMatrix, inverseHandMatrix);
        PSMTXConcat(inverseHandMatrix, worldMatrix, state->carryMatrix);
        state->phase = SH_STAFF_PHASE_RENDER_ATTACHED;
    }

    if (state->phase == SH_STAFF_PHASE_ATTACH_LOCAL) {
        ObjPath_GetPointLocalMtx(player, 0, &state->carryMatrix[0][0]);
        state->phase = SH_STAFF_PHASE_RENDER_ATTACHED;
    }

    if (state->phase == SH_STAFF_PHASE_RENDER_ATTACHED) {
        MtxPtr handMatrix = (MtxPtr)ObjPath_GetPointModelMtx(player, 0);
        PSMTXConcat(handMatrix, state->carryMatrix, worldMatrix);
        objSetCurrentMatrix(worldMatrix);
        objRenderModel(obj);
    } else {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }

    Vec3f pathStart;
    Vec3f pathEnd;
    ObjPath_GetPointWorldPosition(obj, 0, &pathStart.x, &pathStart.y, &pathStart.z, 0);
    ObjPath_GetPointWorldPosition(obj, 1, &pathEnd.x, &pathEnd.y, &pathEnd.z, 0);
    Vec3f pathDelta = {
        pathEnd.x - pathStart.x,
        pathEnd.y - pathStart.y,
        pathEnd.z - pathStart.z,
    };

    int i;
    if ((state->hazeFlags & SH_STAFF_HAZE_SPAWN_EVEN) != 0 && (state->hazeFlags & SH_STAFF_HAZE_EVEN_COMPLETE) == 0) {
        for (i = 2; i < SH_STAFF_HAZE_COUNT; i += 2) {
            if (state->hazeChildren[i] == NULL) {
                state->hazeSpawnPending[i] = 1;
                break;
            }
        }
        if (i >= SH_STAFF_HAZE_COUNT) {
            state->hazeFlags |= SH_STAFF_HAZE_EVEN_COMPLETE;
        }
    }

    if ((state->hazeFlags & SH_STAFF_HAZE_SPAWN_ODD) != 0 && (state->hazeFlags & SH_STAFF_HAZE_ODD_COMPLETE) == 0) {
        for (i = 1; i < SH_STAFF_HAZE_COUNT; i += 2) {
            if (state->hazeChildren[i] == NULL) {
                state->hazeSpawnPending[i] = 1;
                break;
            }
        }
        if (i >= SH_STAFF_HAZE_COUNT) {
            state->hazeFlags |= SH_STAFF_HAZE_ODD_COMPLETE;
        }
    }
    if (state->hazeFlags == 0) {
        f32 scale = 0.5f;
        if (state->hazeFadeTimer != 0.0f) {
            state->hazeFadeTimer -= timeDelta;
            if (state->hazeFadeTimer <= 0.0f) {
                GameObject* hazeChild = state->hazeChildren[0];
                if (hazeChild != NULL) {
                    hazeChild->anim.flags |= OBJANIM_FLAG_HIDDEN;
                    state->hazeChildren[0] = NULL;
                    state->hazeFadeTimer = 0.0f;
                }
            } else {
                scale = (1.0f / 120.0f) * state->hazeFadeTimer;
            }
        }

        if (state->hazeChildren[0] != NULL) {
            state->hazeChildren[0]->anim.localPosX = pathDelta.x * state->hazeClimbProgress + pathStart.x;
            state->hazeChildren[0]->anim.localPosY = pathDelta.y * state->hazeClimbProgress + pathStart.y;
            state->hazeChildren[0]->anim.localPosZ = pathDelta.z * state->hazeClimbProgress + pathStart.z;
            state->hazeChildren[0]->anim.rootMotionScale = scale;
        }

        return;
    }

    if ((state->hazeFlags & SH_STAFF_HAZE_CONVERGE) == 0) {
        f32 scatterScale = 0.01f;
        if ((state->hazeFlags & SH_STAFF_HAZE_FADE_OUT) != 0) {
            state->hazeFadeTimer -= timeDelta;
            if (state->hazeFadeTimer <= 0.0f) {
                state->hazeFlags &= ~SH_STAFF_HAZE_FADE_OUT;
            } else {
                scatterScale = 1.0f / 120.0f * state->hazeFadeTimer;
            }
        }

        for (int j = 0; j < SH_STAFF_HAZE_COUNT; j++) {
            if (state->hazeChildren[j] != NULL) {
                f32 t = 1.0f / 9.0f * j;
                t += (f32)randomGetRange(-0x32, 0x32) / 1000.0f;
                state->hazeChildren[j]->anim.localPosX = pathDelta.x * t + pathStart.x;
                state->hazeChildren[j]->anim.localPosY = pathDelta.y * t + pathStart.y;
                state->hazeChildren[j]->anim.localPosZ = pathDelta.z * t + pathStart.z;
                state->hazeChildren[j]->anim.rootMotionScale = scatterScale;
            }
        }

        return;
    }

    f32 foldScale;
    if ((state->hazeFlags & SH_STAFF_HAZE_FADE_OUT) != 0) {
        state->hazeFadeTimer -= timeDelta;
        if (state->hazeFadeTimer <= 0.0f) {
            foldScale = 0.01f;
        } else {
            state->hazeFadeTimer -= timeDelta;
            foldScale = (1.0f / 3000.0f) * state->hazeFadeTimer;
        }
    } else {
        state->hazeFadeTimer += timeDelta;
        if (state->hazeFadeTimer >= 60.0f) {
            state->hazeFadeTimer = 60.0f;
        }
        foldScale = (1.0f / 120.0f) * state->hazeFadeTimer;
    }

    GameObject* startAnchor = state->hazeChildren[4];
    for (int j = 0; j < 5; j++) {
        if (state->hazeChildren[j] != NULL && startAnchor != NULL) {
            f32 t = 0.2f + j / 5.0f;
            state->hazeChildren[j]->anim.localPosX =
                t * (pathStart.x - startAnchor->anim.localPosX) + startAnchor->anim.localPosX;
            state->hazeChildren[j]->anim.localPosY =
                t * (pathStart.y - startAnchor->anim.localPosY) + startAnchor->anim.localPosY;
            state->hazeChildren[j]->anim.localPosZ =
                t * (pathStart.z - startAnchor->anim.localPosZ) + startAnchor->anim.localPosZ;
            state->hazeChildren[j]->anim.rootMotionScale = foldScale;
        }
    }

    GameObject* endAnchor = state->hazeChildren[5];
    for (int j = 9; j > 4; j--) {
        if (state->hazeChildren[j] != NULL && endAnchor != NULL) {
            f32 t = 0.2f + (9.0f - j) / 5.0f;
            state->hazeChildren[j]->anim.localPosX =
                t * (pathEnd.x - endAnchor->anim.localPosX) + endAnchor->anim.localPosX;
            state->hazeChildren[j]->anim.localPosY =
                t * (pathEnd.y - endAnchor->anim.localPosY) + endAnchor->anim.localPosY;
            state->hazeChildren[j]->anim.localPosZ =
                t * (pathEnd.z - endAnchor->anim.localPosZ) + endAnchor->anim.localPosZ;
            state->hazeChildren[j]->anim.rootMotionScale = foldScale;
        }
    }
}

static int sh_staff_sequenceCallback(GameObject* obj, int unused, ObjSeqState* animUpdate) {
    ShStaffState* state = obj->extra;

    (void)unused;

    for (int i = 0; i < SH_STAFF_HAZE_COUNT; i++) {
        if (state->hazeSpawnPending[i] != 0) {
            state->hazeChildren[i] = shStaff_spawnHaze(obj);
            state->hazeSpawnPending[i] = 0;
        }
    }

    for (int i = 0; i < animUpdate->eventCount; i++) {
        switch (animUpdate->eventIds[i]) {
        case SH_STAFF_EVENT_ATTACH_WORLD:
            state->phase = SH_STAFF_PHASE_ATTACH_WORLD;
            break;
        case SH_STAFF_EVENT_SHOW_HELP_TEXT:
            state->helpTextVisible = 1;
            break;
        case SH_STAFF_EVENT_HIDE_HELP_TEXT:
            state->helpTextVisible = 0;
            break;
        case SH_STAFF_EVENT_DEACTIVATE:
            sh_staff_deactivate(obj, state, 1);
            break;
        case SH_STAFF_EVENT_ATTACH_LOCAL:
            state->phase = SH_STAFF_PHASE_ATTACH_LOCAL;
            break;
        case SH_STAFF_EVENT_SHOW_STAFF_HUD:
            setHudForceShowMask(1);
            break;
        case SH_STAFF_EVENT_SPAWN_EVEN_HAZE:
            state->hazeFlags |= SH_STAFF_HAZE_SPAWN_EVEN;
            break;
        case SH_STAFF_EVENT_SPAWN_ODD_HAZE:
            state->hazeFlags |= SH_STAFF_HAZE_SPAWN_ODD;
            break;
        case SH_STAFF_EVENT_FADE_HAZE_OUT:
            state->hazeFlags |= SH_STAFF_HAZE_FADE_OUT;
            state->hazeFadeTimer = 60.0f;
            break;
        case SH_STAFF_EVENT_CONVERGE_HAZE:
            state->hazeFlags |= SH_STAFF_HAZE_CONVERGE;
            state->hazeFadeTimer = 0.0f;
            break;
        case SH_STAFF_EVENT_FINISH_HAZE_EFFECT:
            state->hazeFlags |= SH_STAFF_HAZE_FADE_OUT;
            state->hazeFlags |= SH_STAFF_HAZE_EVEN_COMPLETE | SH_STAFF_HAZE_ODD_COMPLETE;
            state->hazeFadeTimer = 1500.0f;
            break;
        case 0:
        case 1:
            break;
        }
    }

    if (state->helpTextVisible != 0) {
        (*gGameUIInterface)->showMinimapInfoText(obj->anim.modelInstance->helpTextIds[1], 0xa0, 0x8c);
    }
    state->hazeClimbProgress += 0.01f * timeDelta;
    if (state->hazeClimbProgress > 1.0f) {
        state->hazeClimbProgress = 0.0f;
    }
    return 0;
}

static void sh_staff_deactivate(GameObject* obj, ShStaffState* state, int clearChildren) {
    ObjHits_DisableObject(obj);
    obj->anim.flags |= OBJANIM_FLAG_HIDDEN;
    obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;

    if (clearChildren == 0) {
        state->phase = SH_STAFF_PHASE_DONE;
        return;
    }

    GameObject* player = Obj_GetPlayerObject();
    staffToggle(player, 1);
    playerPutAwayStaff(player, 1);
    for (int i = 0; i < SH_STAFF_HAZE_COUNT; i++) {
        if (state->hazeChildren[i] != NULL) {
            state->hazeChildren[i]->anim.flags |= OBJANIM_FLAG_HIDDEN;
            state->hazeChildren[i] = NULL;
        }
    }

    state->phase = SH_STAFF_PHASE_DONE;
}

static void sh_staff_update(GameObject* obj) {
    ShStaffState* state = obj->extra;
    const ShStaffPlacement* placement = (const ShStaffPlacement*)obj->anim.placementData;
    GameObject* player = Obj_GetPlayerObject();
    f32 distanceSq = getXZDistanceSquared(&obj->anim.worldPosX, &player->anim.worldPosX);
    u8 currentPhase = state->phase;

    if (currentPhase == SH_STAFF_PHASE_IDLE) {
        if (player != NULL && Player_GetStaffObject(player) != NULL) {
            if (mainGetBit(GAMEBIT_STAFF_ACQUIRED) != 0) {
                sh_staff_deactivate(obj, state, 0);
            } else {
                staffToggle(player, 0);
                ObjAnim_SetMoveProgress(&obj->anim, 1.0f);
                obj->anim.rotY = (s16)((u16)placement->initialRotationY * 0x100);
                obj->anim.rotZ = (s16)((u16)placement->initialRotationZ * 0x100);
                obj->animEventCallback = sh_staff_sequenceCallback;
                state->phase = SH_STAFF_PHASE_ARMED;
                state->hazeChildren[0] = shStaff_spawnHaze(obj);
                state->fizzSfxTimer = 0.9f;
            }
        }
    } else if (currentPhase == SH_STAFF_PHASE_ARMED) {
        if (ObjTrigger_IsSet(obj) != 0) {
            GameObject* target = objGetNearestTypeTo(SH_STAFF_SEQUENCE_TARGET_GROUP, obj, 0);
            (*gObjectTriggerInterface)->runSequence(0, target, -1);
            state->phase = SH_STAFF_PHASE_PICKUP;
            state->hazeFadeTimer = 60.0f;
            mainSetBits(GAMEBIT_STAFF_ACQUIRED, 1);
        } else if (distanceSq > 490000.0f) {
            if (state->pickupMapLoaded != 0) {
                state->pickupMapLoaded = 0;
                mapUnload(0x13, 0x20000000);
            }
        } else if (distanceSq < 250000.0f) {
            if (state->pickupMapLoaded == 0) {
                state->pickupMapLoaded = 1;
                loadMapAndParent(8);
            }
        }
    } else if (state->pickupMapLoaded != 0) {
        state->pickupMapLoaded = 0;
        mapUnload(0x13, 0x20000000);
        mainSetBits(GAMEBIT_STAFF_PICKUP_MAP_UNLOADED, 1);
    }
    setHudForceShowMask(0);
    state->hazeClimbProgress += 0.01f * timeDelta;
    if (state->hazeClimbProgress > 1.0f) {
        state->hazeClimbProgress = 0.0f;
    }
    state->fizzSfxTimer += 0.01f * timeDelta;
    if (state->fizzSfxTimer > 1.0f) {
        state->fizzSfxTimer = 0.0f;
        if (state->phase == SH_STAFF_PHASE_ARMED) {
            Sfx_PlayFromObject(obj, SFXTRIG_pk_staff_fizz);
        }
    }
}

ObjectDescriptor gSH_staffObjDescriptor = {
    .slotCountAndFlags = OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    .update = (ObjectDescriptorCallback)sh_staff_update,
    .render = (ObjectDescriptorCallback)sh_staff_render,
    .free = (ObjectDescriptorCallback)sh_staff_free,
    .getExtraSize = sh_staff_getExtraSize,
};
