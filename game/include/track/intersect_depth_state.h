#ifndef TRACK_INTERSECT_DEPTH_STATE_H_
#define TRACK_INTERSECT_DEPTH_STATE_H_

#include "types.h"

void gxSetPeControl_ZCompLoc_(u8 zCompLoc);
void gxSetZMode_(u8 compareEnable, int compareFunc, u8 updateEnable);

#endif /* TRACK_INTERSECT_DEPTH_STATE_H_ */
