#ifndef DLLS_OBJECTS_220_MIKABOMBSHADOW_H_
#define DLLS_OBJECTS_220_MIKABOMBSHADOW_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef enum MikaBombObjectId {
    MIKA_BOMB_SHADOW_OBJECT_ID = 0xC,
} MikaBombObjectId;

typedef struct MikaBombShadowPlacement {
    ObjPlacement base;
    u8 unk18[8];
} MikaBombShadowPlacement;

STATIC_ASSERT(sizeof(MikaBombShadowPlacement) == 0x20);

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

#endif /* DLLS_OBJECTS_220_MIKABOMBSHADOW_H_ */
