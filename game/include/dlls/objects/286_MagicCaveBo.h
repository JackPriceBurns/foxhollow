#ifndef DLLS_OBJECTS_286_MAGICCAVEBO_H_
#define DLLS_OBJECTS_286_MAGICCAVEBO_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define MAGIC_CAVE_BOTTOM_STATE_SIZE 0x1

typedef enum MagicCaveBottomPhase {
    MAGIC_CAVE_BOTTOM_PHASE_SETUP = 0,
    MAGIC_CAVE_BOTTOM_PHASE_START_MUSIC = 1,
    MAGIC_CAVE_BOTTOM_PHASE_IDLE = 2,
    MAGIC_CAVE_BOTTOM_PHASE_WARP = 3,
} MagicCaveBottomPhase;

/* Only the accessed placement prefix is recovered; the complete retail width is not established. */
typedef struct MagicCaveBottomPlacement {
    ObjPlacement base; /* 0x00 */
    u8 pad18[2];       /* 0x18 */
    u8 rotationX;      /* 0x1A: high byte of the initial X rotation */
    u8 sequenceBank;   /* 0x1B: selects the pair of object sequences */
} MagicCaveBottomPlacement;

typedef struct MagicCaveBottomState {
    u8 phase; /* MagicCaveBottomPhase */
} MagicCaveBottomState;

STATIC_ASSERT(offsetof(MagicCaveBottomPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(MagicCaveBottomPlacement, pad18) == 0x18);
STATIC_ASSERT(offsetof(MagicCaveBottomPlacement, rotationX) == 0x1A);
STATIC_ASSERT(offsetof(MagicCaveBottomPlacement, sequenceBank) == 0x1B);

STATIC_ASSERT(sizeof(MagicCaveBottomState) == MAGIC_CAVE_BOTTOM_STATE_SIZE);

int MagicCaveBottom_getExtraSize(void);
void MagicCaveBottom_free(GameObject* obj);
void MagicCaveBottom_update(GameObject* obj);

extern ObjectDescriptor gMagicCaveBottomObjDescriptor;

#endif /* DLLS_OBJECTS_286_MAGICCAVEBO_H_ */
