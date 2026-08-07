#ifndef DLLS_OBJECTS_253_H_
#define DLLS_OBJECTS_253_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

/* Fields read by this DLL; the complete retail placement width is not yet proven. */
typedef struct DllFDPlacement {
    ObjPlacement base;     /* 0x00 */
    s16 enableGameBit;     /* 0x18: -1 = always enabled */
    s16 stateGameBit;      /* 0x1A: -1 = do not persist activation */
    s16 eventId;           /* 0x1C: -1 = no event-readiness gate */
    s16 preemptSequenceId; /* 0x1E: -1 = no initial preemption */
    u8 sequenceArg;        /* 0x20: initial runSequence argument */
    u8 targetGroup;        /* 0x21: object group to follow */
    u8 sequenceId;         /* 0x22 */
    u8 flags;              /* 0x23: DLL_FD_FLAG_* */
} DllFDPlacement;

typedef struct DllFDState {
    u8 mode;            /* 0x00: DLL_FD_MODE_* */
    u8 isActivated;     /* 0x01 */
    u8 pad02[2];        /* 0x02 */
    GameObject* target; /* 0x04: followed object */
} DllFDState;

#define DLL_FD_MODE_UNINITIALISED        0
#define DLL_FD_MODE_RUN_INITIAL_SEQUENCE 1
#define DLL_FD_MODE_INTERACTIVE          2
#define DLL_FD_MODE_WAIT_ENABLE          3
#define DLL_FD_MODE_FINISHED             4

#define DLL_FD_FLAG_KEEP_INTERACTIVE_WHEN_ACTIVATED 0x1
#define DLL_FD_FLAG_CLEAR_ENABLE_BIT                0x2

STATIC_ASSERT(offsetof(DllFDPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(DllFDPlacement, enableGameBit) == 0x18);
STATIC_ASSERT(offsetof(DllFDPlacement, stateGameBit) == 0x1A);
STATIC_ASSERT(offsetof(DllFDPlacement, eventId) == 0x1C);
STATIC_ASSERT(offsetof(DllFDPlacement, preemptSequenceId) == 0x1E);
STATIC_ASSERT(offsetof(DllFDPlacement, sequenceArg) == 0x20);
STATIC_ASSERT(offsetof(DllFDPlacement, targetGroup) == 0x21);
STATIC_ASSERT(offsetof(DllFDPlacement, sequenceId) == 0x22);
STATIC_ASSERT(offsetof(DllFDPlacement, flags) == 0x23);

STATIC_ASSERT(offsetof(DllFDState, mode) == 0x0);
STATIC_ASSERT(offsetof(DllFDState, isActivated) == 0x1);
STATIC_ASSERT(offsetof(DllFDState, pad02) == 0x2);
STATIC_ASSERT(offsetof(DllFDState, target) == 0x4);
STATIC_ASSERT(sizeof(DllFDState) == 0x8);

int dll_FD_getExtraSize(void);
int dll_FD_getObjectTypeId(void);
void dll_FD_free(void);
void dll_FD_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void dll_FD_hitDetect(GameObject* obj);
void dll_FD_update(GameObject* obj);
void dll_FD_init(GameObject* obj);
void dll_FD_release(void);
void dll_FD_initialise(void);

extern ObjectDescriptor gDllFDObjDescriptor;

#endif /* DLLS_OBJECTS_253_H_ */
