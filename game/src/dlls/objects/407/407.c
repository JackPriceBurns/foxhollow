#include "dlls/objects/407.h"

#include "dolphin/math.h"
#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/audio/sfx.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/camera.h"
#include "main/dll/dll_0069_modgfx.h"
#include "main/dll/expgfx_interface.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/partfx_interface.h"
#include "main/frame_timing.h"
#include "main/gamebit_ids.h"
#include "main/gamebits.h"
#include "main/objhits.h"
#include "main/resource.h"
#include "main/shader.h"
#include "main/vecmath.h"
#include "main/voxmaps.h"
#include "sys/objects.h"

enum Dll407Mode {
    DLL407_MODE_PASSIVE_EFFECT,
    DLL407_MODE_PUZZLE_STAGE,
};

enum Dll407SequenceStage {
    DLL407_SEQUENCE_STAGE_NONE,
    DLL407_SEQUENCE_STAGE_FIRST,
    DLL407_SEQUENCE_STAGE_SECOND,
    DLL407_SEQUENCE_STAGE_COMPLETE,
};

enum Dll407ParticleId {
    DLL407_PARTICLE_ACTIVATION = 0x1A3,
    DLL407_PARTICLE_IDLE = 0x1F7,
};

enum Dll407EffectParamId {
    DLL407_EFFECT_PARAM_STAGE_0_A = 0x19D,
    DLL407_EFFECT_PARAM_STAGE_0_B = 0x19E,
};

typedef struct Dll407Placement {
    ObjPlacement base;
    s8 rotationIndex;
    u8 mode;
    s16 scalePacked;
    s16 sequenceIndex;
    s16 gameBitId;
} Dll407Placement;

typedef struct Dll407State {
    s32 gameBitId;
    s16 delayTimer;
    s16 resetTimer;
    s16 settleTimer;
    u8 lineOfSightVisible;
    u8 mode;
    u8 active;
    u8 needsOpenSfx;
    u8 previousActive;
    u8 sequenceIndex;
} Dll407State;

typedef struct Dll407EffectSpawnParams {
    u8 args[0x10];
    f32 positionY;
} Dll407EffectSpawnParams;

STATIC_ASSERT(sizeof(Dll407Placement) == 0x20);
STATIC_ASSERT(offsetof(Dll407Placement, rotationIndex) == 0x18);
STATIC_ASSERT(offsetof(Dll407Placement, mode) == 0x19);
STATIC_ASSERT(offsetof(Dll407Placement, scalePacked) == 0x1A);
STATIC_ASSERT(offsetof(Dll407Placement, sequenceIndex) == 0x1C);
STATIC_ASSERT(offsetof(Dll407Placement, gameBitId) == 0x1E);

STATIC_ASSERT(sizeof(Dll407State) == 0x10);
STATIC_ASSERT(offsetof(Dll407State, gameBitId) == 0x00);
STATIC_ASSERT(offsetof(Dll407State, delayTimer) == 0x04);
STATIC_ASSERT(offsetof(Dll407State, resetTimer) == 0x06);
STATIC_ASSERT(offsetof(Dll407State, settleTimer) == 0x08);
STATIC_ASSERT(offsetof(Dll407State, lineOfSightVisible) == 0x0A);
STATIC_ASSERT(offsetof(Dll407State, mode) == 0x0B);
STATIC_ASSERT(offsetof(Dll407State, active) == 0x0C);
STATIC_ASSERT(offsetof(Dll407State, needsOpenSfx) == 0x0D);
STATIC_ASSERT(offsetof(Dll407State, previousActive) == 0x0E);
STATIC_ASSERT(offsetof(Dll407State, sequenceIndex) == 0x0F);

STATIC_ASSERT(sizeof(Dll407EffectSpawnParams) == 0x14);
STATIC_ASSERT(offsetof(Dll407EffectSpawnParams, positionY) == 0x10);

static const Dll69EffectParams sDll407EffectParams = {
    .param0 = 0x3E7,
    .param1 = 0x8C,
    .param2 = 0x8D,
    .param3 = 0x28,
};

static s8 sDll407SequenceStage;

static int dll407_getExtraSize(void) {
    return sizeof(Dll407State);
}

static int dll407_getObjectTypeId(void) {
    return 1;
}

static void dll407_free(GameObject* obj) {
    (*gModgfxInterface)->detachSource(obj);
    (*gExpgfxInterface)->freeSource2((uintptr_t)obj);
}

static void dll407_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    Dll407State* state = obj->extra;

    if (visible == 0) {
        state->delayTimer = 0;
        state->lineOfSightVisible = 0;
        return;
    }
    if (state->active == 0) {
        return;
    }

    state->lineOfSightVisible = 1;
    Camera* camera = Camera_GetCurrent();
    Vec3f cameraDelta = {
        .x = camera->x - obj->anim.localPosX,
        .y = camera->y - obj->anim.localPosY,
        .z = camera->z - obj->anim.localPosZ,
    };
    f32 distance =
        sqrtf(cameraDelta.z * cameraDelta.z + (cameraDelta.x * cameraDelta.x + cameraDelta.y * cameraDelta.y));

    if (distance > 50.0f) {
        f32 inverseDistance = 1.0f / distance;

        cameraDelta.x *= inverseDistance;
        cameraDelta.y *= inverseDistance;
        cameraDelta.z *= inverseDistance;

        Vec3f objectTraceStart = {
            .x = 32.0f * cameraDelta.x + obj->anim.localPosX,
            .y = 32.0f * cameraDelta.y + obj->anim.localPosY,
            .z = 32.0f * cameraDelta.z + obj->anim.localPosZ,
        };
        Vec3f cameraTraceEnd = {
            .x = -20.0f * cameraDelta.x + camera->x,
            .y = -20.0f * cameraDelta.y + camera->y,
            .z = -20.0f * cameraDelta.z + camera->z,
        };
        VoxPos startGrid;
        VoxPos endGrid;
        VoxPos traceOut;

        voxmaps_worldToGrid(&objectTraceStart.x, &startGrid.x);
        voxmaps_worldToGrid(&cameraTraceEnd.x, &endGrid.x);
        if (voxmaps_traceLine(&startGrid, &endGrid, &traceOut, NULL, 0) == 0) {
            state->lineOfSightVisible = 0;
            (*gExpgfxInterface)->freeSource((uintptr_t)obj);
        }
    }

    if (state->delayTimer > 0) {
        state->delayTimer -= framesThisStep;
        return;
    }

    if (state->lineOfSightVisible != 0) {
        PartFxSpawnParams particleParams;

        particleParams.pos = (Vec3f){0.0f, 5.0f, 0.0f};
        (*gPartfxInterface)->spawnObject(obj, DLL407_PARTICLE_IDLE, &particleParams, 0x12, -1, NULL);
    }
    state->delayTimer = (s16)(randomGetRange(-10, 10) + 60);
}

static void dll407_hitDetect(void) {
}

static void dll407_update(GameObject* obj) {
    Dll407State* state = obj->extra;
    Dll69EffectParams effectParams = sDll407EffectParams;
    Dll407EffectSpawnParams effectSpawn;
    GameObject* player = Obj_GetPlayerObject();
    f32 distance = Vec_distance(&player->anim.worldPosX, &obj->anim.worldPosX);

    if (Sfx_IsPlayingFromObjectChannel(obj, 0x40) != 0) {
        if (distance >= 90.0f && state->active != 0) {
            Sfx_StopObjectChannel(obj, 0x40);
        }
    } else if (distance < 90.0f && state->active != 0) {
        Sfx_PlayFromObject(obj, SFXTRIG_mushdizzylp12);
    }

    objUpdateOpacity(obj);
    if (state->settleTimer > 0) {
        state->settleTimer -= framesThisStep;
    }
    if (state->mode != DLL407_MODE_PUZZLE_STAGE) {
        return;
    }

    effectSpawn.positionY = -2.0f;
    state->previousActive = state->active;
    if (ObjHits_GetPriorityHit(obj, 0, 0, 0) != 0 || (state->settleTimer != 0 && state->settleTimer <= 20)) {
        state->active = 1 - state->active;
        if (state->active != 0) {
            state->resetTimer = 1000;
        }
        if (state->settleTimer != 0) {
            state->settleTimer = 0;
            sDll407SequenceStage = DLL407_SEQUENCE_STAGE_COMPLETE;
            state->resetTimer = 300;
            if (state->sequenceIndex == 2) {
                mainSetBits(GAMEBIT_DBSH_ActNo, 1);
            }
        }
    }

    if (state->active != 0 && state->resetTimer != 0) {
        state->resetTimer -= framesThisStep;
        if (state->resetTimer <= 0) {
            state->resetTimer = 0;
            state->active = 0;
        }
    }

    if (state->active != 0 && state->delayTimer <= 0 && state->needsOpenSfx != 0) {
        state->needsOpenSfx = 0;
        Sfx_PlayFromObject(obj, SFXTRIG_cvdrip1c);
    }
    if (state->active == state->previousActive) {
        return;
    }

    if (state->active != 0) {
        Dll69Interface** effectResource = Resource_Acquire(DLL_69_RESOURCE_ID, 1);
        int effectParamOffset = state->sequenceIndex * 2;

        effectParams.param1 = effectParamOffset + DLL407_EFFECT_PARAM_STAGE_0_A;
        effectParams.param2 = effectParamOffset + DLL407_EFFECT_PARAM_STAGE_0_B;
        (*effectResource)->spawn(obj, 1, &effectSpawn, PARTFXFLAG_10000 | PARTFXFLAG_4, -1, &effectParams);
        Resource_Release(effectResource);

        for (int particleIndex = 0; particleIndex < 200; particleIndex++) {
            (*gPartfxInterface)->spawnObject(obj, DLL407_PARTICLE_ACTIVATION, NULL, 0, -1, NULL);
        }

        if (state->gameBitId != -1 && mainGetBit(state->gameBitId) == 0) {
            mainSetBits(state->gameBitId, 1);
        }
        if (sDll407SequenceStage == DLL407_SEQUENCE_STAGE_NONE && state->sequenceIndex == 0 &&
            mainGetBit(state->gameBitId) != 0) {
            sDll407SequenceStage = DLL407_SEQUENCE_STAGE_FIRST;
        }
        if (sDll407SequenceStage == DLL407_SEQUENCE_STAGE_FIRST && state->sequenceIndex == 1 &&
            mainGetBit(state->gameBitId) != 0) {
            sDll407SequenceStage = DLL407_SEQUENCE_STAGE_SECOND;
        }
        if (sDll407SequenceStage == DLL407_SEQUENCE_STAGE_SECOND && state->sequenceIndex == 2 &&
            mainGetBit(state->gameBitId) != 0) {
            mainSetBits(GAMEBIT_DBSH_ActNo, 1);
            sDll407SequenceStage = DLL407_SEQUENCE_STAGE_COMPLETE;
        }
        state->needsOpenSfx = 1;
        state->delayTimer = 1;
    } else {
        Sfx_StopObjectChannel(obj, 0x7F);
        (*gModgfxInterface)->detachSource(obj);
        (*gExpgfxInterface)->freeSource((uintptr_t)obj);
        if (state->gameBitId != -1 && mainGetBit(state->gameBitId) != 0) {
            mainSetBits(state->gameBitId, 0);
        }
        if (sDll407SequenceStage == DLL407_SEQUENCE_STAGE_FIRST && state->sequenceIndex == 0) {
            sDll407SequenceStage = DLL407_SEQUENCE_STAGE_NONE;
        }
        if (sDll407SequenceStage == DLL407_SEQUENCE_STAGE_SECOND && state->sequenceIndex == 1) {
            sDll407SequenceStage = DLL407_SEQUENCE_STAGE_NONE;
        }
        if (sDll407SequenceStage == DLL407_SEQUENCE_STAGE_COMPLETE && state->sequenceIndex == 2 &&
            mainGetBit(GAMEBIT_DBSH_Related0474) == 0) {
            mainSetBits(GAMEBIT_DBSH_ActNo, 0);
            sDll407SequenceStage = DLL407_SEQUENCE_STAGE_NONE;
        }
    }
}

static void dll407_init(GameObject* obj, const Dll407Placement* placement) {
    Dll407State* state = obj->extra;
    Dll407EffectSpawnParams effectSpawn;
    s16 scalePacked = ObjAnim_ReadPlacementS16(&obj->anim, &placement->scalePacked);

    obj->anim.rotX = (s16)((placement->rotationIndex & 0x3F) << 10);
    if (scalePacked > 0) {
        obj->anim.rootMotionScale = scalePacked / 8192.0f;
    } else {
        obj->anim.rootMotionScale = 0.1f;
    }

    state->mode = placement->mode;
    state->active = 0;
    state->sequenceIndex = 0;
    state->gameBitId = ObjAnim_ReadPlacementS16(&obj->anim, &placement->gameBitId);
    effectSpawn.positionY = -2.0f;

    switch (state->mode) {
    case DLL407_MODE_PASSIVE_EFFECT: {
        Dll69Interface** effectResource;

        state->active = 1;
        effectResource = Resource_Acquire(DLL_69_RESOURCE_ID, 1);
        if (ObjAnim_ReadPlacementS16(&obj->anim, &placement->sequenceIndex) == 0) {
            (*effectResource)->spawn(obj, 0, &effectSpawn, PARTFXFLAG_10000 | PARTFXFLAG_4, -1, NULL);
        }
        break;
    }
    case DLL407_MODE_PUZZLE_STAGE:
        state->sequenceIndex = (u8)ObjAnim_ReadPlacementS16(&obj->anim, &placement->sequenceIndex);
        state->needsOpenSfx = 0;
        state->settleTimer = (s16)(state->sequenceIndex * 0x28 + 0x398);
        state->previousActive = 0;
        break;
    }
    state->delayTimer = 0;
}

static void dll407_release(void) {
}

static void dll407_initialise(void) {
}

OBJECT_INIT_ADAPTER(gDll197ObjDescriptorInitAdapter, dll407_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gDll197ObjDescriptorHitDetectAdapter, dll407_hitDetect)
OBJECT_FREE_ADAPTER(gDll197ObjDescriptorFreeAdapter, dll407_free, obj)
OBJECT_TYPE_ID_ADAPTER(gDll197ObjDescriptorTypeIdAdapter, dll407_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gDll197ObjDescriptorExtraSizeAdapter, dll407_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gDll197ObjDescriptorAcquire, dll407_initialise)

ObjectDescriptor gDll197ObjDescriptor = {
    .header = {
        .metadata = { 0, 0, 0, OBJECT_DESCRIPTOR_FLAGS_10_SLOTS },
        .acquire = gDll197ObjDescriptorAcquire,
        .release = dll407_release,
    },
    .init = gDll197ObjDescriptorInitAdapter,
    .update = dll407_update,
    .hitDetect = gDll197ObjDescriptorHitDetectAdapter,
    .render = dll407_render,
    .free = gDll197ObjDescriptorFreeAdapter,
    .getObjectTypeId = gDll197ObjDescriptorTypeIdAdapter,
    .getExtraSize = gDll197ObjDescriptorExtraSizeAdapter,
};;
