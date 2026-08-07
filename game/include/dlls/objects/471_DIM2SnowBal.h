#ifndef DLLS_OBJECTS_471_DIM2SNOWBAL_H_
#define DLLS_OBJECTS_471_DIM2SNOWBAL_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/curve_types.h"

/*
 * DIM2PathGen allocates the child setup as a 0x24-byte record. This DLL
 * consumes the shared placement head's map-id slot as a target-object id.
 */
typedef struct Dim2SnowBallPlacement {
    union {
        ObjPlacement base;
        struct {
            u8 unknown00[0x14];
            s32 targetObjectId;
        };
    };
    s8 rotationXByte;
    u8 unknown19;
    s16 unknown1A;
    s16 unknown1C;
    u8 unknown1E[0x24 - 0x1E];
} Dim2SnowBallPlacement;

/* getExtraSize() allocates the complete 0xB0-byte state block. */
typedef struct Dim2SnowBallState {
    Curve path;
    GameObject* pathGenerator;
    s32 targetObjectId;
    f32 floorHeight;
    u8* pathNodeData;
    u8 flags;
    u8 unknownAD[3];
} Dim2SnowBallState;

/* gDIM2SnowBallObjDescriptor from slot02 onwards: the export table other
   objects reach through obj->anim.dll. DIM2PathGen re-seeds a pooled
   snowball by re-entering its init slot. */
typedef struct Dim2SnowBallInterface {
    void* pad00;
    void (*init)(GameObject* obj, Dim2SnowBallPlacement* placement, int reinit);
} Dim2SnowBallInterface;

#define DIM2_SNOW_BALL_INTERFACE(snowball) ((Dim2SnowBallInterface*)*((GameObject*)(snowball))->anim.dll)

STATIC_ASSERT(offsetof(Dim2SnowBallInterface, init) == 0x04);

STATIC_ASSERT(offsetof(Dim2SnowBallPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(Dim2SnowBallPlacement, targetObjectId) == 0x14);
STATIC_ASSERT(offsetof(Dim2SnowBallPlacement, rotationXByte) == 0x18);
STATIC_ASSERT(offsetof(Dim2SnowBallPlacement, unknown19) == 0x19);
STATIC_ASSERT(offsetof(Dim2SnowBallPlacement, unknown1A) == 0x1A);
STATIC_ASSERT(offsetof(Dim2SnowBallPlacement, unknown1C) == 0x1C);
STATIC_ASSERT(offsetof(Dim2SnowBallPlacement, unknown1E) == 0x1E);
STATIC_ASSERT(sizeof(Dim2SnowBallPlacement) == 0x24);

STATIC_ASSERT(offsetof(Dim2SnowBallState, path) == 0x00);
STATIC_ASSERT(offsetof(Dim2SnowBallState, pathGenerator) == 0x9C);
STATIC_ASSERT(offsetof(Dim2SnowBallState, targetObjectId) == 0xA0);
STATIC_ASSERT(offsetof(Dim2SnowBallState, floorHeight) == 0xA4);
STATIC_ASSERT(offsetof(Dim2SnowBallState, pathNodeData) == 0xA8);
STATIC_ASSERT(offsetof(Dim2SnowBallState, flags) == 0xAC);
STATIC_ASSERT(offsetof(Dim2SnowBallState, unknownAD) == 0xAD);
STATIC_ASSERT(sizeof(Dim2SnowBallState) == 0xB0);

int dim2snowball_getExtraSize(void);
int dim2snowball_getObjectTypeId(void);
void dim2snowball_free(void);
void dim2snowball_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dim2snowball_hitDetect(void);
void dim2snowball_update(GameObject* obj);
void dim2snowball_init(GameObject* obj, Dim2SnowBallPlacement* placement);
void dim2snowball_release(void);
void dim2snowball_initialise(void);

extern ObjectDescriptor gDIM2SnowBallObjDescriptor;

#endif /* DLLS_OBJECTS_471_DIM2SNOWBAL_H_ */
