#ifndef DLLS_OBJECTS_483_DIM_BOSSGUT_H_
#define DLLS_OBJECTS_483_DIM_BOSSGUT_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "main/dll/baddie_state.h"
#include "main/model_light.h"

#define DIMBOSSGUT2_STATE_SIZE 0x42C

typedef struct DimBossGut2Control {
    f32 verticalVelocity;
    f32 swayVelocity;
    f32 turnHeightBias;
    f32 surfaceY;
    f32 pathSpeed;
    s16 bobPhase;
    u16 breathFxTimer;
    ModelLightStruct* light;
} DimBossGut2Control;

/*
 * initGroundBaddie points groundBaddie.control at the allocation's 0x410-byte
 * tail. DIM_BossGut2_getExtraSize() proves that this control record occupies
 * the complete remaining 0x1C bytes.
 */
typedef struct DimBossGut2State {
    GroundBaddieState groundBaddie;
    DimBossGut2Control controlStorage;
} DimBossGut2State;

STATIC_ASSERT(offsetof(DimBossGut2State, groundBaddie) == 0x000);
STATIC_ASSERT(offsetof(DimBossGut2State, controlStorage) == 0x410);
STATIC_ASSERT(sizeof(DimBossGut2State) == DIMBOSSGUT2_STATE_SIZE);

STATIC_ASSERT(offsetof(DimBossGut2Control, verticalVelocity) == 0x00);
STATIC_ASSERT(offsetof(DimBossGut2Control, swayVelocity) == 0x04);
STATIC_ASSERT(offsetof(DimBossGut2Control, turnHeightBias) == 0x08);
STATIC_ASSERT(offsetof(DimBossGut2Control, surfaceY) == 0x0C);
STATIC_ASSERT(offsetof(DimBossGut2Control, pathSpeed) == 0x10);
STATIC_ASSERT(offsetof(DimBossGut2Control, bobPhase) == 0x14);
STATIC_ASSERT(offsetof(DimBossGut2Control, breathFxTimer) == 0x16);
STATIC_ASSERT(offsetof(DimBossGut2Control, light) == 0x18);
STATIC_ASSERT(sizeof(DimBossGut2Control) == 0x1C);

void dimbossgut2_updateBobAndSway(GameObject* obj, DimBossGut2State* state);
void dimbossgut2_updateTracking(GameObject* obj, DimBossGut2State* state);
void DIM_BossGut2_func0B(void);
int DIM_BossGut2_func0A(void);
int DIM_BossGut2_getExtraSize(void);
int DIM_BossGut2_getObjectTypeId(void);
void DIM_BossGut2_free(GameObject* obj);
void DIM_BossGut2_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void DIM_BossGut2_hitDetect(void);
void DIM_BossGut2_update(GameObject* obj);
void DIM_BossGut2_init(GameObject* obj, u8* placementAddress, int isAltVariant);
void DIM_BossGut2_release(void);
void DIM_BossGut2_initialise(void);

extern ObjectDescriptor12 gDIM_BossGut2ObjDescriptor;

#endif /* DLLS_OBJECTS_483_DIM_BOSSGUT_H_ */
