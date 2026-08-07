#ifndef DLLS_OBJECTS_385_MMP_TRENCH_F_H_
#define DLLS_OBJECTS_385_MMP_TRENCH_F_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/dll/partfx_interface.h"

typedef struct MMPTrenchFxPlacement {
    ObjPlacement base;
    s8 triggerRadius;
    s8 emitAngleZ;
    s8 emitAngleY;
    s8 emitAngleX;
    u8 extentX;
    u8 extentZ;
    u8 extentY;
    u8 emitType;
    u16 effectId;
    s16 emitCount;
    s16 enableGameBit;
    s16 stopGameBit;
} MMPTrenchFxPlacement;

STATIC_ASSERT(sizeof(MMPTrenchFxPlacement) == 0x28);
STATIC_ASSERT(offsetof(MMPTrenchFxPlacement, triggerRadius) == 0x18);
STATIC_ASSERT(offsetof(MMPTrenchFxPlacement, emitAngleZ) == 0x19);
STATIC_ASSERT(offsetof(MMPTrenchFxPlacement, emitAngleY) == 0x1A);
STATIC_ASSERT(offsetof(MMPTrenchFxPlacement, emitAngleX) == 0x1B);
STATIC_ASSERT(offsetof(MMPTrenchFxPlacement, extentX) == 0x1C);
STATIC_ASSERT(offsetof(MMPTrenchFxPlacement, extentZ) == 0x1D);
STATIC_ASSERT(offsetof(MMPTrenchFxPlacement, extentY) == 0x1E);
STATIC_ASSERT(offsetof(MMPTrenchFxPlacement, emitType) == 0x1F);
STATIC_ASSERT(offsetof(MMPTrenchFxPlacement, effectId) == 0x20);
STATIC_ASSERT(offsetof(MMPTrenchFxPlacement, emitCount) == 0x22);
STATIC_ASSERT(offsetof(MMPTrenchFxPlacement, enableGameBit) == 0x24);
STATIC_ASSERT(offsetof(MMPTrenchFxPlacement, stopGameBit) == 0x26);

typedef struct MMPTrenchFxState {
    s16 enableGameBit;
    u16 extentX;
    u16 extentZ;
    u16 extentY;
    s16 emitAngles[3];
    u8 unknown0E[2];
    PartFxSpawnParams burstSpawnParams;
    f32 burstCooldown;
    f32 burstTimer;
} MMPTrenchFxState;

STATIC_ASSERT(sizeof(MMPTrenchFxState) == 0x30);
STATIC_ASSERT(offsetof(MMPTrenchFxState, enableGameBit) == 0x00);
STATIC_ASSERT(offsetof(MMPTrenchFxState, extentX) == 0x02);
STATIC_ASSERT(offsetof(MMPTrenchFxState, extentZ) == 0x04);
STATIC_ASSERT(offsetof(MMPTrenchFxState, extentY) == 0x06);
STATIC_ASSERT(offsetof(MMPTrenchFxState, emitAngles) == 0x08);
STATIC_ASSERT(offsetof(MMPTrenchFxState, unknown0E) == 0x0E);
STATIC_ASSERT(offsetof(MMPTrenchFxState, burstSpawnParams) == 0x10);
STATIC_ASSERT(offsetof(MMPTrenchFxState, burstCooldown) == 0x28);
STATIC_ASSERT(offsetof(MMPTrenchFxState, burstTimer) == 0x2C);

extern PartFxSpawnParams gMMPTrenchFxAmbientSpawnParams;
extern ObjectDescriptor gMMPTrenchFxObjDescriptor;

int mmpTrenchFx_getExtraSize(void);
int mmpTrenchFx_getObjectTypeId(void);
void mmpTrenchFx_free(GameObject* obj);
void mmpTrenchFx_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void mmpTrenchFx_hitDetect(void);
void mmpTrenchFx_update(GameObject* obj);
void mmpTrenchFx_init(GameObject* obj, const MMPTrenchFxPlacement* placement);
void mmpTrenchFx_release(void);
void mmpTrenchFx_initialise(void);

#endif /* DLLS_OBJECTS_385_MMP_TRENCH_F_H_ */
