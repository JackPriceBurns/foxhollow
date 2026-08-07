#ifndef DLLS_OBJECTS_446_H_
#define DLLS_OBJECTS_446_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "global.h"
#include "main/vec_types.h"

#define DIM_LAVA_PROJECTILE_RELAUNCH_VTABLE_OFFSET    0x20
#define DIM_LAVA_PROJECTILE_IS_INACTIVE_VTABLE_OFFSET 0x24
#define DIM_LAVA_PROJECTILE_SEQUENCE_ID               0x18D

typedef struct ModelLightStruct ModelLightStruct;

typedef struct DimLavaProjectileInterfaceVTable {
    void* unknown00[8];
    void (*relaunch)(GameObject* obj, int verticalSpeed, int horizontalSpeed);
    u32 (*isInactive)(GameObject* obj);
} DimLavaProjectileInterfaceVTable;

typedef struct DimLavaProjectilePlacement {
    union {
        ObjPlacement base;
        struct {
            u8 unknown00[0x14];
            s32 targetObjectId;
        };
    };
    s8 launchYaw;
    u8 unknown19;
    s16 verticalSpeed;
    s16 horizontalSpeed;
    u8 unknown1E[0x24 - 0x1E];
} DimLavaProjectilePlacement;

typedef struct DimLavaProjectileState {
    GameObject* target;
    ModelLightStruct* light;
    f32 floorY;
    int targetObjectId;
    u8 statusFlags;
    u8 explosionCooldown;
    u8 unknown12[0x14 - 0x12];
} DimLavaProjectileState;

STATIC_ASSERT(offsetof(DimLavaProjectileInterfaceVTable, relaunch) == DIM_LAVA_PROJECTILE_RELAUNCH_VTABLE_OFFSET);
STATIC_ASSERT(offsetof(DimLavaProjectileInterfaceVTable, isInactive) == DIM_LAVA_PROJECTILE_IS_INACTIVE_VTABLE_OFFSET);
STATIC_ASSERT(sizeof(DimLavaProjectileInterfaceVTable) == 0x28);

STATIC_ASSERT(offsetof(DimLavaProjectilePlacement, base) == 0x00);
STATIC_ASSERT(offsetof(DimLavaProjectilePlacement, targetObjectId) == 0x14);
STATIC_ASSERT(offsetof(DimLavaProjectilePlacement, launchYaw) == 0x18);
STATIC_ASSERT(offsetof(DimLavaProjectilePlacement, unknown19) == 0x19);
STATIC_ASSERT(offsetof(DimLavaProjectilePlacement, verticalSpeed) == 0x1A);
STATIC_ASSERT(offsetof(DimLavaProjectilePlacement, horizontalSpeed) == 0x1C);
STATIC_ASSERT(offsetof(DimLavaProjectilePlacement, unknown1E) == 0x1E);
STATIC_ASSERT(sizeof(DimLavaProjectilePlacement) == 0x24);

STATIC_ASSERT(offsetof(DimLavaProjectileState, target) == 0x00);
STATIC_ASSERT(offsetof(DimLavaProjectileState, light) == 0x04);
STATIC_ASSERT(offsetof(DimLavaProjectileState, floorY) == 0x08);
STATIC_ASSERT(offsetof(DimLavaProjectileState, targetObjectId) == 0x0C);
STATIC_ASSERT(offsetof(DimLavaProjectileState, statusFlags) == 0x10);
STATIC_ASSERT(offsetof(DimLavaProjectileState, explosionCooldown) == 0x11);
STATIC_ASSERT(offsetof(DimLavaProjectileState, unknown12) == 0x12);
STATIC_ASSERT(sizeof(DimLavaProjectileState) == 0x14);

void lavaball1be_relaunch(GameObject* obj, int verticalSpeed, int horizontalSpeed);
u32 lavaball1be_isInactive(GameObject* obj);
int lavaball1be_getExtraSize(GameObject* obj);
int lavaball1be_getObjectTypeId(GameObject* obj);
void lavaball1be_free(GameObject* obj);
void lavaball1be_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5);
void lavaball1be_hitDetect(void);
void lavaball1be_update(GameObject* obj);
void lavaball1be_init(GameObject* obj, DimLavaProjectilePlacement* placement);
void lavaball1be_release(void);
void lavaball1be_initialise(void);

extern const Vec3f gDimLavaDebrisBaseVec;
extern ObjectDescriptor12 gLavaBall1BEObjDescriptor;

#endif /* DLLS_OBJECTS_446_H_ */
