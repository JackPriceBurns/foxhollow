#ifndef DLLS_OBJECTS_385_MMP_TRENCH_F_H_
#define DLLS_OBJECTS_385_MMP_TRENCH_F_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

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
