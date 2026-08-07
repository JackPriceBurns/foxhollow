#ifndef DLLS_OBJECTS_435_SH_BEACON_H_
#define DLLS_OBJECTS_435_SH_BEACON_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef enum ShBeaconMode {
    SH_BEACON_MODE_UNLIT = 0,
    SH_BEACON_MODE_LIT = 1,
    SH_BEACON_MODE_IGNITING = 2,
} ShBeaconMode;

typedef struct ShBeaconPlacement {
    ObjPlacement base;
    s8 rotXByte;
    u8 unknown19[0x1E - 0x19];
    s16 litGameBit;
    s16 igniteGameBit;
    u8 unknown22[0x24 - 0x22];
} ShBeaconPlacement;

typedef struct ShBeaconFlags {
    u8 loopSoundActive : 1;
    u8 : 7;
} ShBeaconFlags;

typedef struct ShBeaconState {
    GameObject* twinkleObject;
    f32 pulseTimer;
    f32 fadeTimer;
    f32 pulseSpawnTimer;
    f32 pulseModeTimer;
    u8 mode;
    ShBeaconFlags flags;
    u8 unknown16[0x18 - 0x16];
} ShBeaconState;

STATIC_ASSERT(offsetof(ShBeaconPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(ShBeaconPlacement, rotXByte) == 0x18);
STATIC_ASSERT(offsetof(ShBeaconPlacement, litGameBit) == 0x1E);
STATIC_ASSERT(offsetof(ShBeaconPlacement, igniteGameBit) == 0x20);
STATIC_ASSERT(offsetof(ShBeaconPlacement, unknown22) == 0x22);
/* All six active EN retail placements are fixed nine-word (0x24-byte) records. */
STATIC_ASSERT(sizeof(ShBeaconPlacement) == 0x24);

STATIC_ASSERT(sizeof(ShBeaconFlags) == 0x01);
STATIC_ASSERT(offsetof(ShBeaconState, twinkleObject) == 0x00);
STATIC_ASSERT(offsetof(ShBeaconState, pulseTimer) == 0x04);
STATIC_ASSERT(offsetof(ShBeaconState, fadeTimer) == 0x08);
STATIC_ASSERT(offsetof(ShBeaconState, pulseSpawnTimer) == 0x0C);
STATIC_ASSERT(offsetof(ShBeaconState, pulseModeTimer) == 0x10);
STATIC_ASSERT(offsetof(ShBeaconState, mode) == 0x14);
STATIC_ASSERT(offsetof(ShBeaconState, flags) == 0x15);
STATIC_ASSERT(offsetof(ShBeaconState, unknown16) == 0x16);
STATIC_ASSERT(sizeof(ShBeaconState) == 0x18);

int sh_beacon_sequenceCallback(GameObject* obj);
int sh_beacon_resetFadeTimerCallback(GameObject* obj);
int sh_beacon_getExtraSize(void);
void sh_beacon_free(GameObject* obj, int keepChild);
void sh_beacon_update(GameObject* obj);
void sh_beacon_init(GameObject* obj, const ShBeaconPlacement* placement);

extern ObjectDescriptor gSH_BeaconObjDescriptor;

#endif /* DLLS_OBJECTS_435_SH_BEACON_H_ */
