#ifndef DLLS_OBJECTS_505_WM_OBJCREAT_H_
#define DLLS_OBJECTS_505_WM_OBJCREAT_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct WMObjCreatorState {
    s16 gameBit;     /* Spawn gate; -1 is always enabled. */
    s16 spawnPeriod; /* Base delay between periodic spawns. */
    s16 spawnTimer;  /* Remaining delay before the next spawn. */
    s16 spawnJitter; /* Random delay added to spawnPeriod. */
} WMObjCreatorState;

/*
 * Only the placement prefix consumed by this DLL is modeled. The active-target
 * retail placement width is not yet evidenced.
 */
typedef struct WMObjCreatorPlacementView {
    ObjPlacement base;
    s16 gameBit;     /* Spawn gate; -1 is always enabled. */
    s16 spawnMode;   /* Selects the object/effect spawning behavior. */
    s16 spawnPeriod; /* Base delay between periodic spawns. */
    s8 yaw;          /* Initial object yaw in 1/256 turns. */
    s8 spawnJitter;  /* Random delay added to spawnPeriod. */
} WMObjCreatorPlacementView;

STATIC_ASSERT(offsetof(WMObjCreatorState, gameBit) == 0x00);
STATIC_ASSERT(offsetof(WMObjCreatorState, spawnPeriod) == 0x02);
STATIC_ASSERT(offsetof(WMObjCreatorState, spawnTimer) == 0x04);
STATIC_ASSERT(offsetof(WMObjCreatorState, spawnJitter) == 0x06);
STATIC_ASSERT(sizeof(WMObjCreatorState) == 0x08);

STATIC_ASSERT(offsetof(WMObjCreatorPlacementView, base) == 0x00);
STATIC_ASSERT(offsetof(WMObjCreatorPlacementView, gameBit) == 0x18);
STATIC_ASSERT(offsetof(WMObjCreatorPlacementView, spawnMode) == 0x1A);
STATIC_ASSERT(offsetof(WMObjCreatorPlacementView, spawnPeriod) == 0x1C);
STATIC_ASSERT(offsetof(WMObjCreatorPlacementView, yaw) == 0x1E);
STATIC_ASSERT(offsetof(WMObjCreatorPlacementView, spawnJitter) == 0x1F);

int WM_ObjCreator_getExtraSize(void);
int WM_ObjCreator_getObjectTypeId(void);
void WM_ObjCreator_free(void);
void WM_ObjCreator_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void WM_ObjCreator_hitDetect(void);
void WM_ObjCreator_update(GameObject* obj);
void WM_ObjCreator_init(GameObject* obj, const WMObjCreatorPlacementView* placement);
void WM_ObjCreator_release(void);
void WM_ObjCreator_initialise(void);

extern ObjectDescriptor gWM_ObjCreatorObjDescriptor;

#endif /* DLLS_OBJECTS_505_WM_OBJCREAT_H_ */
