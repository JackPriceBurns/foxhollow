#ifndef MAIN_DLL_DLL_02B7_MCUPGRADE_H
#define MAIN_DLL_DLL_02B7_MCUPGRADE_H

#include "game/objects/object.h"
#include "dlls/object_descriptor.h"

extern ObjectDescriptor gMCUpgradeObjDescriptor;

void mcupgrade_update(GameObject* obj);
void mcupgrade_init(GameObject* obj);

#endif
