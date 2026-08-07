#ifndef DLLS_OBJECTS_305_H_
#define DLLS_OBJECTS_305_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct CFDoorLightFlags {
    u8 curEventId : 1;
    u8 done : 1;   /* 0x40: completion game bit granted */
    u8 active : 1; /* 0x20: texture animation running */
    u8 unk1F : 5;
} CFDoorLightFlags;

/* CF_DoorLight_getExtraSize proves the complete 0x18-byte allocation. */
typedef struct CFDoorLightState {
    s32 textureId;          /* 0x00: texture selected for animation */
    u8 frameStep;           /* 0x04: 1/256-frame units per update */
    u8 pad05[3];            /* 0x05 */
    s32 maxFrame;           /* 0x08: 1/256-frame units */
    s32 resetFrame;         /* 0x0C: 1/256-frame units */
    s32 currentFrame;       /* 0x10: 1/256-frame units */
    CFDoorLightFlags flags; /* 0x14 */
    u8 pad15[3];            /* 0x15 */
} CFDoorLightState;

/*
 * The sole active retail EN placement uses this complete fixed-width
 * 0x24-byte record.
 */
typedef struct CFDoorLightPlacement {
    ObjPlacement base;  /* 0x00 */
    s8 resetFrame;      /* 0x18: whole-frame loop target */
    s8 initialRotX;     /* 0x19: shifted left by nine */
    s16 maxFrame;       /* 0x1A: whole-frame final frame */
    s16 frameStep;      /* 0x1C: low byte retained in state */
    s16 doneGameBit;    /* 0x1E: -1 loops instead of completing */
    s16 triggerGameBit; /* 0x20: starts the animation */
    u8 pad22[2];        /* 0x22 */
} CFDoorLightPlacement;

STATIC_ASSERT(sizeof(CFDoorLightFlags) == 0x01);

STATIC_ASSERT(offsetof(CFDoorLightState, textureId) == 0x00);
STATIC_ASSERT(offsetof(CFDoorLightState, frameStep) == 0x04);
STATIC_ASSERT(offsetof(CFDoorLightState, pad05) == 0x05);
STATIC_ASSERT(offsetof(CFDoorLightState, maxFrame) == 0x08);
STATIC_ASSERT(offsetof(CFDoorLightState, resetFrame) == 0x0C);
STATIC_ASSERT(offsetof(CFDoorLightState, currentFrame) == 0x10);
STATIC_ASSERT(offsetof(CFDoorLightState, flags) == 0x14);
STATIC_ASSERT(offsetof(CFDoorLightState, pad15) == 0x15);
STATIC_ASSERT(sizeof(CFDoorLightState) == 0x18);

STATIC_ASSERT(offsetof(CFDoorLightPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(CFDoorLightPlacement, resetFrame) == 0x18);
STATIC_ASSERT(offsetof(CFDoorLightPlacement, initialRotX) == 0x19);
STATIC_ASSERT(offsetof(CFDoorLightPlacement, maxFrame) == 0x1A);
STATIC_ASSERT(offsetof(CFDoorLightPlacement, frameStep) == 0x1C);
STATIC_ASSERT(offsetof(CFDoorLightPlacement, doneGameBit) == 0x1E);
STATIC_ASSERT(offsetof(CFDoorLightPlacement, triggerGameBit) == 0x20);
STATIC_ASSERT(offsetof(CFDoorLightPlacement, pad22) == 0x22);
STATIC_ASSERT(sizeof(CFDoorLightPlacement) == 0x24);

int CF_DoorLight_getExtraSize(void);
int CF_DoorLight_getObjectTypeId(void);
void CF_DoorLight_free(void);
void CF_DoorLight_render(void);
void CF_DoorLight_hitDetect(void);
void CF_DoorLight_update(GameObject* obj);
void CF_DoorLight_init(GameObject* obj, CFDoorLightPlacement* placement);
void CF_DoorLight_release(void);
void CF_DoorLight_initialise(void);

extern ObjectDescriptor gCF_DoorLightObjDescriptor;

#endif /* DLLS_OBJECTS_305_H_ */
