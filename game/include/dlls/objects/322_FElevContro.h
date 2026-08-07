#ifndef DLLS_OBJECTS_322_FELEVCONTRO_H_
#define DLLS_OBJECTS_322_FELEVCONTRO_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"

int FElevControl_getExtraSize(void);
int FElevControl_getObjectTypeId(void);
void FElevControl_free(void);
void FElevControl_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void FElevControl_hitDetect(void);
void FElevControl_update(void);
void FElevControl_init(GameObject* obj);
void FElevControl_release(void);
void FElevControl_initialise(void);

extern ObjectDescriptor gFElevControlObjDescriptor;

#endif /* DLLS_OBJECTS_322_FELEVCONTRO_H_ */
