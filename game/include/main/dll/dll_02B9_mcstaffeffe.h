#ifndef MAIN_DLL_DLL_02B9_MCSTAFFEFFE_H
#define MAIN_DLL_DLL_02B9_MCSTAFFEFFE_H

#include "main/dll/mcstaffeffe_state.h"
#include "dlls/object_descriptor.h"
#include "main/objseq.h"

extern ObjectDescriptor gMCStaffEffeObjDescriptor;

int mcstaffeffe_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate);
void mcstaffeffe_render(GameObject* obj);
void mcstaffeffe_update(void);
void mcstaffeffe_init(GameObject* obj, McStaffEffectSetup* setup);

#endif
