#ifndef DLLS_OBJECTS_443_SC_TOTEMBON_H_
#define DLLS_OBJECTS_443_SC_TOTEMBON_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

#define SC_TOTEM_BOND_GAMEBIT_COUNT 8

typedef struct ScTotemBondPlacement {
    ObjPlacement base;
    u8 unknown18[0x24 - 0x18];
} ScTotemBondPlacement;

typedef struct ScTotemBondState {
    s16 cameraYaw;
    s16 cameraPitch;
    s16 cameraRoll;
    u8 unknown06[0x08 - 0x06];
    f32 cameraX;
    f32 cameraY;
    f32 cameraZ;
    f32 cameraDistance;
    f32 spawnTimer;
    f32 completionTimer;
    s32 active;
    s16 ringIndex;
    u8 eventFlags;
    u8 unknown27;
} ScTotemBondState;

STATIC_ASSERT(offsetof(ScTotemBondPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(ScTotemBondPlacement, unknown18) == 0x18);
STATIC_ASSERT(sizeof(ScTotemBondPlacement) == 0x24);

STATIC_ASSERT(offsetof(ScTotemBondState, cameraYaw) == 0x00);
STATIC_ASSERT(offsetof(ScTotemBondState, cameraPitch) == 0x02);
STATIC_ASSERT(offsetof(ScTotemBondState, cameraRoll) == 0x04);
STATIC_ASSERT(offsetof(ScTotemBondState, unknown06) == 0x06);
STATIC_ASSERT(offsetof(ScTotemBondState, cameraX) == 0x08);
STATIC_ASSERT(offsetof(ScTotemBondState, cameraY) == 0x0C);
STATIC_ASSERT(offsetof(ScTotemBondState, cameraZ) == 0x10);
STATIC_ASSERT(offsetof(ScTotemBondState, cameraDistance) == 0x14);
STATIC_ASSERT(offsetof(ScTotemBondState, spawnTimer) == 0x18);
STATIC_ASSERT(offsetof(ScTotemBondState, completionTimer) == 0x1C);
STATIC_ASSERT(offsetof(ScTotemBondState, active) == 0x20);
STATIC_ASSERT(offsetof(ScTotemBondState, ringIndex) == 0x24);
STATIC_ASSERT(offsetof(ScTotemBondState, eventFlags) == 0x26);
STATIC_ASSERT(offsetof(ScTotemBondState, unknown27) == 0x27);
STATIC_ASSERT(sizeof(ScTotemBondState) == 0x28);

void sc_totembond_spawnGameBitOrbs(GameObject* obj, ScTotemBondState* state, f32 radius);
u32 sc_totembond_SeqFn(GameObject* obj, u32 unused, ObjSeqState* animUpdate);
int sc_totembond_getExtraSize(void);
int sc_totembond_getObjectTypeId(void);
void sc_totembond_free(GameObject* obj);
void sc_totembond_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void sc_totembond_hitDetect(void);
void sc_totembond_update(GameObject* obj);
void sc_totembond_init(GameObject* obj, const ScTotemBondPlacement* placement);
void sc_totembond_release(void);
void sc_totembond_initialise(void);
int sc_totembond_insertOrderedGameBit(u16* gameBitIds, u16 newValue);

extern u16 gTotemBondRingGameBits[SC_TOTEM_BOND_GAMEBIT_COUNT];
extern u16 gTotemBondOrbGameBits[SC_TOTEM_BOND_GAMEBIT_COUNT];
extern ObjectDescriptor gSC_totembondObjDescriptor;

#endif /* DLLS_OBJECTS_443_SC_TOTEMBON_H_ */
