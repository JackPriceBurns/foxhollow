#ifndef DLLS_OBJECTS_297_CAMPFIRE_H_
#define DLLS_OBJECTS_297_CAMPFIRE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/model_light.h"

typedef enum CampFireStateFlags {
    CAMPFIRE_STATE_FLAG_GAME_BIT_8C_SET = 0x1,
    CAMPFIRE_STATE_FLAG_PLACEMENT_GAME_BIT_SET = 0x4,
} CampFireStateFlags;

/*
 * Retail EN romlists prove a fixed 0x20-byte placement. This TU accesses the
 * parameter tail only through 0x1B.
 */
typedef struct CampFirePlacement {
    ObjPlacement base;         /* 0x00 */
    s16 gameBit;               /* 0x18: -1 when the placement has no game bit */
    u8 scalePercent;           /* 0x1A: 100 produces a scale of 1.0 */
    u8 unk1B;                  /* 0x1B: copied into state but otherwise unused */
    u8 unk1C[4];               /* 0x1C: unused by this TU */
} CampFirePlacement;

/* CampFire_getExtraSize proves the complete 0x14-byte runtime allocation. */
typedef struct CampFireState {
    ModelLightStruct* light;   /* 0x00 */
    f32 dayBurstTimer;         /* 0x04 */
    f32 nightBurstTimer;       /* 0x08 */
    s16 placementGameBit;      /* 0x0C */
    u8 pad0E[2];               /* 0x0E */
    u8 placementParam;         /* 0x10: copied from placement offset 0x1B */
    u8 flags;                  /* 0x11: CampFireStateFlags */
    u8 loopSoundPlaying;       /* 0x12 */
    u8 pad13;                  /* 0x13 */
} CampFireState;

STATIC_ASSERT(offsetof(CampFirePlacement, base) == 0x00);
STATIC_ASSERT(offsetof(CampFirePlacement, gameBit) == 0x18);
STATIC_ASSERT(offsetof(CampFirePlacement, scalePercent) == 0x1A);
STATIC_ASSERT(offsetof(CampFirePlacement, unk1B) == 0x1B);
STATIC_ASSERT(offsetof(CampFirePlacement, unk1C) == 0x1C);
STATIC_ASSERT(sizeof(CampFirePlacement) == 0x20);

STATIC_ASSERT(offsetof(CampFireState, light) == 0x00);
STATIC_ASSERT(offsetof(CampFireState, dayBurstTimer) == 0x04);
STATIC_ASSERT(offsetof(CampFireState, nightBurstTimer) == 0x08);
STATIC_ASSERT(offsetof(CampFireState, placementGameBit) == 0x0C);
STATIC_ASSERT(offsetof(CampFireState, pad0E) == 0x0E);
STATIC_ASSERT(offsetof(CampFireState, placementParam) == 0x10);
STATIC_ASSERT(offsetof(CampFireState, flags) == 0x11);
STATIC_ASSERT(offsetof(CampFireState, loopSoundPlaying) == 0x12);
STATIC_ASSERT(offsetof(CampFireState, pad13) == 0x13);
STATIC_ASSERT(sizeof(CampFireState) == 0x14);

int CampFire_getExtraSize(void);
int CampFire_getObjectTypeId(void);
void CampFire_free(GameObject* obj);
void CampFire_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void CampFire_update(GameObject* obj);
void CampFire_init(GameObject* obj, CampFirePlacement* placement);

extern ObjectDescriptor gCampFireObjDescriptor;

#endif /* DLLS_OBJECTS_297_CAMPFIRE_H_ */
