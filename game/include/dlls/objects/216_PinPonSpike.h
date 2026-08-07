#ifndef DLLS_OBJECTS_216_PINPONSPIKE_H_
#define DLLS_OBJECTS_216_PINPONSPIKE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"

int pinponspike_calculateLaunchAngle(const f32* source, const f32* target, f32 speed, u8 useHighArc, f32 gravity);
int pinponspike_getExtraSize(void);
int pinponspike_getObjectTypeId(void);
void pinponspike_free(GameObject* obj);
void pinponspike_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void pinponspike_hitDetect(GameObject* obj);
void pinponspike_update(GameObject* obj);
void pinponspike_init(GameObject* obj);
void pinponspike_release(void);
void pinponspike_initialise(void);

extern ObjectDescriptor gPinPonSpikeObjDescriptor;

#endif /* DLLS_OBJECTS_216_PINPONSPIKE_H_ */
