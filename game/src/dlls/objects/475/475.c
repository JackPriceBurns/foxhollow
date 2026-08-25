/*
 * DLL 0x1DB drives a vertically moving platform between fixed upper and lower
 * stops.
 */

#include "dlls/objects/475.h"

#include "main/audio/sfx_trigger_ids.h"
#include "main/frame_timing.h"
#include "main/gamebits.h"
#include "main/object_render.h"
#include "sys/objects.h"
#include "main/audio/sfx.h"

enum {
    DLL1DB_MOTION_STATE_TOP = 1,
    DLL1DB_MOTION_STATE_BOTTOM = 2,
    DLL1DB_MOTION_STATE_RISING = 3,
    DLL1DB_MOTION_STATE_FALLING = 4
};

int dll_1DB_getExtraSize(void) {
    return sizeof(Dll1DBState);
}

int dll_1DB_getObjectTypeId(void) {
    return 0;
}

void dll_1DB_free(void) {
}

void dll_1DB_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible) {
    s32 isVisible = visible;

    if (isVisible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

void dll_1DB_hitDetect(void) {
}

void dll_1DB_update(GameObject* obj) {
    Dll1DBState* state;
    const Dll1DBPlacementView* placement;
    int playerContacted;
    GameObject* player;
    int contactOffset;
    int remainingContacts;
    int hitboxStateAddress;

    state = obj->extra;
    player = Obj_GetPlayerObject();
    placement = (const Dll1DBPlacementView*)obj->anim.placementData;
    playerContacted = 0;
    contactOffset = 0;
    hitboxStateAddress = (int)obj->anim.hitboxTransformState;
    for (remainingContacts = ((ObjHitboxTransformState*)hitboxStateAddress)->contactObjectCount; remainingContacts > 0;
         remainingContacts--) {
        GameObject* contactObject =
            *(GameObject**)(hitboxStateAddress + contactOffset + offsetof(ObjHitboxTransformState, contactObjects));

        if (contactObject == player) {
            playerContacted = 1;
            break;
        }

        contactOffset += sizeof(((ObjHitboxTransformState*)hitboxStateAddress)->contactObjects[0]);
    }

    switch (state->motionState) {
    case DLL1DB_MOTION_STATE_TOP:
        Sfx_StopObjectChannel(obj, 8);
        if (playerContacted == 0) {
            state->contactLost = 1;
        } else if (state->contactLost != 0 && state->boarded != 0) {
            Sfx_PlayFromObject(obj, SFXTRIG_mv_wickpickup16);
            state->motionState = DLL1DB_MOTION_STATE_FALLING;
            state->verticalVelocity = 0.0f;
        }

        if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &(placement->triggerGameBit))) != 0) {
            Sfx_PlayFromObject(obj, SFXTRIG_mv_wickpickup16);
            state->motionState = DLL1DB_MOTION_STATE_FALLING;
            state->verticalVelocity = 0.0f;
        }
        break;
    case DLL1DB_MOTION_STATE_BOTTOM:
        Sfx_StopObjectChannel(obj, 8);
        if (state->boarded != 0) {
            if (playerContacted == 0) {
                Sfx_PlayFromObject(obj, SFXTRIG_mv_wickpickup16);
                state->motionState = DLL1DB_MOTION_STATE_RISING;
                state->verticalVelocity = 0.0f;
                state->boarded = 0;
                mainSetBits(ObjAnim_ReadPlacementS16(&obj->anim, &(placement->boardedGameBit)), 0);
            }
        } else {
            if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &(placement->triggerGameBit))) == 0) {
                Sfx_PlayFromObject(obj, SFXTRIG_mv_wickpickup16);
                state->motionState = DLL1DB_MOTION_STATE_RISING;
                state->verticalVelocity = 0.0f;
                state->boarded = 0;
                mainSetBits(ObjAnim_ReadPlacementS16(&obj->anim, &(placement->boardedGameBit)), 0);
            }
        }
        break;
    case DLL1DB_MOTION_STATE_RISING:
        state->verticalVelocity =
            state->verticalVelocity + (0.02f * timeDelta + 0.1f * (f32)(s32)(state->verticalVelocity < 0.0f));
        {
            f32 verticalVelocity = state->verticalVelocity;

            if (verticalVelocity > 1.5f) {
                state->verticalVelocity = 1.5f;
            }
        }

        obj->anim.localPosY = state->verticalVelocity * timeDelta + obj->anim.localPosY;
        if (obj->anim.localPosY > placement->base.posY) {
            Sfx_PlayFromObject(obj, SFXTRIG_en_lflsh2_b);
            obj->anim.localPosY = placement->base.posY;
            state->motionState = DLL1DB_MOTION_STATE_TOP;
            if (playerContacted != 0) {
                state->boarded = 1;
                state->contactLost = 0;
            }
        }
        break;
    case DLL1DB_MOTION_STATE_FALLING:
        state->verticalVelocity = -0.02f * timeDelta + state->verticalVelocity;
        {
            f32 verticalVelocity = state->verticalVelocity;

            if (verticalVelocity < -1.5f) {
                state->verticalVelocity = -1.5f;
            }
        }

        obj->anim.localPosY = state->verticalVelocity * timeDelta + obj->anim.localPosY;
        if (obj->anim.localPosY < placement->base.posY - 235.5f) {
            Sfx_PlayFromObject(obj, SFXTRIG_en_lflsh2_b);
            obj->anim.localPosY = placement->base.posY - 235.5f;
            state->motionState = DLL1DB_MOTION_STATE_BOTTOM;
            mainSetBits(ObjAnim_ReadPlacementS16(&obj->anim, &(placement->boardedGameBit)), 1);
        }

        if (state->boarded == 0) {
            if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &(placement->triggerGameBit))) == 0) {
                state->motionState = DLL1DB_MOTION_STATE_RISING;
                mainSetBits(ObjAnim_ReadPlacementS16(&obj->anim, &(placement->boardedGameBit)), 0);
            }
        }
        break;
    }
}

void dll_1DB_init(GameObject* obj, const Dll1DBPlacementView* placement) {
    Dll1DBState* state = obj->extra;
    s16 rotationX = (s16)((s32)placement->rotationXByte << 8);

    obj->anim.rotX = rotationX;
    if (mainGetBit(ObjAnim_ReadPlacementS16(&obj->anim, &(placement->boardedGameBit))) != 0) {
        state->motionState = DLL1DB_MOTION_STATE_BOTTOM;
    } else {
        state->motionState = DLL1DB_MOTION_STATE_TOP;
    }

    obj->objectFlags |= OBJECT_OBJFLAG_HITDETECT_DISABLED;
}

void dll_1DB_release(void) {
}

void dll_1DB_initialise(void) {
}

OBJECT_INIT_ADAPTER(gDll1DBObjDescriptorInitAdapter, dll_1DB_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gDll1DBObjDescriptorHitDetectAdapter, dll_1DB_hitDetect)
OBJECT_FREE_ADAPTER(gDll1DBObjDescriptorFreeAdapter, dll_1DB_free)
OBJECT_TYPE_ID_ADAPTER(gDll1DBObjDescriptorTypeIdAdapter, dll_1DB_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gDll1DBObjDescriptorExtraSizeAdapter, dll_1DB_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gDll1DBObjDescriptorAcquire, dll_1DB_initialise)

ObjectDescriptor gDll1DBObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        gDll1DBObjDescriptorAcquire,
        dll_1DB_release,
    },
    0,
    gDll1DBObjDescriptorInitAdapter,
    dll_1DB_update,
    gDll1DBObjDescriptorHitDetectAdapter,
    dll_1DB_render,
    gDll1DBObjDescriptorFreeAdapter,
    gDll1DBObjDescriptorTypeIdAdapter,
    gDll1DBObjDescriptorExtraSizeAdapter,
};
