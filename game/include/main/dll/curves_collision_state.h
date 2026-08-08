#ifndef MAIN_DLL_CURVES_COLLISION_STATE_H_
#define MAIN_DLL_CURVES_COLLISION_STATE_H_

#include "types.h"
#include "global.h"
#include "game/objects/object.h"
#include "main/track_dolphin_api.h"
#include "main/track_bbox_api.h"

#define CURVES_COLLISION_STATE_SIZE                   0x268
#define CURVES_COLLISION_STATE_ACTIVE                 0x04000000
#define CURVES_COLLISION_STATE_LOCAL_POINTS           0x00000008
#define CURVES_COLLISION_STATE_HIT_SEGMENTS           0x00002000
#define CURVES_COLLISION_STATE_SECONDARY_LOCAL_POINTS 0x02000000
#define CURVES_COLLISION_STATE_X_ROTATION_ONLY        0x00000020
#define CURVES_COLLISION_STATE_KEEP_POSITION          0x00100000
#define CURVES_POINT_COUNT_LOCAL_MASK                 0x0f
#define CURVES_POINT_COUNT_SEGMENT_MASK               0xf0
#define CURVES_POINT_COUNT_SEGMENT_SHIFT              4
#define CURVES_COLLISION_SUBTYPE_NONE                 0
#define CURVES_COLLISION_SUBTYPE_OBJECT               1
#define CURVES_COLLISION_SUBTYPE_POINT                2

typedef struct CurvesCollisionState
{
    u32 flags;
    u8 pad004[0x008 - 0x004];
    f32 points[4][3];           /* 0x008 world-space segment points; double as trace ends */
    f32 traceStart[4][3];       /* 0x038 per-point raised trace starts */
    TrackHitResults segmentHits; /* 0x068 trackGetIntersect record for the segment sweep */
    f32 localPointWorld[4][3];  /* 0x0E4 localPointPositions transformed to world */
    f32 localPointTarget[4][3]; /* 0x114 raised copies; bbox-swept against localPointWorld */
    TrackBBoxHit localHit;      /* 0x144 trackGetLineIntersect record for the local points */
    s16 tiltPitch;       /* 0x198 smoothed toward tiltPitchTarget */
    s16 tiltRoll;        /* 0x19A */
    s16 tiltPitchTarget; /* 0x19C from surface normal */
    s16 tiltRollTarget;  /* 0x19E */
    f32 surfaceNormalX;  /* 0x1A0 */
    f32 surfaceNormalY;
    f32 surfaceNormalZ;
    f32 resultFloorGap;   /* 0x1AC latest-point copies of the arrays below */
    f32 resultCeilingY;   /* 0x1B0 */
    f32 resultWaterDepth; /* 0x1B4 */
    f32 resultFloorY;     /* 0x1B8 */
    f32 resultWaterY;     /* 0x1BC */
    f32 floorGap[4];      /* 0x1C0 posY - floorY per point */
    f32 ceilingY[4];      /* 0x1D0 */
    f32 waterDepth[4];    /* 0x1E0 waterY - posY */
    f32 floorY[4];        /* 0x1F0 */
    f32 waterY[4];        /* 0x200 type-0xE surface height */
    f32 waterNormalX[4];  /* 0x210 */
    f32 waterNormalY[4];  /* 0x220 init 1.0 */
    f32 waterNormalZ[4];  /* 0x230 */
    TrackQueryBounds hitBounds; /* 0x240 swept-sphere bounds */
    u8 heightPadding;
    u8 pad259[2];
    s8 subtype;
    u8 pointCounts;
    s8 primaryHitType;
    u8 localPointHitMask;
    u8 surfaceHitMask;
    u8 surfaceFlags;
    s8 surfaceCounter;
    u8 updateMode;
    s8 secondaryHitType;
    u8 activeTimer;
    u8 pad265[CURVES_COLLISION_STATE_SIZE - 0x265];
    f32* segmentLocalPoints;
    GameObject* contactObj;
    f32* localPointPositions;
    f32* localPointRadii;
} CurvesCollisionState;

STATIC_ASSERT(offsetof(CurvesCollisionState, flags) == 0x000);
STATIC_ASSERT(offsetof(CurvesCollisionState, points) == 0x008);
STATIC_ASSERT(offsetof(CurvesCollisionState, traceStart) == 0x038);
STATIC_ASSERT(offsetof(CurvesCollisionState, segmentHits) == 0x068);

#endif /* MAIN_DLL_CURVES_COLLISION_STATE_H_ */
