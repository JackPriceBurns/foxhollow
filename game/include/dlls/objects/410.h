#ifndef DLLS_OBJECTS_410_H_
#define DLLS_OBJECTS_410_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct Dll19APlacement {
    ObjPlacement base;
    u8 unknown18[0x1E - 0x18];
    s8 initialYaw;
    s8 triggerGameBitOffset;
} Dll19APlacement;

typedef struct Dll19AState {
    s16 spawnTimer;
    s16 spawnTimerRate;
} Dll19AState;

STATIC_ASSERT(offsetof(Dll19APlacement, base) == 0x00);
STATIC_ASSERT(offsetof(Dll19APlacement, unknown18) == 0x18);
STATIC_ASSERT(offsetof(Dll19APlacement, initialYaw) == 0x1E);
STATIC_ASSERT(offsetof(Dll19APlacement, triggerGameBitOffset) == 0x1F);

STATIC_ASSERT(sizeof(Dll19AState) == 0x04);
STATIC_ASSERT(offsetof(Dll19AState, spawnTimer) == 0x00);
STATIC_ASSERT(offsetof(Dll19AState, spawnTimerRate) == 0x02);

extern ObjectDescriptor gDll19AObjDescriptor;

int dll410_getExtraSize(void);
int dll410_getObjectTypeId(void);
void dll410_free(void);
void dll410_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dll410_hitDetect(void);
void dll410_update(GameObject* obj);
void dll410_init(GameObject* obj, const Dll19APlacement* placement);
void dll410_release(void);
void dll410_initialise(void);

#endif /* DLLS_OBJECTS_410_H_ */
