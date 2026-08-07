#ifndef DLLS_OBJECTS_397_MMSH_SCALES_H_
#define DLLS_OBJECTS_397_MMSH_SCALES_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

typedef struct MMSHScalesPlacement {
    ObjPlacement base;
    s16 animDataIndex;
    s16 sequenceGameBit;
    u8 unknown1C[0x24 - 0x1C];
    u8 positionDamping;
} MMSHScalesPlacement;

typedef struct MMSHScalesState {
    ObjSeqState sequence;
    u8 unknown138[0x140 - 0x138];
} MMSHScalesState;

typedef struct MMSHScalesChildSetup {
    ObjPlacement base;
    u8 unknown18[0x24 - 0x18];
} MMSHScalesChildSetup;

STATIC_ASSERT(offsetof(MMSHScalesPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(MMSHScalesPlacement, animDataIndex) == 0x18);
STATIC_ASSERT(offsetof(MMSHScalesPlacement, sequenceGameBit) == 0x1A);
STATIC_ASSERT(offsetof(MMSHScalesPlacement, unknown1C) == 0x1C);
STATIC_ASSERT(offsetof(MMSHScalesPlacement, positionDamping) == 0x24);

STATIC_ASSERT(sizeof(MMSHScalesState) == 0x140);
STATIC_ASSERT(offsetof(MMSHScalesState, sequence) == 0x000);
STATIC_ASSERT(offsetof(MMSHScalesState, unknown138) == 0x138);

STATIC_ASSERT(sizeof(MMSHScalesChildSetup) == 0x24);
STATIC_ASSERT(offsetof(MMSHScalesChildSetup, base) == 0x00);
STATIC_ASSERT(offsetof(MMSHScalesChildSetup, unknown18) == 0x18);

extern ObjectDescriptor gMMSHScalesObjDescriptor;

int mmshScales_getExtraSize(void);
int mmshScales_getObjectTypeId(void);
void mmshScales_free(GameObject* obj, int keepChild);
void mmshScales_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void mmshScales_hitDetect(void);
void mmshScales_update(GameObject* obj);
void mmshScales_init(GameObject* obj, const MMSHScalesPlacement* placement);
void mmshScales_release(void);
void mmshScales_initialise(void);

#endif /* DLLS_OBJECTS_397_MMSH_SCALES_H_ */
