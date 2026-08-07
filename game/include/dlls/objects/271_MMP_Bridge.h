#ifndef DLLS_OBJECTS_271_MMP_BRIDGE_H_
#define DLLS_OBJECTS_271_MMP_BRIDGE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

/* Only the accessed placement prefix is recovered; the complete retail width is not established. */
typedef struct MMPBridgePlacement {
    ObjPlacement base; /* 0x00 */
    s8 rotXByte;       /* 0x18: X rotation in 1/256 turns */
    u8 pad19[5];       /* 0x19 */
    s16 enableGameBit; /* 0x1E */
} MMPBridgePlacement;

STATIC_ASSERT(offsetof(MMPBridgePlacement, base) == 0x0);
STATIC_ASSERT(offsetof(MMPBridgePlacement, rotXByte) == 0x18);
STATIC_ASSERT(offsetof(MMPBridgePlacement, pad19) == 0x19);
STATIC_ASSERT(offsetof(MMPBridgePlacement, enableGameBit) == 0x1E);

int MMP_Bridge_getExtraSize(void);
int MMP_Bridge_getObjectTypeId(void);
void MMP_Bridge_free(GameObject* obj);
void MMP_Bridge_render(void);
void MMP_Bridge_hitDetect(void);
void MMP_Bridge_update(GameObject* obj);
void MMP_Bridge_init(GameObject* obj);
void MMP_Bridge_release(void);
void MMP_Bridge_initialise(void);

extern ObjectDescriptor gMMP_BridgeObjDescriptor;

#endif /* DLLS_OBJECTS_271_MMP_BRIDGE_H_ */
