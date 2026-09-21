#ifndef DLLS_OBJECTS_354_CFMAGICWALL_H_
#define DLLS_OBJECTS_354_CFMAGICWALL_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct CfMagicWallPlacement {
    ObjPlacement base;
    s8 rotXByte;
    u8 pad19;
    s16 fadeRange;
    u8 pad1C[0x04];
    s16 visibleGameBit;
} CfMagicWallPlacement;

STATIC_ASSERT(offsetof(CfMagicWallPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(CfMagicWallPlacement, rotXByte) == 0x18);
STATIC_ASSERT(offsetof(CfMagicWallPlacement, pad19) == 0x19);
STATIC_ASSERT(offsetof(CfMagicWallPlacement, fadeRange) == 0x1A);
STATIC_ASSERT(offsetof(CfMagicWallPlacement, pad1C) == 0x1C);
STATIC_ASSERT(offsetof(CfMagicWallPlacement, visibleGameBit) == 0x20);

int cfmagicwall_getExtraSize(void);
int cfmagicwall_getObjectTypeId(void);
void cfmagicwall_free(void);
void cfmagicwall_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void cfmagicwall_hitDetect(void);
void cfmagicwall_update(GameObject* obj);
void cfmagicwall_init(GameObject* obj, CfMagicWallPlacement* placement);
void cfmagicwall_release(void);
void cfmagicwall_initialise(void);

extern ObjectDescriptor gCFMagicWallObjDescriptor;

#endif /* DLLS_OBJECTS_354_CFMAGICWALL_H_ */
