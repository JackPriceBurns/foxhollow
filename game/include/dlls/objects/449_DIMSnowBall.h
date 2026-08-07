#ifndef DLLS_OBJECTS_449_DIMSNOWBALL_H_
#define DLLS_OBJECTS_449_DIMSNOWBALL_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "global.h"

#define DIM_SNOWBALL_PATH_COORDINATE_COUNT 2994
#define DIM_SNOWBALL_SEQUENCE_ID           0x196

typedef struct DimSnowBallPlacement {
    union {
        ObjPlacement base;
        struct {
            u8 unknown00[0x14];
            s32 targetObjectId;
        };
    };
    s8 rotationXByte;
    u8 unknown19;
    s16 rotationParam1A;
    s16 rotationParam1C;
    u8 unknown1E[0x24 - 0x1E];
} DimSnowBallPlacement;

typedef struct DimSnowBallState {
    GameObject* target;
    s32 targetObjectId;
    s32 pathPointIndex;
    s8 jingleCooldown;
    u8 unknown0D[0x10 - 0x0D];
} DimSnowBallState;

STATIC_ASSERT(offsetof(DimSnowBallPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(DimSnowBallPlacement, targetObjectId) == 0x14);
STATIC_ASSERT(offsetof(DimSnowBallPlacement, rotationXByte) == 0x18);
STATIC_ASSERT(offsetof(DimSnowBallPlacement, unknown19) == 0x19);
STATIC_ASSERT(offsetof(DimSnowBallPlacement, rotationParam1A) == 0x1A);
STATIC_ASSERT(offsetof(DimSnowBallPlacement, rotationParam1C) == 0x1C);
STATIC_ASSERT(offsetof(DimSnowBallPlacement, unknown1E) == 0x1E);
STATIC_ASSERT(sizeof(DimSnowBallPlacement) == 0x24);

STATIC_ASSERT(offsetof(DimSnowBallState, target) == 0x00);
STATIC_ASSERT(offsetof(DimSnowBallState, targetObjectId) == 0x04);
STATIC_ASSERT(offsetof(DimSnowBallState, pathPointIndex) == 0x08);
STATIC_ASSERT(offsetof(DimSnowBallState, jingleCooldown) == 0x0C);
STATIC_ASSERT(offsetof(DimSnowBallState, unknown0D) == 0x0D);
STATIC_ASSERT(sizeof(DimSnowBallState) == 0x10);

int dimsnowball_getExtraSize(void);
int dimsnowball_getObjectTypeId(void);
void dimsnowball_free(void);
void dimsnowball_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dimsnowball_hitDetect(GameObject* obj);
void dimsnowball_update(GameObject* obj);
void dimsnowball_init(GameObject* obj, DimSnowBallPlacement* placement);
void dimsnowball_release(void);
void dimsnowball_initialise(void);

extern s16 gDimSnowballPathPointCount;
extern s16 gDimSnowballCoords[DIM_SNOWBALL_PATH_COORDINATE_COUNT];
extern ObjectDescriptor gDIMSnowBallObjDescriptor;

#endif /* DLLS_OBJECTS_449_DIMSNOWBALL_H_ */
