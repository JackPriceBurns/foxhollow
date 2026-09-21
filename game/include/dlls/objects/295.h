#ifndef DLLS_OBJECTS_295_H_
#define DLLS_OBJECTS_295_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

/* Only the accessed placement prefix is recovered; the complete retail width is not established. */
typedef struct Dll127Placement {
    ObjPlacement base; /* 0x00 */
    u8 modelBankIndex; /* 0x18 */
    u8 modelScale;     /* 0x19: clamped to 10, then scaled by 1/64 */
    u8 initialYaw;     /* 0x1A: low six bits encode the initial yaw */
} Dll127Placement;

STATIC_ASSERT(offsetof(Dll127Placement, base) == 0x0);
STATIC_ASSERT(offsetof(Dll127Placement, modelBankIndex) == 0x18);
STATIC_ASSERT(offsetof(Dll127Placement, modelScale) == 0x19);
STATIC_ASSERT(offsetof(Dll127Placement, initialYaw) == 0x1A);

int dll_127_getExtraSize(void);
int dll_127_getObjectTypeId(void);
void dll_127_free(void);
void dll_127_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dll_127_hitDetect(void);
void dll_127_update(GameObject* obj);
void dll_127_init(GameObject* obj, Dll127Placement* placement);
void dll_127_release(void);
void dll_127_initialise(void);

extern ObjectDescriptor gDll127ObjDescriptor;

#endif /* DLLS_OBJECTS_295_H_ */
