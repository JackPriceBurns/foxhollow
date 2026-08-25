#ifndef DLLS_OBJECTS_228_FLAMETHROWERSPE_H_
#define DLLS_OBJECTS_228_FLAMETHROWERSPE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct FlameThrowerspePlacement {
    ObjPlacement base;   /* 0x00 */
    u8 pad18;            /* 0x18 */
    s8 hitVolumeProfile; /* 0x19 */
    s16 scaleParam;      /* 0x1A */
} FlameThrowerspePlacement;

STATIC_ASSERT(offsetof(FlameThrowerspePlacement, base) == 0x0);
STATIC_ASSERT(offsetof(FlameThrowerspePlacement, pad18) == 0x18);
STATIC_ASSERT(offsetof(FlameThrowerspePlacement, hitVolumeProfile) == 0x19);
STATIC_ASSERT(offsetof(FlameThrowerspePlacement, scaleParam) == 0x1A);
STATIC_ASSERT(sizeof(FlameThrowerspePlacement) == 0x1C);

void FlameThrowerspe_modelMtxFn(void);
void FlameThrowerspe_launch(GameObject* obj);
void FlameThrowerspe_setTransform(GameObject* obj, s16 rotY, s16 rotX, f32 x, f32 y, f32 z);
int FlameThrowerspe_getExtraSize(void);
int FlameThrowerspe_getObjectTypeId(void);
void FlameThrowerspe_free(GameObject* obj);
void FlameThrowerspe_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void FlameThrowerspe_hitDetect(GameObject* obj);
void FlameThrowerspe_update(GameObject* obj);
void FlameThrowerspe_init(GameObject* obj, FlameThrowerspePlacement* placement);
void FlameThrowerspe_release(void);
void FlameThrowerspe_initialise(void);

struct FlameThrowerspeObjDescriptorType;
extern struct FlameThrowerspeObjDescriptorType gFlameThrowerspeObjDescriptor;

#endif /* DLLS_OBJECTS_228_FLAMETHROWERSPE_H_ */
