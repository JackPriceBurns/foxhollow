#ifndef DLLS_OBJECTS_375_H_
#define DLLS_OBJECTS_375_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct Dll177Placement {
    ObjPlacement base;
    u8 unknown18[0x0A];
    s16 gameBit;
} Dll177Placement;

typedef enum Dll177TextureState {
    DLL_177_TEXTURE_STATE_WAIT_FOR_GAME_BIT = 0,
    DLL_177_TEXTURE_STATE_FADE_IN = 1,
    DLL_177_TEXTURE_STATE_PULSE = 2,
} Dll177TextureState;

typedef struct Dll177State {
    u16 pulsePhase;
    u8 unknown02;
    u8 textureState;
    u8 unknown04[0x02];
} Dll177State;

STATIC_ASSERT(offsetof(Dll177Placement, base) == 0x00);
STATIC_ASSERT(offsetof(Dll177Placement, unknown18) == 0x18);
STATIC_ASSERT(offsetof(Dll177Placement, gameBit) == 0x22);

STATIC_ASSERT(offsetof(Dll177State, pulsePhase) == 0x00);
STATIC_ASSERT(offsetof(Dll177State, unknown02) == 0x02);
STATIC_ASSERT(offsetof(Dll177State, textureState) == 0x03);
STATIC_ASSERT(offsetof(Dll177State, unknown04) == 0x04);
STATIC_ASSERT(sizeof(Dll177State) == 0x06);

int dll_177_updateTextureAnimation(GameObject* obj);
int dll_177_getExtraSize(void);
int dll_177_getObjectTypeId(void);
void dll_177_free(void);
void dll_177_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dll_177_hitDetect(void);
void dll_177_update(void);
void dll_177_init(GameObject* obj, const Dll177Placement* placement);
void dll_177_release(void);
void dll_177_initialise(void);

extern ObjectDescriptor gDll177ObjDescriptor;

#endif /* DLLS_OBJECTS_375_H_ */
