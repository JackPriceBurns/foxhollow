#ifndef MAIN_DFPPOWERSL_H_
#define MAIN_DFPPOWERSL_H_

#include "global.h"
#include "types.h"
#include "main/objseq.h"
#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "dlls/object_descriptor.h"

#define DFPPOWERSL_OBJECT_DEF_ID 0x0344
#define DFPPOWERSL_DLL_ID 0x023C
#define DFPPOWERSL_CLASS_ID 0x0030

#define DFPPOWERSL_DEFAULT_PARAM_OBJECT_ID 1
#define DFPPOWERSL_MODE_WORD_SHIFT 8
#define DFPPOWERSL_SPAWN_MODE_PRELOAD 4
#define DFPPOWERSL_SPAWN_MODE_ACTIVE 1
#define DFPPOWERSL_HIT_VOLUME_SLOT 0x13
#define DFPPOWERSL_HIT_VOLUME_ENABLED 1
#define DFPPOWERSL_SPAWN_OBJECT_ID 0x39e
#define DFPPOWERSL_SPAWN_COUNT 0x14

typedef struct DfpPowerSlState {
  s32 activateObjectId;
  s32 spawnObjectId;
  s32 eventId;
} DfpPowerSlState;

typedef struct DfpPowerSlMapData {
  ObjPlacement base;
  s8 mode;
  u8 pad19;
  s16 activateObjectId;
  s16 spawnObjectId;
  u8 pad1E[0x20 - 0x1E];
  s16 eventId;
} DfpPowerSlMapData;

STATIC_ASSERT(sizeof(DfpPowerSlState) == 0x0C);
STATIC_ASSERT(offsetof(DfpPowerSlState, activateObjectId) == 0x00);
STATIC_ASSERT(offsetof(DfpPowerSlState, spawnObjectId) == 0x04);
STATIC_ASSERT(offsetof(DfpPowerSlState, eventId) == 0x08);

STATIC_ASSERT(offsetof(DfpPowerSlMapData, mode) == 0x18);
STATIC_ASSERT(offsetof(DfpPowerSlMapData, activateObjectId) == 0x1A);
STATIC_ASSERT(offsetof(DfpPowerSlMapData, spawnObjectId) == 0x1C);
STATIC_ASSERT(offsetof(DfpPowerSlMapData, eventId) == 0x20);
STATIC_ASSERT(sizeof(DfpPowerSlMapData) == 0x24);

extern ObjectDescriptor gDfppowerslObjDescriptor;

int dfppowersl_getExtraSize(void);
int dfppowersl_spawnSeqObjectsOnHit(GameObject *obj);
void dfppowersl_free(GameObject *obj);
void dfppowersl_render(GameObject *obj);
void dfppowersl_update(GameObject *obj);
void dfppowersl_init(GameObject *obj,DfpPowerSlMapData *mapData);

#endif /* MAIN_DFPPOWERSL_H_ */
