#include "dlls/objects/412.h"

#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/audio/sfx.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/dll/dll_0082_modgfx.h"
#include "main/frame_timing.h"
#include "main/gamebit_ids.h"
#include "main/gamebits.h"
#include "main/object_render.h"
#include "main/resource.h"
#include "sys/objects.h"
#include "sys/objects/lifecycle.h"

enum Dll412ObjectId {
    DLL412_CHILD_OBJECT = 0x248,
};

typedef struct Dll412Placement {
    ObjPlacement base;
    u8 pad18[6];
    s8 rotationX;
    s8 disableChildSpawn;
} Dll412Placement;

typedef struct Dll412State {
    s32 unused0;
    s16 spawnTimer;
    s16 spawnTimerRate;
} Dll412State;

STATIC_ASSERT(sizeof(Dll412Placement) == 0x20);
STATIC_ASSERT(offsetof(Dll412Placement, rotationX) == 0x1E);
STATIC_ASSERT(offsetof(Dll412Placement, disableChildSpawn) == 0x1F);

STATIC_ASSERT(sizeof(Dll412State) == 0x08);
STATIC_ASSERT(offsetof(Dll412State, unused0) == 0x00);
STATIC_ASSERT(offsetof(Dll412State, spawnTimer) == 0x04);
STATIC_ASSERT(offsetof(Dll412State, spawnTimerRate) == 0x06);

static int dll412_getExtraSize(void) {
    return sizeof(Dll412State);
}

static int dll412_getObjectTypeId(void) {
    return 0;
}

static void dll412_free(void) {
}

static void dll412_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

static void dll412_hitDetect(void) {
}

static void dll412_update(GameObject* obj) {
    const Dll412Placement* placement = (const Dll412Placement*)obj->anim.placementData;
    Dll412State* state = obj->extra;

    if (obj->userData2 != 0 && mainGetBit(GAMEBIT_WM_KrazTest1ResetTorches) != 0) {
        obj->userData2 = 0;
    }
    if (obj->userData2 == 0 && mainGetBit(GAMEBIT_WM_KrazTest1TorchesActive) != 0) {
        Dll82Interface** effectResource = Resource_Acquire(DLL_82_RESOURCE_ID, 1);

        (*effectResource)->spawn(obj, 0, NULL, 1, -1, NULL);
        (*effectResource)->spawn(obj, 1, NULL, 1, -1, NULL);
        Sfx_PlayFromObject(NULL, SFXTRIG_hitpos_6);
        Resource_Release(effectResource);
        state->spawnTimerRate = 1;
        obj->userData2 = 1;
    }

    if (state->spawnTimerRate != 0) {
        state->spawnTimer = (s16)(state->spawnTimer - state->spawnTimerRate * framesThisStep);
    }
    if (state->spawnTimer > 0 || placement->disableChildSpawn != 0 || Obj_IsLoadingLocked() == 0) {
        return;
    }

    ObjPlacement* childSetup = Obj_AllocObjectSetup(sizeof(ObjPlacement), DLL412_CHILD_OBJECT);

    childSetup->posX = placement->base.posX;
    childSetup->posY = placement->base.posY + 50.0f;
    childSetup->posZ = placement->base.posZ;
    childSetup->objectId = DLL412_CHILD_OBJECT;
    childSetup->ident = -1;
    for (int channel = 0; channel < ARRAY_COUNT(childSetup->color); channel++) {
        childSetup->color[channel] = placement->base.color[channel];
    }
    objSetupObject(childSetup, 5, obj->anim.mapEventSlot, -1, obj->anim.parent);
    state->spawnTimer = 100;
    state->spawnTimerRate = 0;
}

static void dll412_init(GameObject* obj, const Dll412Placement* placement) {
    Dll412State* state = obj->extra;

    obj->anim.rotX = (s16)(placement->rotationX * 0x100);
    obj->userData2 = 0;
    state->spawnTimer = 100;
    state->spawnTimerRate = 0;
    state->unused0 = 0;
    obj->anim.renderAlpha = 0xFF;
    obj->anim.alpha = 0xFF;
}

static void dll412_release(void) {
}

static void dll412_initialise(void) {
}

OBJECT_INIT_ADAPTER(gDll19CObjDescriptorInitAdapter, dll412_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gDll19CObjDescriptorHitDetectAdapter, dll412_hitDetect)
OBJECT_FREE_ADAPTER(gDll19CObjDescriptorFreeAdapter, dll412_free)
OBJECT_TYPE_ID_ADAPTER(gDll19CObjDescriptorTypeIdAdapter, dll412_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gDll19CObjDescriptorExtraSizeAdapter, dll412_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gDll19CObjDescriptorAcquire, dll412_initialise)

ObjectDescriptor gDll19CObjDescriptor = {
    .header = {
        .metadata = { 0, 0, 0, OBJECT_DESCRIPTOR_FLAGS_10_SLOTS },
        .acquire = gDll19CObjDescriptorAcquire,
        .release = dll412_release,
    },
    .init = gDll19CObjDescriptorInitAdapter,
    .update = dll412_update,
    .hitDetect = gDll19CObjDescriptorHitDetectAdapter,
    .render = dll412_render,
    .free = gDll19CObjDescriptorFreeAdapter,
    .getObjectTypeId = gDll19CObjDescriptorTypeIdAdapter,
    .getExtraSize = gDll19CObjDescriptorExtraSizeAdapter,
};;
