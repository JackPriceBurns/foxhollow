/*
 * DLL 0xF9 - projectile-activated switches.
 *
 * The packed placement mode selects latched, toggle, or timed-reset behavior.
 * Switch state is mirrored to a game bit and to the model's first texture.
 */
#include "dlls/objects/249.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/frame_timing.h"
#include "main/gamebits_api.h"
#include "main/objtexture.h"
#include "main/object_render.h"
#include "main/audio/sfx_play_api.h"
#include "main/maketex_sequence_api.h"
#include "main/objhits.h"
#include "main/objprint_api.h"

#define PROJECTILE_SWITCH_OBJECT_TYPE_BASE  0x400
#define PROJECTILE_SWITCH_OBJECT_TYPE_SHIFT 11
#define PROJECTILE_SWITCH_MODEL_INDEX_SHIFT 2

#define PROJECTILE_SWITCH_HIT_PRIORITY_0E 0xE
#define PROJECTILE_SWITCH_HIT_PRIORITY_0F 0xF
#define PROJECTILE_SWITCH_FIREBALL_SEQ_ID 0x14B

#define PROJECTILE_SWITCH_SPECIAL_MAP_EVENT_SLOT 0x2C
#define PROJECTILE_SWITCH_TEXTURE_OFF            0
#define PROJECTILE_SWITCH_TEXTURE_ON             0x100

#define PROJECTILE_SWITCH_SCALE_UNITS           64
#define PROJECTILE_SWITCH_FRAMES_PER_SECOND     60.0f
#define PROJECTILE_SWITCH_TENTHS_PER_SECOND     0.1f
#define PROJECTILE_SWITCH_PARENT_GAME_BIT_COUNT 2
#define PROJECTILE_SWITCH_NO_GAME_BIT           -1

typedef struct ProjectileSwitchParentGameBitEntry {
    s32 mapId;
    s32 gameBitId;
} ProjectileSwitchParentGameBitEntry;

STATIC_ASSERT(offsetof(ProjectileSwitchParentGameBitEntry, mapId) == 0x0);
STATIC_ASSERT(offsetof(ProjectileSwitchParentGameBitEntry, gameBitId) == 0x4);
STATIC_ASSERT(sizeof(ProjectileSwitchParentGameBitEntry) == 0x8);

ProjectileSwitchParentGameBitEntry gProjectileSwitchParentGameBitMap[PROJECTILE_SWITCH_PARENT_GAME_BIT_COUNT] = {
    {0x00031CCF, 0x00000522},
    {0x00031CE0, 0x00000E6E},
};

int ProjectileSwitch_getExtraSize(void) {
    return sizeof(ProjectileSwitchState);
}

int ProjectileSwitch_getObjectTypeId(GameObject* obj) {
    ProjectileSwitchPlacement* placement = (ProjectileSwitchPlacement*)obj->anim.placementData;
    int modelIndex = (int)placement->modelIndexAndMode >> PROJECTILE_SWITCH_MODEL_INDEX_SHIFT;
    int modelCount = obj->anim.modelInstance->modelCount;

    if (modelIndex >= modelCount) {
        modelIndex = 0;
    }
    return ((u32)modelIndex << PROJECTILE_SWITCH_OBJECT_TYPE_SHIFT) | PROJECTILE_SWITCH_OBJECT_TYPE_BASE;
}

void ProjectileSwitch_free(void) {
}

void ProjectileSwitch_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible) {
    ProjectileSwitchPlacement* placement = (ProjectileSwitchPlacement*)obj->anim.placementData;

    if (visible != 0) {
        if ((placement->renderFlags & PROJECTILE_SWITCH_RENDER_CUSTOM_COLOR) != 0) {
            objSetColorFilter(placement->colorR, placement->colorG, placement->colorB);
        }
        objRenderModelAndHitVolumes(obj, fwdArg2, fwdArg3, fwdArg4, fwdArg5, 1.0f);
    }
}

void ProjectileSwitch_hitDetect(GameObject* obj) {
    ProjectileSwitchState* switchState;
    ProjectileSwitchState* updatedState;
    ProjectileSwitchPlacement* placement;
    int hitPriority;
    GameObject* hitObject;
    ObjTextureRuntimeSlot* texture;
    int rejectFireballHit;

    placement = (ProjectileSwitchPlacement*)obj->anim.placementData;
    switchState = obj->extra;
    hitPriority = ObjHits_GetPriorityHit(obj, &hitObject, NULL, NULL);
    if (hitPriority != PROJECTILE_SWITCH_HIT_PRIORITY_0E && hitPriority != PROJECTILE_SWITCH_HIT_PRIORITY_0F) {
        return;
    }

    rejectFireballHit = 0;
    if (hitObject->anim.romDefNo == PROJECTILE_SWITCH_FIREBALL_SEQ_ID) {
        ObjHitsPriorityState* hitState = (ObjHitsPriorityState*)hitObject->anim.hitReactState;
        if ((hitState->contactFlags & OBJHITS_CONTACT_FLAG_KIND_NONZERO) != 0) {
            rejectFireballHit = 1;
        }
    }
    if (rejectFireballHit != 0) {
        return;
    }

    if (switchState->isOn != 0) {
        if ((placement->modelIndexAndMode & PROJECTILE_SWITCH_MODE_MASK) != PROJECTILE_SWITCH_MODE_TOGGLE) {
            return;
        }
        updatedState = obj->extra;
        if (obj->anim.mapEventSlot == PROJECTILE_SWITCH_SPECIAL_MAP_EVENT_SLOT) {
            Sfx_PlayFromObject(obj, SFXTRIG_menuups16k);
        } else {
            Sfx_PlayFromObject(obj, SFXTRIG_dn_boar1_c_63);
        }
        texture = objFindTexture(obj, 0, 0);
        if (texture != NULL) {
            texture->textureId = PROJECTILE_SWITCH_TEXTURE_OFF;
        }
        updatedState->isOn = 0;
        mainSetBits(switchState->gameBitId, 0);
    } else {
        updatedState = obj->extra;
        if (obj->anim.mapEventSlot == PROJECTILE_SWITCH_SPECIAL_MAP_EVENT_SLOT) {
            Sfx_PlayFromObject(obj, SFXTRIG_menuups16k);
        } else {
            Sfx_PlayFromObject(obj, SFXTRIG_wp_mpwru1_62);
        }
        texture = objFindTexture(obj, 0, 0);
        if (texture != NULL) {
            texture->textureId = PROJECTILE_SWITCH_TEXTURE_ON;
        }
        updatedState->isOn = 1;
        mainSetBits(switchState->gameBitId, 1);
        if ((placement->modelIndexAndMode & PROJECTILE_SWITCH_MODE_MASK) == PROJECTILE_SWITCH_MODE_TIMED_RESET) {
            switchState->autoResetTimerFrames =
                PROJECTILE_SWITCH_FRAMES_PER_SECOND *
                (PROJECTILE_SWITCH_TENTHS_PER_SECOND * (f32)placement->autoResetDelayTenths);
        }
    }
}

void ProjectileSwitch_update(GameObject* obj) {
    ProjectileSwitchState* switchState;
    ProjectileSwitchState* updatedState;
    ObjTextureRuntimeSlot* texture;

    switchState = obj->extra;
    if (switchState->isOn != 0) {
        if (mainGetBit(switchState->gameBitId) == 0) {
            updatedState = obj->extra;
            texture = objFindTexture(obj, 0, 0);
            if (texture != NULL) {
                texture->textureId = PROJECTILE_SWITCH_TEXTURE_OFF;
            }
            updatedState->isOn = 0;
        }
    } else {
        if (mainGetBit(switchState->gameBitId) != 0) {
            updatedState = obj->extra;
            texture = objFindTexture(obj, 0, 0);
            if (texture != NULL) {
                texture->textureId = PROJECTILE_SWITCH_TEXTURE_ON;
            }
            updatedState->isOn = 1;
        }
    }
    if (switchState->autoResetTimerFrames > 0.0f) {
        switchState->autoResetTimerFrames = switchState->autoResetTimerFrames - (f32)(u32)framesThisStep;
        if (switchState->autoResetTimerFrames <= 0.0f) {
            switchState->autoResetTimerFrames = 0.0f;
            mainSetBits(switchState->gameBitId, 0);
        }
    }
}

void ProjectileSwitch_init(GameObject* obj, ProjectileSwitchPlacement* placement) {
    ProjectileSwitchState* switchState;
    GameObject* parentObj;
    ObjPlacement* parentPlacement;
    ObjTextureRuntimeSlot* texture;

    switchState = obj->extra;
    obj->anim.rotX = (s16)(placement->rotXByte << 8);
    obj->anim.rotY = (s16)(placement->rotYByte << 8);
    if (placement->scale64 == 0) {
        obj->anim.rootMotionScale = obj->anim.modelInstance->rootMotionScaleBase;
    } else {
        f32 scaledScale = (f32)(u32)placement->scale64 * obj->anim.modelInstance->rootMotionScaleBase;
        obj->anim.rootMotionScale = scaledScale / PROJECTILE_SWITCH_SCALE_UNITS;
    }
    ObjHitbox_SetSphereRadius(&obj->anim,
                              (s16)(((int)placement->scale64 * (int)obj->anim.modelInstance->primaryHitboxRadius) /
                                    PROJECTILE_SWITCH_SCALE_UNITS));
    obj->anim.bankIndex = placement->modelIndexAndMode >> PROJECTILE_SWITCH_MODEL_INDEX_SHIFT;
    if ((int)obj->anim.bankIndex >= obj->anim.modelInstance->modelCount) {
        obj->anim.bankIndex = 0;
    }

    parentObj = (GameObject*)obj->anim.parent;
    if (parentObj != NULL) {
        parentPlacement = (ObjPlacement*)parentObj->anim.placementData;
        if (parentPlacement != NULL) {
            switchState->gameBitId = seqPairTableLookup(
                gProjectileSwitchParentGameBitMap, PROJECTILE_SWITCH_PARENT_GAME_BIT_COUNT, parentPlacement->ident);
        } else {
            switchState->gameBitId = PROJECTILE_SWITCH_NO_GAME_BIT;
        }
    } else {
        switchState->gameBitId = placement->gameBitId;
    }
    switchState->isOn = mainGetBit(switchState->gameBitId);
    if (switchState->isOn != 0) {
        switchState = obj->extra;
        texture = objFindTexture(obj, 0, 0);
        if (texture != NULL) {
            texture->textureId = PROJECTILE_SWITCH_TEXTURE_ON;
        }
        switchState->isOn = 1;
    } else {
        switchState = obj->extra;
        texture = objFindTexture(obj, 0, 0);
        if (texture != NULL) {
            texture->textureId = PROJECTILE_SWITCH_TEXTURE_OFF;
        }
        switchState->isOn = 0;
    }
    if ((placement->renderFlags & PROJECTILE_SWITCH_RENDER_CUSTOM_COLOR) == 0) {
        obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN;
    }
}

void ProjectileSwitch_release(void) {
}

void ProjectileSwitch_initialise(void) {
}

ObjectDescriptor gProjectileSwitchObjDescriptor = {
    0,                                                          /* reserved0 */
    0,                                                          /* reserved1 */
    0,                                                          /* reserved2 */
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,                           /* slotCountAndFlags */
    (ObjectDescriptorCallback)ProjectileSwitch_initialise,      /* initialise */
    (ObjectDescriptorCallback)ProjectileSwitch_release,         /* release */
    0,                                                          /* slot02 */
    (ObjectDescriptorCallback)ProjectileSwitch_init,            /* init */
    (ObjectDescriptorCallback)ProjectileSwitch_update,          /* update */
    (ObjectDescriptorCallback)ProjectileSwitch_hitDetect,       /* hitDetect */
    (ObjectDescriptorCallback)ProjectileSwitch_render,          /* render */
    (ObjectDescriptorCallback)ProjectileSwitch_free,            /* free */
    (ObjectDescriptorCallback)ProjectileSwitch_getObjectTypeId, /* getObjectTypeId */
    ProjectileSwitch_getExtraSize,                              /* getExtraSize */
};
