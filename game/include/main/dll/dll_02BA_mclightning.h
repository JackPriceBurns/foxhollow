#ifndef MAIN_DLL_DLL_02BA_MCLIGHTNING_H
#define MAIN_DLL_DLL_02BA_MCLIGHTNING_H

#include "main/dll/mclightning_state.h"
#include "game/objects/object.h"
#include "dlls/object_descriptor.h"
#include "main/objseq.h"

extern ObjectDescriptor gMCLightningObjDescriptor;

int mclightning_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate);
int mclightning_getExtraSize(void);
void mclightning_free(GameObject* obj);
void mclightning_render(GameObject* obj, int p2, int p3, int p4, int p5, f32 scale);
void mclightning_update(GameObject* obj);
void mclightning_init(GameObject* obj, McLightningSetup* setup);

#endif
