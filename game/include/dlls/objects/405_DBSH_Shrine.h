#ifndef DLLS_OBJECTS_405_DBSH_SHRINE_H_
#define DLLS_OBJECTS_405_DBSH_SHRINE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "dlls/objects/430_SH_LevelCon.h"
#include "main/model_light.h"
#include "main/objseq.h"

enum {
    DBSH_GAMEBIT_SYMBOL_RISE_COMPLETE = 0x16A,
    DBSH_GAMEBIT_SYMBOL_SPIN_SUCCEEDED = 0x16B,
    DBSH_GAMEBIT_SYMBOL_SPIN_FAILED = 0x16C,
};

typedef struct DBSHShrinePlacement {
    ObjPlacement base;
    u8 unknown18[0x0C];
} DBSHShrinePlacement;

typedef struct DBSHShrineFlags {
    u8 riseSequenceReady : 1;
    u8 unknown7F : 7;
} DBSHShrineFlags;

typedef struct DBSHShrineState {
    ModelLightStruct* light;
    GameBitLatchState gameBitLatch;
    f32 idleSfxTimer;
    s16 unknown0C;
    s16 orbitPhaseA;
    s16 orbitPhaseB;
    s16 orbitPhaseC;
    u8 phase;
    DBSHShrineFlags flags;
    u8 unknown16[2];
} DBSHShrineState;

STATIC_ASSERT(sizeof(DBSHShrinePlacement) == 0x24);
STATIC_ASSERT(offsetof(DBSHShrinePlacement, base) == 0x00);
STATIC_ASSERT(offsetof(DBSHShrinePlacement, unknown18) == 0x18);

STATIC_ASSERT(sizeof(DBSHShrineFlags) == 0x01);
STATIC_ASSERT(sizeof(DBSHShrineState) == 0x18);
STATIC_ASSERT(offsetof(DBSHShrineState, light) == 0x00);
STATIC_ASSERT(offsetof(DBSHShrineState, gameBitLatch) == 0x04);
STATIC_ASSERT(offsetof(DBSHShrineState, idleSfxTimer) == 0x08);
STATIC_ASSERT(offsetof(DBSHShrineState, unknown0C) == 0x0C);
STATIC_ASSERT(offsetof(DBSHShrineState, orbitPhaseA) == 0x0E);
STATIC_ASSERT(offsetof(DBSHShrineState, orbitPhaseB) == 0x10);
STATIC_ASSERT(offsetof(DBSHShrineState, orbitPhaseC) == 0x12);
STATIC_ASSERT(offsetof(DBSHShrineState, phase) == 0x14);
STATIC_ASSERT(offsetof(DBSHShrineState, flags) == 0x15);
STATIC_ASSERT(offsetof(DBSHShrineState, unknown16) == 0x16);

extern ObjectDescriptor gDBSHShrineObjDescriptor;

void dbshShrine_updateHoverMotion(GameObject* obj);
int dbshShrine_processAnimEvents(GameObject* obj, int unused, ObjSeqState* animUpdate);
int dbshShrine_getExtraSize(void);
int dbshShrine_getObjectTypeId(void);
void dbshShrine_free(GameObject* obj);
void dbshShrine_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dbshShrine_hitDetect(void);
void dbshShrine_update(GameObject* obj);
void dbshShrine_init(GameObject* obj, const DBSHShrinePlacement* placement);
void dbshShrine_release(void);
void dbshShrine_initialise(void);

#endif /* DLLS_OBJECTS_405_DBSH_SHRINE_H_ */
