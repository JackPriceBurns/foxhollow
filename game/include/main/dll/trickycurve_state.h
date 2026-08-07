#ifndef MAIN_DLL_TRICKYCURVE_STATE_H_
#define MAIN_DLL_TRICKYCURVE_STATE_H_

#include "types.h"
#include "global.h"

/*
 * TrickyCurveObjState - the obj+0xB8 extra record shared by all four
 * TrickyCurve variants; TrickyCurve_getExtraSize() returns its 0x14 bytes.
 */
typedef struct TrickyCurveObjState {
    s16 rangeX; /* half-extent of the axis-aligned trigger box; the object's world position is the centre */
    s16 rangeZ;
    s16 rangeY;
    s16 cooldown; /* ticks until the cooldown variant may hit again; counted down by timeDelta */
    s16 gateGameBit; /* -1 = ungated, else the trigger only fires while this game bit is clear */
    s16 triggerGameBit; /* set on a burst, and tested/cleared to arm the next one */
    u8 variant; /* placement's variant byte, kept unread alongside the live copy in mode */
    u8 padD;
    u8 mode; /* 0 burst trigger, 1 cooldown trigger, 2 burst hit, 3 cooldown hit - TrickyCurve_update dispatches on it */
    u8 padF;
    u8 xSide; /* which side of the box midline the player was on last tick, per axis; a crossing is what fires the trigger */
    u8 ySide;
    u8 zSide;
    u8 pad13;
} TrickyCurveObjState;

STATIC_ASSERT(offsetof(TrickyCurveObjState, cooldown) == 0x06);
STATIC_ASSERT(offsetof(TrickyCurveObjState, mode) == 0x0E);
STATIC_ASSERT(offsetof(TrickyCurveObjState, xSide) == 0x10);
STATIC_ASSERT(sizeof(TrickyCurveObjState) == 0x14);

#endif /* MAIN_DLL_TRICKYCURVE_STATE_H_ */
