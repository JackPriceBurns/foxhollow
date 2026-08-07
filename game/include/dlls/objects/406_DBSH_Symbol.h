#ifndef DLLS_OBJECTS_406_DBSH_SYMBOL_H_
#define DLLS_OBJECTS_406_DBSH_SYMBOL_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "main/objseq.h"

typedef struct DBSHSymbolFlags {
    u8 spinCompleted : 1;
    u8 sequenceInactive : 1;
    u8 unknown3F : 6;
} DBSHSymbolFlags;

typedef struct DBSHSymbolState {
    GameObject* partnerSymbol;
    f32 spinSpeed;
    f32 objectSfxTimer;
    f32 playerSfxTimer;
    s32 spinProgress;
    s32 previousSpinProgress;
    s32 sequenceHandle;
    u8 unknown1C[2];
    s16 phase;
    DBSHSymbolFlags flags;
    u8 unknown21[3];
} DBSHSymbolState;

STATIC_ASSERT(sizeof(DBSHSymbolFlags) == 0x01);
STATIC_ASSERT(sizeof(DBSHSymbolState) == 0x24);
STATIC_ASSERT(offsetof(DBSHSymbolState, partnerSymbol) == 0x00);
STATIC_ASSERT(offsetof(DBSHSymbolState, spinSpeed) == 0x04);
STATIC_ASSERT(offsetof(DBSHSymbolState, objectSfxTimer) == 0x08);
STATIC_ASSERT(offsetof(DBSHSymbolState, playerSfxTimer) == 0x0C);
STATIC_ASSERT(offsetof(DBSHSymbolState, spinProgress) == 0x10);
STATIC_ASSERT(offsetof(DBSHSymbolState, previousSpinProgress) == 0x14);
STATIC_ASSERT(offsetof(DBSHSymbolState, sequenceHandle) == 0x18);
STATIC_ASSERT(offsetof(DBSHSymbolState, unknown1C) == 0x1C);
STATIC_ASSERT(offsetof(DBSHSymbolState, phase) == 0x1E);
STATIC_ASSERT(offsetof(DBSHSymbolState, flags) == 0x20);
STATIC_ASSERT(offsetof(DBSHSymbolState, unknown21) == 0x21);

extern u8 gDBSHSymbolScuffSfxEnabled;
extern ObjectDescriptor gDBSHSymbolObjDescriptor;

int dbshSymbol_processAnimEvents(GameObject* obj, int unused, ObjSeqState* animUpdate);
int dbshSymbol_getExtraSize(void);
void dbshSymbol_free(void);
void dbshSymbol_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dbshSymbol_update(GameObject* obj);
void dbshSymbol_init(GameObject* obj);

#endif /* DLLS_OBJECTS_406_DBSH_SYMBOL_H_ */
