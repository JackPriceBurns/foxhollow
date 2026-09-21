#ifndef DLLS_OBJECTS_434_SH_STAFF_HAZ_H_
#define DLLS_OBJECTS_434_SH_STAFF_HAZ_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"

extern ObjectDescriptor gSH_staffHazeObjDescriptor;

void SH_StaffHaze_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5);
void SH_StaffHaze_update(GameObject* obj);

#endif /* DLLS_OBJECTS_434_SH_STAFF_HAZ_H_ */
