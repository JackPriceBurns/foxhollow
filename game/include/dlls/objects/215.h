#ifndef DLLS_OBJECTS_215_H_
#define DLLS_OBJECTS_215_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "main/modellight_api.h"

typedef struct KaldachomSpitState {
    ModelLightStruct* light; /* 0x00 */
} KaldachomSpitState;

STATIC_ASSERT(offsetof(KaldachomSpitState, light) == 0x0);
STATIC_ASSERT(sizeof(KaldachomSpitState) == 0x4);

int KaldachomSpit_getExtraSize(void);
int KaldachomSpit_getObjectTypeId(void);
void KaldachomSpit_free(GameObject* obj);
void KaldachomSpit_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void KaldachomSpit_hitDetect(GameObject* obj);
void KaldachomSpit_update(GameObject* obj);
void KaldachomSpit_init(GameObject* obj);
void KaldachomSpit_release(void);
void KaldachomSpit_initialise(void);

extern ObjectDescriptor gKaldachomSpObjDescriptor;

#endif /* DLLS_OBJECTS_215_H_ */
