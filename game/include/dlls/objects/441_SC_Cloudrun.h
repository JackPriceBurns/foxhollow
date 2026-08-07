#ifndef DLLS_OBJECTS_441_SC_CLOUDRUN_H_
#define DLLS_OBJECTS_441_SC_CLOUDRUN_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

typedef struct ScCloudrunnerAPlacement {
    ObjPlacement base;
    s16 animDataIndex;
    s16 sequenceGameBit;
    u8 unknown1C[0x24 - 0x1C];
    u8 positionDamping;
} ScCloudrunnerAPlacement;

typedef struct ScCloudrunnerAState {
    ObjSeqState sequence;
    u8 unknown138[0x140 - 0x138];
} ScCloudrunnerAState;

STATIC_ASSERT(offsetof(ScCloudrunnerAPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(ScCloudrunnerAPlacement, animDataIndex) == 0x18);
STATIC_ASSERT(offsetof(ScCloudrunnerAPlacement, sequenceGameBit) == 0x1A);
STATIC_ASSERT(offsetof(ScCloudrunnerAPlacement, unknown1C) == 0x1C);
STATIC_ASSERT(offsetof(ScCloudrunnerAPlacement, positionDamping) == 0x24);

STATIC_ASSERT(sizeof(ScCloudrunnerAState) == 0x140);
STATIC_ASSERT(offsetof(ScCloudrunnerAState, sequence) == 0x000);
STATIC_ASSERT(offsetof(ScCloudrunnerAState, unknown138) == 0x138);

int sc_cloudrunnera_getExtraSize(void);
int sc_cloudrunnera_getObjectTypeId(void);
void sc_cloudrunnera_free(GameObject* obj);
void sc_cloudrunnera_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                            s8 visible);
void sc_cloudrunnera_hitDetect(void);
void sc_cloudrunnera_update(GameObject* obj);
void sc_cloudrunnera_init(GameObject* obj, const ScCloudrunnerAPlacement* placement);
void sc_cloudrunnera_release(void);
void sc_cloudrunnera_initialise(void);

extern ObjectDescriptor gSC_CloudrunnerAObjDescriptor;

#endif /* DLLS_OBJECTS_441_SC_CLOUDRUN_H_ */
