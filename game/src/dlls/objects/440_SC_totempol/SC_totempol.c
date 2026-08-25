#include "dlls/objects/440_SC_totempol.h"

#include "dlls/objects/438_SC_levelcon.h"
#include "game/objects/object_setup.h"
#include "main/audio/sfx.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/frame_timing.h"
#include "main/gamebit_ids.h"
#include "main/gamebits.h"
#include "main/model_engine.h"
#include "main/obj_list.h"
#include "main/object_render.h"
#include "main/objhits.h"
#include "sys/objects.h"

enum ScTotemPoleRecord {
    SC_TOTEM_POLE_RECORD_COUNT = 3,
};

enum ScTotemPoleMapId {
    SC_TOTEM_POLE_MAP_ID_REAR = 0x44916,
    SC_TOTEM_POLE_MAP_ID_RIGHT = 0x44909,
    SC_TOTEM_POLE_MAP_ID_FRONT = 0x4490C,
    SC_TOTEM_POLE_MAP_ID_LEFT = 0x4490F,
};

typedef struct ScTotemPolePlacement {
    ObjPlacement base;
    u8 unused18[2];
    u8 rotXByte;
    u8 unused1B[5];
} ScTotemPolePlacement;

typedef struct ScTotemPoleState {
    u16 litGameBit;
    u8 lit;
    u8 wasLit;
    f32 animationSpeed;
} ScTotemPoleState;

STATIC_ASSERT(sizeof(ScTotemPolePlacement) == 0x20);
STATIC_ASSERT(offsetof(ScTotemPolePlacement, base) == 0x00);
STATIC_ASSERT(offsetof(ScTotemPolePlacement, unused18) == 0x18);
STATIC_ASSERT(offsetof(ScTotemPolePlacement, rotXByte) == 0x1A);
STATIC_ASSERT(offsetof(ScTotemPolePlacement, unused1B) == 0x1B);

STATIC_ASSERT(sizeof(ScTotemPoleState) == 0x08);
STATIC_ASSERT(offsetof(ScTotemPoleState, litGameBit) == 0x00);
STATIC_ASSERT(offsetof(ScTotemPoleState, lit) == 0x02);
STATIC_ASSERT(offsetof(ScTotemPoleState, wasLit) == 0x03);
STATIC_ASSERT(offsetof(ScTotemPoleState, animationSpeed) == 0x04);

static const u16 sScTotemPoleRecordGameBits[SC_TOTEM_POLE_RECORD_COUNT] = {
    GAMEBIT_LV_TestTrackingBestTime1,
    GAMEBIT_LV_TestTrackingBestTime2,
    GAMEBIT_LV_TestTrackingBestTime3,
};

static f32 sScTotemPoleHitEffectCooldown;

static int sc_totempole_sortCompletionGameBits(const u16* recordGameBits, u16 completionTime) {
    u16 completionTimes[SC_TOTEM_POLE_RECORD_COUNT + 1];
    s32 changed = 0;

    for (int recordIndex = 0; recordIndex < SC_TOTEM_POLE_RECORD_COUNT; recordIndex++) {
        completionTimes[recordIndex] = mainGetBit(recordGameBits[recordIndex]);
    }
    completionTimes[SC_TOTEM_POLE_RECORD_COUNT] = completionTime;
    for (int pass = 0; pass < SC_TOTEM_POLE_RECORD_COUNT; pass++) {
        for (int recordIndex = 0; recordIndex < SC_TOTEM_POLE_RECORD_COUNT; recordIndex++) {
            if (completionTimes[recordIndex + 1] != 0) {
                if ((completionTimes[recordIndex + 1] < completionTimes[recordIndex]) ||
                    (completionTimes[recordIndex] == 0)) {
                    u16 previousTime = completionTimes[recordIndex];
                    completionTimes[recordIndex] = completionTimes[recordIndex + 1];
                    completionTimes[recordIndex + 1] = previousTime;
                    changed = 1;
                }
            }
        }
    }
    for (int recordIndex = 0; recordIndex < SC_TOTEM_POLE_RECORD_COUNT; recordIndex++) {
        mainSetBits(recordGameBits[recordIndex], completionTimes[recordIndex]);
    }
    return changed;
}

static int sc_totempole_getExtraSize(void) {
    return sizeof(ScTotemPoleState);
}

static int sc_totempole_getObjectTypeId(void) {
    return 0;
}

static void sc_totempole_free(void) {
}

static void sc_totempole_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                                s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

static void sc_totempole_hitDetect(void) {
}

static void sc_totempole_update(GameObject* obj) {
    ScTotemPoleState* state = obj->extra;
    ObjAnimEventList animEvents;
    int allPolesLit;
    GameObject** objects;
    int objectCount;
    int objectIndex;

    state->wasLit = state->lit;
    state->lit = mainGetBit(state->litGameBit);
    if (state->wasLit != state->lit) {
        if (state->lit != 0) {
            Sfx_PlayFromObject(obj, SFXTRIG_cflap2_c);
            state->animationSpeed = 0.01f;
            allPolesLit = 0;
            if (mainGetBit(GAMEBIT_SC_TotemPoleFrontLit) != 0 && mainGetBit(GAMEBIT_SC_TotemPoleLeftLit) != 0 &&
                mainGetBit(GAMEBIT_SC_TotemPoleRightLit) != 0 && mainGetBit(GAMEBIT_SC_TotemPoleRearLit) != 0) {
                Sfx_PlayFromObject(0, SFXTRIG_mpick1_b);
                allPolesLit = 1;
                objects = ObjList_GetObjects(&objectIndex, &objectCount);
                for (; objectIndex < objectCount; objectIndex++) {
                    if (objects[objectIndex] != obj &&
                        objects[objectIndex]->anim.romDefNo == SC_LEVEL_CONTROL_ROM_DEF_NO) {
                        sc_levelcontrol_setAnimEventState(objects[objectIndex],
                                                         SC_LEVEL_CONTROL_ANIM_STATE_FINISH_TIMED_CHALLENGE);
                        break;
                    }
                }
                sc_totempole_sortCompletionGameBits(sScTotemPoleRecordGameBits,
                                                     (s32)(gameTimerGetElapsedMilliseconds() / 10.0f));
            }
            if (!allPolesLit) {
                Sfx_PlayFromObject(0, SFXTRIG_menuups16k);
            }
        } else {
            Sfx_PlayFromObject(obj, SFXTRIG_cflap2_c);
            state->animationSpeed = -0.01f;
        }
    }
    ObjAnim_AdvanceCurrentMove(obj, state->animationSpeed, timeDelta, &animEvents);
    ObjHits_PollPriorityHitEffectWithCooldown(obj, 8, 0xFF, 0xFF, 0x78, SFXTRIG_swdtest222,
                                              &sScTotemPoleHitEffectCooldown);
}

static void sc_totempole_init(GameObject* obj, const ScTotemPolePlacement* placement) {
    ScTotemPoleState* state = obj->extra;

    switch (placement->base.ident) {
    case SC_TOTEM_POLE_MAP_ID_REAR:
        state->litGameBit = GAMEBIT_SC_TotemPoleRearLit;
        break;
    case SC_TOTEM_POLE_MAP_ID_RIGHT:
        state->litGameBit = GAMEBIT_SC_TotemPoleRightLit;
        break;
    case SC_TOTEM_POLE_MAP_ID_FRONT:
        state->litGameBit = GAMEBIT_SC_TotemPoleFrontLit;
        break;
    case SC_TOTEM_POLE_MAP_ID_LEFT:
        state->litGameBit = GAMEBIT_SC_TotemPoleLeftLit;
        break;
    }
    obj->anim.rotX = (s16)((u32)placement->rotXByte << 8);
}

static void sc_totempole_release(void) {
}

static void sc_totempole_initialise(void) {
}

OBJECT_INIT_ADAPTER(gSC_totempoleObjDescriptorInitAdapter, sc_totempole_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gSC_totempoleObjDescriptorHitDetectAdapter, sc_totempole_hitDetect)
OBJECT_FREE_ADAPTER(gSC_totempoleObjDescriptorFreeAdapter, sc_totempole_free)
OBJECT_TYPE_ID_ADAPTER(gSC_totempoleObjDescriptorTypeIdAdapter, sc_totempole_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gSC_totempoleObjDescriptorExtraSizeAdapter, sc_totempole_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gSC_totempoleObjDescriptorAcquire, sc_totempole_initialise)

ObjectDescriptor gSC_totempoleObjDescriptor = {
    .header = {
        .metadata = { 0, 0, 0, OBJECT_DESCRIPTOR_FLAGS_10_SLOTS },
        .acquire = gSC_totempoleObjDescriptorAcquire,
        .release = sc_totempole_release,
    },
    .init = gSC_totempoleObjDescriptorInitAdapter,
    .update = sc_totempole_update,
    .hitDetect = gSC_totempoleObjDescriptorHitDetectAdapter,
    .render = sc_totempole_render,
    .free = gSC_totempoleObjDescriptorFreeAdapter,
    .getObjectTypeId = gSC_totempoleObjDescriptorTypeIdAdapter,
    .getExtraSize = gSC_totempoleObjDescriptorExtraSizeAdapter,
};;
