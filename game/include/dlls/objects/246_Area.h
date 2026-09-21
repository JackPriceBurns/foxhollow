#ifndef DLLS_OBJECTS_246_AREA_H_
#define DLLS_OBJECTS_246_AREA_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"

int area_getExtraSize(void);
int area_getObjectTypeId(void);
void area_free(void);
void area_render(void);
void area_hitDetect(void);
void area_update(void);
void area_init(GameObject* obj);
void area_release(void);
void area_initialise(void);

extern ObjectDescriptor gAreaObjDescriptor;

#endif /* DLLS_OBJECTS_246_AREA_H_ */
