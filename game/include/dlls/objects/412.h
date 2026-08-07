#ifndef DLLS_OBJECTS_412_H_
#define DLLS_OBJECTS_412_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

/* Only the accessed placement prefix is recovered; the complete retail width is not established. */
typedef struct Dll19CPlacement {
    ObjPlacement base;
    u8 unknown18[0x1E - 0x18];
    s8 initialYaw;
    s8 disableChildSpawn;
} Dll19CPlacement;

typedef struct Dll19CState {
    s32 unknown0;
    s16 spawnTimer;
    s16 spawnTimerRate;
} Dll19CState;

STATIC_ASSERT(offsetof(Dll19CPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(Dll19CPlacement, unknown18) == 0x18);
STATIC_ASSERT(offsetof(Dll19CPlacement, initialYaw) == 0x1E);
STATIC_ASSERT(offsetof(Dll19CPlacement, disableChildSpawn) == 0x1F);

STATIC_ASSERT(sizeof(Dll19CState) == 0x08);
STATIC_ASSERT(offsetof(Dll19CState, unknown0) == 0x00);
STATIC_ASSERT(offsetof(Dll19CState, spawnTimer) == 0x04);
STATIC_ASSERT(offsetof(Dll19CState, spawnTimerRate) == 0x06);

extern ObjectDescriptor gDll19CObjDescriptor;

int dll412_getExtraSize(void);
int dll412_getObjectTypeId(void);
void dll412_free(void);
void dll412_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dll412_hitDetect(void);
void dll412_update(GameObject* obj);
void dll412_init(GameObject* obj, const Dll19CPlacement* placement);
void dll412_release(void);
void dll412_initialise(void);

#endif /* DLLS_OBJECTS_412_H_ */
