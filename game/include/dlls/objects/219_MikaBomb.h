#ifndef DLLS_OBJECTS_219_MIKABOMB_H_
#define DLLS_OBJECTS_219_MIKABOMB_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"

int MikaBomb_getExtraSize(void);
int MikaBomb_getObjectTypeId(void);
void MikaBomb_free(GameObject* obj, int mode);
void MikaBomb_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void MikaBomb_hitDetect(GameObject* obj);
void MikaBomb_update(GameObject* obj);
void MikaBomb_init(GameObject* obj);
void MikaBomb_release(void);
void MikaBomb_initialise(void);

extern ObjectDescriptor gMikaBombObjDescriptor;

#endif /* DLLS_OBJECTS_219_MIKABOMB_H_ */
