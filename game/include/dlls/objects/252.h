#ifndef DLLS_OBJECTS_252_H_
#define DLLS_OBJECTS_252_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

/* Fields read by this DLL; the complete retail placement width is not yet proven. */
typedef struct DllFCPlacement {
    ObjPlacement base;     /* 0x00 */
    s16 gateGameBit;       /* 0x18: -1 = no gate */
    s16 rememberedGameBit; /* 0x1A: -1 = do not persist activation */
    u8 targetGroup;        /* 0x1C: object group to follow */
    u8 triggerIdMin;       /* 0x1D */
    u8 triggerIdMax;       /* 0x1E */
    u8 flags;              /* 0x1F: DLL_FC_FLAG_* */
} DllFCPlacement;

typedef struct DllFCState {
    u8 mode;                   /* 0x00: DLL_FC_MODE_* */
    u8 triggerId;              /* 0x01 */
    u8 rememberedGameBitValue; /* 0x02 */
    u8 pad03;                  /* 0x03 */
    GameObject* target;        /* 0x04: followed object */
} DllFCState;

#define DLL_FC_MODE_UNINITIALISED 0
#define DLL_FC_MODE_LATCHED       1
#define DLL_FC_MODE_WAIT_GATE     2
#define DLL_FC_MODE_FINISHED      3

#define DLL_FC_FLAG_REMEMBERED_DONE 0x1
#define DLL_FC_FLAG_CLEAR_GATE_BIT  0x2
#define DLL_FC_FLAG_RANDOM_TRIGGER  0x4

STATIC_ASSERT(offsetof(DllFCPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(DllFCPlacement, gateGameBit) == 0x18);
STATIC_ASSERT(offsetof(DllFCPlacement, rememberedGameBit) == 0x1A);
STATIC_ASSERT(offsetof(DllFCPlacement, targetGroup) == 0x1C);
STATIC_ASSERT(offsetof(DllFCPlacement, triggerIdMin) == 0x1D);
STATIC_ASSERT(offsetof(DllFCPlacement, triggerIdMax) == 0x1E);
STATIC_ASSERT(offsetof(DllFCPlacement, flags) == 0x1F);

STATIC_ASSERT(offsetof(DllFCState, mode) == 0x0);
STATIC_ASSERT(offsetof(DllFCState, triggerId) == 0x1);
STATIC_ASSERT(offsetof(DllFCState, rememberedGameBitValue) == 0x2);
STATIC_ASSERT(offsetof(DllFCState, pad03) == 0x3);
STATIC_ASSERT(offsetof(DllFCState, target) == 0x4);
STATIC_ASSERT(sizeof(DllFCState) == 0x8);

int dll_FC_getExtraSize_ret_8(void);
int dll_FC_getObjectTypeId(void);
void dll_FC_free_nop(void);
void dll_FC_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void dll_FC_hitDetect(GameObject* obj);
void dll_FC_update(GameObject* obj);
void dll_FC_init(GameObject* obj, DllFCPlacement* placement);
void dll_FC_release_nop(void);
void dll_FC_initialise_nop(void);

extern ObjectDescriptor gDllFCObjDescriptor;

#endif /* DLLS_OBJECTS_252_H_ */
