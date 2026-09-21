#include "dlls/objects/413.h"

#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/audio/sfx.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/dll/partfx_interface.h"
#include "main/dll_000A_expgfx.h"
#include "main/frame_timing.h"
#include "main/objhits.h"
#include "main/render_lactions.h"
#include "sys/objects/lifecycle.h"

enum Dll413Variant {
    DLL413_VARIANT_DEFAULT,
    DLL413_VARIANT_ONE,
    DLL413_VARIANT_TWO,
};

enum Dll413ParticleId {
    DLL413_PARTICLE_TRAIL = 0x29D,
    DLL413_PARTICLE_PULSE_A = 0x29E,
    DLL413_PARTICLE_PULSE_B = 0x29F,
    DLL413_PARTICLE_IMPACT = 0x2A0,
    DLL413_PARTICLE_PULSE_C = 0x2A1,
};

enum Dll413ParticleMode {
    DLL413_PARTICLE_MODE_IMPACT = 1,
    DLL413_PARTICLE_MODE_FLIGHT = 4,
};

enum Dll413ActionId {
    DLL413_ACTION_STOP = 1,
    DLL413_ACTION_DEFAULT = 0x201,
    DLL413_ACTION_VARIANT_ONE = 0x203,
    DLL413_ACTION_VARIANT_TWO = 0x204,
};

enum Dll413ObjectId {
    DLL413_OBJECT_IGNORED_ON_HIT = 0x248,
};

enum Dll413HitType {
    DLL413_HIT_TYPE_DEFAULT = 1,
    DLL413_HIT_TYPE_VARIANT = 3,
};

enum Dll413StateFlags {
    DLL413_STATE_POSITION_INITIALIZED = 1 << 0,
    DLL413_STATE_STOP_ACTION_INITIALIZED = 1 << 1,
};

typedef struct Dll413Placement {
    ObjPlacement base;
    u8 unused18;
    s8 variant;
} Dll413Placement;

typedef struct Dll413State {
    u8 unused00[0x08];
    Vec3f position;
    u8 unused14[0x18];
    s16 angularVelocityZ;
    s16 angularVelocityX;
    s16 effectTimer;
    s16 despawnTimer;
    u16 spinPhase;
    u8 flags;
    u8 unused37;
} Dll413State;

STATIC_ASSERT(sizeof(Dll413Placement) == 0x1C);
STATIC_ASSERT(offsetof(Dll413Placement, variant) == 0x19);

STATIC_ASSERT(sizeof(Dll413State) == 0x38);
STATIC_ASSERT(offsetof(Dll413State, position) == 0x08);
STATIC_ASSERT(offsetof(Dll413State, angularVelocityZ) == 0x2C);
STATIC_ASSERT(offsetof(Dll413State, angularVelocityX) == 0x2E);
STATIC_ASSERT(offsetof(Dll413State, effectTimer) == 0x30);
STATIC_ASSERT(offsetof(Dll413State, despawnTimer) == 0x32);
STATIC_ASSERT(offsetof(Dll413State, spinPhase) == 0x34);
STATIC_ASSERT(offsetof(Dll413State, flags) == 0x36);

static void dll413_startStopAction(GameObject* obj, Dll413State* state) {
    if ((state->flags & DLL413_STATE_STOP_ACTION_INITIALIZED) == 0) {
        getLActions(obj, obj, DLL413_ACTION_STOP, 0, 0, 0);
        state->flags |= DLL413_STATE_STOP_ACTION_INITIALIZED;
    }
}

static void dll413_spawnImpactParticles(GameObject* obj, PartFxSpawnParams* params) {
    for (int i = 0; i < 3; i++) {
        (*gPartfxInterface)->spawnObject(obj, DLL413_PARTICLE_IMPACT, params, DLL413_PARTICLE_MODE_IMPACT, -1, NULL);
    }
}

static int dll413_getExtraSize(void) {
    return sizeof(Dll413State);
}

static int dll413_getObjectTypeId(void) {
    return 0;
}

static void dll413_free(GameObject* obj) {
    dll413_startStopAction(obj, obj->extra);
    (*gExpgfxInterface)->freeSource2((uintptr_t)obj);
}

static void dll413_render(void) {
}

static void dll413_hitDetect(GameObject* obj) {
    Dll413State* state = obj->extra;
    const Dll413Placement* placement = (const Dll413Placement*)obj->anim.placementData;
    ObjHitsPriorityState* hitState = (ObjHitsPriorityState*)obj->anim.hitReactState;
    GameObject* lastHitObject = (GameObject*)hitState->lastHitObject;
    PartFxSpawnParams spawnParams;

    spawnParams.scale = (f32)placement->variant;
    spawnParams.pos = (Vec3f){0.0f, 0.0f, 0.0f};

    if (lastHitObject == NULL || lastHitObject->anim.romDefNo == DLL413_OBJECT_IGNORED_ON_HIT) {
        return;
    }

    dll413_spawnImpactParticles(obj, &spawnParams);
    state->despawnTimer = 50;
}

static void dll413_update(GameObject* obj) {
    Dll413State* state = obj->extra;
    const Dll413Placement* placement = (const Dll413Placement*)obj->anim.placementData;
    ObjHitsPriorityState* hitState = (ObjHitsPriorityState*)obj->anim.hitReactState;
    PartFxSpawnParams spawnParams;

    spawnParams.scale = (f32)placement->variant;
    spawnParams.pos = (Vec3f){0.0f, 0.0f, 0.0f};

    if ((state->flags & DLL413_STATE_POSITION_INITIALIZED) == 0) {
        state->position.x = obj->anim.localPosX;
        state->position.y = obj->anim.localPosY;
        state->position.z = obj->anim.localPosZ;
        state->flags |= DLL413_STATE_POSITION_INITIALIZED;
    }

    if (hitState->contactFlags != 0) {
        Sfx_PlayFromObject(obj, SFXTRIG_npu_216);
        dll413_spawnImpactParticles(obj, &spawnParams);
        state->despawnTimer = 50;
    }

    if (state->despawnTimer != 0) {
        dll413_startStopAction(obj, state);
        obj->anim.velocityX = 0.0f;
        obj->anim.velocityY = 0.0f;
        obj->anim.velocityZ = 0.0f;
        ObjHits_ClearHitVolumes(&obj->anim);
        state->despawnTimer--;
        if (state->despawnTimer <= 0) {
            Obj_FreeObject(obj);
        }
        return;
    }

    obj->anim.previousLocalPosX = obj->anim.localPosX;
    obj->anim.previousLocalPosY = obj->anim.localPosY;
    obj->anim.previousLocalPosZ = obj->anim.localPosZ;

    obj->anim.rotX = (s16)(obj->anim.rotX + state->angularVelocityX * framesThisStep);
    obj->anim.rotZ = (s16)(obj->anim.rotZ + state->angularVelocityZ * framesThisStep);
    (*gPartfxInterface)->spawnObject(obj, DLL413_PARTICLE_TRAIL, &spawnParams, DLL413_PARTICLE_MODE_FLIGHT, -1, NULL);

    state->effectTimer -= framesThisStep;
    if (state->effectTimer <= 0) {
        (*gPartfxInterface)
            ->spawnObject(obj, DLL413_PARTICLE_PULSE_A, &spawnParams, DLL413_PARTICLE_MODE_FLIGHT, -1, NULL);
        (*gPartfxInterface)
            ->spawnObject(obj, DLL413_PARTICLE_PULSE_B, &spawnParams, DLL413_PARTICLE_MODE_FLIGHT, -1, NULL);
        (*gPartfxInterface)
            ->spawnObject(obj, DLL413_PARTICLE_PULSE_C, &spawnParams, DLL413_PARTICLE_MODE_FLIGHT, -1, NULL);
        state->effectTimer = 50;
    }

    state->position.x += obj->anim.velocityX * timeDelta;
    state->position.y += obj->anim.velocityY * timeDelta;
    state->position.z += obj->anim.velocityZ * timeDelta;
    state->spinPhase += framesThisStep * 0x5DC;
    obj->anim.localPosX = state->position.x;
    obj->anim.localPosY = state->position.y;
    obj->anim.localPosZ = state->position.z;

    obj->userData1 -= (intptr_t)framesThisStep;
    if (obj->userData1 < 0) {
        Obj_FreeObject(obj);
    }
}

static void dll413_init(GameObject* obj) {
    const Dll413Placement* placement = (const Dll413Placement*)obj->anim.placementData;
    int hitType = placement->variant != DLL413_VARIANT_DEFAULT ? DLL413_HIT_TYPE_VARIANT : DLL413_HIT_TYPE_DEFAULT;

    ObjHits_SetHitVolumeSlot(&obj->anim, 0xE, hitType, 0);

    switch (placement->variant) {
    case DLL413_VARIANT_ONE:
        getLActions(obj, obj, DLL413_ACTION_VARIANT_ONE, 0, 0, 0);
        break;
    case DLL413_VARIANT_TWO:
        getLActions(obj, obj, DLL413_ACTION_VARIANT_TWO, 0, 0, 0);
        break;
    default:
        getLActions(obj, obj, DLL413_ACTION_DEFAULT, 0, 0, 0);
        break;
    }
}

static void dll413_release(void) {
}

static void dll413_initialise(void) {
}

OBJECT_INIT_ADAPTER(gDll19DObjDescriptorInitAdapter, dll413_init, obj)
OBJECT_RENDER_ADAPTER(gDll19DObjDescriptorRenderAdapter, dll413_render)
OBJECT_FREE_ADAPTER(gDll19DObjDescriptorFreeAdapter, dll413_free, obj)
OBJECT_TYPE_ID_ADAPTER(gDll19DObjDescriptorTypeIdAdapter, dll413_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gDll19DObjDescriptorExtraSizeAdapter, dll413_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gDll19DObjDescriptorAcquire, dll413_initialise)

ObjectDescriptor gDll19DObjDescriptor = {
    .header = {
        .metadata = { 0, 0, 0, OBJECT_DESCRIPTOR_FLAGS_10_SLOTS },
        .acquire = gDll19DObjDescriptorAcquire,
        .release = dll413_release,
    },
    .slot02 = NULL,
    .init = gDll19DObjDescriptorInitAdapter,
    .update = dll413_update,
    .hitDetect = dll413_hitDetect,
    .render = gDll19DObjDescriptorRenderAdapter,
    .free = gDll19DObjDescriptorFreeAdapter,
    .getObjectTypeId = gDll19DObjDescriptorTypeIdAdapter,
    .getExtraSize = gDll19DObjDescriptorExtraSizeAdapter,
};;
