#ifndef DLLS_OBJECTS_516_WM_TORCH_H_
#define DLLS_OBJECTS_516_WM_TORCH_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

typedef struct WMTorchPlacement {
    ObjPlacement base;
    u8 unknown18;
    u8 torchType;
    s16 motionRate;
    s16 colorIndex;
} WMTorchPlacement;

STATIC_ASSERT(offsetof(WMTorchPlacement, base) == 0x00);
STATIC_ASSERT(offsetof(WMTorchPlacement, unknown18) == 0x18);
STATIC_ASSERT(offsetof(WMTorchPlacement, torchType) == 0x19);
STATIC_ASSERT(offsetof(WMTorchPlacement, motionRate) == 0x1A);
STATIC_ASSERT(offsetof(WMTorchPlacement, colorIndex) == 0x1C);

int wmtorch_getExtraSize(void);
int wmtorch_getObjectTypeId(void);
void wmtorch_free(GameObject* obj, int mode);
void wmtorch_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void wmtorch_hitDetect(void);
void wmtorch_update(GameObject* obj);
void wmtorch_init(GameObject* obj, const WMTorchPlacement* placement);
void wmtorch_release(void);
void wmtorch_initialise(void);

extern ObjectDescriptor gWM_TorchObjDescriptor;

#endif /* DLLS_OBJECTS_516_WM_TORCH_H_ */
