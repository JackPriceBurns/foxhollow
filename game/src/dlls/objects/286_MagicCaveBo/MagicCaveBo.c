/*
 * Bottom-area Magic Cave warp object (DLL slot 286 / 0x11E).
 *
 * The object starts the cave ambience and entry sequence, waits for player
 * activation, then raises the exit handoff and warps to the destination
 * stored by the top-area object.
 */
#include "dlls/objects/286_MagicCaveBo.h"
#include "game/objects/object.h"
#include "main/audio/music.h"
#include "main/audio/music_trigger_ids.h"
#include "main/dll/tricky.h"
#include "main/gamebits.h"
#include "main/obj_trigger.h"
#include "main/objprint_render.h"
#include "main/objseq.h"
#include "main/rcp_dolphin.h"
#include "main/render_envfx.h"
#include "main/sky.h"

#define MAGIC_CAVE_BOTTOM_ENVFX_FIRST   0x2C
#define MAGIC_CAVE_BOTTOM_ENVFX_SECOND  0x2D
#define MAGIC_CAVE_BOTTOM_A_BUTTON_ICON 0x19

int MagicCaveBottom_getExtraSize(void) {
    return sizeof(MagicCaveBottomState);
}

void MagicCaveBottom_free(GameObject* obj) {
    mainSetBits(GAMEBIT_MC_IsActive, 0);
    Music_Trigger(MUSICTRIG_PU3_Adventure, 0);
}

void MagicCaveBottom_update(GameObject* obj) {
    MagicCaveBottomPlacement* placement = (MagicCaveBottomPlacement*)obj->anim.placementData;
    MagicCaveBottomState* state = obj->extra;

    obj->anim.rotX = placement->rotationX << 8;

    switch (state->phase) {
    case MAGIC_CAVE_BOTTOM_PHASE_SETUP:
        mainSetBits(GAMEBIT_MC_IsActive, 1);
        skySetEnvFxFlags(0);
        getEnvfxAct(obj, obj, MAGIC_CAVE_BOTTOM_ENVFX_FIRST, 0);
        getEnvfxAct(obj, obj, MAGIC_CAVE_BOTTOM_ENVFX_SECOND, 0);
        state->phase = MAGIC_CAVE_BOTTOM_PHASE_START_MUSIC;

        if (placement->sequenceBank != 0) {
            (*gObjectTriggerInterface)->runSequence(0, obj, -1);
        } else {
            (*gObjectTriggerInterface)->runSequence(2, obj, -1);
        }
        break;
    case MAGIC_CAVE_BOTTOM_PHASE_START_MUSIC:
        Music_Trigger(MUSICTRIG_PU3_Adventure, 1);
        state->phase = MAGIC_CAVE_BOTTOM_PHASE_IDLE;
        break;
    case MAGIC_CAVE_BOTTOM_PHASE_IDLE:
        if ((obj->anim.resetHitboxFlags & INTERACT_FLAG_IN_RANGE) != 0) {
            setAButtonIcon(MAGIC_CAVE_BOTTOM_A_BUTTON_ICON);
        }

        if (ObjTrigger_IsSet(obj) != 0) {
            state->phase = MAGIC_CAVE_BOTTOM_PHASE_WARP;
            if (placement->sequenceBank != 0) {
                (*gObjectTriggerInterface)->runSequence(1, obj, -1);
            } else {
                (*gObjectTriggerInterface)->runSequence(3, obj, -1);
            }
        } else {
            objUpdateHitVolumeTransforms(obj);
        }
        break;
    case MAGIC_CAVE_BOTTOM_PHASE_WARP:
        mainSetBits(GAMEBIT_MC_IsExiting, 1);
        warpToMap(mainGetBit(GAMEBIT_MagicCaveExitWarp), 0);
        break;
    }
}

OBJECT_FREE_ADAPTER(gMagicCaveBottomObjDescriptorFreeAdapter, MagicCaveBottom_free, obj)
OBJECT_EXTRA_SIZE_ADAPTER(gMagicCaveBottomObjDescriptorExtraSizeAdapter, MagicCaveBottom_getExtraSize)

ObjectDescriptor gMagicCaveBottomObjDescriptor = {
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
    0,
    MagicCaveBottom_update,
    0,
    0,
    gMagicCaveBottomObjDescriptorFreeAdapter,
    0,
    gMagicCaveBottomObjDescriptorExtraSizeAdapter,
};
