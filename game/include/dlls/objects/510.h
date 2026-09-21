#ifndef DLLS_OBJECTS_510_H_
#define DLLS_OBJECTS_510_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

/*
 * Only the placement prefix consumed by this DLL is modeled. The active-target
 * retail placement width has not been established, so no complete size is
 * claimed.
 */
typedef struct PressureSwitchPlacementView {
    ObjPlacement base;
    s8 rotationXHighByte;
    u8 unknown19;
    u8 unknown1A[2];
    s16 triggerGameBit;
    s16 retriggerDelay;
} PressureSwitchPlacementView;

typedef struct PressureSwitchFlags {
    u8 triggerObjectPresent : 1; /* 0x80 on the big-endian target */
    u8 mapBitLatched : 1;        /* 0x40 on the big-endian target */
    u8 unknown : 6;
} PressureSwitchFlags;

typedef struct PressureSwitchState {
    s8 holdTimer;              /* 0x00: frames the switch stays pressed */
    s8 chimeLatch;             /* 0x01 */
    s16 retriggerTimer;        /* 0x02 */
    s16 mapGameBit;            /* 0x04: per-map bit, or -1 */
    PressureSwitchFlags flags; /* 0x06 */
    u8 unknown07;              /* 0x07 */
} PressureSwitchState;

STATIC_ASSERT(offsetof(PressureSwitchPlacementView, base) == 0x00);
STATIC_ASSERT(offsetof(PressureSwitchPlacementView, rotationXHighByte) == 0x18);
STATIC_ASSERT(offsetof(PressureSwitchPlacementView, unknown19) == 0x19);
STATIC_ASSERT(offsetof(PressureSwitchPlacementView, unknown1A) == 0x1A);
STATIC_ASSERT(offsetof(PressureSwitchPlacementView, triggerGameBit) == 0x1C);
STATIC_ASSERT(offsetof(PressureSwitchPlacementView, retriggerDelay) == 0x1E);

STATIC_ASSERT(sizeof(PressureSwitchFlags) == 0x01);
STATIC_ASSERT(offsetof(PressureSwitchState, holdTimer) == 0x00);
STATIC_ASSERT(offsetof(PressureSwitchState, chimeLatch) == 0x01);
STATIC_ASSERT(offsetof(PressureSwitchState, retriggerTimer) == 0x02);
STATIC_ASSERT(offsetof(PressureSwitchState, mapGameBit) == 0x04);
STATIC_ASSERT(offsetof(PressureSwitchState, flags) == 0x06);
STATIC_ASSERT(offsetof(PressureSwitchState, unknown07) == 0x07);
STATIC_ASSERT(sizeof(PressureSwitchState) == 0x08);

int PressureSwitch_SeqFn(GameObject* unusedObj, int unused, ObjSeqState* animUpdate);
int PressureSwitch_getExtraSize(void);
int PressureSwitch_getObjectTypeId(void);
void PressureSwitch_free(void);
void PressureSwitch_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void PressureSwitch_hitDetect(void);
void PressureSwitch_update(GameObject* obj);
void PressureSwitch_init(GameObject* obj, const PressureSwitchPlacementView* placement);
void PressureSwitch_release(void);
void PressureSwitch_initialise(void);

extern ObjectDescriptor gPressureSwitchObjDescriptor;

#endif /* DLLS_OBJECTS_510_H_ */
