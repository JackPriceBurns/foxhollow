#ifndef DLLS_OBJECTS_390_CCGASVENTCO_H_
#define DLLS_OBJECTS_390_CCGASVENTCO_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define CC_GAS_VENT_CONTROL_PUZZLE_COMPLETE_GAMEBIT 0x0A3

typedef struct CCGasVentControlPlacement {
    ObjPlacement base;
    u8 unknown18[0x1A - 0x18];
    u8 rotXByte;
    u8 unknown1B[0x20 - 0x1B];
} CCGasVentControlPlacement;

STATIC_ASSERT(sizeof(CCGasVentControlPlacement) == 0x20);
STATIC_ASSERT(offsetof(CCGasVentControlPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(CCGasVentControlPlacement, unknown18) == 0x18);
STATIC_ASSERT(offsetof(CCGasVentControlPlacement, rotXByte) == 0x1A);
STATIC_ASSERT(offsetof(CCGasVentControlPlacement, unknown1B) == 0x1B);

typedef struct CCGasVentControlState {
    u8 phase;
    u8 loopedSoundActive;
    u8 unknown02[0x04 - 0x02];
    f32 airRemaining;
    f32 gasHeightOffset;
    u8 previousUnblockedVentCount;
    u8 unknown0D[0x10 - 0x0D];
} CCGasVentControlState;

STATIC_ASSERT(sizeof(CCGasVentControlState) == 0x10);
STATIC_ASSERT(offsetof(CCGasVentControlState, phase) == 0x00);
STATIC_ASSERT(offsetof(CCGasVentControlState, loopedSoundActive) == 0x01);
STATIC_ASSERT(offsetof(CCGasVentControlState, unknown02) == 0x02);
STATIC_ASSERT(offsetof(CCGasVentControlState, airRemaining) == 0x04);
STATIC_ASSERT(offsetof(CCGasVentControlState, gasHeightOffset) == 0x08);
STATIC_ASSERT(offsetof(CCGasVentControlState, previousUnblockedVentCount) == 0x0C);
STATIC_ASSERT(offsetof(CCGasVentControlState, unknown0D) == 0x0D);

int ccGasVentControl_sequenceCallback(GameObject* obj);
u8 ccGasVentControl_countUnblockedVents(GameObject* obj, CCGasVentControlState* state);
int ccGasVentControl_getExtraSize(void);
void ccGasVentControl_free(GameObject* obj);
void ccGasVentControl_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                             s8 visible);
void ccGasVentControl_update(GameObject* obj);
void ccGasVentControl_init(GameObject* obj, const CCGasVentControlPlacement* placement);

extern ObjectDescriptor gCCGasVentControlObjDescriptor;

#endif /* DLLS_OBJECTS_390_CCGASVENTCO_H_ */
