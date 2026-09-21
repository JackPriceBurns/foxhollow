#ifndef MAIN_DLL_MCSTAFFEFFE_STATE_H_
#define MAIN_DLL_MCSTAFFEFFE_STATE_H_

#include "global.h"
#include "game/objects/object.h"

typedef struct McStaffEffectSetup {
    u8 pad00[0x1B];
    u8 effectProfile;
} McStaffEffectSetup;

STATIC_ASSERT(offsetof(McStaffEffectSetup, effectProfile) == 0x1B);

#endif /* MAIN_DLL_MCSTAFFEFFE_STATE_H_ */
