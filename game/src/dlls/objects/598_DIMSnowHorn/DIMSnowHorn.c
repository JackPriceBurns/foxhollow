/*
 * DIMSnowHorn (DLL 0x256) - the rideable SnowHorn mammoth found in
 * DIM (Dinosaur InfernoMountain).  Fox can mount the mammoth and use it to
 * clear puzzle obstacles.  The object runs a 12-state BaddieState machine
 * (stateHandler00-0B); the riding sub-loop (DIMSnowHorn1_ridingUpdate) handles stick/button
 * input and the air-meter while mounted, and DIMSnowHorn1_update coordinates
 * the full per-frame tick.
 */
#include "dlls/objects/457_DIMDismount.h"
#include "main/dll/partfx_interface.h"
#include "main/obj_path.h"
#include "main/texture.h"
#include "main/objHitReact.h"
#include "main/vecmath.h"
#include "main/newclouds.h"
#include "main/dll/DIM/dll_0256_dimsnowhorn1.h"
#include "dolphin/math.h"
#include "main/frame_timing.h"
#include "main/gamebits.h"
#include "main/game_ui_interface.h"
#include "main/mapEventTypes.h"
#include "main/newshadows_audio.h"
#include "main/object_render.h"
#include "main/pad.h"
#include "main/rcp_dolphin.h"
#include "main/shader.h"
#include "sys/objects.h"
#include "main/dll/dll_002E_moveLib.h"
#include "main/dll/path_control_interface.h"
#include "main/dll/tricky.h"
#include "main/gamebit_ids.h"
#include "main/dll/baddie_state.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/player_control_interface.h"
#include "dlls/object_descriptor.h"
#include "dlls/objects/common/vehicle.h"
#include "main/objprint.h"
#include "main/objprint_anim.h"
#include "main/objprint_character.h"
#include "main/dll/dll_00C9_enemy.h"
#include "main/objtype.h"
#include "dolphin/pad.h"
#include "main/camera.h"
#include "main/objseq.h"
#include "main/audio/sfx.h"

f32 gDIMSnowHorn1ModelMtx[16];
void* gDIMSnowHorn1StateHandlers[12];
void* gDIMSnowHorn1DefaultStateHandler;
void* gDIMSnowHorn1Texture;

s16 gDIMSnowHorn1TextureId = 0x1C8;
f32 gDIMSnowHorn1PathCollisionData[2] = {25.0f, 25.0f};
s16 gDIMSnowHorn1MoveIds[2] = {0x103, 0xB};
f32 gDIMSnowHorn1MoveSpeeds[2] = {0.0031f, 0.005f};
s16 gDIMSnowHorn1LocomotionMoveIds[4] = {0, 3, 0, 0};

#define DIMSNOWHORN1_OBJGROUP           0xa   /* snowhorn own add/remove group */
#define DIMSNOWHORN1_AIRMETER_BGTEXTURE 0x5d0 /* HUD air-meter background texture id */
#define GAMEBIT_SNOWHORN_RIDING         0x3e3 /* set while Fox is mounted on the SnowHorn */
#define GAMEBIT_SNOWHORN_AIR_DRAIN      0x3e2 /* set to drain the air meter each frame */
#define GAMEBIT_SNOWHORN_AIR_RESET      0x3e9 /* set to reset the air meter to full */
#define GAMEBIT_SNOWHORN_PUZZLE         0x170 /* puzzle-step trigger, counts pushes */

/* DIMSnowHorn1State.flags bits */
#define SNOWHORN1_FLAG_RIDING        0x2  /* GAMEBIT_SNOWHORN_RIDING active (set cross-DLL) */
#define SNOWHORN1_FLAG_HITVOL_PRIO   0x8  /* suppress hit-volume priority this frame */
#define SNOWHORN1_FLAG_SEQ_TRIGGERED 0x20 /* interaction sequence armed */

#define CLAMP_EXPR(value, low, high) ((value) < (low) ? (low) : ((value) > (high) ? (high) : (value)))

typedef struct DIMSnowHorn1PieceCounts {
    u8 counts[4];
} DIMSnowHorn1PieceCounts;

static const DIMSnowHorn1PieceCounts sDIMSnowHorn1DefaultPieceCounts = {{1, 1, 1, 1}};

static int DIMSnowHorn1_hasInput(DIMSnowHorn1State* state) {
    return state->baddie.pressedButtons != 0 || state->baddie.moveInputX != 0.0f || state->baddie.moveInputZ != 0.0f;
}

static void DIMSnowHorn1_transformLocalPoint(GameObject* obj, f32 x, f32 y, f32 z, f32* outX, f32* outY, f32* outZ) {
    MatrixTransform transform;
    f32 matrix[16];

    transform.x = obj->anim.localPosX;
    transform.y = obj->anim.localPosY;
    transform.z = obj->anim.localPosZ;
    transform.rotX = obj->anim.rotX;
    transform.rotY = obj->anim.rotY;
    transform.rotZ = obj->anim.rotZ;
    transform.scale = 1.0f;
    setMatrixFromObjectPos(matrix, &transform);
    Matrix_TransformPoint(matrix, x, y, z, outX, outY, outZ);
}

void DIMSnowHorn1_func23(void) {
}

int DIMSnowHorn1_defaultStateHandler(void) {
    return 0;
}

int DIMSnowHorn1_stateHandler0B(GameObject* obj, DIMSnowHorn1State* state) {
    DIMSnowHorn1State* inner = obj->extra;
    ObjHitsPriorityState* sub = (ObjHitsPriorityState*)obj->anim.hitReactState;

    state->baddie.flags0 |= 0x200000;
    state->baddie.animSpeedC = 0.0f;
    state->baddie.animSpeedB = 0.0f;
    state->baddie.animSpeedA = 0.0f;
    obj->anim.velocityX = 0.0f;
    obj->anim.velocityY = 0.0f;
    obj->anim.velocityZ = 0.0f;

    if (state->baddie.moveJustStartedA != 0) {
        inner->flags &= ~SNOWHORN1_FLAG_HITVOL_PRIO;
        sub->flags |= OBJHITS_PRIORITY_STATE_TRACK_CONTACT;
        ObjAnim_SetCurrentMove(obj, 0x204, 0.0f, 0);
        state->baddie.moveSpeed = 0.013f;
        Sfx_PlayFromObject(obj, SFXTRIG_thorntail_chew2);
    }

    if (sub->flags & OBJHITS_PRIORITY_STATE_TRACK_CONTACT && sub->contactFlags & OBJHITS_CONTACT_FLAG_KIND_NONZERO) {
        inner->flags |= SNOWHORN1_FLAG_HITVOL_PRIO;
    }

    if (inner->flags & SNOWHORN1_FLAG_HITVOL_PRIO) {
        sub->hitVolumePriority = 0;
        sub->hitVolumeId = 0;
        sub->flags &= ~OBJHITS_PRIORITY_STATE_TRACK_CONTACT;
    } else {
        sub->hitVolumePriority = 0xb;
        sub->hitVolumeId = 1;
        sub->flags |= OBJHITS_PRIORITY_STATE_TRACK_CONTACT;
    }

    return obj->anim.currentMoveProgress > 0.9f ? 8 : 0;
}

int DIMSnowHorn1_stateHandler0A(GameObject* obj, DIMSnowHorn1State* state, f32 t) {
    f32 nearDist = 300.0f;
    GameObject* near = objGetNearestTypeTo(DIM_DISMOUNT_POINT_OBJECT_GROUP, obj, &nearDist);
    DIMSnowHorn1State* inner = obj->extra;

    if (mainGetBit(GAMEBIT_SNOWHORN_RIDING) != 0 &&
        RandomTimer_UpdateRangeTrigger(&inner->randomTimerD04, 3.0f, 6.0f) != 0) {
        Sfx_PlayFromObject(obj, SFXTRIG_hightop_call1);
    }

    state->baddie.flags0 |= 0x200000;
    if (state->baddie.inputMagnitude < 0.05f) {
        state->baddie.turnRateAbs = 0;
        state->baddie.turnRate = 0;
        state->baddie.inputMagnitude = 0.0f;
    }

    if (state->baddie.turnRateAbs >= 0x5a) {
        return 8;
    }

    obj->anim.rotX = 182.0f * ((f32)state->baddie.turnRate * t / 36.0f) + (f32)obj->anim.rotX;

    f32 speed = inner->airMeterValue == 0 ? 0.0f : CLAMP_EXPR(state->baddie.inputMagnitude, 0.0f, 1.0f);
    f32 target = 0.85f * speed;
    state->baddie.animSpeedC += t * ((target - state->baddie.animSpeedC) / state->baddie.velSmoothTime);

    f32 slopeScale = obj->anim.rotY > 0 ? 0.3f : 0.15f;
    target -= slopeScale * mathSinf(3.1415927f * (f32)obj->anim.rotY / 32768.0f);
    if (target < gDIMSnowHorn1LocomotionSpeedRanges[2]) {
        target = gDIMSnowHorn1LocomotionSpeedRanges[2];
    }
    state->baddie.animSpeedA += t * ((target - state->baddie.animSpeedA) / state->baddie.velSmoothTime);

    f32 blend = obj->anim.currentMoveProgress;
    int phase = 0;
    while (gDIMSnowHorn1LocomotionMoveIds[phase] != obj->anim.currentMove && phase < 2) {
        phase++;
    }
    if (phase >= 2) {
        phase = 0;
    }
    if (obj->anim.currentMove == 0x208) {
        phase = 1;
    }

    int changed = 0;
    if (state->baddie.animSpeedC < gDIMSnowHorn1LocomotionSpeedRanges[phase * 2]) {
        if (phase == 1) {
            return 8;
        }
        phase--;
        changed = 1;
    } else if (state->baddie.animSpeedC >= gDIMSnowHorn1LocomotionSpeedRanges[phase * 2 + 1]) {
        if (phase == 0) {
            blend = 0.0f;
        }
        phase++;
        changed = 1;
    }

    int useNormal = 1;
    if (state->baddie.moveDone != 0 && obj->anim.currentMove == 0x208) {
        changed = 1;
        useNormal = 0;
    }

    if (changed) {
        if (phase == 1 && useNormal) {
            ObjAnim_SetCurrentMove(obj, 0x208, blend, 0);
        } else {
            ObjAnim_SetCurrentMove(obj, gDIMSnowHorn1LocomotionMoveIds[phase], blend, 0);
        }
    }

    ObjAnim_SampleRootCurvePhase(&obj->anim, state->baddie.animSpeedA, &state->baddie.moveSpeed);
    if (state->baddie.pressedButtons & PAD_BUTTON_A) {
        if (near == NULL || (near->anim.resetHitboxFlags & INTERACT_FLAG_IN_RANGE) == 0) {
            return 0xc;
        }
    }

    return 0;
}

int DIMSnowHorn1_stateHandler09(GameObject* obj, DIMSnowHorn1State* state, f32 fv) {
    f32 nearDist = 300.0f;
    GameObject* near = objGetNearestTypeTo(DIM_DISMOUNT_POINT_OBJECT_GROUP, obj, &nearDist);
    DIMSnowHorn1State* inner = obj->extra;

    state->baddie.flags0 |= 0x200000;

    if (state->baddie.turnRateAbs < inner->advanceCountThreshold || state->baddie.inputMagnitude == 0.0f) {
        return 8;
    }

    if (state->baddie.turnRate < -0xaf) {
        state->baddie.turnRate = -state->baddie.turnRate;
    }

    s16 turnMove = state->baddie.turnRate > 0 ? 0x201 : 0x200;
    if (obj->anim.currentMove != turnMove) {
        ObjAnim_SetCurrentMove(obj, turnMove, 0.0f, 0);
    }

    state->baddie.moveSpeed = 0.012f;
    (*gPlayerInterface)->updateAnimRootMotion(obj, state, fv, 8);

    if (state->baddie.pressedButtons & PAD_BUTTON_A) {
        if (near == NULL || (near->anim.resetHitboxFlags & INTERACT_FLAG_IN_RANGE) == 0) {
            return 0xc;
        }
    }

    return 0;
}

int DIMSnowHorn1_stateHandler08(GameObject* obj, DIMSnowHorn1State* state) {
    DIMSnowHorn1State* inner = obj->extra;

    state->baddie.flags0 |= 0x200000;
    obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;

    switch (obj->anim.currentMove) {
    case 0x206:
        if (state->baddie.moveDone != 0) {
            if (state->baddie.moveSpeed > 0.0f) {
                ObjAnim_SetCurrentMove(obj, 0x205, 0.0f, 0);
                state->baddie.moveSpeed = 0.005f;
            } else {
                return 8;
            }
        }
        if (inner->airMeterValue != 0 && state->baddie.moveSpeed > 0.0f && DIMSnowHorn1_hasInput(state)) {
            state->baddie.moveSpeed = -state->baddie.moveSpeed;
        }
        break;
    case 0x205:
        if (inner->airMeterValue != 0 && DIMSnowHorn1_hasInput(state)) {
            ObjAnim_SetCurrentMove(obj, 0x207, 0.0f, 0);
            state->baddie.moveSpeed = 0.014f;
        }
        break;
    case 0x207:
        if (state->baddie.moveDone != 0) {
            return 8;
        }
        break;
    default:
        ObjAnim_SetCurrentMove(obj, 0x206, 0.0f, 0);
        state->baddie.moveSpeed = 0.014f;
        break;
    }
    return 0;
}

int DIMSnowHorn1_stateHandler07(GameObject* obj, DIMSnowHorn1State* state) {
    f32 nearDist = 300.0f;
    GameObject* near = objGetNearestTypeTo(DIM_DISMOUNT_POINT_OBJECT_GROUP, obj, &nearDist);
    DIMSnowHorn1State* inner = obj->extra;
    obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
    state->baddie.animSpeedC = 0.0f;
    state->baddie.animSpeedB = 0.0f;
    state->baddie.animSpeedA = 0.0f;
    obj->anim.velocityX = 0.0f;
    obj->anim.velocityY = 0.0f;
    obj->anim.velocityZ = 0.0f;
    state->baddie.flags0 |= 0x200000;
    if (state->baddie.moveJustStartedA != 0) {
        state->baddie.controlTimer = 0;
        state->baddie.moveSpeed = 0.005f;
        state->baddie.velSmoothTime = 8.0f;
        if (obj->anim.currentMove != gDIMSnowHorn1LocomotionMoveIds[0]) {
            ObjAnim_SetCurrentMove(obj, gDIMSnowHorn1LocomotionMoveIds[0], 0.0f, 0);
        }
    }

    if ((obj->anim.currentMove == 0x209 || obj->anim.currentMove == 0x20a) && state->baddie.moveDone != 0) {
        ObjAnim_SetCurrentMove(obj, gDIMSnowHorn1LocomotionMoveIds[0], 0.0f, 0);
        state->baddie.moveSpeed = 0.005f;
    }

    if (state->baddie.inputMagnitude < 0.05f) {
        state->baddie.turnRateAbs = 0;
        state->baddie.turnRate = 0;
        state->baddie.inputMagnitude = 0.0f;
    }

    f32 v = *(f32*)&state->baddie.trackedObj;
    if (v > 0.0f && state->baddie.inputMagnitude > 0.0f && state->baddie.turnRateAbs >= inner->advanceCountThreshold) {
        return 0xa;
    }

    if (v > 0.1f && state->baddie.inputMagnitude > 0.1f && state->baddie.turnRateAbs < inner->advanceCountThreshold) {
        return 0xb;
    }

    if (state->baddie.pressedButtons & PAD_BUTTON_A) {
        if (near == NULL || (near->anim.resetHitboxFlags & INTERACT_FLAG_IN_RANGE) == 0) {
            return 0xc;
        }
    }

    if (mainGetBit(GAMEBIT_SNOWHORN_RIDING) != 0 &&
        RandomTimer_UpdateRangeTrigger(&inner->randomTimerD04, 3.0f, 6.0f) != 0) {
        Sfx_PlayFromObject(obj, SFXTRIG_hightop_call1);
    }
    return 0;
}

int DIMSnowHorn1_stateHandler06(GameObject* obj, DIMSnowHorn1State* state) {
    state->baddie.animSpeedC = 0.0f;
    state->baddie.animSpeedB = 0.0f;
    state->baddie.animSpeedA = 0.0f;
    obj->anim.velocityX = 0.0f;
    obj->anim.velocityY = 0.0f;
    obj->anim.velocityZ = 0.0f;
    state->baddie.flags0 |= 0x200000;

    DIMSnowHorn1State* inner = obj->extra;
    obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_DISABLED;
    obj->hitVolumeIndex = mainGetBit(GAMEBIT_SNOWHORN_PUZZLE) != 0;
    if (state->baddie.moveJustStartedA != 0) {
        state->baddie.moveSpeed = 0.005f;
        if (obj->anim.currentMove != 0x13) {
            ObjAnim_SetCurrentMove(obj, 0x13, 0.0f, 0);
        }
    }
    if (!(obj->anim.resetHitboxFlags & INTERACT_FLAG_IN_RANGE)) {
        return 0;
    }

    if ((*gGameUIInterface)->isItemBeingUsed(GAMEBIT_SNOWHORN_PUZZLE) != 0) {
        u8 puzzleStep = mainGetBit(GAMEBIT_SNOWHORN_PUZZLE);
        if (mainGetBit(GAMEBIT_ITEM_AlpineRoot_028) == 0) {
            switch (puzzleStep) {
            case 1:
                mainSetBits(GAMEBIT_ITEM_AlpineRoot_028, 1);
                inner->triggerMode = 2;
                break;
            case 2:
                inner->triggerMode = 4;
                mainSetBits(GAMEBIT_ITEM_DIMAlpineRoot_16F, 1);
                break;
            }
        } else {
            inner->triggerMode = 4;
            mainSetBits(GAMEBIT_ITEM_DIMAlpineRoot_16F, 1);
        }
        (*gObjectTriggerInterface)->runSequence(inner->triggerMode, obj, -1);
        mainSetBits(GAMEBIT_SNOWHORN_PUZZLE, mainGetBit(GAMEBIT_SNOWHORN_PUZZLE) - puzzleStep);
        buttonDisable(0, PAD_BUTTON_A);
    } else if (obj->anim.resetHitboxFlags & INTERACT_FLAG_ACTIVATED) {
        inner->triggerMode = mainGetBit(GAMEBIT_ITEM_AlpineRoot_028) != 0 ? 3 : 1;
        (*gObjectTriggerInterface)->runSequence(inner->triggerMode, obj, -1);
        buttonDisable(0, PAD_BUTTON_A);
    }

    return 0;
}

int DIMSnowHorn1_stateHandler05(GameObject* obj, DIMSnowHorn1State* state) {
    int bit_a, bit_b;
    int id_a, id_b, id_c, id_d;
    GameObject* tracker;
    GameObject* target;

    state->baddie.animSpeedC = 0.0f;
    state->baddie.animSpeedB = 0.0f;
    state->baddie.animSpeedA = 0.0f;
    obj->anim.velocityX = 0.0f;
    obj->anim.velocityY = 0.0f;
    obj->anim.velocityZ = 0.0f;
    state->baddie.flags0 |= 0x200000;

    DIMSnowHorn1State* inner = obj->extra;
    GameObject* player = Obj_GetPlayerObject();
    switch (inner->mode) {
    case 1:
        id_a = 0x1602;
        id_b = 0x454bc;
        id_c = 0x454b8;
        id_d = 0x454b9;
        bit_a = 0x172;
        bit_b = 0x9ed;
        break;
    case 4:
        id_a = 0x4963b;
        id_b = 0x4963c;
        id_c = 0x4963d;
        id_d = 0x4963e;
        bit_a = 0x8f9;
        bit_b = 0x85d;
        break;
    }

    if (state->baddie.moveJustStartedA != 0) {
        state->baddie.moveSpeed = 0.005f;
        if (obj->anim.currentMove != 0x13) {
            ObjAnim_SetCurrentMove(obj, 0x13, 0.0f, 0);
        }
    }

    if (mainGetBit(bit_a) != 0 && mainGetBit(bit_b) != 0 && player != NULL &&
        Vec_distance(&player->anim.worldPosX, &obj->anim.worldPosX) < 150.0f) {
        switch (inner->mode) {
        case 1:
            inner->triggerMode = 0;
            mainSetBits(GAMEBIT_ITEM_TrickyFlame_Got, 1);
            mainSetBits(GAMEBIT_DIM_FoundInjuredSnowHorn, 1);
            break;
        case 4:
            inner->triggerMode = 9;
            mainSetBits(0x1db, 1);
            break;
        }
        (*gObjectTriggerInterface)->runSequence(inner->triggerMode, obj, -1);
        buttonDisable(0, PAD_BUTTON_A);
        return 0;
    }

    obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
    int phase = inner->proximityPhase;
    switch (phase) {
    case 1:
        if (Vec_distance(&player->anim.worldPosX, &obj->anim.worldPosX) < 200.0f) {
            tracker = ObjList_FindObjectById(id_a);
            if (tracker != NULL) {
                enemy_trackPlayer(tracker);
            }
            tracker = ObjList_FindObjectById(id_b);
            if (tracker != NULL) {
                enemy_trackPlayer(tracker);
            }
            inner->proximityPhase = 2;
        }
        break;
    case 0:
    case 2:
        if (phase == 0 || Vec_distance(&player->anim.worldPosX, &obj->anim.worldPosX) > 300.0f) {
            tracker = ObjList_FindObjectById(id_a);
            target = ObjList_FindObjectById(id_c);
            if (tracker != NULL && target != NULL) {
                enemy_setTrackedObj(tracker, target);
            }
            tracker = ObjList_FindObjectById(id_b);
            target = ObjList_FindObjectById(id_d);
            if (tracker != NULL && target != NULL) {
                enemy_setTrackedObj(tracker, target);
            }
            inner->proximityPhase = 1;
        } else if (RandomTimer_UpdateRangeTrigger(&inner->randomTimerD08, 4.0f, 8.0f) != 0) {
            Sfx_PlayFromObject(obj, SFXTRIG_thorntail_chew1);
        }
        break;
    }
    return 0;
}

int DIMSnowHorn1_stateHandler04(GameObject* obj, DIMSnowHorn1State* state) {
    state->baddie.animSpeedC = 0.0f;
    state->baddie.animSpeedB = 0.0f;
    state->baddie.animSpeedA = 0.0f;
    obj->anim.velocityX = 0.0f;
    obj->anim.velocityY = 0.0f;
    obj->anim.velocityZ = 0.0f;
    state->baddie.flags0 |= 0x200000;

    if (state->baddie.moveJustStartedA != 0) {
        int idx = randomGetRange(0, 1);
        state->baddie.moveSpeed = gDIMSnowHorn1MoveSpeeds[idx];
        ObjAnim_SetCurrentMove(obj, gDIMSnowHorn1MoveIds[idx], 0.0f, 0);
    }

    if (state->baddie.moveDone != 0) {
        return -2;
    }

    if (obj->anim.resetHitboxFlags & INTERACT_FLAG_ACTIVATED) {
        (*gObjectTriggerInterface)->runSequence(randomGetRange(0, 2) + 6, obj, -1);
        buttonDisable(0, PAD_BUTTON_A);
    }

    return 0;
}

int DIMSnowHorn1_stateHandler03(GameObject* obj, DIMSnowHorn1State* state) {
    DIMSnowHorn1State* inner = obj->extra;

    state->baddie.animSpeedC = 0.0f;
    state->baddie.animSpeedB = 0.0f;
    state->baddie.animSpeedA = 0.0f;
    obj->anim.velocityX = 0.0f;
    obj->anim.velocityY = 0.0f;
    obj->anim.velocityZ = 0.0f;
    state->baddie.flags0 |= 0x200000;

    if (state->baddie.moveJustStartedA != 0) {
        int idx = randomGetRange(0, 1);
        state->baddie.moveSpeed = gDIMSnowHorn1MoveSpeeds[idx];
        ObjAnim_SetCurrentMove(obj, gDIMSnowHorn1MoveIds[idx], 0.0f, 0);
    }

    if (state->baddie.moveDone != 0) {
        return -1;
    }

    if (obj->anim.resetHitboxFlags & INTERACT_FLAG_ACTIVATED) {
        if (inner->flags & SNOWHORN1_FLAG_SEQ_TRIGGERED) {
            (*gObjectTriggerInterface)->runSequence(randomGetRange(0, 2) + 6, obj, -1);
        } else {
            (*gObjectTriggerInterface)->runSequence(5, obj, -1);
        }
        buttonDisable(0, PAD_BUTTON_A);
    }
    return 0;
}

int DIMSnowHorn1_stateHandler02(GameObject* obj, DIMSnowHorn1State* state, f32 fv) {
    DIMSnowHorn1State* inner = obj->extra;

    state->baddie.animSpeedC = 0.0f;
    state->baddie.animSpeedB = 0.0f;
    state->baddie.animSpeedA = 0.0f;
    obj->anim.velocityX = 0.0f;
    obj->anim.velocityY = 0.0f;
    obj->anim.velocityZ = 0.0f;
    state->baddie.flags0 |= 0x200000;
    state->baddie.moveSpeed = 0.005f;

    if (obj->anim.currentMove != gDIMSnowHorn1LocomotionMoveIds[0]) {
        ObjAnim_SetCurrentMove(obj, gDIMSnowHorn1LocomotionMoveIds[0], 0.0f, 0);
    }

    inner->countdownTimer = randomGetRange(0x4b0, 0x960);
    inner->countdownTimer -= (int)fv;
    if (inner->countdownTimer <= 0) {
        return -4;
    }

    if (obj->anim.resetHitboxFlags & INTERACT_FLAG_ACTIVATED) {
        (*gObjectTriggerInterface)->runSequence(randomGetRange(0, 2) + 6, obj, -1);
        buttonDisable(0, PAD_BUTTON_A);
    }

    return 0;
}

int DIMSnowHorn1_stateHandler01(GameObject* obj, DIMSnowHorn1State* state, f32 fv) {
    DIMSnowHorn1State* inner = obj->extra;

    state->baddie.animSpeedC = 0.0f;
    state->baddie.animSpeedB = 0.0f;
    state->baddie.animSpeedA = 0.0f;
    obj->anim.velocityX = 0.0f;
    obj->anim.velocityY = 0.0f;
    obj->anim.velocityZ = 0.0f;
    state->baddie.flags0 |= 0x200000;

    if (state->baddie.moveJustStartedA != 0) {
        state->baddie.moveSpeed = 0.005f;
        if (obj->anim.currentMove != gDIMSnowHorn1LocomotionMoveIds[0]) {
            ObjAnim_SetCurrentMove(obj, gDIMSnowHorn1LocomotionMoveIds[0], 0.0f, 0);
        }
        inner->countdownTimer = randomGetRange(0x4b0, 0x960);
    }

    inner->countdownTimer -= (int)fv;
    if (inner->countdownTimer <= 0) {
        return -3;
    }

    if (!(obj->anim.resetHitboxFlags & INTERACT_FLAG_ACTIVATED)) {
        return 0;
    }

    if (inner->flags & SNOWHORN1_FLAG_SEQ_TRIGGERED) {
        (*gObjectTriggerInterface)->runSequence(randomGetRange(0, 2) + 6, obj, -1);
    } else {
        (*gObjectTriggerInterface)->runSequence(5, obj, -1);
    }
    buttonDisable(0, PAD_BUTTON_A);

    return 0;
}

int DIMSnowHorn1_stateHandler00(GameObject* obj) {
    DIMSnowHorn1State* inner = obj->extra;

    switch (inner->mode) {
    case 0:
        if (mainGetBit(0xf3)) {
            inner->flags |= SNOWHORN1_FLAG_SEQ_TRIGGERED;
        }
        return 2;
    case 5:
        return 3;
    case 4:
        return mainGetBit(0x1db) ? 8 : 6;
    case 1:
        if (mainGetBit(GAMEBIT_ITEM_DIMAlpineRoot_16F)) {
            return 8;
        }
        if (mainGetBit(GAMEBIT_ITEM_AlpineRoot_028) || mainGetBit(GAMEBIT_DIM_FoundInjuredSnowHorn)) {
            return 7;
        }
        return 6;
    default:
        return 8;
    }
}

int DIMSnowHorn1_animEventCallback(GameObject* obj, int, ObjSeqState* animUpdate) {
    DIMSnowHorn1State* state = obj->extra;

    obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;

    switch (state->mode) {
    case 0:
        animUpdate->movementState = 0;
        if (obj->seqIndex == -1) {
            for (int i = 0; i < (int)(u32)animUpdate->eventCount; i++) {
                mainSetBits(GAMEBIT_ITEM_DIMCog1_Got, 1);
                state->flags |= SNOWHORN1_FLAG_SEQ_TRIGGERED;
            }
        }
        (*gPlayerInterface)->setState(obj, state, 1);
        break;
    case 5:
        animUpdate->movementState = 0;
        (*gPlayerInterface)->setState(obj, state, 2);
        break;
    case 4:
        animUpdate->movementState = 0;
        (*gPlayerInterface)->setState(obj, state, 7);
        break;
    case 1:
        animUpdate->movementState = 0;
        (*gPlayerInterface)->setState(obj, state, obj->seqIndex != -1 && state->triggerMode <= 3 ? 6 : 7);
        break;
    case 3:
        animUpdate->movementState = 0;
        state->baddie.moveJustStartedA = 1;
        (*gPlayerInterface)->setState(obj, state, 7);
        break;
    default:
        break;
    }

    (*gPathControlInterface)->attachObject(obj, &state->baddie.curvesCollision);
    state->baddie.animSpeedC = 0.0f;
    state->baddie.animSpeedB = 0.0f;
    state->baddie.animSpeedA = 0.0f;
    obj->anim.velocityX = 0.0f;
    obj->anim.velocityY = 0.0f;
    obj->anim.velocityZ = 0.0f;
    return animUpdate->movementState != 0;
}

void DIMSnowHorn1_handleRiderScale(GameObject* obj, f32 scale) {
    MatrixTransform transform;
    f32* pathMtx = (f32*)ObjPath_GetPointModelMtx(obj, 1);

    ObjPath_GetPointLocalPosition(obj, 1, &transform.x, &transform.y, &transform.z);
    transform.rotX = 0;
    transform.rotY = 0;
    transform.rotZ = 0;
    transform.scale = scale / obj->anim.modelInstance->rootMotionScaleBase;
    setMatrixFromObjectPos(gDIMSnowHorn1ModelMtx, &transform);
    mtx44_mult(gDIMSnowHorn1ModelMtx, pathMtx, gDIMSnowHorn1ModelMtx);
    objSetModelMatrixOverride(gDIMSnowHorn1ModelMtx);
}

void DIMSnowHorn1_func21(void) {
}

int DIMSnowHorn1_getRacePosition(void) {
    return 0;
}

f32 DIMSnowHorn1_func19(GameObject* obj, f32* out) {
    DIMSnowHorn1State* state = obj->extra;
    *out = state->baddie.controlMode == 0xa ? -state->baddie.moveSpeed : 0.005f;
    return 0.0f;
}

void DIMSnowHorn1_getPlayerAnim(void* unused, f32* out_f, int* out_i) {
    (void)unused;
    *out_f = 0.0f;
    *out_i = 0;
}

void DIMSnowHorn1_setMountState(GameObject* obj, int value) {
    ((DIMSnowHorn1State*)obj->extra)->mountMode = (u8)value;
}

int DIMSnowHorn1_getMountState(void) {
    return 0;
}

void DIMSnowHorn1_getCameraPosition(GameObject* obj, f32* outX, f32* outY, f32* outZ) {
    DIMSnowHorn1_transformLocalPoint(obj, 0.0f, 80.0f, -25.0f, outX, outY, outZ);
}

int DIMSnowHorn1_getDismountSide(GameObject* obj) {
    return ((DIMSnowHorn1State*)obj->extra)->dismountSide != 0 ? 2 : 1;
}

int DIMSnowHorn1_canDismount(GameObject* obj) {
    DIMSnowHorn1State* state = obj->extra;
    if (!(state->flags & SNOWHORN1_FLAG_RIDING)) {
        return 0;
    }
    mainSetBits(GAMEBIT_SNOWHORN_RIDING, 0);
    state->flags &= ~SNOWHORN1_FLAG_RIDING;
    return 1;
}

void DIMSnowHorn1_getRiderPosition(GameObject* obj, f32* out_x, f32* out_y, f32* out_z) {
    DIMSnowHorn1State* state = obj->extra;
    *out_x = state->pathPosX;
    *out_y = state->pathPosY;
    *out_z = state->pathPosZ;
}

int DIMSnowHorn1_getMountSide(GameObject* obj) {
    return ((DIMSnowHorn1State*)obj->extra)->mountSide != 0 ? 1 : 2;
}

int DIMSnowHorn1_canMount(GameObject* obj) {
    DIMSnowHorn1State* state = obj->extra;

    if (state->mode == 0 || state->mode == 5 || state->baddie.controlMode != 7 || obj->pendingParentObj != NULL) {
        return 0;
    }

    f32 range = 300.0f;
    GameObject* nearest = objGetNearestTypeTo(DIM_DISMOUNT_POINT_OBJECT_GROUP, obj, &range);
    if (nearest != NULL && nearest->anim.resetHitboxFlags & INTERACT_FLAG_IN_RANGE) {
        buttonDisable(0, PAD_BUTTON_A);
        return 1;
    }

    return 0;
}

void DIMSnowHorn1_spawnFootstepEffects(void* obj, DIMSnowHorn1State* pointState, DIMSnowHorn1State* inputState) {
    struct {
        u32 unk0;
        u32 unk4;
        f32 scale;
        f32 x;
        f32 y;
        f32 z;
    } args;
    u8 flags = (inputState->baddie.eventFlags >> 1) & 3;

    for (u8 pointIndex = 0; flags != 0; flags >>= 1, pointIndex++) {
        if (!(flags & 1)) {
            continue;
        }
        args.x = pointState->pathPointArray[pointIndex * 3];
        args.y = pointState->pathPointArray[pointIndex * 3 + 1];
        args.z = pointState->pathPointArray[pointIndex * 3 + 2];
        args.scale = 0.004f;

        for (u8 count = (u8)randomGetRange(2, 6); count != 0; count--) {
            (*gPartfxInterface)->spawnObject(obj, randomGetRange(0, 1) + 0x1f9, &args, 0x10001, -1, NULL);
        }

        Sfx_PlayFromObject(obj, surfaceSfxSelectTrigger(inputState->baddie.paletteSlot, 9));
        doRumble(3.0f);
    }
}

int DIMSnowHorn1_getExtraSize(void) {
    return sizeof(DIMSnowHorn1State);
}

int DIMSnowHorn1_getObjectTypeId(void) {
    return 0x43;
}

void DIMSnowHorn1_free(GameObject* obj) {
    objFreeObjectType(obj, DIMSNOWHORN1_OBJGROUP);
}

void DIMSnowHorn1_render(GameObject* obj, int p2, int p3, int p4, int p5, s8 visible) {
    DIMSnowHorn1State* state = obj->extra;

    if (visible == -1) {
        objRenderModelAndHitVolumes(obj, p2, p3, p4, p5, 1.0f);
        ObjPath_GetPointWorldPosition(obj, 1, &state->pathPosX, &state->pathPosY, &state->pathPosZ, 0);
        ObjPath_GetPointWorldPositionArray(obj, 2, 4, state->pathPointArray);
    }

    if (state->mountMode != 2 && visible != 0) {
        objRenderModelAndHitVolumes(obj, p2, p3, p4, p5, 1.0f);
        ObjPath_GetPointWorldPosition(obj, 1, &state->pathPosX, &state->pathPosY, &state->pathPosZ, 0);
        ObjPath_GetPointWorldPositionArray(obj, 2, 4, state->pathPointArray);
    }
}

void DIMSnowHorn1_hitDetect(void) {
}

void DIMSnowHorn1_ridingUpdate(GameObject* obj, int frameStep, int slot) {
    int matchFrame = slot == -1 || slot == framesThisStep - 1;
    Camera* camera = Camera_GetCurrent();
    DIMSnowHorn1State* state = obj->extra;

    state->baddie.hitPoints = 0;
    state->baddie.flags0 &= ~0x8000;

    if (state->mountMode == 2) {
        if (mainGetBit(GAMEBIT_SNOWHORN_AIR_DRAIN) != 0) {
            state->airMeterValue -= 1;
        } else {
            state->airMeterValue = 0x3e8;
        }
        (*gGameUIInterface)->runAirMeter(state->airMeterValue);
        if (mainGetBit(GAMEBIT_SNOWHORN_AIR_RESET) != 0) {
            mainSetBits(GAMEBIT_SNOWHORN_AIR_RESET, 0);
            state->airMeterValue = 0x3e8;
        }
        if (state->airMeterValue < 0) {
            state->airMeterValue = 0;
            (*gMapEventInterface)->gotoRestartPoint();
        }
        state->baddie.moveInputX = (f32)padGetStickX(0);
        state->baddie.moveInputZ = (f32)padGetStickY(0);
        state->baddie.pressedButtons = getButtonsJustPressed(0);
        state->baddie.heldButtons = getButtonsHeld(0);
        state->baddie.cameraYaw = camera->yaw;
    } else {
        state->baddie.moveInputX = 0.0f;
        state->baddie.moveInputZ = 0.0f;
        state->baddie.pressedButtons = 0;
        state->baddie.heldButtons = 0;
        state->baddie.cameraYaw = 0;
    }

    if (matchFrame) {
        state->baddie.flags0 &= ~0x00400000;
    } else {
        state->baddie.flags0 |= 0x00400000;
    }

    if (state->baddie.physicsActive != 0) {
        obj->anim.velocityY -= 0.14f * (f32)frameStep;
    }
    obj->anim.velocityY = CLAMP_EXPR(obj->anim.velocityY, -4.0f, 0.0f);

    (*gPlayerInterface)
        ->update(obj, state, timeDelta, timeDelta, gDIMSnowHorn1StateHandlers, &gDIMSnowHorn1DefaultStateHandler);
    DIMSnowHorn1_spawnFootstepEffects(obj, state, state);
}

static inline int DIMSnowHorn1_isFacingAway(GameObject* obj, GameObject* found) {
    s16 angleDelta = obj->anim.rotX - (u16)found->anim.rotX;
    return angleDelta > 0x4000 || angleDelta < -0x4000;
}

const f32 gDIMSnowHorn1OverrideOffsetY[1] = {-30.0f};
const f32 gDIMSnowHorn1OverrideOffsetZ[1] = {-20.0f};

static void DIMSnowHorn1_updateLookAtPlayer(GameObject* obj, DIMSnowHorn1State* data, GameObject* player) {
    if (player != NULL && Vec_distance(&player->anim.worldPosX, &obj->anim.worldPosX) < 300.0f &&
        data->mountMode == 0) {
        data->eyeAnimState.lookAtActive = 1;
        data->eyeAnimState.lookAtPosX = player->anim.localPosX;
        data->eyeAnimState.lookAtPosY = player->anim.localPosY;
        data->eyeAnimState.lookAtPosZ = player->anim.localPosZ;
    } else {
        data->eyeAnimState.lookAtActive = 0;
    }

    characterHeadLookCalm(obj, (s16*)&data->eyeAnimState, 0.0f);
}

static void DIMSnowHorn1_updateRidePrompt(GameObject* obj, DIMSnowHorn1State* data, GameObject* player) {
    f32 nearDist = 300.0f;
    GameObject* found = objGetNearestTypeTo(DIM_DISMOUNT_POINT_OBJECT_GROUP, obj, &nearDist);
    int foundInRange = found != NULL && found->anim.resetHitboxFlags & INTERACT_FLAG_IN_RANGE;

    if (data->mountMode == 0 && data->baddie.controlMode == 7 &&
        getXZDistanceSquared(&player->anim.worldPosX, &obj->anim.worldPosX) < 10000.0f) {
        if (!foundInRange) {
            return;
        }

        setAButtonIcon(0x14);
        if (!(found->anim.resetHitboxFlags & INTERACT_FLAG_ACTIVATED)) {
            return;
        }

        (*gMapEventInterface)->restartPoint(&player->anim.localPosX, 0x584, getCurMapLayer(), 0);
        buttonDisable(0, PAD_BUTTON_A);
        mainSetBits(GAMEBIT_SNOWHORN_RIDING, 1);
        if (DIMSnowHorn1_isFacingAway(obj, found)) {
            mainSetBits(GAMEBIT_NW_ClimbOnSnowHorn, 1);
        } else {
            mainSetBits(GAMEBIT_NW_SnowHown05BA, 1);
        }
        if (data->mode == 3) {
            data->airMeterValue = 1000;
            (*gGameUIInterface)->initAirMeter(1000, DIMSNOWHORN1_AIRMETER_BGTEXTURE);
        }
        return;
    }

    if (data->mountMode != 2) {
        return;
    }

    if (!foundInRange) {
        setAButtonIcon(0x13);
        return;
    }

    setAButtonIcon(0x15);
    if (!(found->anim.resetHitboxFlags & INTERACT_FLAG_ACTIVATED)) {
        return;
    }

    buttonDisable(0, PAD_BUTTON_A);
    mainSetBits(GAMEBIT_SNOWHORN_RIDING, 0);

    s8 modeIndex = -1;
    switch (data->mode) {
    case 1:
        modeIndex = 0;
        break;
    case 3:
        modeIndex = 1;
        break;
    case 4:
        modeIndex = 2;
        break;
    }

    int facingAway = DIMSnowHorn1_isFacingAway(obj, found);
    if (modeIndex >= 0) {
        SnowHornEntry* entry = &((SnowHornEntry*)gDIMSnowHorn1ConfigTable)[modeIndex];
        mainSetBits(entry->altPoseGameBit, ObjAnim_ReadPlacementS16(&found->anim, &found->anim.placementData[0xd]));
        mainSetBits(entry->flipRotGameBit, modeIndex ^ facingAway);
    }

    if (facingAway) {
        mainSetBits(GAMEBIT_NW_ClimbOffSnowHorn, 1);
    } else {
        mainSetBits(GAMEBIT_NW_SnowHown05BB, 1);
    }

    data->baddie.pressedButtons = 0;
    (*gGameUIInterface)->airMeterShutdown();
    (*gMapEventInterface)->clearRestartPoint();
}

void DIMSnowHorn1_update(GameObject* obj) {
    u8* base = gDIMSnowHorn1ConfigTable;
    GameObject* player = Obj_GetPlayerObject();
    DIMSnowHorn1State* data = obj->extra;

    data->advanceCountThreshold = 5;
    if (fhConfigRevision() == 1) {
        obj->anim.resetHitboxFlags |= INTERACT_FLAG_DISABLED;
    } else {
        obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_DISABLED;
    }
    ((ObjHitsPriorityState*)obj->anim.hitReactState)->trackContactMask = 9;

    int flags = base[0x94 + data->baddie.controlMode];
    if (!(flags & 8)) {
        ObjHitReactEntry* arm = (ObjHitReactEntry*)(base + (flags & 2 ? 0x80 : 0x6c));
        data->hitReactState = ObjHitReact_Update(obj, arm, 1, data->hitReactState, &data->hitReactStepScale);
        if (data->hitReactState != 0) {
            characterHeadLookRelax(obj, &data->eyeAnimState);
            characterDoEyeAnims(obj, &data->eyeAnimState);
            return;
        }
    }

    if (fhConfigRevision() == 1) {
        obj->anim.resetHitboxFlags &= ~INTERACT_FLAG_DISABLED;
    }

    if (data->mountMode == 2) {
        data->baddie.physicsActive = 1;
    } else {
        data->baddie.physicsActive = 0;
        data->baddie.animSpeedC = 0.0f;
        data->baddie.animSpeedB = 0.0f;
        data->baddie.animSpeedA = 0.0f;
        obj->anim.velocityX = 0.0f;
        obj->anim.velocityY = 0.0f;
        obj->anim.velocityZ = 0.0f;
        (*gPathControlInterface)->attachObject(obj, &data->baddie.curvesCollision);
    }
    DIMSnowHorn1_ridingUpdate(obj, framesThisStep, -1);
    (*gNewCloudsInterface)->func0ANop(data->mountMode != 0);

    if (data->mode == 0 || data->mode == 5) {
        DIMSnowHorn1_updateLookAtPlayer(obj, data, player);
    } else if (data->mode == 1 || data->mode == 3 || data->mode == 4) {
        DIMSnowHorn1_updateRidePrompt(obj, data, player);
    }

    characterDoEyeAnims(obj, &data->eyeAnimState);
    DIMSnowHorn1_transformLocalPoint(obj, 0.0f, gDIMSnowHorn1OverrideOffsetY[0], gDIMSnowHorn1OverrideOffsetZ[0],
                                     &obj->anim.modelState->overrideWorldPosX, &obj->anim.modelState->overrideWorldPosY,
                                     &obj->anim.modelState->overrideWorldPosZ);
}

void DIMSnowHorn1_init(GameObject* obj, DIMSnowHorn1Placement* def, int spawnFlag) {
    u8* base = gDIMSnowHorn1ConfigTable;
    DIMSnowHorn1PieceCounts pieceCounts = sDIMSnowHorn1DefaultPieceCounts;

    obj->anim.rotX = (s16)(def->spawnRot << 8);
    obj->animEventCallback = (void*)DIMSnowHorn1_animEventCallback;
    objAddObjectType(obj, DIMSNOWHORN1_OBJGROUP);

    DIMSnowHorn1State* inner = obj->extra;
    inner->mode = def->spawnVariant;
    inner->advanceCountThreshold = 5;
    inner->airMeterValue = 0x3e8;

    if (obj->anim.modelState != NULL) {
        obj->anim.modelState->flags |=
            OBJ_MODEL_STATE_UNREAD_0800 | OBJ_MODEL_STATE_UNREAD_0200 | OBJ_MODEL_STATE_UNREAD_0010;
    }

    if (obj->anim.hitReactState != NULL) {
        ((ObjHitsPriorityState*)obj->anim.hitReactState)->trackContactMask = 9;
    }

    (*gPlayerInterface)->init(obj, inner, 0xc, 1);
    inner->baddie.gravity = 0.17f;

    u8* pathState = (u8*)&inner->baddie.curvesCollision;
    pathState[0x25b] = 0;
    if (inner->mode == 1 || inner->mode == 3 || inner->mode == 4) {
        (*gPathControlInterface)->init(pathState, 3, 0x200020, 1);
        (*gPathControlInterface)->setLocalPointCollision(pathState, 2, base + 0xe0, &gDIMSnowHorn1PathCollisionData, 8);
        (*gPathControlInterface)->setup(pathState, 4, base + 0xa0, base + 0xd0, pieceCounts.counts);
        (*gPathControlInterface)->attachObject(obj, pathState);
    }

    dll_2E_initState(obj, &inner->lookController, -0x2000, 0x2aaa, 3);
    inner->lookController.modeBits |= 8;
    if (spawnFlag != 0) {
        return;
    }

    s8 idx = -1;
    switch (inner->mode) {
    case 1:
        if (mainGetBit(GAMEBIT_ITEM_DIMAlpineRoot_16F)) {
            idx = 0;
        }
        break;
    case 3:
        idx = 1;
        break;
    case 4:
        if (mainGetBit(0x1db)) {
            idx = 2;
        }
        break;
    }
    if (idx < 0) {
        return;
    }

    SnowHornEntry* entry = &((SnowHornEntry*)base)[idx];
    if (mainGetBit(entry->altPoseGameBit)) {
        obj->anim.localPosX = entry->altPosX;
        obj->anim.localPosY = entry->altPosY;
        obj->anim.localPosZ = entry->altPosZ;
        obj->anim.rotX = entry->altRotX;
    } else {
        obj->anim.localPosX = entry->posX;
        obj->anim.localPosY = entry->posY;
        obj->anim.localPosZ = entry->posZ;
        obj->anim.rotX = entry->rotX;
    }
    if (mainGetBit(entry->flipRotGameBit)) {
        obj->anim.rotX += 0x8000;
    }
}

void DIMSnowHorn1_release(void) {
    if (gDIMSnowHorn1Texture != NULL) {
        textureFree(gDIMSnowHorn1Texture);
    }
    gDIMSnowHorn1Texture = NULL;
}

void DIMSnowHorn1_initialise(void) {
    gDIMSnowHorn1StateHandlers[0] = DIMSnowHorn1_stateHandler00;
    gDIMSnowHorn1StateHandlers[1] = DIMSnowHorn1_stateHandler01;
    gDIMSnowHorn1StateHandlers[2] = DIMSnowHorn1_stateHandler02;
    gDIMSnowHorn1StateHandlers[3] = DIMSnowHorn1_stateHandler03;
    gDIMSnowHorn1StateHandlers[4] = DIMSnowHorn1_stateHandler04;
    gDIMSnowHorn1StateHandlers[5] = DIMSnowHorn1_stateHandler05;
    gDIMSnowHorn1StateHandlers[6] = DIMSnowHorn1_stateHandler06;
    gDIMSnowHorn1StateHandlers[7] = DIMSnowHorn1_stateHandler07;
    gDIMSnowHorn1StateHandlers[8] = DIMSnowHorn1_stateHandler08;
    gDIMSnowHorn1StateHandlers[9] = DIMSnowHorn1_stateHandler09;
    gDIMSnowHorn1StateHandlers[10] = DIMSnowHorn1_stateHandler0A;
    gDIMSnowHorn1StateHandlers[11] = DIMSnowHorn1_stateHandler0B;
    gDIMSnowHorn1DefaultStateHandler = (void*)DIMSnowHorn1_defaultStateHandler;
    gDIMSnowHorn1Texture = textureLoad(gDIMSnowHorn1TextureId, 0);
}

u8 gDIMSnowHorn1ConfigTable[] = {
    0x00, 0xD0, 0xE0, 0xC5, 0x00, 0xA0, 0x9E, 0xC4, 0x00, 0xC4, 0x49, 0x46, 0x10, 0x81, 0x00, 0x00, 0x00, 0xB8,
    0xFD, 0xC5, 0x00, 0x00, 0x97, 0xC4, 0x00, 0x7C, 0x54, 0x46, 0x00, 0xBD, 0x00, 0x01, 0x05, 0x01, 0x00, 0x00,
    0x00, 0x50, 0xFA, 0xC5, 0x00, 0xE0, 0x8F, 0xC4, 0x00, 0x88, 0x68, 0x46, 0xAC, 0x85, 0x00, 0x00, 0x00, 0x14,
    0x1E, 0xC6, 0x00, 0xC0, 0x40, 0xC4, 0x00, 0xBC, 0x6A, 0x46, 0x9D, 0x80, 0x01, 0x01, 0x06, 0x01, 0x00, 0x00,
    0x00, 0xB0, 0x13, 0xC6, 0x00, 0x30, 0x24, 0xC5, 0x00, 0x50, 0x7E, 0x46, 0x36, 0xBD, 0x00, 0x00, 0x00, 0xB0,
    0x13, 0xC6, 0x00, 0x30, 0x24, 0xC5, 0x00, 0x50, 0x7E, 0x46, 0x36, 0xBD, 0x01, 0x01, 0x43, 0x06, 0x00, 0x00,
    0xDA, 0x02, 0x75, 0x03, 0x30, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xA6, 0x9B, 0x44, 0x3C, 0x00, 0x00,
    0x00, 0x00, 0xDA, 0x02, 0x75, 0x03, 0x2F, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0xA6, 0x9B, 0x44, 0x3C,
    0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0x00, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00,
    0x40, 0xC1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA0, 0xC1, 0x00, 0x00, 0x40, 0x41, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0xA0, 0xC1, 0x00, 0x00, 0x40, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA0, 0x41, 0x00, 0x00,
    0x40, 0xC1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA0, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x0C, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0xC2,
};

f32 gDIMSnowHorn1LocomotionSpeedRanges[] = {0.0f, 0.05f, 0.03f, 0.85f};

const f32 gDIMSnowHorn1ZeroOffset = 0.0f;

OBJECT_INIT_ADAPTER(gDIMSnowHorn1ObjDescriptorInitAdapter, DIMSnowHorn1_init, obj, placement, flags)
OBJECT_HIT_DETECT_ADAPTER(gDIMSnowHorn1ObjDescriptorHitDetectAdapter, DIMSnowHorn1_hitDetect)
OBJECT_FREE_ADAPTER(gDIMSnowHorn1ObjDescriptorFreeAdapter, DIMSnowHorn1_free, obj)
OBJECT_TYPE_ID_ADAPTER(gDIMSnowHorn1ObjDescriptorTypeIdAdapter, DIMSnowHorn1_getObjectTypeId)
OBJECT_EXTRA_SIZE_ADAPTER(gDIMSnowHorn1ObjDescriptorExtraSizeAdapter, DIMSnowHorn1_getExtraSize)

VEHICLE_CAN_MOUNT_ADAPTER(gDIMSnowHorn1ObjDescriptorCanMountAdapter, DIMSnowHorn1_canMount, obj)
VEHICLE_CAN_DISMOUNT_ADAPTER(gDIMSnowHorn1ObjDescriptorCanDismountAdapter, DIMSnowHorn1_canDismount, obj)
VEHICLE_MOUNT_STATE_ADAPTER(gDIMSnowHorn1ObjDescriptorMountStateAdapter, DIMSnowHorn1_getMountState)
VEHICLE_PLAYER_ANIM_ADAPTER(gDIMSnowHorn1ObjDescriptorPlayerAnimAdapter, DIMSnowHorn1_getPlayerAnim, obj, blend, anim)
VEHICLE_RACE_POSITION_ADAPTER(gDIMSnowHorn1ObjDescriptorRacePositionAdapter, DIMSnowHorn1_getRacePosition)
VEHICLE_RESET_POSITION_ADAPTER(gDIMSnowHorn1ObjDescriptorResetPositionAdapter, DIMSnowHorn1_func21)
VEHICLE_LOOK_TARGET_ADAPTER(gDIMSnowHorn1ObjDescriptorLookTargetAdapter, DIMSnowHorn1_func23)

RESOURCE_ACQUIRE_ADAPTER(gDIMSnowHorn1ObjDescriptorAcquire, DIMSnowHorn1_initialise)

VehicleDescriptor gDIMSnowHorn1ObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_24_SLOTS,
        },
        gDIMSnowHorn1ObjDescriptorAcquire,
        DIMSnowHorn1_release,
    },
    {
        0,
        gDIMSnowHorn1ObjDescriptorInitAdapter,
        DIMSnowHorn1_update,
        gDIMSnowHorn1ObjDescriptorHitDetectAdapter,
        DIMSnowHorn1_render,
        gDIMSnowHorn1ObjDescriptorFreeAdapter,
        gDIMSnowHorn1ObjDescriptorTypeIdAdapter,
        gDIMSnowHorn1ObjDescriptorExtraSizeAdapter,
        gDIMSnowHorn1ObjDescriptorCanMountAdapter,
        DIMSnowHorn1_getMountSide,
        DIMSnowHorn1_getRiderPosition,
        gDIMSnowHorn1ObjDescriptorCanDismountAdapter,
        DIMSnowHorn1_getDismountSide,
        DIMSnowHorn1_getCameraPosition,
        gDIMSnowHorn1ObjDescriptorMountStateAdapter,
        DIMSnowHorn1_setMountState,
        gDIMSnowHorn1ObjDescriptorPlayerAnimAdapter,
        DIMSnowHorn1_func19,
        gDIMSnowHorn1ObjDescriptorRacePositionAdapter,
        gDIMSnowHorn1ObjDescriptorResetPositionAdapter,
        DIMSnowHorn1_handleRiderScale,
        gDIMSnowHorn1ObjDescriptorLookTargetAdapter,
    },
};
