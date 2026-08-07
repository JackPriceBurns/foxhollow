#ifndef DLLS_OBJECTS_359_SPIRITDOORL_H_
#define DLLS_OBJECTS_359_SPIRITDOORL_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/modellight_api.h"

typedef struct SpiritDoorLockFlags {
    u8 unknown80 : 1;
    u8 unused : 7;
} SpiritDoorLockFlags;

typedef struct SpiritDoorLockState {
    ModelLightStruct* light;
    s32 spinAngle;
    s32 active;
    s32 orbitCount;
    SpiritDoorLockFlags flags;
    u8 pad11[0x03];
} SpiritDoorLockState;

typedef struct SpiritDoorLockPlacement {
    ObjPlacement base;
    s8 yaw;
    s8 scale;
    s16 orbitCount;
    u8 pad1C[0x02];
    s16 doneGameBit;
    s16 activeGameBit;
    u8 pad22[0x02];
} SpiritDoorLockPlacement;

STATIC_ASSERT(sizeof(SpiritDoorLockFlags) == 0x01);

STATIC_ASSERT(offsetof(SpiritDoorLockState, light) == 0x00);
STATIC_ASSERT(offsetof(SpiritDoorLockState, spinAngle) == 0x04);
STATIC_ASSERT(offsetof(SpiritDoorLockState, active) == 0x08);
STATIC_ASSERT(offsetof(SpiritDoorLockState, orbitCount) == 0x0C);
STATIC_ASSERT(offsetof(SpiritDoorLockState, flags) == 0x10);
STATIC_ASSERT(offsetof(SpiritDoorLockState, pad11) == 0x11);
STATIC_ASSERT(sizeof(SpiritDoorLockState) == 0x14);

STATIC_ASSERT(offsetof(SpiritDoorLockPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(SpiritDoorLockPlacement, yaw) == 0x18);
STATIC_ASSERT(offsetof(SpiritDoorLockPlacement, scale) == 0x19);
STATIC_ASSERT(offsetof(SpiritDoorLockPlacement, orbitCount) == 0x1A);
STATIC_ASSERT(offsetof(SpiritDoorLockPlacement, pad1C) == 0x1C);
STATIC_ASSERT(offsetof(SpiritDoorLockPlacement, doneGameBit) == 0x1E);
STATIC_ASSERT(offsetof(SpiritDoorLockPlacement, activeGameBit) == 0x20);
STATIC_ASSERT(offsetof(SpiritDoorLockPlacement, pad22) == 0x22);
STATIC_ASSERT(sizeof(SpiritDoorLockPlacement) == 0x24);

int SpiritDoorLock_getExtraSize(void);
int SpiritDoorLock_getObjectTypeId(void);
void SpiritDoorLock_free(GameObject* obj);
void SpiritDoorLock_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void SpiritDoorLock_hitDetect(void);
void SpiritDoorLock_update(GameObject* obj);
void SpiritDoorLock_init(GameObject* obj, const SpiritDoorLockPlacement* placement, int startHidden);
void SpiritDoorLock_release(void);
void SpiritDoorLock_initialise(void);

extern s16 gSpiritDoorLockSpinSpeed;
extern s32 gSpiritDoorLockTexScrollSpeed;
extern s32 gSpiritDoorLockTexScrollWrap;
extern const u32 gSpiritDoorLockOrbitOffsetBase[4];
extern ObjectDescriptor gSpiritDoorLockObjDescriptor;

#endif /* DLLS_OBJECTS_359_SPIRITDOORL_H_ */
