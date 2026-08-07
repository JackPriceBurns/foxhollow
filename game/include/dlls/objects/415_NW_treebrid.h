#ifndef DLLS_OBJECTS_415_NW_TREEBRID_H_
#define DLLS_OBJECTS_415_NW_TREEBRID_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

/*
 * Partial placement view. The init callback proves the fields through 0x1E,
 * but the complete retail placement width is not established.
 */
typedef struct NwTreeBridgePlacement {
    ObjPlacement base;
    s8 initialRotX;
    s8 sequenceId;
    s16 initialRotY;
    s16 initialRotZ;
    s16 gameBit;
} NwTreeBridgePlacement;

typedef struct NwTreeBridgeState {
    s16 gameBit;
    s16 sequenceId;
    s16 preemptSequenceId;
    u8 sequenceStarted;
    u8 targetSearchAttempts;
    GameObject* pathTarget;
} NwTreeBridgeState;

STATIC_ASSERT(offsetof(NwTreeBridgePlacement, base) == 0x00);
STATIC_ASSERT(offsetof(NwTreeBridgePlacement, initialRotX) == 0x18);
STATIC_ASSERT(offsetof(NwTreeBridgePlacement, sequenceId) == 0x19);
STATIC_ASSERT(offsetof(NwTreeBridgePlacement, initialRotY) == 0x1A);
STATIC_ASSERT(offsetof(NwTreeBridgePlacement, initialRotZ) == 0x1C);
STATIC_ASSERT(offsetof(NwTreeBridgePlacement, gameBit) == 0x1E);

STATIC_ASSERT(sizeof(NwTreeBridgeState) == 0x0C);
STATIC_ASSERT(offsetof(NwTreeBridgeState, gameBit) == 0x00);
STATIC_ASSERT(offsetof(NwTreeBridgeState, sequenceId) == 0x02);
STATIC_ASSERT(offsetof(NwTreeBridgeState, preemptSequenceId) == 0x04);
STATIC_ASSERT(offsetof(NwTreeBridgeState, sequenceStarted) == 0x06);
STATIC_ASSERT(offsetof(NwTreeBridgeState, targetSearchAttempts) == 0x07);
STATIC_ASSERT(offsetof(NwTreeBridgeState, pathTarget) == 0x08);

extern ObjectDescriptor gNWTreeBridgeObjDescriptor;

int nwTreeBridge_processAnimEvents(GameObject* obj, int unusedArg, ObjSeqState* animUpdate);
int nwTreeBridge_getExtraSize(void);
void nwTreeBridge_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                         s8 unusedVisible);
void nwTreeBridge_update(GameObject* obj);
void nwTreeBridge_init(GameObject* obj, const NwTreeBridgePlacement* placement);

#endif /* DLLS_OBJECTS_415_NW_TREEBRID_H_ */
