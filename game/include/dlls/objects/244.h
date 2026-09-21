#ifndef DLLS_OBJECTS_244_H_
#define DLLS_OBJECTS_244_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

typedef enum DoorF4GateMode {
    DOORF4_GATE_MODE_PROXIMITY = 0,
    DOORF4_GATE_MODE_DIRECTIONAL = 1,
    DOORF4_GATE_MODE_INTERACTION = 2,
    DOORF4_GATE_MODE_WIDE_PROXIMITY = 3,
    DOORF4_GATE_MODE_EXPLODABLE = 4,
    DOORF4_GATE_MODE_POWERED_INTERACTION = 5,
    DOORF4_GATE_MODE_GAMEBIT = 6
} DoorF4GateMode;

typedef struct DoorF4Placement {
    ObjPlacement base;   /* 0x00 */
    s8 yawByte;          /* 0x18: yaw in 1/256 turns */
    s8 gateMode;         /* 0x19: DoorF4GateMode */
    s16 farSideGameBit;  /* 0x1A */
    s16 toggleMask;      /* 0x1C: low byte near side, high byte far side */
    s16 openGameBit;     /* 0x1E */
    s16 nearSideGameBit; /* 0x20 */
    u8 pad22[2];         /* 0x22 */
} DoorF4Placement;

typedef struct DoorF4State {
    f32 planeNormalX;       /* 0x00 */
    f32 planeNormalZ;       /* 0x04 */
    f32 planeOffset;        /* 0x08 */
    f32 openRange;          /* 0x0C */
    s32 openGameBit;        /* 0x10 */
    s32 requiredGameBit;    /* 0x14 */
    s32 nearSideGameBit;    /* 0x18 */
    u16 openSfxId;          /* 0x1C */
    u16 closeSfxId;         /* 0x1E */
    s8 isOpen;              /* 0x20 */
    u8 sequenceLatch;       /* 0x21 */
    u8 environmentFxActive; /* 0x22 */
    u8 pad23;               /* 0x23 */
} DoorF4State;

STATIC_ASSERT(offsetof(DoorF4Placement, base) == 0x0);
STATIC_ASSERT(offsetof(DoorF4Placement, yawByte) == 0x18);
STATIC_ASSERT(offsetof(DoorF4Placement, gateMode) == 0x19);
STATIC_ASSERT(offsetof(DoorF4Placement, farSideGameBit) == 0x1A);
STATIC_ASSERT(offsetof(DoorF4Placement, toggleMask) == 0x1C);
STATIC_ASSERT(offsetof(DoorF4Placement, openGameBit) == 0x1E);
STATIC_ASSERT(offsetof(DoorF4Placement, nearSideGameBit) == 0x20);
STATIC_ASSERT(offsetof(DoorF4Placement, pad22) == 0x22);
STATIC_ASSERT(sizeof(DoorF4Placement) == 0x24);

STATIC_ASSERT(offsetof(DoorF4State, planeNormalX) == 0x0);
STATIC_ASSERT(offsetof(DoorF4State, planeNormalZ) == 0x4);
STATIC_ASSERT(offsetof(DoorF4State, planeOffset) == 0x8);
STATIC_ASSERT(offsetof(DoorF4State, openRange) == 0xC);
STATIC_ASSERT(offsetof(DoorF4State, openGameBit) == 0x10);
STATIC_ASSERT(offsetof(DoorF4State, requiredGameBit) == 0x14);
STATIC_ASSERT(offsetof(DoorF4State, nearSideGameBit) == 0x18);
STATIC_ASSERT(offsetof(DoorF4State, openSfxId) == 0x1C);
STATIC_ASSERT(offsetof(DoorF4State, closeSfxId) == 0x1E);
STATIC_ASSERT(offsetof(DoorF4State, isOpen) == 0x20);
STATIC_ASSERT(offsetof(DoorF4State, sequenceLatch) == 0x21);
STATIC_ASSERT(offsetof(DoorF4State, environmentFxActive) == 0x22);
STATIC_ASSERT(offsetof(DoorF4State, pad23) == 0x23);
STATIC_ASSERT(sizeof(DoorF4State) == 0x24);

int DoorF4_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate);
int DoorF4_getExtraSize(void);
int DoorF4_getObjectTypeId(void);
void DoorF4_free(GameObject* obj);
void DoorF4_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void DoorF4_hitDetect(void);
void DoorF4_update(GameObject* obj);
void DoorF4_init(GameObject* obj, DoorF4Placement* placement);
void DoorF4_release(void);
void DoorF4_initialise(void);

extern ObjectDescriptor gDoorF4ObjDescriptor;

#endif /* DLLS_OBJECTS_244_H_ */
