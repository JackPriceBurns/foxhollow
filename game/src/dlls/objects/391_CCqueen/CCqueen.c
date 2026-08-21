#include "dlls/objects/391_CCqueen.h"

#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/dll/dll_002E_moveLib.h"
#include "main/frame_timing.h"
#include "main/gamebit_ids.h"
#include "main/gamebits_api.h"
#include "main/objhits.h"
#include "main/objprint_character_api.h"
#include "main/object_render.h"
#include "main/vecmath_distance_api.h"
#include "sys/objects.h"

enum CcQueenMoveConfig {
    CC_QUEEN_MOVE_POINT_COUNT = 3,
};

typedef struct CcQueenPlacement {
    ObjPlacement base;
    u8 unknown18[2];
    u8 rotXByte;
    u8 unknown1B[5];
} CcQueenPlacement;

STATIC_ASSERT(sizeof(CcQueenPlacement) == 0x20);
STATIC_ASSERT(offsetof(CcQueenPlacement, rotXByte) == 0x1A);

typedef struct CcQueenState {
    MoveLibState moveLib;
    CharacterEyeAnimState eyeAnimState;
    u8 pad64C[8];
} CcQueenState;

STATIC_ASSERT(sizeof(CcQueenState) == 0x654);
STATIC_ASSERT(offsetof(CcQueenState, eyeAnimState) == 0x624);

static int ccQueen_getExtraSize(void) {
    return sizeof(CcQueenState);
}

static void ccQueen_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                           s8 unusedVisible) {
    CcQueenState* state = obj->extra;

    objRenderModelAndHitVolumes(obj, renderArg2, renderArg3, renderArg4, renderArg5, 1.0f);
    dll_2E_setTargetFromPathPoint(obj, &state->moveLib, 0);
}

static void ccQueen_update(GameObject* obj) {
    CcQueenState* state = obj->extra;

    if (mainGetBit(GAMEBIT_CC_QueenApproached) == 0 && mainGetBit(GAMEBIT_CC_GasVentPuzzleComplete) != 0) {
        GameObject* player = Obj_GetPlayerObject();

        if (vec3f_distanceSquared(&obj->anim.worldPos.x, &player->anim.worldPos.x) < 18225.0f) {
            mainSetBits(GAMEBIT_CC_QueenApproached, 1);
        }
    }
    if (mainGetBit(GAMEBIT_ITEM_NWKey_Got2) != 0) {
        obj->anim.flags |= OBJANIM_FLAG_HIDDEN;
        obj->objectFlags |= OBJECT_OBJFLAG_UPDATE_DISABLED;
        ObjHits_DisableObject(obj);
    } else {
        ObjAnim_AdvanceCurrentMove(obj, 0.005f, timeDelta, NULL);
        dll_2E_updateLookAt(obj, &state->moveLib);
        characterDoEyeAnims(obj, &state->eyeAnimState);
    }
}

static void ccQueen_init(GameObject* obj, const CcQueenPlacement* placement) {
    CcQueenState* state = obj->extra;
    s16 eventTable[CC_QUEEN_MOVE_POINT_COUNT] = {0x1E, 0, 0};
    s16 turnTable[CC_QUEEN_MOVE_POINT_COUNT] = {0x19, 0x19, 0x19};

    obj->anim.rotX = (s16)(placement->rotXByte << 8);
    dll_2E_initState(obj, &state->moveLib, 0x71C7, 0x3555, CC_QUEEN_MOVE_POINT_COUNT);
    dll_2E_setReattackDelay(&state->moveLib, 0x258, 0xF0);
    dll_2E_setMoveTables(&state->moveLib, turnTable, eventTable, CC_QUEEN_MOVE_POINT_COUNT);
    state->moveLib.modeBits |= 0x0A;
}

ObjectDescriptor gCCQueenObjDescriptor = {
    .slotCountAndFlags = OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
    .init = (ObjectDescriptorCallback)ccQueen_init,
    .update = (ObjectDescriptorCallback)ccQueen_update,
    .render = (ObjectDescriptorCallback)ccQueen_render,
    .getExtraSize = ccQueen_getExtraSize,
};
