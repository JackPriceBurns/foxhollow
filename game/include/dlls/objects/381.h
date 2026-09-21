#ifndef DLLS_OBJECTS_381_H_
#define DLLS_OBJECTS_381_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/dll/curve_walker.h"

typedef struct RollingBarrelPlacement {
    ObjPlacement base;
    u8 unknown18[0x1A - 0x18];
    s16 initialVerticalSpeed;
    s16 curveSpeed;
} RollingBarrelPlacement;

typedef struct RollingBarrelState {
    RomCurveWalker curve;
    f32 curveSpeed;
    f32 verticalSpeed;
    f32 timer;
    u8 mode;
    u8 pitchRising;
    u8 hitVolumeSlot;
    u8 unknown117;
} RollingBarrelState;

STATIC_ASSERT(offsetof(RollingBarrelPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(RollingBarrelPlacement, unknown18) == 0x18);
STATIC_ASSERT(offsetof(RollingBarrelPlacement, initialVerticalSpeed) == 0x1A);
STATIC_ASSERT(offsetof(RollingBarrelPlacement, curveSpeed) == 0x1C);

STATIC_ASSERT(sizeof(RollingBarrelState) == 0x118);
STATIC_ASSERT(offsetof(RollingBarrelState, curve) == 0x000);
STATIC_ASSERT(offsetof(RollingBarrelState, curveSpeed) == 0x108);
STATIC_ASSERT(offsetof(RollingBarrelState, verticalSpeed) == 0x10C);
STATIC_ASSERT(offsetof(RollingBarrelState, timer) == 0x110);
STATIC_ASSERT(offsetof(RollingBarrelState, mode) == 0x114);
STATIC_ASSERT(offsetof(RollingBarrelState, pitchRising) == 0x115);
STATIC_ASSERT(offsetof(RollingBarrelState, hitVolumeSlot) == 0x116);
STATIC_ASSERT(offsetof(RollingBarrelState, unknown117) == 0x117);

extern ObjectDescriptor gRollingBarrelObjDescriptor;

void rollingBarrel_explode(GameObject* obj, int unusedExplosionVariant);
int rollingBarrel_getExtraSize(void);
int rollingBarrel_getObjectTypeId(void);
void rollingBarrel_free(GameObject* obj);
void rollingBarrel_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void rollingBarrel_hitDetect(void);
void rollingBarrel_update(GameObject* obj);
void rollingBarrel_init(GameObject* obj, RollingBarrelPlacement* placement);
void rollingBarrel_release(void);
void rollingBarrel_initialise(void);

#endif /* DLLS_OBJECTS_381_H_ */
