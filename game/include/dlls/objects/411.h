#ifndef DLLS_OBJECTS_411_H_
#define DLLS_OBJECTS_411_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

typedef enum Dll19BPhase {
    DLL19B_PHASE_IDLE = 0,
    DLL19B_PHASE_WAIT_EVENT = 1,
    DLL19B_PHASE_COUNTDOWN = 2,
    DLL19B_PHASE_RESOLVE = 3,
    DLL19B_PHASE_COMPLETE = 4,
    DLL19B_PHASE_DONE = 5,
    DLL19B_PHASE_RESET = 6,
} Dll19BPhase;

/*
 * Partial setup view. The callback proves the field at 0x1A, but retail has no
 * OBJECTS.bin definition or romlist placement establishing a complete width.
 */
typedef struct Dll19BPlacement {
    ObjPlacement base;
    u8 unknown18[0x1A - 0x18];
    s16 activationDistancePacked;
} Dll19BPlacement;

typedef struct Dll19BState {
    s16 activationDistance;
    s16 timer;
    s16 brightnessA;
    s16 brightnessAVelocity;
    s16 brightnessB;
    s16 brightnessBVelocity;
    s16 gfxHandle;
    s16 countdown;
    s16 unknown10;
    u8 unlockCount;
    u8 phase;
    u8 pendingEvent;
    u8 unknown15;
    u8 timerDisplayTriggered;
    u8 unknown17;
} Dll19BState;

STATIC_ASSERT(offsetof(Dll19BPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(Dll19BPlacement, unknown18) == 0x18);
STATIC_ASSERT(offsetof(Dll19BPlacement, activationDistancePacked) == 0x1A);

STATIC_ASSERT(sizeof(Dll19BState) == 0x18);
STATIC_ASSERT(offsetof(Dll19BState, activationDistance) == 0x00);
STATIC_ASSERT(offsetof(Dll19BState, timer) == 0x02);
STATIC_ASSERT(offsetof(Dll19BState, brightnessA) == 0x04);
STATIC_ASSERT(offsetof(Dll19BState, brightnessAVelocity) == 0x06);
STATIC_ASSERT(offsetof(Dll19BState, brightnessB) == 0x08);
STATIC_ASSERT(offsetof(Dll19BState, brightnessBVelocity) == 0x0A);
STATIC_ASSERT(offsetof(Dll19BState, gfxHandle) == 0x0C);
STATIC_ASSERT(offsetof(Dll19BState, countdown) == 0x0E);
STATIC_ASSERT(offsetof(Dll19BState, unknown10) == 0x10);
STATIC_ASSERT(offsetof(Dll19BState, unlockCount) == 0x12);
STATIC_ASSERT(offsetof(Dll19BState, phase) == 0x13);
STATIC_ASSERT(offsetof(Dll19BState, pendingEvent) == 0x14);
STATIC_ASSERT(offsetof(Dll19BState, unknown15) == 0x15);
STATIC_ASSERT(offsetof(Dll19BState, timerDisplayTriggered) == 0x16);
STATIC_ASSERT(offsetof(Dll19BState, unknown17) == 0x17);

extern ObjectDescriptor gDll19BObjDescriptor;
extern u32 gDll19BShaderResult;

int dll411_processAnimEvents(GameObject* obj, int unusedArg, ObjSeqState* animUpdate);
int dll411_getExtraSize(void);
int dll411_getObjectTypeId(void);
void dll411_free(GameObject* obj);
void dll411_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dll411_hitDetect(void);
void dll411_update(GameObject* obj);
void dll411_init(GameObject* obj, const Dll19BPlacement* placement);
void dll411_release(void);
void dll411_initialise(void);

#endif /* DLLS_OBJECTS_411_H_ */
