#ifndef DLLS_OBJECTS_220_MIKABOMBSHADOW_H_
#define DLLS_OBJECTS_220_MIKABOMBSHADOW_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"

typedef struct MikaBombShadowState {
    f32 groundOffset; /* 0x00 */
} MikaBombShadowState;

STATIC_ASSERT(offsetof(MikaBombShadowState, groundOffset) == 0x0);
STATIC_ASSERT(sizeof(MikaBombShadowState) == 0x4);

int MikaBombShadow_getExtraSize(void);
int MikaBombShadow_getObjectTypeId(void);
void MikaBombShadow_free(GameObject* obj);
void MikaBombShadow_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 visible);
void MikaBombShadow_hitDetect(GameObject* obj);
void MikaBombShadow_update(GameObject* obj);
void MikaBombShadow_init(GameObject* obj);
void MikaBombShadow_release(void);
void MikaBombShadow_initialise(void);

extern ObjectDescriptor gMikaBombShadowObjDescriptor;
extern const f32 gMikaBombRenderScale;
extern const f32 gMikaBombFadeRate;
extern const f32 gMikaBombZero;
extern const f32 gMikaBombGravityAccel;
extern const f32 gMikaBombMinFallVelocity;
extern const f32 gMikaBombInitialVelocityY;

#endif /* DLLS_OBJECTS_220_MIKABOMBSHADOW_H_ */
