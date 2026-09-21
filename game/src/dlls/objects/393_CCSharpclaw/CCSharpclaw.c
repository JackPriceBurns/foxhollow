#include "dlls/objects/393_CCSharpclaw.h"

#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/audio/sfx.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/dll/partfx_interface.h"
#include "main/dll/dll_0000_gameui.h"
#include "main/dll/player.h"
#include "main/frame_timing.h"
#include "main/gamebit_ids.h"
#include "main/gamebits.h"
#include "main/minimap.h"
#include "main/obj_trigger.h"
#include "main/objfx.h"
#include "main/vecmath_distance.h"
#include "sys/objects.h"

enum CcSharpClawPadParticleKind {
    CC_SHARPCLAW_PAD_PARTICLE_LIT = 2,
    CC_SHARPCLAW_PAD_PARTICLE_UNLIT = 5,
};

typedef struct CcSharpClawPadPlacement {
    ObjPlacement base;
    u8 rotXByte;
    u8 pad19;
    s16 activationGameBit;
} CcSharpClawPadPlacement;

STATIC_ASSERT(sizeof(CcSharpClawPadPlacement) == 0x1C);
STATIC_ASSERT(offsetof(CcSharpClawPadPlacement, rotXByte) == 0x18);
STATIC_ASSERT(offsetof(CcSharpClawPadPlacement, activationGameBit) == 0x1A);

typedef struct CcSharpClawPadState {
    f32 helpTimer;
} CcSharpClawPadState;

STATIC_ASSERT(sizeof(CcSharpClawPadState) == 4);

static int ccSharpClawPad_getExtraSize(void) {
    return sizeof(CcSharpClawPadState);
}

static void ccSharpClawPad_spawnParticles(GameObject* obj, enum CcSharpClawPadParticleKind kind) {
    PartFxSpawnParams origin = {
        .pos = {-5.0f, 5.0f, 0.0f},
    };

    objfx_spawnArcedBurst(obj, 5, 0.75f, kind, 2, 0x19, 2.0f, 2.0f, 10.0f, &origin, 0);
    origin.pos.x = 5.0f;
    objfx_spawnArcedBurst(obj, 5, 0.75f, kind, 2, 0x19, 2.0f, 2.0f, 10.0f, &origin, 0);
}

static void ccSharpClawPad_update(GameObject* obj) {
    const CcSharpClawPadPlacement* placement = (const CcSharpClawPadPlacement*)obj->anim.placement;
    CcSharpClawPadState* state = obj->extra;
    s16 activationGameBit = ObjAnim_ReadPlacementS16(&obj->anim, &placement->activationGameBit);

    if (mainGetBit(activationGameBit) != 0) {
        obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
        ccSharpClawPad_spawnParticles(obj, CC_SHARPCLAW_PAD_PARTICLE_LIT);
    } else {
        obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_DISABLED;
        if (mainGetBit(GAMEBIT_STAFF_ABILITY_SHARPCLAW_DISGUISE) == 0) {
            obj->anim.resetHitboxFlags |= INTERACT_FLAG_PROMPT_SUPPRESSED;
        } else {
            obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_PROMPT_SUPPRESSED;
        }
        if (ObjTrigger_IsSet(obj) != 0 && isAreaNameTextActive() == 0) {
            state->helpTimer = 600.0f;
        }
        if (state->helpTimer > 0.0f) {
            if ((obj->anim.resetHitboxFlags & INTERACT_FLAG_IN_RANGE) == 0) {
                state->helpTimer = 0.0f;
            } else {
                state->helpTimer -= timeDelta;
                showHelpText(obj->anim.modelInstance->helpTextIds[0]);
            }
        }
        GameObject* player = Obj_GetPlayerObject();
        if (vec3f_distanceSquared(&obj->anim.worldPosX, &player->anim.worldPosX) < 100.0f &&
            playerIsDisguised(player) != 0) {
            Sfx_PlayFromObject(obj, SFXTRIG_menuups16k);
            mainSetBits(activationGameBit, 1);
            obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
        }
        ccSharpClawPad_spawnParticles(obj, CC_SHARPCLAW_PAD_PARTICLE_UNLIT);
    }
}

static void ccSharpClawPad_init(GameObject* obj, const CcSharpClawPadPlacement* placement) {
    obj->anim.rotX = (s16)(placement->rotXByte << 8);
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN;
}

OBJECT_INIT_ADAPTER(gCCSharpClawPadObjDescriptorInitAdapter, ccSharpClawPad_init, obj, placement)
OBJECT_EXTRA_SIZE_ADAPTER(gCCSharpClawPadObjDescriptorExtraSizeAdapter, ccSharpClawPad_getExtraSize)

ObjectDescriptor gCCSharpClawPadObjDescriptor = {
    .header =
        {
            .metadata = {0, 0, 0, OBJECT_DESCRIPTOR_FLAGS_10_SLOTS},
            .acquire = NULL,
            .release = NULL,
        },
    .init = gCCSharpClawPadObjDescriptorInitAdapter,
    .update = ccSharpClawPad_update,
    .getExtraSize = gCCSharpClawPadObjDescriptorExtraSizeAdapter,
};
;
