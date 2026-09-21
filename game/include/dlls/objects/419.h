#ifndef DLLS_OBJECTS_419_H_
#define DLLS_OBJECTS_419_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "main/objseq.h"

#define DLL1A3_OBJECT_GROUP_ID 0x3D

extern ObjectDescriptor gDll1A3ObjDescriptor;

int dll419_processAnimEvents(GameObject* unusedObj, int unusedArg, ObjSeqState* unusedAnimUpdate);
int dll419_getExtraSize(void);
int dll419_getObjectTypeId(void);
void dll419_free(GameObject* obj);
void dll419_render(GameObject* unusedObj, int unusedArg2, int unusedArg3, int unusedArg4, int unusedArg5,
                   s8 unusedVisible);
void dll419_hitDetect(void);
void dll419_update(GameObject* unusedObj);
void dll419_init(GameObject* obj);
void dll419_release(void);
void dll419_initialise(void);

#endif /* DLLS_OBJECTS_419_H_ */
