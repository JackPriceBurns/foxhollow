#ifndef DLLS_OBJECTS_330_CFPOWERBASE_H_
#define DLLS_OBJECTS_330_CFPOWERBASE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

typedef struct CfPowerBasePlacement {
    ObjPlacement base;
    s8 initialYaw;
    u8 pad19[0x05];
    s16 poweredGameBit;
} CfPowerBasePlacement;

typedef struct CfPowerBaseState {
    s16 poweredGameBit;
    s16 heldCrystalGameBit;
    s8 baseIndex;
    u8 pad05;
} CfPowerBaseState;

typedef enum CfPowerBasePylonMessage {
    CFPOWERBASE_PYLON_MESSAGE_1 = 0x110001,
    CFPOWERBASE_PYLON_MESSAGE_2 = 0x110002,
    CFPOWERBASE_PYLON_MESSAGE_3 = 0x110003,
} CfPowerBasePylonMessage;

typedef enum CfPowerBaseObjectId {
    CFPOWERBASE_OBJECT_ID = 0xDA,
} CfPowerBaseObjectId;

STATIC_ASSERT(offsetof(CfPowerBasePlacement, base) == 0x00);
STATIC_ASSERT(offsetof(CfPowerBasePlacement, initialYaw) == 0x18);
STATIC_ASSERT(offsetof(CfPowerBasePlacement, pad19) == 0x19);
STATIC_ASSERT(offsetof(CfPowerBasePlacement, poweredGameBit) == 0x1E);
STATIC_ASSERT(sizeof(CfPowerBasePlacement) == 0x20);

STATIC_ASSERT(offsetof(CfPowerBaseState, poweredGameBit) == 0x00);
STATIC_ASSERT(offsetof(CfPowerBaseState, heldCrystalGameBit) == 0x02);
STATIC_ASSERT(offsetof(CfPowerBaseState, baseIndex) == 0x04);
STATIC_ASSERT(offsetof(CfPowerBaseState, pad05) == 0x05);
STATIC_ASSERT(sizeof(CfPowerBaseState) == 0x06);

int cfPowerBase_sequenceCallback(GameObject* obj, int unused, ObjSeqState* animUpdate);
int cfPowerBase_getExtraSize(void);
int cfPowerBase_getObjectTypeId(void);
void cfPowerBase_free(void);
void cfPowerBase_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void cfPowerBase_hitDetect(void);
void cfPowerBase_update(GameObject* obj);
void cfPowerBase_init(GameObject* obj, CfPowerBasePlacement* placement);
void cfPowerBase_release(void);
void cfPowerBase_initialise(void);

extern ObjectDescriptor gCFPowerBaseObjDescriptor;

#endif /* DLLS_OBJECTS_330_CFPOWERBASE_H_ */
