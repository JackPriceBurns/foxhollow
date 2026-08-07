#ifndef DLLS_OBJECTS_475_H_
#define DLLS_OBJECTS_475_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

/*
 * The active-target retail placement files are unavailable. This type models
 * only the prefix read by DLL 0x1DB; it does not claim a complete placement
 * width.
 */
typedef struct Dll1DBPlacementView {
    ObjPlacement base;
    s8 rotationXByte;
    u8 unknown19[5];
    s16 boardedGameBit;
    s16 triggerGameBit;
} Dll1DBPlacementView;

/* dll_1DB_getExtraSize() allocates the complete 0x08-byte state block. */
typedef struct Dll1DBState {
    f32 verticalVelocity;
    u8 motionState;
    u8 boarded;
    u8 contactLost;
    u8 unknown07;
} Dll1DBState;

STATIC_ASSERT(offsetof(Dll1DBPlacementView, base) == 0x00);
STATIC_ASSERT(offsetof(Dll1DBPlacementView, rotationXByte) == 0x18);
STATIC_ASSERT(offsetof(Dll1DBPlacementView, unknown19) == 0x19);
STATIC_ASSERT(offsetof(Dll1DBPlacementView, boardedGameBit) == 0x1E);
STATIC_ASSERT(offsetof(Dll1DBPlacementView, triggerGameBit) == 0x20);

STATIC_ASSERT(offsetof(Dll1DBState, verticalVelocity) == 0x00);
STATIC_ASSERT(offsetof(Dll1DBState, motionState) == 0x04);
STATIC_ASSERT(offsetof(Dll1DBState, boarded) == 0x05);
STATIC_ASSERT(offsetof(Dll1DBState, contactLost) == 0x06);
STATIC_ASSERT(offsetof(Dll1DBState, unknown07) == 0x07);
STATIC_ASSERT(sizeof(Dll1DBState) == 0x08);

int dll_1DB_getExtraSize(void);
int dll_1DB_getObjectTypeId(void);
void dll_1DB_free(void);
void dll_1DB_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dll_1DB_hitDetect(void);
void dll_1DB_update(GameObject* obj);
void dll_1DB_init(GameObject* obj, const Dll1DBPlacementView* placement);
void dll_1DB_release(void);
void dll_1DB_initialise(void);

extern ObjectDescriptor gDll1DBObjDescriptor;

#endif /* DLLS_OBJECTS_475_H_ */
