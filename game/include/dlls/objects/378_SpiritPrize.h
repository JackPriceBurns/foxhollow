#ifndef DLLS_OBJECTS_378_SPIRITPRIZE_H_
#define DLLS_OBJECTS_378_SPIRITPRIZE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/model_light.h"
#include "main/objseq.h"

typedef struct SpiritPrizePlacement {
    ObjPlacement base;         /* 0x00 */
    s16 animDataIndex;         /* 0x18: animation-data selector; -1 disables updates */
    s16 sequenceGameBit;       /* 0x1A: copied into ObjSeqState.gameBit */
    u8 unknown1C[0x24 - 0x1C]; /* 0x1C */
    u8 positionDamping;        /* 0x24: posOffsetDecay = 1 / (1 + positionDamping) */
    u8 unknown25[0x28 - 0x25]; /* 0x25 */
} SpiritPrizePlacement;

typedef struct SpiritPrizeState {
    ObjSeqState sequence;         /* 0x000 */
    u8 unknown138[0x140 - 0x138]; /* 0x138 */
    ModelLightStruct* light;      /* 0x140 */
    u8 useDetachedLight;          /* 0x144: selects explicit detached-light rendering */
    u8 unknown145[0x148 - 0x145]; /* 0x145 */
    f32 ambientSfxTimer;          /* 0x148 */
} SpiritPrizeState;

STATIC_ASSERT(sizeof(SpiritPrizePlacement) == 0x28);
STATIC_ASSERT(offsetof(SpiritPrizePlacement, base) == 0x00);
STATIC_ASSERT(offsetof(SpiritPrizePlacement, animDataIndex) == 0x18);
STATIC_ASSERT(offsetof(SpiritPrizePlacement, sequenceGameBit) == 0x1A);
STATIC_ASSERT(offsetof(SpiritPrizePlacement, unknown1C) == 0x1C);
STATIC_ASSERT(offsetof(SpiritPrizePlacement, positionDamping) == 0x24);
STATIC_ASSERT(offsetof(SpiritPrizePlacement, unknown25) == 0x25);

STATIC_ASSERT(sizeof(SpiritPrizeState) == 0x14C);
STATIC_ASSERT(offsetof(SpiritPrizeState, sequence) == 0x000);
STATIC_ASSERT(offsetof(SpiritPrizeState, unknown138) == 0x138);
STATIC_ASSERT(offsetof(SpiritPrizeState, light) == 0x140);
STATIC_ASSERT(offsetof(SpiritPrizeState, useDetachedLight) == 0x144);
STATIC_ASSERT(offsetof(SpiritPrizeState, unknown145) == 0x145);
STATIC_ASSERT(offsetof(SpiritPrizeState, ambientSfxTimer) == 0x148);

extern ObjectDescriptor gSpiritPrizeObjDescriptor;

int spiritPrize_getExtraSize(void);
int spiritPrize_getObjectTypeId(void);
void spiritPrize_free(GameObject* obj);
void spiritPrize_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void spiritPrize_hitDetect(void);
void spiritPrize_update(GameObject* obj);
void spiritPrize_init(GameObject* obj, const SpiritPrizePlacement* placement);
void spiritPrize_release(void);
void spiritPrize_initialise(void);

#endif /* DLLS_OBJECTS_378_SPIRITPRIZE_H_ */
