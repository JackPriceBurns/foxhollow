#ifndef DLLS_OBJECTS_383_H_
#define DLLS_OBJECTS_383_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

typedef struct MoonSeedBushPlacement {
    ObjPlacement base;
    s16 growthTriggerGameBit;
    s16 plantedGameBit;
    s16 preemptTriggerId;
    s8 sequenceIndex;
    u8 rotXByte;
    u8 sequenceFlags;
    u8 scaleByte;
    u8 unknown22[0x24 - 0x22];
} MoonSeedBushPlacement;

typedef struct MoonSeedBushState {
    u8 seedState;
    u8 updateFlags;
} MoonSeedBushState;

STATIC_ASSERT(sizeof(MoonSeedBushPlacement) == 0x24);
STATIC_ASSERT(offsetof(MoonSeedBushPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(MoonSeedBushPlacement, growthTriggerGameBit) == 0x18);
STATIC_ASSERT(offsetof(MoonSeedBushPlacement, plantedGameBit) == 0x1A);
STATIC_ASSERT(offsetof(MoonSeedBushPlacement, preemptTriggerId) == 0x1C);
STATIC_ASSERT(offsetof(MoonSeedBushPlacement, sequenceIndex) == 0x1E);
STATIC_ASSERT(offsetof(MoonSeedBushPlacement, rotXByte) == 0x1F);
STATIC_ASSERT(offsetof(MoonSeedBushPlacement, sequenceFlags) == 0x20);
STATIC_ASSERT(offsetof(MoonSeedBushPlacement, scaleByte) == 0x21);
STATIC_ASSERT(offsetof(MoonSeedBushPlacement, unknown22) == 0x22);

STATIC_ASSERT(sizeof(MoonSeedBushState) == 0x02);
STATIC_ASSERT(offsetof(MoonSeedBushState, seedState) == 0x00);
STATIC_ASSERT(offsetof(MoonSeedBushState, updateFlags) == 0x01);

extern ObjectDescriptor gMoonSeedBushObjDescriptor;

int moonSeedBush_processAnimEvents(GameObject* obj, int unusedArg2, ObjSeqState* animUpdate);
int moonSeedBush_getExtraSize(void);
int moonSeedBush_getObjectTypeId(void);
void moonSeedBush_free(void);
void moonSeedBush_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void moonSeedBush_hitDetect(void);
void moonSeedBush_update(GameObject* obj);
void moonSeedBush_init(GameObject* obj, const MoonSeedBushPlacement* placement);
void moonSeedBush_release(void);
void moonSeedBush_initialise(void);

#endif /* DLLS_OBJECTS_383_H_ */
