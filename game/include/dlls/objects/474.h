#ifndef DLLS_OBJECTS_474_H_
#define DLLS_OBJECTS_474_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"

/* dll_1DA_getExtraSize() allocates the complete 0x8-byte state block. */
typedef struct Dll1DAState {
    f32 floorHeight; /* persistent lower clamp, seeded from the initial Y position */
    u8 grounded;     /* set when the latest ground query produces a contact */
    u8 unknown05[3];
} Dll1DAState;

STATIC_ASSERT(offsetof(Dll1DAState, floorHeight) == 0x00);
STATIC_ASSERT(offsetof(Dll1DAState, grounded) == 0x04);
STATIC_ASSERT(offsetof(Dll1DAState, unknown05) == 0x05);
STATIC_ASSERT(sizeof(Dll1DAState) == 0x08);

int dll_1DA_getExtraSize(void);
int dll_1DA_getObjectTypeId(void);
void dll_1DA_free(void);
void dll_1DA_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dll_1DA_hitDetect(GameObject* obj);
void dll_1DA_update(GameObject* obj);
void dll_1DA_init(GameObject* obj);
void dll_1DA_release(void);
void dll_1DA_initialise(void);

extern ObjectDescriptor gDll1DAObjDescriptor;

#endif /* DLLS_OBJECTS_474_H_ */
