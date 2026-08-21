#ifndef MAIN_DLL_DLL_0243_DBHOLECONTROL1_H_
#define MAIN_DLL_DLL_0243_DBHOLECONTROL1_H_

#include "dlls/object_descriptor.h"
#include "types.h"
#include "game/objects/object.h"
#include "main/objseq.h"
#include "game/objects/object_setup.h"

typedef enum DbHoleControlObjectGroup {
    DBHOLE_CONTROL1_OBJECT_GROUP = 0x1E
} DbHoleControlObjectGroup;

extern ObjectDescriptor gDBHoleControl1ObjDescriptor;

typedef struct DbHoleControl1Placement
{
    ObjPlacement base;
    s8 rotXByte;
    s8 triggerSequenceId;
    s16 gameBitA;
    s16 gameBitB;
    s16 hideGameBit;
    s16 triggerGameBit;
    u8 unknown22[2];
    s16 unknown24;
    u8 unknown26[5];
    u8 unknown2B;
    u8 unknown2C[2];
    s8 unknown2E;
    u8 unknown2F;
} DbHoleControl1Placement;

STATIC_ASSERT(offsetof(DbHoleControl1Placement, rotXByte) == 0x18);
STATIC_ASSERT(offsetof(DbHoleControl1Placement, triggerSequenceId) == 0x19);
STATIC_ASSERT(offsetof(DbHoleControl1Placement, gameBitA) == 0x1A);
STATIC_ASSERT(offsetof(DbHoleControl1Placement, hideGameBit) == 0x1E);
STATIC_ASSERT(offsetof(DbHoleControl1Placement, triggerGameBit) == 0x20);
STATIC_ASSERT(sizeof(DbHoleControl1Placement) == 0x30);

int dbholecontrol1_getExtraSize(void);
int dbholecontrol1_getObjectTypeId(void);
void dbholecontrol1_free(GameObject* obj);
void dbholecontrol1_render(GameObject* obj, int p2, int p3, int p4, int p5, s8 visible);
void dbholecontrol1_hitDetect(void);
void dbholecontrol1_update(GameObject* obj);
void dbholecontrol1_init(GameObject* obj, const DbHoleControl1Placement* placement);
int dbholecontrol1_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate);
void dbholecontrol1_release(void);
void dbholecontrol1_initialise(void);

#endif
