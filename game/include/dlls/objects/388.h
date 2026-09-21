#ifndef DLLS_OBJECTS_388_H_
#define DLLS_OBJECTS_388_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

typedef struct Dll184Placement {
    ObjPlacement base;
    s16 animDataIndex;
    s16 sequenceGameBit;
    u8 unknown1C[0x24 - 0x1C];
    u8 positionDamping;
} Dll184Placement;

typedef struct Dll184State {
    ObjSeqState sequence;
    u8 unknown138[0x140 - 0x138];
} Dll184State;

typedef struct Dll184ChildSetup {
    ObjPlacement base;
    u8 unknown18[0x20 - 0x18];
} Dll184ChildSetup;

STATIC_ASSERT(offsetof(Dll184Placement, base) == 0x00);
STATIC_ASSERT(offsetof(Dll184Placement, animDataIndex) == 0x18);
STATIC_ASSERT(offsetof(Dll184Placement, sequenceGameBit) == 0x1A);
STATIC_ASSERT(offsetof(Dll184Placement, unknown1C) == 0x1C);
STATIC_ASSERT(offsetof(Dll184Placement, positionDamping) == 0x24);

STATIC_ASSERT(sizeof(Dll184State) == 0x140);
STATIC_ASSERT(offsetof(Dll184State, sequence) == 0x000);
STATIC_ASSERT(offsetof(Dll184State, unknown138) == 0x138);

STATIC_ASSERT(sizeof(Dll184ChildSetup) == 0x20);
STATIC_ASSERT(offsetof(Dll184ChildSetup, base) == 0x00);
STATIC_ASSERT(offsetof(Dll184ChildSetup, unknown18) == 0x18);

int dll_184_handleAnimEvents(GameObject* obj, const ObjSeqState* sequence);
int dll_184_getExtraSize(void);
int dll_184_getObjectTypeId(void);
void dll_184_free(GameObject* obj);
void dll_184_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dll_184_hitDetect(void);
void dll_184_update(GameObject* obj);
void dll_184_init(GameObject* obj, const Dll184Placement* placement);
void dll_184_release(void);
void dll_184_initialise(void);

extern ObjectDescriptor gDll184ObjDescriptor;

#endif /* DLLS_OBJECTS_388_H_ */
