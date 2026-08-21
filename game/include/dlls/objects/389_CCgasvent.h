#ifndef DLLS_OBJECTS_389_CCGASVENT_H_
#define DLLS_OBJECTS_389_CCGASVENT_H_

#include "dlls/object_descriptor.h"

enum CcGasVentObjectGroup {
    CC_GAS_VENT_BLOCKER_OBJECT_GROUP = 5,
    CC_GAS_VENT_OBJECT_GROUP = 0x3F,
};

extern ObjectDescriptor gCCGasVentObjDescriptor;

#endif /* DLLS_OBJECTS_389_CCGASVENT_H_ */
