#ifndef DLLS_OBJECTS_212_SKEETLAWALL_H_
#define DLLS_OBJECTS_212_SKEETLAWALL_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct SkeetlaWallState {
    u8 negXExtent; /* 0x00 */
    u8 posXExtent; /* 0x01 */
    u8 posZExtent; /* 0x02 */
    u8 negZExtent; /* 0x03 */
    u8 posYExtent; /* 0x04 */
    u8 negYExtent; /* 0x05 */
    u8 shapeFlag;  /* 0x06 */
} SkeetlaWallState;

typedef struct SkeetlaWallPlacement {
    ObjPlacement base; /* 0x00 */
    u8 negXExtent;     /* 0x18 */
    u8 posXExtent;     /* 0x19 */
    u8 posZExtent;     /* 0x1A */
    u8 negZExtent;     /* 0x1B */
    u8 posYExtent;     /* 0x1C */
    u8 negYExtent;     /* 0x1D */
    u8 shapeFlag;      /* 0x1E */
    u8 pad1F;          /* 0x1F */
} SkeetlaWallPlacement;

STATIC_ASSERT(offsetof(SkeetlaWallState, negXExtent) == 0x0);
STATIC_ASSERT(offsetof(SkeetlaWallState, posXExtent) == 0x1);
STATIC_ASSERT(offsetof(SkeetlaWallState, posZExtent) == 0x2);
STATIC_ASSERT(offsetof(SkeetlaWallState, negZExtent) == 0x3);
STATIC_ASSERT(offsetof(SkeetlaWallState, posYExtent) == 0x4);
STATIC_ASSERT(offsetof(SkeetlaWallState, negYExtent) == 0x5);
STATIC_ASSERT(offsetof(SkeetlaWallState, shapeFlag) == 0x6);
STATIC_ASSERT(sizeof(SkeetlaWallState) == 0x7);

STATIC_ASSERT(offsetof(SkeetlaWallPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(SkeetlaWallPlacement, negXExtent) == 0x18);
STATIC_ASSERT(offsetof(SkeetlaWallPlacement, posXExtent) == 0x19);
STATIC_ASSERT(offsetof(SkeetlaWallPlacement, posZExtent) == 0x1A);
STATIC_ASSERT(offsetof(SkeetlaWallPlacement, negZExtent) == 0x1B);
STATIC_ASSERT(offsetof(SkeetlaWallPlacement, posYExtent) == 0x1C);
STATIC_ASSERT(offsetof(SkeetlaWallPlacement, negYExtent) == 0x1D);
STATIC_ASSERT(offsetof(SkeetlaWallPlacement, shapeFlag) == 0x1E);
STATIC_ASSERT(sizeof(SkeetlaWallPlacement) == 0x20);

void SkeetlaWall_getBounds(GameObject* obj, f32* outBounds, u8* outShapeFlag);
int SkeetlaWall_getExtraSize(void);
int SkeetlaWall_getObjectTypeId(void);
void SkeetlaWall_free(GameObject* obj);
void SkeetlaWall_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void SkeetlaWall_hitDetect(GameObject* obj);
void SkeetlaWall_update(GameObject* obj);
void SkeetlaWall_init(GameObject* obj, SkeetlaWallPlacement* placement);
void SkeetlaWall_release(void);
void SkeetlaWall_initialise(void);

extern ObjectDescriptor11WithPadding gSkeetlaWallObjDescriptor;

#endif /* DLLS_OBJECTS_212_SKEETLAWALL_H_ */
