#ifndef DLLS_OBJECTS_435_SH_BEACON_H_
#define DLLS_OBJECTS_435_SH_BEACON_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"

int sh_beacon_resetFadeTimerCallback(GameObject* obj, int amount);

extern ObjectDescriptor gSH_BeaconObjDescriptor;

#endif
