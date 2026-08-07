#ifndef DLLS_OBJECTS_324_H_
#define DLLS_OBJECTS_324_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "main/objseq.h"

int dll_144_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate);
int dll_144_getExtraSize(void);
int dll_144_getObjectTypeId(void);
void dll_144_free(void);
void dll_144_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dll_144_hitDetect(void);
void dll_144_update(void);
void dll_144_init(GameObject* obj);
void dll_144_release(void);
void dll_144_initialise(void);

extern ObjectDescriptor gDll144ObjDescriptor;

#endif /* DLLS_OBJECTS_324_H_ */
