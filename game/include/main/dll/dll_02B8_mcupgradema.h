#ifndef MAIN_DLL_DLL_02B8_MCUPGRADEMA_H
#define MAIN_DLL_DLL_02B8_MCUPGRADEMA_H

#include "main/dll/mcupgrade_state.h"
#include "game/objects/object.h"
#include "dlls/object_descriptor.h"
#include "main/objseq.h"

extern ObjectDescriptor gMCUpgradeMaObjDescriptor;

int mcupgradema_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate);
void mcupgradema_update(GameObject* obj);
void mcupgradema_init(GameObject* obj);

#endif
