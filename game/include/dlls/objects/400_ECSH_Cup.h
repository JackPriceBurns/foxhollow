#ifndef DLLS_OBJECTS_400_ECSH_CUP_H_
#define DLLS_OBJECTS_400_ECSH_CUP_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct ECSHCupPlacement {
    ObjPlacement base;
    u8 unknown18[0x1A - 0x18];
    s16 cupIndex;
} ECSHCupPlacement;

typedef struct ECSHCupState {
    f32 startPosX;
    f32 startPosY;
    f32 startPosZ;
    f32 velocityX;
    f32 velocityY;
    f32 velocityZ;
    f32 transitionHeight;
    f32 particleTimer;
    f32 bobTimer;
    s32 currentAnimState;
    s32 cupIndex;
    s16 spinRate;
    s8 bobDirection;
    u8 unknown2F;
} ECSHCupState;

STATIC_ASSERT(offsetof(ECSHCupPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(ECSHCupPlacement, unknown18) == 0x18);
STATIC_ASSERT(offsetof(ECSHCupPlacement, cupIndex) == 0x1A);

STATIC_ASSERT(sizeof(ECSHCupState) == 0x30);
STATIC_ASSERT(offsetof(ECSHCupState, startPosX) == 0x00);
STATIC_ASSERT(offsetof(ECSHCupState, startPosY) == 0x04);
STATIC_ASSERT(offsetof(ECSHCupState, startPosZ) == 0x08);
STATIC_ASSERT(offsetof(ECSHCupState, velocityX) == 0x0C);
STATIC_ASSERT(offsetof(ECSHCupState, velocityY) == 0x10);
STATIC_ASSERT(offsetof(ECSHCupState, velocityZ) == 0x14);
STATIC_ASSERT(offsetof(ECSHCupState, transitionHeight) == 0x18);
STATIC_ASSERT(offsetof(ECSHCupState, particleTimer) == 0x1C);
STATIC_ASSERT(offsetof(ECSHCupState, bobTimer) == 0x20);
STATIC_ASSERT(offsetof(ECSHCupState, currentAnimState) == 0x24);
STATIC_ASSERT(offsetof(ECSHCupState, cupIndex) == 0x28);
STATIC_ASSERT(offsetof(ECSHCupState, spinRate) == 0x2C);
STATIC_ASSERT(offsetof(ECSHCupState, bobDirection) == 0x2E);
STATIC_ASSERT(offsetof(ECSHCupState, unknown2F) == 0x2F);

extern ObjectDescriptor gECSHCupObjDescriptor;

int ecshCup_getExtraSize(void);
int ecshCup_getObjectTypeId(void);
void ecshCup_free(GameObject* obj);
void ecshCup_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void ecshCup_hitDetect(void);
void ecshCup_update(GameObject* obj);
void ecshCup_init(GameObject* obj, const ECSHCupPlacement* placement);
void ecshCup_release(void);
void ecshCup_initialise(void);

#endif /* DLLS_OBJECTS_400_ECSH_CUP_H_ */
