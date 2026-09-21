#ifndef DLLS_OBJECTS_205_ICEBALL_H_
#define DLLS_OBJECTS_205_ICEBALL_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"

void iceBall_handleSurfaceImpact(GameObject* obj);
void iceBall_handleCharacterImpact(GameObject* obj);
int IceBall_getExtraSize(void);
int IceBall_getObjectTypeId(void);
void IceBall_free(GameObject* obj);
void IceBall_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void IceBall_hitDetect(GameObject* obj);
void IceBall_update(GameObject* obj);
void IceBall_init(GameObject* obj);
void IceBall_release(void);
void IceBall_initialise(void);

extern ObjectDescriptor gIceBallObjDescriptor;

#endif /* DLLS_OBJECTS_205_ICEBALL_H_ */
