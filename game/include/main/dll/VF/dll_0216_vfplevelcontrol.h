#ifndef MAIN_DLL_VF_DLL_0216_VFPLEVELCONTROL_H_
#define MAIN_DLL_VF_DLL_0216_VFPLEVELCONTROL_H_

#include "game/objects/object.h"
#include "types.h"
#include "global.h"
#include "main/gamebit_latch.h"

typedef struct VfpLevelControlLatch
{
    int musicLatch;
    u8 sequenceStep;
    u8 pad05[3];
} VfpLevelControlLatch;

typedef struct VfpLevelControlState
{
    u8 pad00[2];
    s16 unk02[6]; /* 0x02: cleared at init, never read back */
    s16 areaMode; /* 0x0E: 1..2, from setup (defaults to 1) */
    u8 pad10[4];
    VfpLevelControlLatch latch; /* 0x14 */
} VfpLevelControlState;

typedef struct VfpLevelControlSetup
{
    u8 pad00[0x1a];
    s16 areaMode; /* 0x1A */
} VfpLevelControlSetup;

STATIC_ASSERT(offsetof(VfpLevelControlState, unk02) == 0x02);
STATIC_ASSERT(offsetof(VfpLevelControlState, areaMode) == 0x0E);
STATIC_ASSERT(offsetof(VfpLevelControlState, latch) == 0x14);
STATIC_ASSERT(sizeof(VfpLevelControlState) == 0x1c);
STATIC_ASSERT(offsetof(VfpLevelControlLatch, musicLatch) == 0x00);
STATIC_ASSERT(offsetof(VfpLevelControlLatch, sequenceStep) == 0x04);
STATIC_ASSERT(offsetof(VfpLevelControlSetup, areaMode) == 0x1A);

extern int gVfpLevelControlTimer;

void VFP_LevelControl_updateSpellTabletPuzzle(GameObject* obj);
int VFP_LevelControl_getExtraSize(void);
int VFP_LevelControl_getObjectTypeId(void);
void VFP_LevelControl_free(GameObject* obj);
void VFP_LevelControl_render(void);
void VFP_LevelControl_hitDetect(void);
void VFP_LevelControl_update(GameObject* obj);
void VFP_LevelControl_init(GameObject* obj, VfpLevelControlSetup* setup);
void VFP_LevelControl_release(void);
void VFP_LevelControl_initialise(void);

#endif /* MAIN_DLL_VF_DLL_0216_VFPLEVELCONTROL_H_ */
