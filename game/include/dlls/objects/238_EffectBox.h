#ifndef DLLS_OBJECTS_238_EFFECTBOX_H_
#define DLLS_OBJECTS_238_EFFECTBOX_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef enum EffectBoxTargetMode {
    EFFECTBOX_TARGET_PLAYER = 0,
    EFFECTBOX_TARGET_TRICKY = 1,
    EFFECTBOX_TARGET_GROUP = 2
} EffectBoxTargetMode;

typedef struct EffectBoxPlacement {
    ObjPlacement base;     /* 0x00 */
    u8 rotYaw;             /* 0x18: yaw in 1/256 turns */
    u8 rotPitch;           /* 0x19: pitch in 1/256 turns */
    u8 extentX;            /* 0x1A */
    u8 extentY;            /* 0x1B */
    u8 extentZ;            /* 0x1C */
    u8 actionArg;          /* 0x1D: action argument */
    u8 pad1E;              /* 0x1E */
    u8 gameBitValue;       /* 0x1F: gate value compared against the game bit */
    s16 gameBitIndex;      /* 0x20: game bit index */
    u8 targetMode;         /* 0x22: EffectBoxTargetMode candidate set */
    u8 pad23[0x28 - 0x23]; /* 0x23 */
} EffectBoxPlacement;

STATIC_ASSERT(offsetof(EffectBoxPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(EffectBoxPlacement, rotYaw) == 0x18);
STATIC_ASSERT(offsetof(EffectBoxPlacement, rotPitch) == 0x19);
STATIC_ASSERT(offsetof(EffectBoxPlacement, extentX) == 0x1A);
STATIC_ASSERT(offsetof(EffectBoxPlacement, extentY) == 0x1B);
STATIC_ASSERT(offsetof(EffectBoxPlacement, extentZ) == 0x1C);
STATIC_ASSERT(offsetof(EffectBoxPlacement, actionArg) == 0x1D);
STATIC_ASSERT(offsetof(EffectBoxPlacement, pad1E) == 0x1E);
STATIC_ASSERT(offsetof(EffectBoxPlacement, gameBitValue) == 0x1F);
STATIC_ASSERT(offsetof(EffectBoxPlacement, gameBitIndex) == 0x20);
STATIC_ASSERT(offsetof(EffectBoxPlacement, targetMode) == 0x22);
STATIC_ASSERT(offsetof(EffectBoxPlacement, pad23) == 0x23);
STATIC_ASSERT(sizeof(EffectBoxPlacement) == 0x28);

int EffectBox_getExtraSize(void);
int EffectBox_getObjectTypeId(void);
void EffectBox_free(GameObject* obj);
void EffectBox_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void EffectBox_hitDetect(GameObject* obj);
void EffectBox_update(GameObject* obj);
void EffectBox_init(GameObject* obj, EffectBoxPlacement* placement);
void EffectBox_release(void);
void EffectBox_initialise(void);

extern ObjectDescriptor gEffectBoxObjDescriptor;

#endif /* DLLS_OBJECTS_238_EFFECTBOX_H_ */
