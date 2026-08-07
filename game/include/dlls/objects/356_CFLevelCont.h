#ifndef DLLS_OBJECTS_356_CFLEVELCONT_H_
#define DLLS_OBJECTS_356_CFLEVELCONT_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "dlls/objects/430_SH_LevelCon.h"
#include "main/objseq.h"
#include "main/vec_types.h"

#define CFLEVELCONTROL_RESET_GAME_BIT_COUNT       23
#define CFLEVELCONTROL_RESET_GAME_BIT_TABLE_COUNT 24

typedef struct CfLevelControlFlags {
    u8 unknown80 : 1;
    u8 unknown40 : 1;
    u8 lever974WasSet : 1;
    u8 lever975WasSet : 1;
    u8 runObjectLoadCallbacks : 1;
    u8 unused : 3;
} CfLevelControlFlags;

typedef struct CfLevelControlPlacement {
    ObjPlacement base;
    u8 params18[0x0C];
} CfLevelControlPlacement;

typedef struct CfLevelControlState {
    f32 timer;
    u8 pad04[0x04];
    GameBitLatchState gameBitLatch;
    CfLevelControlFlags flags;
    s8 previousCameraMode;
    u8 pad0E[0x02];
} CfLevelControlState;

typedef struct CfLevelControlRestartPoint {
    Vec3f position;
    f32 unused;
} CfLevelControlRestartPoint;

STATIC_ASSERT(sizeof(CfLevelControlFlags) == 0x01);

STATIC_ASSERT(offsetof(CfLevelControlPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(CfLevelControlPlacement, params18) == 0x18);
STATIC_ASSERT(sizeof(CfLevelControlPlacement) == 0x24);

STATIC_ASSERT(offsetof(CfLevelControlState, timer) == 0x00);
STATIC_ASSERT(offsetof(CfLevelControlState, pad04) == 0x04);
STATIC_ASSERT(offsetof(CfLevelControlState, gameBitLatch) == 0x08);
STATIC_ASSERT(offsetof(CfLevelControlState, flags) == 0x0C);
STATIC_ASSERT(offsetof(CfLevelControlState, previousCameraMode) == 0x0D);
STATIC_ASSERT(offsetof(CfLevelControlState, pad0E) == 0x0E);
STATIC_ASSERT(sizeof(CfLevelControlState) == 0x10);

STATIC_ASSERT(offsetof(CfLevelControlRestartPoint, position) == 0x00);
STATIC_ASSERT(offsetof(CfLevelControlRestartPoint, unused) == 0x0C);
STATIC_ASSERT(sizeof(CfLevelControlRestartPoint) == 0x10);

int cflevelcontrol_sequenceCallback(GameObject* obj, int unused, ObjSeqState* animUpdate);
int cflevelcontrol_getExtraSize(void);
int cflevelcontrol_getObjectTypeId(void);
void cflevelcontrol_free(GameObject* obj);
void cflevelcontrol_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void cflevelcontrol_hitDetect(void);
void cflevelcontrol_update(GameObject* obj);
void cflevelcontrol_init(GameObject* obj, CfLevelControlPlacement* unusedPlacement);
void cflevelcontrol_release(void);
void cflevelcontrol_initialise(void);

extern s16 gCfLevelControlResetGameBits[CFLEVELCONTROL_RESET_GAME_BIT_TABLE_COUNT];
extern const CfLevelControlRestartPoint gCfLevelControlRestartPoint;
extern ObjectDescriptor gCFLevelControlObjDescriptor;

#endif /* DLLS_OBJECTS_356_CFLEVELCONT_H_ */
