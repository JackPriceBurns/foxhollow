#ifndef MAIN_TRACK_DOLPHIN_H_
#define MAIN_TRACK_DOLPHIN_H_

#include "types.h"
#include "game/objects/object.h"
#include "main/vec_types.h"

typedef struct TrackBlockDescriptor
{
    void* object;
    s16 firstTriangle;
    u8 pad06[2];
    void* currentMatrix;
    void* currentCollisionMatrix;
    void* alternateMatrix;
    void* alternateCollisionMatrix;
} TrackBlockDescriptor;

typedef struct TrackShadowTriangle
{
    Vec3f normal;
    f32 planeDistance;
    s8 flags;
    u8 pad11[3];
} TrackShadowTriangle;

struct TrackTriangle;

void trackDolphin_buildSweptBounds(u32 *boundsOut,float *startPoints,float *endPoints,
                                   float *radii,int pointCount);

/* extern-cleanup: defining-file public prototypes */
int collectShadowTrackTriangles(GameObject* obj, int triBuf, void* planesOut, int vertsOut, int unusedTriangleCount,
                                f32 offX, f32 offZ, int unusedRenderMode, int kindSelector);
void objDrawShadowCasterMesh(Vec3f* vertices, ObjModelState* modelState, GameObject* obj, int triangleCount,
                             void* unusedDrawScratch, void* unusedBounds, f32 unusedYOffset);
void trackCollectGroundHits(struct TrackTriangle* triStart, struct TrackTriangle* triEnd, struct TrackBlockDescriptor* desc,
                 f32 qx, f32 qz, int allowDown);
#endif /* MAIN_TRACK_DOLPHIN_H_ */
