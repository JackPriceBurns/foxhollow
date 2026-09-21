#ifndef MAIN_DLL_DF_DLL_022A_DFPOBJCREATOR_H_
#define MAIN_DLL_DF_DLL_022A_DFPOBJCREATOR_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"

typedef struct DfpObjectCreatorPlacement DfpObjectCreatorPlacement;

extern ObjectDescriptor gDFP_ObjCreatorObjDescriptor;

int DFP_ObjCreator_getExtraSize(void);
int DFP_ObjCreator_getObjectTypeId(void);
void DFP_ObjCreator_free(GameObject* obj, int flag);
void DFP_ObjCreator_render(GameObject* obj, int p2, int p3, int p4, int p5, s8 visible);
void DFP_ObjCreator_hitDetect(void);
void DFP_ObjCreator_update(GameObject* obj);
void DFP_ObjCreator_init(GameObject* obj, const DfpObjectCreatorPlacement* placement);
void DFP_ObjCreator_release(void);
void DFP_ObjCreator_initialise(void);

#endif
