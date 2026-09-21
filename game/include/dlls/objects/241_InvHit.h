#ifndef DLLS_OBJECTS_241_INVHIT_H_
#define DLLS_OBJECTS_241_INVHIT_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef enum InvHitMode {
    INVHIT_MODE_PROXIMITY_DAMAGE = 0,
    INVHIT_MODE_ATTACH = 1,
    INVHIT_MODE_PASSIVE_VOLUME = 2,
    INVHIT_MODE_PUBLISH_POS = 3,
    INVHIT_MODE_HOMING_PROJECTILE = 4,
    INVHIT_MODE_LOCKON_GATE = 5,
    INVHIT_MODE_FIXED_RADIUS = 6,
    INVHIT_MODE_SELF_FREE = 7
} InvHitMode;

typedef struct InvHitState {
    f32 anchorX; /* 0x00 */
    f32 anchorZ; /* 0x04 */
    u8 mode;     /* 0x08: InvHitMode */
    u8 pad09[3]; /* 0x09 */
} InvHitState;

typedef struct InvHitObjectDef {
    ObjPlacement base;     /* 0x00 */
    u8 radius;             /* 0x18: primary radius / userData2 seed */
    u8 shapeFlags;         /* 0x19 */
    u8 mode;               /* 0x1A: InvHitMode */
    u8 pad1B;              /* 0x1B */
    GameObject* anchorObj; /* 0x1C */
} InvHitObjectDef;

STATIC_ASSERT(offsetof(InvHitState, anchorX) == 0x0);
STATIC_ASSERT(offsetof(InvHitState, anchorZ) == 0x4);
STATIC_ASSERT(offsetof(InvHitState, mode) == 0x8);
STATIC_ASSERT(offsetof(InvHitState, pad09) == 0x9);
STATIC_ASSERT(sizeof(InvHitState) == 0xC);

STATIC_ASSERT(offsetof(InvHitObjectDef, base) == 0x0);
STATIC_ASSERT(offsetof(InvHitObjectDef, radius) == 0x18);
STATIC_ASSERT(offsetof(InvHitObjectDef, shapeFlags) == 0x19);
STATIC_ASSERT(offsetof(InvHitObjectDef, mode) == 0x1A);
STATIC_ASSERT(offsetof(InvHitObjectDef, pad1B) == 0x1B);
STATIC_ASSERT(offsetof(InvHitObjectDef, anchorObj) == 0x1C);
STATIC_ASSERT(sizeof(InvHitObjectDef) == 0x20);

int InvHit_getExtraSize(void);
int InvHit_getObjectTypeId(void);
void InvHit_free(GameObject* obj);
void InvHit_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5);
void InvHit_hitDetect(void);
void InvHit_update(GameObject* obj);
void InvHit_init(GameObject* obj, InvHitObjectDef* setup);
void InvHit_release(void);
void InvHit_initialise(void);

extern f32 gInvHitPublishedPos[4];
extern ObjectDescriptor gInvHitObjDescriptor;

#endif /* DLLS_OBJECTS_241_INVHIT_H_ */
