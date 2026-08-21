#ifndef DLLS_OBJECTS_437_H_
#define DLLS_OBJECTS_437_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_setup.h"

enum LightfootObjectId {
    LIGHTFOOT_OBJECT_NPC = 0x27B,
    LIGHTFOOT_OBJECT_BABY = 0x27C,
};

typedef struct LightfootPlacement {
    ObjPlacement base;
    s16 presenceGameBit;
    s16 completionGameBit;
    s16 eventGameBit;
    s16 soundIdB;
    s16 soundIdA;
    s16 triggerId;
    s16 unknown24;
    u8 unknown26;
    u8 initialWeaponId;
    s8 objectFlags;
    u8 aggroRange;
    s8 rotation;
    u8 configFlags;
    s16 respawnDelay;
    s8 sequenceId;
    u8 aggression;
    s16 activeGameBit;
    u8 hitPoints;
    u8 unknown33;
} LightfootPlacement;

STATIC_ASSERT(sizeof(LightfootPlacement) == 0x34);
STATIC_ASSERT(offsetof(LightfootPlacement, presenceGameBit) == 0x18);
STATIC_ASSERT(offsetof(LightfootPlacement, completionGameBit) == 0x1A);
STATIC_ASSERT(offsetof(LightfootPlacement, eventGameBit) == 0x1C);
STATIC_ASSERT(offsetof(LightfootPlacement, initialWeaponId) == 0x27);
STATIC_ASSERT(offsetof(LightfootPlacement, objectFlags) == 0x28);
STATIC_ASSERT(offsetof(LightfootPlacement, rotation) == 0x2A);
STATIC_ASSERT(offsetof(LightfootPlacement, respawnDelay) == 0x2C);
STATIC_ASSERT(offsetof(LightfootPlacement, activeGameBit) == 0x30);
STATIC_ASSERT(offsetof(LightfootPlacement, hitPoints) == 0x32);

extern ObjectDescriptor gLightfootObjDescriptor;

#endif
