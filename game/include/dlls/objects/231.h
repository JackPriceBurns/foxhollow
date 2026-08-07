#ifndef DLLS_OBJECTS_231_H_
#define DLLS_OBJECTS_231_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct FlammableVinePlacement {
    ObjPlacement base; /* 0x00 */
    s8 rotXByte;       /* 0x18: rotX in 1/256 turns */
    u8 setupParam;     /* 0x19: copied to state; 1 marks the position dirty */
    s16 scaleParam;    /* 0x1A: drives rootMotionScale */
    s16 unk1C;         /* 0x1C */
    s16 burnedBit;     /* 0x1E: game bit set when burned; -1 = none */
    s16 gateBit;       /* 0x20: game bit gating use; -1 = none */
    u8 pad22[6];       /* 0x22 */
} FlammableVinePlacement;

typedef struct FlammableVineState {
    u8 flags;          /* 0x00: bit0 burning, bit1 consumed */
    u8 setupParam;     /* 0x01: copied from placement+0x19 */
    u8 pad2[2];        /* 0x02 */
    f32 burnTimer;     /* 0x04 */
    u8 pad8[4];        /* 0x08 */
    f32 pulseTimer;    /* 0x0C */
    f32 burnIntensity; /* 0x10 */
} FlammableVineState;

STATIC_ASSERT(offsetof(FlammableVinePlacement, base) == 0x0);
STATIC_ASSERT(offsetof(FlammableVinePlacement, rotXByte) == 0x18);
STATIC_ASSERT(offsetof(FlammableVinePlacement, setupParam) == 0x19);
STATIC_ASSERT(offsetof(FlammableVinePlacement, scaleParam) == 0x1A);
STATIC_ASSERT(offsetof(FlammableVinePlacement, unk1C) == 0x1C);
STATIC_ASSERT(offsetof(FlammableVinePlacement, burnedBit) == 0x1E);
STATIC_ASSERT(offsetof(FlammableVinePlacement, gateBit) == 0x20);
STATIC_ASSERT(offsetof(FlammableVinePlacement, pad22) == 0x22);
STATIC_ASSERT(sizeof(FlammableVinePlacement) == 0x28);

STATIC_ASSERT(offsetof(FlammableVineState, flags) == 0x0);
STATIC_ASSERT(offsetof(FlammableVineState, setupParam) == 0x1);
STATIC_ASSERT(offsetof(FlammableVineState, pad2) == 0x2);
STATIC_ASSERT(offsetof(FlammableVineState, burnTimer) == 0x4);
STATIC_ASSERT(offsetof(FlammableVineState, pad8) == 0x8);
STATIC_ASSERT(offsetof(FlammableVineState, pulseTimer) == 0xC);
STATIC_ASSERT(offsetof(FlammableVineState, burnIntensity) == 0x10);
STATIC_ASSERT(sizeof(FlammableVineState) == 0x14);

int FlammableVine_getExtraSize(void);
int FlammableVine_getObjectTypeId(void);
void FlammableVine_free(GameObject* obj);
void FlammableVine_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void FlammableVine_hitDetect(GameObject* obj);
void FlammableVine_update(GameObject* obj);
void FlammableVine_init(GameObject* obj, FlammableVinePlacement* placement);
void FlammableVine_release(void);
void FlammableVine_initialise(void);

extern ObjectDescriptor gFlammableVineObjDescriptor;

#endif /* DLLS_OBJECTS_231_H_ */
