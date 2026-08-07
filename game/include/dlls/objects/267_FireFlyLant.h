#ifndef DLLS_OBJECTS_267_FIREFLYLANT_H_
#define DLLS_OBJECTS_267_FIREFLYLANT_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

#define FIREFLY_LANTERN_CHILD_CAPACITY 6
#define FIREFLY_LANTERN_STATE_SIZE     0x24

typedef enum FireFlyLanternMode {
    FIREFLY_LANTERN_MODE_SINGLE = 1,
} FireFlyLanternMode;

/* Only the accessed placement prefix is recovered; the complete retail width is not established. */
typedef struct FireFlyLanternPlacement {
    ObjPlacement base; /* 0x00 */
    u8 pad18;          /* 0x18 */
    s8 mode;           /* 0x19: FireFlyLanternMode */
} FireFlyLanternPlacement;

typedef struct FireFlyLanternStateFlags {
    u8 sequenceFinished : 1;
    u8 unused : 7;
} FireFlyLanternStateFlags;

typedef struct FireFlyLanternState {
    GameObject* fireflies[FIREFLY_LANTERN_CHILD_CAPACITY]; /* 0x00 */
    u8 pad18[4];                                           /* 0x18 */
    u8 fireflyCount;                                       /* 0x1C */
    u8 remainingCount;                                     /* 0x1D */
    FireFlyLanternStateFlags flags;                        /* 0x1E */
    u8 pad1F;                                              /* 0x1F */
    s16 countGameBit;                                      /* 0x20 */
    u8 pad22[2];                                           /* 0x22 */
} FireFlyLanternState;

STATIC_ASSERT(offsetof(FireFlyLanternPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(FireFlyLanternPlacement, pad18) == 0x18);
STATIC_ASSERT(offsetof(FireFlyLanternPlacement, mode) == 0x19);

STATIC_ASSERT(sizeof(FireFlyLanternStateFlags) == 0x1);
STATIC_ASSERT(offsetof(FireFlyLanternState, fireflies) == 0x0);
STATIC_ASSERT(offsetof(FireFlyLanternState, pad18) == 0x18);
STATIC_ASSERT(offsetof(FireFlyLanternState, fireflyCount) == 0x1C);
STATIC_ASSERT(offsetof(FireFlyLanternState, remainingCount) == 0x1D);
STATIC_ASSERT(offsetof(FireFlyLanternState, flags) == 0x1E);
STATIC_ASSERT(offsetof(FireFlyLanternState, pad1F) == 0x1F);
STATIC_ASSERT(offsetof(FireFlyLanternState, countGameBit) == 0x20);
STATIC_ASSERT(offsetof(FireFlyLanternState, pad22) == 0x22);
STATIC_ASSERT(sizeof(FireFlyLanternState) == FIREFLY_LANTERN_STATE_SIZE);

int FireFlyLantern_getExtraSize(void);
int FireFlyLantern_getObjectTypeId(void);
void FireFlyLantern_free(GameObject* obj);
void FireFlyLantern_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void FireFlyLantern_update(GameObject* obj);
void FireFlyLantern_init(GameObject* obj, FireFlyLanternPlacement* placement);

extern ObjectDescriptor gFireFlyLanternObjDescriptor;

#endif /* DLLS_OBJECTS_267_FIREFLYLANT_H_ */
