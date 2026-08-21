#ifndef MAIN_DLL_WM_DLL_0215_WMNEWCRYSTAL_H_
#define MAIN_DLL_WM_DLL_0215_WMNEWCRYSTAL_H_

#include "global.h"
#include "game/objects/object.h"
#include "dlls/object_descriptor.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

int WM_newcrystal_SeqFn(GameObject* obj, int unused, ObjSeqState* actor);
int WM_newcrystal_getExtraSize(void);
int WM_newcrystal_getObjectTypeId(void);
void WM_newcrystal_free(void);
void WM_newcrystal_render(GameObject* obj, int p2, int p3, int p4, int p5, s8 visible);
void WM_newcrystal_hitDetect(void);
void WM_newcrystal_update(void);
void WM_newcrystal_init(GameObject* obj, ObjPlacement* unused);
void WM_newcrystal_release(void);
void WM_newcrystal_initialise(void);

extern ObjectDescriptor gWM_newcrystalObjDescriptor;

#endif
