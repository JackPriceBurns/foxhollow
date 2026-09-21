#include "dlls/objects/439.h"

#include "dlls/objects/279_AppleOnTree.h"
#include "dolphin/math.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/audio/sfx.h"
#include "main/dll/partfx_interface.h"
#include "main/frame_timing.h"
#include "main/gamebit_ids.h"
#include "main/gamebits.h"
#include "main/obj_path.h"
#include "main/objHitReact_types.h"
#include "main/object_render.h"
#include "main/objfx.h"
#include "main/objhits.h"
#include "main/objprint.h"
#include "main/shader.h"
#include "main/vecmath.h"
#include "sys/objects.h"
#include "sys/objects/lifecycle.h"

enum ScMusicTreeAmbientEffectCount {
    SC_MUSIC_TREE_AMBIENT_EFFECT_COUNT = 3,
};

enum ScMusicTreeMapObjectId {
    SC_MUSIC_TREE_MAP_TOTEM_1 = 0x30D9C,
    SC_MUSIC_TREE_MAP_TOTEM_2 = 0x30D9D,
    SC_MUSIC_TREE_MAP_TOTEM_3 = 0x30D9B,
    SC_MUSIC_TREE_MAP_GATE_1 = 0x448C2,
    SC_MUSIC_TREE_MAP_GATE_2 = 0x45178,
    SC_MUSIC_TREE_MAP_GATE_3 = 0x4517C,
};

enum ScMusicTreeFlag {
    SC_MUSIC_TREE_BURST_TYPE_MASK = 0x0F,
    SC_MUSIC_TREE_APPROACH_BURST = 1 << 4,
    SC_MUSIC_TREE_HIT_ACTIVE = 1 << 5,
    SC_MUSIC_TREE_PRIORITY_HIT = 1 << 6,
    SC_MUSIC_TREE_AMBIENT_EFFECTS = 1 << 7,
};

enum ScMusicTreeAmbientEffectState {
    SC_MUSIC_TREE_AMBIENT_EFFECT_ATTACHED_MAX_STATE = 3,
    SC_MUSIC_TREE_AMBIENT_EFFECT_HITTABLE_MIN_STATE = 2,
};

typedef struct ScMusicTreePlacement {
    ObjPlacement base;
    u8 rotZByte;
    u8 rotYByte;
    u8 rotXByte;
    u8 hearRadiusHalf;
    f32 scale;
    u8 colorR;
    u8 colorG;
    u8 colorB;
    u8 flags;
} ScMusicTreePlacement;

typedef struct ScMusicTreeState {
    GameObject* ambientEffects[SC_MUSIC_TREE_AMBIENT_EFFECT_COUNT];
    Vec3f ambientEffectPositions[SC_MUSIC_TREE_AMBIENT_EFFECT_COUNT];
    f32 proximityBurstTimer;
    f32 animationStep;
    f32 effectScale;
    f32 proximityCooldown;
    f32 hitCooldown;
    f32 hitEffectCooldown;
    u16 hearRadius;
    u16 previousDistance;
    u8 flags;
} ScMusicTreeState;

STATIC_ASSERT(sizeof(ScMusicTreePlacement) == 0x24);
STATIC_ASSERT(offsetof(ScMusicTreePlacement, base) == 0x00);
STATIC_ASSERT(offsetof(ScMusicTreePlacement, rotZByte) == 0x18);
STATIC_ASSERT(offsetof(ScMusicTreePlacement, rotYByte) == 0x19);
STATIC_ASSERT(offsetof(ScMusicTreePlacement, rotXByte) == 0x1A);
STATIC_ASSERT(offsetof(ScMusicTreePlacement, hearRadiusHalf) == 0x1B);
STATIC_ASSERT(offsetof(ScMusicTreePlacement, scale) == 0x1C);
STATIC_ASSERT(offsetof(ScMusicTreePlacement, colorR) == 0x20);
STATIC_ASSERT(offsetof(ScMusicTreePlacement, colorG) == 0x21);
STATIC_ASSERT(offsetof(ScMusicTreePlacement, colorB) == 0x22);
STATIC_ASSERT(offsetof(ScMusicTreePlacement, flags) == 0x23);

STATIC_ASSERT(sizeof(ScMusicTreeState) == 0x60);
STATIC_ASSERT(offsetof(ScMusicTreeState, ambientEffects) == 0x00);
STATIC_ASSERT(offsetof(ScMusicTreeState, ambientEffectPositions) == 0x18);
STATIC_ASSERT(offsetof(ScMusicTreeState, proximityBurstTimer) == 0x3C);
STATIC_ASSERT(offsetof(ScMusicTreeState, animationStep) == 0x40);
STATIC_ASSERT(offsetof(ScMusicTreeState, effectScale) == 0x44);
STATIC_ASSERT(offsetof(ScMusicTreeState, proximityCooldown) == 0x48);
STATIC_ASSERT(offsetof(ScMusicTreeState, hitCooldown) == 0x4C);
STATIC_ASSERT(offsetof(ScMusicTreeState, hitEffectCooldown) == 0x50);
STATIC_ASSERT(offsetof(ScMusicTreeState, hearRadius) == 0x54);
STATIC_ASSERT(offsetof(ScMusicTreeState, previousDistance) == 0x56);
STATIC_ASSERT(offsetof(ScMusicTreeState, flags) == 0x58);

static AppleOnTreeInterface* sc_musictree_getAmbientEffectInterface(GameObject* effect) {
    return APPLE_ON_TREE_INTERFACE(effect);
}

static void sc_musictree_spawnAmbientEffect(GameObject* obj, ScMusicTreeState* state, int index) {
    const ScMusicTreePlacement* placement = (const ScMusicTreePlacement*)obj->anim.placementData;
    AppleOnTreePlacement* effectPlacement;

    if (Obj_IsLoadingLocked() != 0) {
        effectPlacement = (AppleOnTreePlacement*)Obj_AllocObjectSetup(sizeof(*effectPlacement), APPLE_ON_TREE_OBJECT_ID);
        effectPlacement->base.color[0] = placement->base.color[0];
        effectPlacement->base.color[2] = placement->base.color[2];
        effectPlacement->base.color[1] = placement->base.color[1];
        effectPlacement->base.color[3] = placement->base.color[3] - 10;
        effectPlacement->base.posX = state->ambientEffectPositions[index].x;
        effectPlacement->base.posY = state->ambientEffectPositions[index].y;
        effectPlacement->base.posZ = state->ambientEffectPositions[index].z;
        effectPlacement->phaseDuration = randomGetRange(0x708, 0x1770);
        effectPlacement->initialElapsedTime = 1;
        effectPlacement->growthEndFraction = 10;
        effectPlacement->ripeEndFraction = 40;
        effectPlacement->fallEndFraction = 50;
        effectPlacement->landedEndFraction = 10;
        effectPlacement->fadeEndFraction = 50;
        effectPlacement->waterAccelerationPercent = -50;
        effectPlacement->despawnGameBit = -1;
        effectPlacement->unk18 = 0;
        state->ambientEffects[index] = objSetupObject(&effectPlacement->base, 5, -1, -1, obj->anim.parent);
    }
}

static void sc_musictree_handleHitObject(GameObject* obj, ScMusicTreeState* state) {
    switch (obj->anim.placement->ident) {
    case SC_MUSIC_TREE_MAP_TOTEM_1:
        Sfx_PlayFromObject(obj, SFXTRIG_sdrstp_c);
        Sfx_PlayFromObject(obj, SFXTRIG_gland2_c);
        mainSetBits(GAMEBIT_SC_TotemTreeHit1, 1);
        break;
    case SC_MUSIC_TREE_MAP_TOTEM_2:
        Sfx_PlayFromObject(obj, SFXTRIG_en_sdrstp_c);
        Sfx_PlayFromObject(obj, SFXTRIG_gland2_c);
        mainSetBits(GAMEBIT_SC_TotemTreeHit2, 1);
        break;
    case SC_MUSIC_TREE_MAP_TOTEM_3:
        Sfx_PlayFromObject(obj, SFXTRIG_en_sdrstp_c_12d);
        Sfx_PlayFromObject(obj, SFXTRIG_gland2_c);
        mainSetBits(GAMEBIT_SC_TotemTreeHit3, 1);
        break;
    case SC_MUSIC_TREE_MAP_GATE_1:
        if (mainGetBit(GAMEBIT_SC_MusicTreeActive) != 0) {
            mainSetBits(GAMEBIT_SC_MusicTreeTarget1Hit, 1);
        }
        break;
    case SC_MUSIC_TREE_MAP_GATE_2:
        if (mainGetBit(GAMEBIT_SC_MusicTreeActive) != 0) {
            mainSetBits(GAMEBIT_SC_MusicTreeTarget2Hit, 1);
        }
        break;
    case SC_MUSIC_TREE_MAP_GATE_3:
        if (mainGetBit(GAMEBIT_SC_MusicTreeActive) != 0) {
            mainSetBits(GAMEBIT_SC_MusicTreeTarget3Hit, 1);
        }
        break;
    }
    state->animationStep = 0.0225f;
}

static int sc_musictree_getExtraSize(void) {
    return sizeof(ScMusicTreeState);
}

static int sc_musictree_getObjectTypeId(void) {
    return 0;
}

static void sc_musictree_free(void) {
}

static void sc_musictree_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                                s8 visible) {
    const ScMusicTreePlacement* placement = (const ScMusicTreePlacement*)obj->anim.placementData;
    ScMusicTreeState* state = obj->extra;

    if (visible == 0) {
        return;
    }
    objSetColorFilter(placement->colorR, placement->colorG, placement->colorB);
    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    if ((state->flags & SC_MUSIC_TREE_AMBIENT_EFFECTS) != 0) {
        for (int i = 0; i < SC_MUSIC_TREE_AMBIENT_EFFECT_COUNT; i++) {
            Vec3f* position = &state->ambientEffectPositions[i];

            ObjPath_GetPointWorldPosition(obj, i, &position->x, &position->y, &position->z, 0);
        }
    }
    obj->userData2 = 1;
}

static void sc_musictree_hitDetect(void) {
}

static void sc_musictree_update(GameObject* obj) {
    ScMusicTreeState* state = obj->extra;
    ObjAnimEventList animEvents;
    PartFxSpawnParams effectParams;
    int hitType;
    GameObject* hitObject;
    int hitSphereIndex;
    u32 hitVolume;

    ObjAnim_AdvanceCurrentMove(obj, state->animationStep, timeDelta, &animEvents);
    if (state->flags == 0) {
        return;
    }
    if (state->proximityCooldown > 0.0f) {
        state->proximityCooldown -= timeDelta;
    }
    if (state->animationStep > 0.0025f) {
        state->animationStep -= 0.001f;
    }
    if (((state->flags & SC_MUSIC_TREE_AMBIENT_EFFECTS) != 0) && (obj->userData2 != 0)) {
        for (int i = 0; i < SC_MUSIC_TREE_AMBIENT_EFFECT_COUNT; i++) {
            GameObject* ambientEffect = state->ambientEffects[i];

            if (ambientEffect == NULL) {
                sc_musictree_spawnAmbientEffect(obj, state, i);
            } else {
                AppleOnTreeInterface* interface = sc_musictree_getAmbientEffectInterface(ambientEffect);
                int ambientEffectState = interface->getAnimState(ambientEffect);

                if (ambientEffectState > SC_MUSIC_TREE_AMBIENT_EFFECT_ATTACHED_MAX_STATE) {
                    state->ambientEffects[i] = NULL;
                } else {
                    interface->setPosition(ambientEffect, &state->ambientEffectPositions[i].x);
                }
            }
        }
    }
    if ((state->flags & SC_MUSIC_TREE_HIT_ACTIVE) != 0) {
        if ((state->flags & (SC_MUSIC_TREE_PRIORITY_HIT | SC_MUSIC_TREE_AMBIENT_EFFECTS)) != 0) {
            hitType = ObjHits_GetPriorityHitWithPosition(obj, &hitObject, &hitSphereIndex, &hitVolume,
                                                         &effectParams.posX, &effectParams.posY, &effectParams.posZ);
        } else {
            hitType = ObjHits_PollPriorityHitEffectWithCooldown(obj, 8, 0xFF, 0xFF, 0x78, SFXTRIG_swdtest222,
                                                                &state->hitEffectCooldown);
        }
        if (state->hitCooldown >= 0.0f) {
            state->hitCooldown -= timeDelta;
        }
        if ((hitType != 0) && (hitType != OBJHITREACT_COLLISION_SKIP_REACTION) && (state->hitCooldown <= 0.0f)) {
            if ((state->flags & (SC_MUSIC_TREE_PRIORITY_HIT | SC_MUSIC_TREE_AMBIENT_EFFECTS)) != 0) {
                effectParams.posX = effectParams.posX + playerMapOffsetX;
                effectParams.posZ = effectParams.posZ + playerMapOffsetZ;
                objDoHitParticleFx(obj, 0.014f, &effectParams, 1, NULL);
                Obj_SetModelColorFadeRecursive(obj, 0xF, 0xC8, 0, 0, 1);
                sc_musictree_handleHitObject(obj, state);
            } else {
                Sfx_PlayFromObject(obj, SFXTRIG_swdtest222);
                Sfx_PlayFromObject(obj, SFXTRIG_gland2_c);
            }
            effectParams.posX = 0.0f;
            effectParams.posY = 200.0f * state->effectScale;
            effectParams.posZ = 0.0f;
            objfx_spawnRandomBurst(obj, state->flags & SC_MUSIC_TREE_BURST_TYPE_MASK, 0x14, &effectParams,
                                   80.0f * state->effectScale, 0);
            state->animationStep = 0.0225f;
            state->hitCooldown = 20.0f;
            if ((state->flags & SC_MUSIC_TREE_AMBIENT_EFFECTS) != 0) {
                for (int i = 0; i < SC_MUSIC_TREE_AMBIENT_EFFECT_COUNT; i++) {
                    GameObject* ambientEffect = state->ambientEffects[i];

                    if (ambientEffect != NULL &&
                        sc_musictree_getAmbientEffectInterface(ambientEffect)->getAnimState(ambientEffect) >=
                            SC_MUSIC_TREE_AMBIENT_EFFECT_HITTABLE_MIN_STATE) {
                        ObjHits_RecordObjectHit(ambientEffect, obj, 0xE, 1, 0);
                    }
                }
            }
        }
    }
    {
        GameObject* player = Obj_GetPlayerObject();
        f32 deltaX = obj->anim.localPosX - player->anim.localPosX;
        f32 deltaZ = obj->anim.localPosZ - player->anim.localPosZ;
        f32 distance = sqrtf(deltaX * deltaX + deltaZ * deltaZ);
        u16 distanceU16 = distance;
        if (distanceU16 < state->hearRadius) {
            if (((state->flags & SC_MUSIC_TREE_APPROACH_BURST) != 0) &&
                (state->previousDistance >= state->hearRadius) && (state->proximityCooldown <= 0.0f)) {
                effectParams.posX = 0.0f;
                effectParams.posY = 0.75f * (200.0f * state->effectScale);
                effectParams.posZ = 0.0f;
                objfx_spawnRandomBurst(obj, state->flags & SC_MUSIC_TREE_BURST_TYPE_MASK, 0xA, &effectParams,
                                       80.0f * state->effectScale, 1);
                state->proximityCooldown = 340.0f;
            }
            state->proximityBurstTimer -= timeDelta;
            if (state->proximityBurstTimer <= 0.0f) {
                effectParams.posX = 0.0f;
                effectParams.posY = 200.0f * state->effectScale;
                effectParams.posZ = 0.0f;
                vecRotateZXY(&obj->anim.rotX, &effectParams.posX);
                objfx_spawnRandomBurst(obj, state->flags & SC_MUSIC_TREE_BURST_TYPE_MASK, 1, &effectParams,
                                       80.0f * state->effectScale, 0);
                state->proximityBurstTimer += 30.0f;
            }
        }
        state->previousDistance = distanceU16;
    }
}

static void sc_musictree_init(GameObject* obj, const ScMusicTreePlacement* placement) {
    ScMusicTreeState* state = obj->extra;
    ObjAnimEventList animEvents;
    f32 ratio;
    f32 scale = ObjAnim_ReadPlacementF32(&obj->anim, &placement->scale);

    state->animationStep = 0.0025f;
    state->proximityBurstTimer = 0.0f;
    state->hearRadius = (u16)((u32)placement->hearRadiusHalf << 1);
    state->flags = placement->flags;
    state->proximityCooldown = 0.0f;
    state->effectScale = scale;
    obj->anim.rotZ = (s16)((placement->rotZByte - 0x7F) << 7);
    obj->anim.rotY = (s16)((placement->rotYByte - 0x7F) << 7);
    obj->anim.rotX = (s16)((u32)placement->rotXByte << 8);
    obj->anim.rootMotionScale = 3.6f * scale;
    obj->userData2 = 0;
    obj->objectFlags |= OBJECT_OBJFLAG_HITDETECT_DISABLED;
    ratio = (f32)(s32)randomGetRange(1, 99) / 100.0f;
    ObjAnim_SetCurrentMove(obj, 0, ratio, 0);
    ObjAnim_AdvanceCurrentMove(obj, 1.0f, 1.0f, &animEvents);
    ObjHitbox_SetCapsuleBounds(&obj->anim, (s32)(15.0f * state->effectScale), -5, 0xFF);
    if ((state->flags & SC_MUSIC_TREE_AMBIENT_EFFECTS) != 0) {
        state->flags |= SC_MUSIC_TREE_HIT_ACTIVE;
    }
}

static void sc_musictree_release(void) {
}

static void sc_musictree_initialise(void) {
}

OBJECT_INIT_ADAPTER(gSC_MusicTreeObjDescriptorInitAdapter, sc_musictree_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gSC_MusicTreeObjDescriptorHitDetectAdapter, sc_musictree_hitDetect)
OBJECT_FREE_ADAPTER(gSC_MusicTreeObjDescriptorFreeAdapter, sc_musictree_free)
OBJECT_TYPE_ID_ADAPTER(gSC_MusicTreeObjDescriptorTypeIdAdapter, sc_musictree_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gSC_MusicTreeObjDescriptorExtraSizeAdapter, sc_musictree_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gSC_MusicTreeObjDescriptorAcquire, sc_musictree_initialise)

ObjectDescriptor gSC_MusicTreeObjDescriptor = {
    .header = {
        .metadata = { 0, 0, 0, OBJECT_DESCRIPTOR_FLAGS_10_SLOTS },
        .acquire = gSC_MusicTreeObjDescriptorAcquire,
        .release = sc_musictree_release,
    },
    .init = gSC_MusicTreeObjDescriptorInitAdapter,
    .update = sc_musictree_update,
    .hitDetect = gSC_MusicTreeObjDescriptorHitDetectAdapter,
    .render = sc_musictree_render,
    .free = gSC_MusicTreeObjDescriptorFreeAdapter,
    .getObjectTypeId = gSC_MusicTreeObjDescriptorTypeIdAdapter,
    .getExtraSize = gSC_MusicTreeObjDescriptorExtraSizeAdapter,
};;
