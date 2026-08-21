#include "dlls/objects/414.h"

#include "dolphin/MSL_C/PPCEABI/bare/H/math_api.h"
#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/audio/sfx_ids.h"
#include "main/audio/sfx_play_api.h"
#include "main/audio/sfx_stop_channel_api.h"
#include "main/camera.h"
#include "main/dll/dll_0069_modgfx.h"
#include "main/dll/expgfx_interface.h"
#include "main/dll/modgfx_interface.h"
#include "main/dll/partfx_interface.h"
#include "main/frame_timing.h"
#include "main/gamebit_ids.h"
#include "main/gamebits_api.h"
#include "main/objhits.h"
#include "main/resource.h"
#include "main/shader_api.h"
#include "main/vecmath.h"
#include "main/voxmaps.h"

enum Dll414Mode {
    DLL414_MODE_PASSIVE_EFFECT,
    DLL414_MODE_PUZZLE_STAGE,
};

enum Dll414SequenceStage {
    DLL414_SEQUENCE_STAGE_NONE,
    DLL414_SEQUENCE_STAGE_FIRST,
    DLL414_SEQUENCE_STAGE_SECOND,
    DLL414_SEQUENCE_STAGE_COMPLETE,
};

enum Dll414ParticleId {
    DLL414_PARTICLE_ACTIVATION = 0x1A3,
    DLL414_PARTICLE_IDLE = 0x1F7,
};

enum Dll414EffectParamId {
    DLL414_EFFECT_PARAM_STAGE_0_A = 0x19D,
    DLL414_EFFECT_PARAM_STAGE_0_B = 0x19E,
};

typedef struct Dll414Placement {
    ObjPlacement base;
    s8 rotationIndex;
    u8 mode;
    s16 scalePacked;
    s16 sequenceIndex;
    s16 gameBitId;
} Dll414Placement;

typedef struct Dll414State {
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
} Dll414State;

typedef struct Dll414EffectSpawnParams {
    u8 args[0x10];
    f32 positionY;
} Dll414EffectSpawnParams;

STATIC_ASSERT(sizeof(Dll414Placement) == 0x20);
STATIC_ASSERT(offsetof(Dll414Placement, rotationIndex) == 0x18);
STATIC_ASSERT(offsetof(Dll414Placement, mode) == 0x19);
STATIC_ASSERT(offsetof(Dll414Placement, scalePacked) == 0x1A);
STATIC_ASSERT(offsetof(Dll414Placement, sequenceIndex) == 0x1C);
STATIC_ASSERT(offsetof(Dll414Placement, gameBitId) == 0x1E);

STATIC_ASSERT(sizeof(Dll414State) == 0x10);
STATIC_ASSERT(offsetof(Dll414State, gameBitId) == 0x00);
STATIC_ASSERT(offsetof(Dll414State, delayTimer) == 0x04);
STATIC_ASSERT(offsetof(Dll414State, resetTimer) == 0x06);
STATIC_ASSERT(offsetof(Dll414State, settleTimer) == 0x08);
STATIC_ASSERT(offsetof(Dll414State, lineOfSightVisible) == 0x0A);
STATIC_ASSERT(offsetof(Dll414State, mode) == 0x0B);
STATIC_ASSERT(offsetof(Dll414State, active) == 0x0C);
STATIC_ASSERT(offsetof(Dll414State, needsOpenSfx) == 0x0D);
STATIC_ASSERT(offsetof(Dll414State, previousActive) == 0x0E);
STATIC_ASSERT(offsetof(Dll414State, sequenceIndex) == 0x0F);

STATIC_ASSERT(sizeof(Dll414EffectSpawnParams) == 0x14);
STATIC_ASSERT(offsetof(Dll414EffectSpawnParams, positionY) == 0x10);

static const Dll69EffectParams sDll414EffectParams = {
    .param0 = 0x3E7,
    .param1 = 0x8C,
    .param2 = 0x8D,
    .param3 = 0x28,
};

static s8 sDll414SequenceStage;

static int dll414_getExtraSize(void) {
    return sizeof(Dll414State);
}

static int dll414_getObjectTypeId(void) {
    return 1;
}

static void dll414_free(GameObject* obj) {
    (*gModgfxInterface)->detachSource(obj);
    (*gExpgfxInterface)->freeSource2((uintptr_t)obj);
}

static void dll414_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    Dll414State* state = obj->extra;

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
        .x = camera->position.x - obj->anim.localPos.x,
        .y = camera->position.y - obj->anim.localPos.y,
        .z = camera->position.z - obj->anim.localPos.z,
    };
    f32 distance =
        sqrtf(cameraDelta.z * cameraDelta.z + (cameraDelta.x * cameraDelta.x + cameraDelta.y * cameraDelta.y));

    if (distance > 50.0f) {
        f32 inverseDistance = 1.0f / distance;

        cameraDelta.x *= inverseDistance;
        cameraDelta.y *= inverseDistance;
        cameraDelta.z *= inverseDistance;

        Vec3f objectTraceStart = {
            .x = 32.0f * cameraDelta.x + obj->anim.localPos.x,
            .y = 32.0f * cameraDelta.y + obj->anim.localPos.y,
            .z = 32.0f * cameraDelta.z + obj->anim.localPos.z,
        };
        Vec3f cameraTraceEnd = {
            .x = -20.0f * cameraDelta.x + camera->position.x,
            .y = -20.0f * cameraDelta.y + camera->position.y,
            .z = -20.0f * cameraDelta.z + camera->position.z,
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
        (*gPartfxInterface)->spawnObject(obj, DLL414_PARTICLE_IDLE, &particleParams, 0x12, -1, NULL);
    }
    state->delayTimer = (s16)(randomGetRange(-10, 10) + 60);
}

static void dll414_hitDetect(void) {
}

static void dll414_update(GameObject* obj) {
    Dll414State* state = obj->extra;
    Dll69EffectParams effectParams = sDll414EffectParams;
    Dll414EffectSpawnParams effectSpawn;

    Sfx_PlayFromObject(obj, SFXmn_eggylaugh216);
    objUpdateOpacity(obj);
    if (state->settleTimer > 0) {
        state->settleTimer -= framesThisStep;
    }
    if (state->mode != DLL414_MODE_PUZZLE_STAGE) {
        return;
    }

    effectSpawn.positionY = -2.0f;
    state->previousActive = state->active;
    if (ObjHits_GetPriorityHit(obj, NULL, NULL, NULL) != 0 || (state->settleTimer != 0 && state->settleTimer <= 20)) {
        state->active = 1 - state->active;
        if (state->active != 0) {
            state->resetTimer = 1000;
        }
        if (state->settleTimer != 0) {
            state->settleTimer = 0;
            sDll414SequenceStage = DLL414_SEQUENCE_STAGE_COMPLETE;
            state->resetTimer = 300;
            if (state->sequenceIndex == 2) {
                mainSetBits(GAMEBIT_WM_KrazTest1Passed, 1);
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
        Sfx_PlayFromObject(obj, SFXmn_sml_trex_snap1);
    }
    if (state->active == state->previousActive) {
        return;
    }

    if (state->active != 0) {
        Dll69Interface** effectResource = Resource_Acquire(DLL_69_RESOURCE_ID, 1);
        int effectParamOffset = state->sequenceIndex * 2;

        effectParams.param1 = effectParamOffset + DLL414_EFFECT_PARAM_STAGE_0_A;
        effectParams.param2 = effectParamOffset + DLL414_EFFECT_PARAM_STAGE_0_B;
        (*effectResource)->spawn(obj, 1, &effectSpawn, PARTFXFLAG_10000 | PARTFXFLAG_4, -1, &effectParams);
        Resource_Release(effectResource);

        for (int particleIndex = 0; particleIndex < 100; particleIndex++) {
            (*gPartfxInterface)->spawnObject(obj, DLL414_PARTICLE_ACTIVATION, NULL, 0, -1, NULL);
        }

        if (state->gameBitId != -1 && mainGetBit(state->gameBitId) == 0) {
            mainSetBits(state->gameBitId, 1);
        }
        if (sDll414SequenceStage == DLL414_SEQUENCE_STAGE_NONE && state->sequenceIndex == 0 &&
            mainGetBit(state->gameBitId) != 0) {
            sDll414SequenceStage = DLL414_SEQUENCE_STAGE_FIRST;
        }
        if (sDll414SequenceStage == DLL414_SEQUENCE_STAGE_FIRST && state->sequenceIndex == 1 &&
            mainGetBit(state->gameBitId) != 0) {
            sDll414SequenceStage = DLL414_SEQUENCE_STAGE_SECOND;
        }
        if (sDll414SequenceStage == DLL414_SEQUENCE_STAGE_SECOND && state->sequenceIndex == 2 &&
            mainGetBit(state->gameBitId) != 0) {
            mainSetBits(GAMEBIT_WM_KrazTest1Passed, 1);
            sDll414SequenceStage = DLL414_SEQUENCE_STAGE_COMPLETE;
        }
        state->needsOpenSfx = 1;
        state->delayTimer = 1;
    } else {
        Sfx_StopObjectChannel(obj, 0x40);
        (*gModgfxInterface)->detachSource(obj);
        (*gExpgfxInterface)->freeSource((uintptr_t)obj);
        if (state->gameBitId != -1 && mainGetBit(state->gameBitId) != 0) {
            mainSetBits(state->gameBitId, 0);
        }
        if (sDll414SequenceStage == DLL414_SEQUENCE_STAGE_FIRST && state->sequenceIndex == 0) {
            sDll414SequenceStage = DLL414_SEQUENCE_STAGE_NONE;
        }
        if (sDll414SequenceStage == DLL414_SEQUENCE_STAGE_SECOND && state->sequenceIndex == 1) {
            sDll414SequenceStage = DLL414_SEQUENCE_STAGE_NONE;
        }
        if (sDll414SequenceStage == DLL414_SEQUENCE_STAGE_COMPLETE && state->sequenceIndex == 2 &&
            mainGetBit(GAMEBIT_WM_KrazTest1Related01D5) == 0) {
            mainSetBits(GAMEBIT_WM_KrazTest1Passed, 0);
            sDll414SequenceStage = DLL414_SEQUENCE_STAGE_NONE;
        }
    }
}

static void dll414_init(GameObject* obj, const Dll414Placement* placement) {
    Dll414State* state = obj->extra;
    Dll414EffectSpawnParams effectSpawn;
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
    case DLL414_MODE_PASSIVE_EFFECT: {
        Dll69Interface** effectResource;

        state->active = 1;
        effectResource = Resource_Acquire(DLL_69_RESOURCE_ID, 1);
        if (ObjAnim_ReadPlacementS16(&obj->anim, &placement->sequenceIndex) == 0) {
            (*effectResource)->spawn(obj, 0, &effectSpawn, PARTFXFLAG_10000 | PARTFXFLAG_4, -1, NULL);
        }
        break;
    }
    case DLL414_MODE_PUZZLE_STAGE:
        state->sequenceIndex = (u8)ObjAnim_ReadPlacementS16(&obj->anim, &placement->sequenceIndex);
        state->needsOpenSfx = 0;
        state->settleTimer = (s16)(state->sequenceIndex * 0x28 + 0x398);
        state->previousActive = 0;
        break;
    }
    state->delayTimer = 0;
}

static void dll414_release(void) {
}

static void dll414_initialise(void) {
}

ObjectDescriptor gDll19EObjDescriptor = {
    .slotCountAndFlags = OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    .initialise = (ObjectDescriptorCallback)dll414_initialise,
    .release = (ObjectDescriptorCallback)dll414_release,
    .init = (ObjectDescriptorCallback)dll414_init,
    .update = (ObjectDescriptorCallback)dll414_update,
    .hitDetect = (ObjectDescriptorCallback)dll414_hitDetect,
    .render = (ObjectDescriptorCallback)dll414_render,
    .free = (ObjectDescriptorCallback)dll414_free,
    .getObjectTypeId = (ObjectDescriptorCallback)dll414_getObjectTypeId,
    .getExtraSize = dll414_getExtraSize,
};
