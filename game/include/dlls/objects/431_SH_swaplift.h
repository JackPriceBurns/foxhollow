#ifndef DLLS_OBJECTS_431_SH_SWAPLIFT_H_
#define DLLS_OBJECTS_431_SH_SWAPLIFT_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define WARP_STONE_LIFT_STATE_GAMEBIT_COUNT 2

typedef enum WarpStoneLiftStateId {
    WARP_STONE_LIFT_STATE_WAITING_FOR_ROCK_CANDY = 0,
    WARP_STONE_LIFT_STATE_ROCK_CANDY_AVAILABLE = 1,
    WARP_STONE_LIFT_STATE_ROCK_CANDY_USED = 2,
} WarpStoneLiftStateId;

typedef struct WarpStoneLiftPlacement {
    ObjPlacement base;
    s8 rotXByte;
} WarpStoneLiftPlacement;

typedef struct WarpStoneLiftState {
    u8 stateId;
} WarpStoneLiftState;

STATIC_ASSERT(offsetof(WarpStoneLiftPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(WarpStoneLiftPlacement, rotXByte) == 0x18);

STATIC_ASSERT(sizeof(WarpStoneLiftState) == 0x01);
STATIC_ASSERT(offsetof(WarpStoneLiftState, stateId) == 0x00);

extern s32 gWarpStoneLiftStateGameBits[WARP_STONE_LIFT_STATE_GAMEBIT_COUNT];
extern ObjectDescriptor gWarpStoneLiftObjDescriptor;

int warpstonelift_getExtraSize(void);
int warpstonelift_getObjectTypeId(void);
void warpstonelift_free(void);
void warpstonelift_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void warpstonelift_hitDetect(void);
void warpstonelift_update(GameObject* obj);
void warpstonelift_init(GameObject* obj, const WarpStoneLiftPlacement* placement);
void warpstonelift_release(void);
void warpstonelift_initialise(void);

#endif /* DLLS_OBJECTS_431_SH_SWAPLIFT_H_ */
