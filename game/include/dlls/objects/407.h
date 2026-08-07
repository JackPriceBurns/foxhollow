#ifndef DLLS_OBJECTS_407_H_
#define DLLS_OBJECTS_407_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/dll/dll_0069_modgfx.h"

#define DLL197_STATE_SIZE 0x10

/* Only the accessed placement prefix is recovered; the complete retail width is not established. */
typedef struct Dll197Placement {
    ObjPlacement base;
    s8 rotationParam;
    u8 mode;
    s16 scale;
    s16 stage;
    s16 gameBit;
} Dll197Placement;

typedef struct Dll197State {
    s32 gameBit;
    s16 sparkTimer;
    s16 activeTimer;
    s16 hitCooldown;
    u8 visibleToCamera;
    u8 mode;
    u8 active;
    u8 sparkArmed;
    u8 previousActive;
    u8 stage;
} Dll197State;

STATIC_ASSERT(offsetof(Dll197Placement, base) == 0x00);
STATIC_ASSERT(offsetof(Dll197Placement, rotationParam) == 0x18);
STATIC_ASSERT(offsetof(Dll197Placement, mode) == 0x19);
STATIC_ASSERT(offsetof(Dll197Placement, scale) == 0x1A);
STATIC_ASSERT(offsetof(Dll197Placement, stage) == 0x1C);
STATIC_ASSERT(offsetof(Dll197Placement, gameBit) == 0x1E);

STATIC_ASSERT(sizeof(Dll197State) == DLL197_STATE_SIZE);
STATIC_ASSERT(offsetof(Dll197State, gameBit) == 0x00);
STATIC_ASSERT(offsetof(Dll197State, sparkTimer) == 0x04);
STATIC_ASSERT(offsetof(Dll197State, activeTimer) == 0x06);
STATIC_ASSERT(offsetof(Dll197State, hitCooldown) == 0x08);
STATIC_ASSERT(offsetof(Dll197State, visibleToCamera) == 0x0A);
STATIC_ASSERT(offsetof(Dll197State, mode) == 0x0B);
STATIC_ASSERT(offsetof(Dll197State, active) == 0x0C);
STATIC_ASSERT(offsetof(Dll197State, sparkArmed) == 0x0D);
STATIC_ASSERT(offsetof(Dll197State, previousActive) == 0x0E);
STATIC_ASSERT(offsetof(Dll197State, stage) == 0x0F);

extern const Dll69EffectParams gDll197EffectParamTemplate;
extern s8 gDll197PuzzleProgress;
extern ObjectDescriptor gDll197ObjDescriptor;

int dll407_getExtraSize(void);
int dll407_getObjectTypeId(void);
void dll407_free(GameObject* obj);
void dll407_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dll407_hitDetect(void);
void dll407_update(GameObject* objectAddress);
void dll407_init(GameObject* obj, const Dll197Placement* placement);
void dll407_release(void);
void dll407_initialise(void);

#endif /* DLLS_OBJECTS_407_H_ */
