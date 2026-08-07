/*
 * CCSharpclaw (DLL 0x189) - Crystal Caves SharpClaw pressure pad.
 *
 * The placement gamebit records whether a disguised player has activated the
 * pad. Activation disables further interaction and changes its particle bursts
 * from unlit kind 5 to lit kind 2.
 */
#include "dlls/objects/393_CCSharpclaw.h"

#include "main/audio/sfx_play_api.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/dll/dll_0000_gameui_api.h"
#include "main/dll/player_api.h"
#include "main/frame_timing.h"
#include "main/gamebit_ids.h"
#include "main/gamebits_api.h"
#include "main/dll/partfx_interface.h"
#include "main/minimap_api.h"
#include "main/obj_trigger.h"
#include "main/objfx.h"
#include "main/vecmath_distance_api.h"
#include "sys/objects.h"

#define CC_SHARPCLAW_PAD_HELP_DURATION               600.0f
#define CC_SHARPCLAW_PAD_ACTIVATION_DISTANCE_SQUARED 100.0f
#define CC_SHARPCLAW_PAD_ROT_X_SHIFT                 8

#define CC_SHARPCLAW_PAD_PARTICLE_OFFSET_X   5.0f
#define CC_SHARPCLAW_PAD_PARTICLE_OFFSET_Y   5.0f
#define CC_SHARPCLAW_PAD_PARTICLE_OFFSET_Z   0.0f
#define CC_SHARPCLAW_PAD_PARTICLE_INDEX      5
#define CC_SHARPCLAW_PAD_PARTICLE_SCALE      0.75f
#define CC_SHARPCLAW_PAD_PARTICLE_KIND_LIT   2
#define CC_SHARPCLAW_PAD_PARTICLE_KIND_UNLIT 5
#define CC_SHARPCLAW_PAD_PARTICLE_MODE       2
#define CC_SHARPCLAW_PAD_PARTICLE_CHANCE     0x19
#define CC_SHARPCLAW_PAD_PARTICLE_ANGLE_BASE 2.0f
#define CC_SHARPCLAW_PAD_PARTICLE_ANGLE_LOW  2.0f
#define CC_SHARPCLAW_PAD_PARTICLE_ANGLE_HIGH 10.0f
#define CC_SHARPCLAW_PAD_PARTICLE_FLAGS      0

int ccSharpClawPad_getExtraSize(void) {
    return sizeof(CCSharpClawPadState);
}

void ccSharpClawPad_update(GameObject* obj) {
    PartFxSpawnParams particleOrigin;
    CCSharpClawPadState* state;
    GameObject* player;

    if (mainGetBit(((const CCSharpClawPadPlacement*)obj->anim.placement)->activationGameBit) != 0) {
        obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
        particleOrigin.posX = -CC_SHARPCLAW_PAD_PARTICLE_OFFSET_X;
        particleOrigin.posY = CC_SHARPCLAW_PAD_PARTICLE_OFFSET_Y;
        particleOrigin.posZ = CC_SHARPCLAW_PAD_PARTICLE_OFFSET_Z;
        objfx_spawnArcedBurst(obj, CC_SHARPCLAW_PAD_PARTICLE_INDEX, CC_SHARPCLAW_PAD_PARTICLE_SCALE,
                              CC_SHARPCLAW_PAD_PARTICLE_KIND_LIT, CC_SHARPCLAW_PAD_PARTICLE_MODE,
                              CC_SHARPCLAW_PAD_PARTICLE_CHANCE, CC_SHARPCLAW_PAD_PARTICLE_ANGLE_BASE,
                              CC_SHARPCLAW_PAD_PARTICLE_ANGLE_LOW, CC_SHARPCLAW_PAD_PARTICLE_ANGLE_HIGH,
                              &particleOrigin, CC_SHARPCLAW_PAD_PARTICLE_FLAGS);
        particleOrigin.posX = CC_SHARPCLAW_PAD_PARTICLE_OFFSET_X;
        objfx_spawnArcedBurst(obj, CC_SHARPCLAW_PAD_PARTICLE_INDEX, CC_SHARPCLAW_PAD_PARTICLE_SCALE,
                              CC_SHARPCLAW_PAD_PARTICLE_KIND_LIT, CC_SHARPCLAW_PAD_PARTICLE_MODE,
                              CC_SHARPCLAW_PAD_PARTICLE_CHANCE, CC_SHARPCLAW_PAD_PARTICLE_ANGLE_BASE,
                              CC_SHARPCLAW_PAD_PARTICLE_ANGLE_LOW, CC_SHARPCLAW_PAD_PARTICLE_ANGLE_HIGH,
                              &particleOrigin, CC_SHARPCLAW_PAD_PARTICLE_FLAGS);
    } else {
        obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_DISABLED;
        if (mainGetBit(GAMEBIT_STAFF_ABILITY_SHARPCLAW_DISGUISE) == 0) {
            obj->anim.resetHitboxFlags |= INTERACT_FLAG_PROMPT_SUPPRESSED;
        } else {
            obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_PROMPT_SUPPRESSED;
        }
        state = obj->extra;
        if (ObjTrigger_IsSet(obj) != 0 && isAreaNameTextActive() == 0) {
            state->helpTimer = CC_SHARPCLAW_PAD_HELP_DURATION;
        }
        if (state->helpTimer > 0.0f) {
            if ((obj->anim.resetHitboxFlags & INTERACT_FLAG_IN_RANGE) == 0) {
                state->helpTimer = 0.0f;
            } else {
                state->helpTimer -= timeDelta;
                showHelpText(obj->anim.modelInstance->helpTextIds[0]);
            }
        }
        player = Obj_GetPlayerObject();
        if (vec3f_distanceSquared(&obj->anim.worldPosX, &player->anim.worldPosX) <
                CC_SHARPCLAW_PAD_ACTIVATION_DISTANCE_SQUARED &&
            playerIsDisguised(player) != 0) {
            Sfx_PlayFromObject(obj, SFXTRIG_menuups16k);
            mainSetBits(((const CCSharpClawPadPlacement*)obj->anim.placement)->activationGameBit, 1);
            obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
        }
        particleOrigin.posX = -CC_SHARPCLAW_PAD_PARTICLE_OFFSET_X;
        particleOrigin.posY = CC_SHARPCLAW_PAD_PARTICLE_OFFSET_Y;
        particleOrigin.posZ = CC_SHARPCLAW_PAD_PARTICLE_OFFSET_Z;
        objfx_spawnArcedBurst(obj, CC_SHARPCLAW_PAD_PARTICLE_INDEX, CC_SHARPCLAW_PAD_PARTICLE_SCALE,
                              CC_SHARPCLAW_PAD_PARTICLE_KIND_UNLIT, CC_SHARPCLAW_PAD_PARTICLE_MODE,
                              CC_SHARPCLAW_PAD_PARTICLE_CHANCE, CC_SHARPCLAW_PAD_PARTICLE_ANGLE_BASE,
                              CC_SHARPCLAW_PAD_PARTICLE_ANGLE_LOW, CC_SHARPCLAW_PAD_PARTICLE_ANGLE_HIGH,
                              &particleOrigin, CC_SHARPCLAW_PAD_PARTICLE_FLAGS);
        particleOrigin.posX = CC_SHARPCLAW_PAD_PARTICLE_OFFSET_X;
        objfx_spawnArcedBurst(obj, CC_SHARPCLAW_PAD_PARTICLE_INDEX, CC_SHARPCLAW_PAD_PARTICLE_SCALE,
                              CC_SHARPCLAW_PAD_PARTICLE_KIND_UNLIT, CC_SHARPCLAW_PAD_PARTICLE_MODE,
                              CC_SHARPCLAW_PAD_PARTICLE_CHANCE, CC_SHARPCLAW_PAD_PARTICLE_ANGLE_BASE,
                              CC_SHARPCLAW_PAD_PARTICLE_ANGLE_LOW, CC_SHARPCLAW_PAD_PARTICLE_ANGLE_HIGH,
                              &particleOrigin, CC_SHARPCLAW_PAD_PARTICLE_FLAGS);
    }
}

void ccSharpClawPad_init(GameObject* obj, const CCSharpClawPadPlacement* placement) {
    obj->anim.rotX = (s16)((u32)placement->rotXByte << CC_SHARPCLAW_PAD_ROT_X_SHIFT);
    obj->objectFlags = (u16)(obj->objectFlags | OBJECT_OBJFLAG_HIDDEN);
}

ObjectDescriptor gCCSharpClawPadObjDescriptor = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    0,
    0,
    0,
    (ObjectDescriptorCallback)ccSharpClawPad_init,
    (ObjectDescriptorCallback)ccSharpClawPad_update,
    0,
    0,
    0,
    0,
    ccSharpClawPad_getExtraSize,
};
