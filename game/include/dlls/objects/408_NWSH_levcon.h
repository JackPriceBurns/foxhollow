#ifndef DLLS_OBJECTS_408_NWSH_LEVCON_H_
#define DLLS_OBJECTS_408_NWSH_LEVCON_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "main/objseq.h"

extern ObjectDescriptor gNWSHLevelControlObjDescriptor;

int nwshLevelControl_processAnimEvents(GameObject* unusedObj, int unusedArg, ObjSeqState* animUpdate);
int nwshLevelControl_getExtraSize(void);
int nwshLevelControl_getObjectTypeId(void);
void nwshLevelControl_free(GameObject* unusedObj);
void nwshLevelControl_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                             s8 visible);
void nwshLevelControl_hitDetect(void);
void nwshLevelControl_update(GameObject* obj);
void nwshLevelControl_init(GameObject* obj, const void* placement);
void nwshLevelControl_release(void);
void nwshLevelControl_initialise(void);

#endif /* DLLS_OBJECTS_408_NWSH_LEVCON_H_ */
