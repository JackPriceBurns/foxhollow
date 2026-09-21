#include "dlls/objects/442_SC_totempuz.h"

#include "game/objects/object_setup.h"
#include "main/audio/sfx.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/frame_timing.h"
#include "main/gamebit_ids.h"
#include "main/gamebits.h"
#include "main/dll/partfx_interface.h"
#include "main/obj_list.h"
#include "main/objHitReact_types.h"
#include "main/objfx.h"
#include "main/objhits.h"
#include "main/objtexture.h"
#include "main/object_render.h"
#include "main/shader.h"
#include "main/vecmath.h"
#include "main/objseq.h"

enum ScTotemPuzzleRomDefNo {
    SC_TOTEM_PUZZLE_ROM_DEF_NO = 0x3C1,
};

enum ScTotemPuzzleFlag {
    SC_TOTEM_PUZZLE_REVERSED = 1 << 0,
    SC_TOTEM_PUZZLE_READY = 1 << 1,
    SC_TOTEM_PUZZLE_PULSE_ACTIVE = 1 << 2,
};

enum ScTotemPuzzleStep {
    SC_TOTEM_PUZZLE_FORWARD_SOLVED_STEP = 4,
    SC_TOTEM_PUZZLE_SOLVED_PIECE_COUNT = 5,
    SC_TOTEM_PUZZLE_CAP_INDEX = 5,
    SC_TOTEM_PUZZLE_STEP_COUNT = 8,
};

enum ScTotemPuzzleTextureId {
    SC_TOTEM_PUZZLE_UNSOLVED_TEXTURE = 0,
    SC_TOTEM_PUZZLE_SOLVED_TEXTURE = 0x100,
};

typedef struct ScTotemPuzzlePlacement {
    ObjPlacement base;
    u8 unused18[3];
    s8 puzzleIndex;
    u8 unused1C[4];
} ScTotemPuzzlePlacement;

typedef struct ScTotemPuzzleState {
    f32 pulseTimer;
    f32 pulseTimerReset;
    f32 peerPhaseOffset;
    f32 angle;
    s16 stepIndex;
    s16 flags;
} ScTotemPuzzleState;

STATIC_ASSERT(sizeof(ScTotemPuzzlePlacement) == 0x20);
STATIC_ASSERT(offsetof(ScTotemPuzzlePlacement, base) == 0x00);
STATIC_ASSERT(offsetof(ScTotemPuzzlePlacement, unused18) == 0x18);
STATIC_ASSERT(offsetof(ScTotemPuzzlePlacement, puzzleIndex) == 0x1B);
STATIC_ASSERT(offsetof(ScTotemPuzzlePlacement, unused1C) == 0x1C);

STATIC_ASSERT(sizeof(ScTotemPuzzleState) == 0x14);
STATIC_ASSERT(offsetof(ScTotemPuzzleState, pulseTimer) == 0x00);
STATIC_ASSERT(offsetof(ScTotemPuzzleState, pulseTimerReset) == 0x04);
STATIC_ASSERT(offsetof(ScTotemPuzzleState, peerPhaseOffset) == 0x08);
STATIC_ASSERT(offsetof(ScTotemPuzzleState, angle) == 0x0C);
STATIC_ASSERT(offsetof(ScTotemPuzzleState, stepIndex) == 0x10);
STATIC_ASSERT(offsetof(ScTotemPuzzleState, flags) == 0x12);

static const s16 sTotemPuzzleStepAngles[] = {-8192, 0, 8192, 16384, 24576, -32768};

static int sc_totempuzzle_animEventCallback(GameObject* unusedObj, int unused, ObjSeqState* unusedAnimUpdate) {
    return mainGetBit(GAMEBIT_SC_totempuzzle_running) == 0;
}

static void sc_totempuzzle_setTexture(GameObject* obj, enum ScTotemPuzzleTextureId textureId) {
    ObjTextureRuntimeSlot* texture = objFindTexture(obj, 0, 0);

    if (texture != NULL) {
        texture->textureId = textureId;
    }
}

static void sc_totempuzzle_adjustPeerPhaseOffset(GameObject* obj, f32 amount) {
    int firstObjectIndex;
    int objectCount;
    GameObject** objects = ObjList_GetObjects(&firstObjectIndex, &objectCount);

    for (int objectIndex = firstObjectIndex; objectIndex < objectCount; objectIndex++) {
        GameObject* peer = objects[objectIndex];

        if (peer != obj && peer->anim.romDefNo == SC_TOTEM_PUZZLE_ROM_DEF_NO) {
            ((ScTotemPuzzleState*)peer->extra)->peerPhaseOffset += amount;
        }
    }
}

static void sc_totempuzzle_playHitEffect(GameObject* obj, PartFxSpawnParams* lightArgs) {
    Sfx_PlayFromObject(obj, SFXTRIG_wp_swdtest222);
    lightArgs->posX += playerMapOffsetX;
    lightArgs->posZ += playerMapOffsetZ;
    objDoHitParticleFx(obj, 0.014f, lightArgs, 1, NULL);
}

static u8 sc_totempuzzle_checkSolvedSequence(GameObject* obj, ScTotemPuzzleState* state) {
    PartFxSpawnParams particleOrigin;
    int firstObjectIndex;
    int objectCount;
    GameObject** objects = ObjList_GetObjects(&firstObjectIndex, &objectCount);
    int solvedCount = 0;
    u8 solvedThisObject = 0;

    for (int objectIndex = firstObjectIndex; objectIndex < objectCount; objectIndex++) {
        GameObject* peer = objects[objectIndex];

        if (peer->anim.romDefNo != SC_TOTEM_PUZZLE_ROM_DEF_NO) {
            continue;
        }

        ScTotemPuzzleState* peerState = peer->extra;
        if ((peerState->flags & SC_TOTEM_PUZZLE_READY) == 0) {
            continue;
        }

        int solvedStep = peerState->stepIndex;
        if ((peerState->flags & SC_TOTEM_PUZZLE_REVERSED) != 0) {
            solvedStep++;
        }
        if (solvedStep == SC_TOTEM_PUZZLE_FORWARD_SOLVED_STEP) {
            solvedCount++;
            if (peer == obj) {
                state->angle = 8192.0f * solvedStep;
                obj->anim.rotX = (s16)(s32)state->angle;
                solvedThisObject = 1;
            }
        } else if (peer == obj) {
            Sfx_PlayFromObject(NULL, SFXTRIG_lowoxy_beep);
        }
    }

    if (solvedThisObject != 0) {
        particleOrigin.posX = 0.0f;
        particleOrigin.posY = 16.5f;
        particleOrigin.posZ = 0.0f;
        particleOrigin.scale = 1.0f;

        for (int particleIndex = 20; particleIndex != 0; particleIndex--) {
            objfx_spawnArcedBurst(obj, 7, 2.0f, 5, 7, 100, 25.0f, 25.0f, 30.0f, &particleOrigin, 0);
        }
        sc_totempuzzle_setTexture(obj, SC_TOTEM_PUZZLE_SOLVED_TEXTURE);
    }

    if (solvedCount == SC_TOTEM_PUZZLE_SOLVED_PIECE_COUNT) {
        if (solvedThisObject != 0) {
            Sfx_PlayFromObject(NULL, SFXTRIG_mpick1_b);
        }
        return 1;
    }

    if (solvedThisObject != 0) {
        Sfx_PlayFromObject(NULL, SFXTRIG_sc_menuups16k_409);
    }
    return 0;
}

static int sc_totempuzzle_getExtraSize(void) {
    return sizeof(ScTotemPuzzleState);
}

static int sc_totempuzzle_getObjectTypeId(void) {
    return 0;
}

static void sc_totempuzzle_free(void) {
}

static void sc_totempuzzle_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                                  s8 visible) {
    if (visible != 0) {
        objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    }
}

static void sc_totempuzzle_hitDetect(void) {
}

static void sc_totempuzzle_update(GameObject* obj) {
    ScTotemPuzzleState* state = obj->extra;
    PartFxSpawnParams lightArgs;
    GameObject* hitObject;
    int hitSphereIndex;
    u32 hitVolume;
    int hitKind = ObjHits_GetPriorityHitWithPosition(obj, &hitObject, &hitSphereIndex, &hitVolume, &lightArgs.posX,
                                                     &lightArgs.posY, &lightArgs.posZ);
    u8 wasHit = hitKind != 0 && hitKind != OBJHITREACT_COLLISION_SKIP_REACTION;

    if ((obj->anim.bankIndex == SC_TOTEM_PUZZLE_CAP_INDEX) || (mainGetBit(GAMEBIT_SC_totempuzzle_running) != 0) ||
        (mainGetBit(GAMEBIT_SC_TotemPuzzleActivated) == 0)) {
        if (wasHit != 0) {
            sc_totempuzzle_playHitEffect(obj, &lightArgs);
        }
        return;
    }

    if (wasHit != 0) {
        sc_totempuzzle_playHitEffect(obj, &lightArgs);
        state->flags ^= SC_TOTEM_PUZZLE_READY;
        if ((state->flags & SC_TOTEM_PUZZLE_READY) != 0) {
            if (state->pulseTimer != 0.0f) {
                mainSetBits(GAMEBIT_SC_totempuzzle_running, sc_totempuzzle_checkSolvedSequence(obj, state));
            }
            sc_totempuzzle_adjustPeerPhaseOffset(obj, 0.65f);
        } else {
            sc_totempuzzle_adjustPeerPhaseOffset(obj, -0.65f);
            sc_totempuzzle_setTexture(obj, SC_TOTEM_PUZZLE_UNSOLVED_TEXTURE);
        }
    }

    if ((state->flags & SC_TOTEM_PUZZLE_READY) != 0) {
        return;
    }

    if ((state->flags & SC_TOTEM_PUZZLE_PULSE_ACTIVE) != 0) {
        state->pulseTimer -= timeDelta;
        if (state->pulseTimer < 0.0f) {
            state->flags &= ~SC_TOTEM_PUZZLE_PULSE_ACTIVE;
            Sfx_PlayFromObjectLimited(obj, SFXTRIG_mv_cagerat01, 2);
            if ((state->flags & SC_TOTEM_PUZZLE_REVERSED) != 0) {
                if (--state->stepIndex < 0) {
                    state->angle += 65535.0f;
                    state->stepIndex = SC_TOTEM_PUZZLE_STEP_COUNT - 1;
                }
            } else if (++state->stepIndex >= SC_TOTEM_PUZZLE_STEP_COUNT) {
                state->angle -= 65535.0f;
                state->stepIndex = 0;
            }
        }
    } else if (((state->flags & SC_TOTEM_PUZZLE_REVERSED) != 0) &&
               (state->angle > (8192.0f * (f32)(s32)(state->stepIndex + 1)))) {
        f32 step = 512.0f * state->peerPhaseOffset;
        state->angle -= step * timeDelta;
    } else if (state->angle < (8192.0f * (f32)(s32)state->stepIndex)) {
        f32 step = 512.0f * state->peerPhaseOffset;
        state->angle += step * timeDelta;
    } else {
        state->pulseTimer = state->pulseTimerReset / state->peerPhaseOffset;
        state->flags |= SC_TOTEM_PUZZLE_PULSE_ACTIVE;
    }

    obj->anim.rotX = (s16)(s32)state->angle;
}

static void sc_totempuzzle_init(GameObject* obj, const ScTotemPuzzlePlacement* placement) {
    ScTotemPuzzleState* state = obj->extra;

    obj->anim.bankIndex = placement->puzzleIndex;
    if (obj->anim.bankIndex < 0 || obj->anim.bankIndex > SC_TOTEM_PUZZLE_CAP_INDEX) {
        obj->anim.bankIndex = 0;
    }
    if (obj->anim.bankIndex == SC_TOTEM_PUZZLE_CAP_INDEX) {
        sc_totempuzzle_setTexture(obj, SC_TOTEM_PUZZLE_SOLVED_TEXTURE);
    }
    state->stepIndex = obj->anim.bankIndex;
    if (mainGetBit(GAMEBIT_SC_totempuzzle_running) == 0) {
        state->angle = (f32)(s32)sTotemPuzzleStepAngles[state->stepIndex];
    } else {
        state->angle = 32768.0f;
        sc_totempuzzle_setTexture(obj, SC_TOTEM_PUZZLE_SOLVED_TEXTURE);
    }
    obj->anim.rotX = (s16)(s32)state->angle;
    f32 pulseTime = 10.0f * randomGetRange(7, 10);
    state->pulseTimerReset = pulseTime;
    state->pulseTimer = pulseTime;
    if (obj->anim.bankIndex & 1) {
        state->flags = SC_TOTEM_PUZZLE_REVERSED;
    }
    state->peerPhaseOffset = 1.0f;
    obj->animEventCallback = sc_totempuzzle_animEventCallback;
    obj->objectFlags |= OBJECT_OBJFLAG_HIDDEN | OBJECT_OBJFLAG_HITDETECT_DISABLED;
}

static void sc_totempuzzle_release(void) {
}

static void sc_totempuzzle_initialise(void) {
}

OBJECT_INIT_ADAPTER(gSC_totempuzzleObjDescriptorInitAdapter, sc_totempuzzle_init, obj, placement)
OBJECT_HIT_DETECT_ADAPTER(gSC_totempuzzleObjDescriptorHitDetectAdapter, sc_totempuzzle_hitDetect)
OBJECT_FREE_ADAPTER(gSC_totempuzzleObjDescriptorFreeAdapter, sc_totempuzzle_free)
OBJECT_TYPE_ID_ADAPTER(gSC_totempuzzleObjDescriptorTypeIdAdapter, sc_totempuzzle_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gSC_totempuzzleObjDescriptorExtraSizeAdapter, sc_totempuzzle_getExtraSize)

RESOURCE_ACQUIRE_ADAPTER(gSC_totempuzzleObjDescriptorAcquire, sc_totempuzzle_initialise)

ObjectDescriptor gSC_totempuzzleObjDescriptor = {
    .header = {
        .metadata = { 0, 0, 0, OBJECT_DESCRIPTOR_FLAGS_10_SLOTS },
        .acquire = gSC_totempuzzleObjDescriptorAcquire,
        .release = sc_totempuzzle_release,
    },
    .init = gSC_totempuzzleObjDescriptorInitAdapter,
    .update = sc_totempuzzle_update,
    .hitDetect = gSC_totempuzzleObjDescriptorHitDetectAdapter,
    .render = sc_totempuzzle_render,
    .free = gSC_totempuzzleObjDescriptorFreeAdapter,
    .getObjectTypeId = gSC_totempuzzleObjDescriptorTypeIdAdapter,
    .getExtraSize = gSC_totempuzzleObjDescriptorExtraSizeAdapter,
};;
