#ifndef MAIN_DLL_SNOWBIKE_INTERNAL_H_
#define MAIN_DLL_SNOWBIKE_INTERNAL_H_

#include "global.h"

typedef struct SnowBikeStateViewFlags
{
    u8 hidden : 1;
    u8 rest : 7;
} SnowBikeStateViewFlags;
STATIC_ASSERT(sizeof(SnowBikeStateViewFlags) == 1);

/*
 * SnowBikeStateView - file-local overlay of this DLL's own SnowBikeState
 * extra block. Only the scalar fields this DLL reads/writes are
 * named; the rest is padding. state is passed as a raw int handle, so it is
 * cast to this type per-access (byte-neutral) rather than retyped as a
 * pointer, which would perturb cross-function base CSE.
 */
typedef struct SnowBikeStateView
{
    u8 pad00[0x18];
    f32 distFar;  /* 0x18: d >= distFar clamps result to valFar */
    f32 distNear; /* 0x1C: d <= distNear clamps result to valNear */
    f32 valFar;   /* 0x20 */
    f32 valNear;  /* 0x24 */
    u8 pad28[0x3F4 - 0x28];
    f32 channel4Vol; /* 0x3F4: sfx channel 4 volume accumulator */
    f32 channel2Vol; /* 0x3F8: sfx channel 2 volume accumulator */
    u8 pad3FC[0x410 - 0x3FC];
    s32 rotZOffset; /* 0x410: added to obj rotZ before matrix build */
    u8 pad414[0x424 - 0x414];
    f32 distanceGate;                 /* 0x424: distance below which wind/engine sfx play */
    SnowBikeStateViewFlags stateFlags; /* 0x428: bit0 hidden */
    u8 pad429[0x434 - 0x429];
    u8 routeGateActive; /* 0x434: 0 => route-distance gate applies */
    u8 pad435[0x440 - 0x435];
    u16 windSfxId; /* 0x440: channel-4 wind sfx id */
    u8 pad442[0x4B4 - 0x442];
    u8 trailColorByte; /* 0x4B4: stored into each new trail point pair */
    u8 pad4B5[0x51C - 0x4B5];
    f32 lastSpawnPosX; /* 0x51C: obj world position at last trail spawn */
    f32 lastSpawnPosY; /* 0x520 */
    f32 lastSpawnPosZ; /* 0x524 */
} SnowBikeStateView;
STATIC_ASSERT(offsetof(SnowBikeStateView, distFar) == 0x18);
STATIC_ASSERT(offsetof(SnowBikeStateView, channel4Vol) == 0x3F4);
STATIC_ASSERT(offsetof(SnowBikeStateView, rotZOffset) == 0x410);
STATIC_ASSERT(offsetof(SnowBikeStateView, distanceGate) == 0x424);
STATIC_ASSERT(offsetof(SnowBikeStateView, stateFlags) == 0x428);
STATIC_ASSERT(offsetof(SnowBikeStateView, routeGateActive) == 0x434);
STATIC_ASSERT(offsetof(SnowBikeStateView, windSfxId) == 0x440);
STATIC_ASSERT(offsetof(SnowBikeStateView, trailColorByte) == 0x4B4);
STATIC_ASSERT(offsetof(SnowBikeStateView, lastSpawnPosX) == 0x51C);

#endif /* MAIN_DLL_SNOWBIKE_INTERNAL_H_ */
