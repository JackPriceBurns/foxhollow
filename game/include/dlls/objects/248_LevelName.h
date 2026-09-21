#ifndef DLLS_OBJECTS_248_LEVELNAME_H_
#define DLLS_OBJECTS_248_LEVELNAME_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

struct GameTextDef;

typedef struct LevelNamePlacement {
    ObjPlacement base; /* 0x00 */
    s16 enableGameBit; /* 0x18: set when the banner starts; -1 = none */
    u8 pad1A[2];       /* 0x1A */
    s32 textId;        /* 0x1C */
    u8 triggerRadius;  /* 0x20 */
    u8 pad21[3];       /* 0x21 */
} LevelNamePlacement;

typedef struct LevelNameState {
    struct GameTextDef* textDef; /* 0x00 */
    char* text;                  /* 0x04 */
    s32 holdDuration;            /* 0x08 */
    u8 triggerRadius;            /* 0x0C */
    u8 pad0D;                    /* 0x0D */
    s16 enableGameBit;           /* 0x0E */
    s16 elapsedFrames;           /* 0x10 */
    s16 bannerY;                 /* 0x12 */
    u8 phase;                    /* 0x14 */
    u8 pad15[3];                 /* 0x15 */
} LevelNameState;

STATIC_ASSERT(offsetof(LevelNamePlacement, base) == 0x0);
STATIC_ASSERT(offsetof(LevelNamePlacement, enableGameBit) == 0x18);
STATIC_ASSERT(offsetof(LevelNamePlacement, pad1A) == 0x1A);
STATIC_ASSERT(offsetof(LevelNamePlacement, textId) == 0x1C);
STATIC_ASSERT(offsetof(LevelNamePlacement, triggerRadius) == 0x20);
STATIC_ASSERT(offsetof(LevelNamePlacement, pad21) == 0x21);
STATIC_ASSERT(sizeof(LevelNamePlacement) == 0x24);

STATIC_ASSERT(offsetof(LevelNameState, textDef) == 0x0);
STATIC_ASSERT(offsetof(LevelNameState, text) == 0x4);
STATIC_ASSERT(offsetof(LevelNameState, holdDuration) == 0x8);
STATIC_ASSERT(offsetof(LevelNameState, triggerRadius) == 0xC);
STATIC_ASSERT(offsetof(LevelNameState, pad0D) == 0xD);
STATIC_ASSERT(offsetof(LevelNameState, enableGameBit) == 0xE);
STATIC_ASSERT(offsetof(LevelNameState, elapsedFrames) == 0x10);
STATIC_ASSERT(offsetof(LevelNameState, bannerY) == 0x12);
STATIC_ASSERT(offsetof(LevelNameState, phase) == 0x14);
STATIC_ASSERT(offsetof(LevelNameState, pad15) == 0x15);
STATIC_ASSERT(sizeof(LevelNameState) == 0x18);

int LevelName_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate);
int LevelName_getExtraSize(void);
int LevelName_getObjectTypeId(void);
void LevelName_free(void);
void LevelName_render(void);
void LevelName_hitDetect(void);
void LevelName_update(GameObject* obj);
void LevelName_init(GameObject* obj, LevelNamePlacement* placement);
void LevelName_release(void);
void LevelName_initialise(void);

extern ObjectDescriptor gLevelNameObjDescriptor;

#endif /* DLLS_OBJECTS_248_LEVELNAME_H_ */
