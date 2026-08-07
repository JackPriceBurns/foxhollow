#ifndef DLLS_OBJECTS_516_WM_TORCH_H_
#define DLLS_OBJECTS_516_WM_TORCH_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

/* Only the accessed placement prefix is recovered; the complete retail width is not established. */
typedef struct WMTorchPlacementView {
    ObjPlacement base; /* 0x00 */
    u8 unknown18;      /* 0x18 */
    u8 torchType;      /* 0x19: 0 / 0x7F use resource 0x69; all other values use resource 0x63 */
    s16 motionRate;    /* 0x1A: defaults to 75.0 when zero */
    s16 colorIndex;    /* 0x1C: defaults to 0x8C when zero */
} WMTorchPlacementView;

STATIC_ASSERT(offsetof(WMTorchPlacementView, base) == 0x00);
STATIC_ASSERT(offsetof(WMTorchPlacementView, unknown18) == 0x18);
STATIC_ASSERT(offsetof(WMTorchPlacementView, torchType) == 0x19);
STATIC_ASSERT(offsetof(WMTorchPlacementView, motionRate) == 0x1A);
STATIC_ASSERT(offsetof(WMTorchPlacementView, colorIndex) == 0x1C);

/* wmtorch_getExtraSize allocates this complete 0x10-byte state. */
typedef struct WMTorchState {
    GameObject* linkedObject; /* 0x00 */
    f32 motionRate;           /* 0x04 */
    u8 unknown08[2];          /* 0x08 */
    s16 colorIndex;           /* 0x0A */
    u8 torchType;             /* 0x0C */
    u8 unknown0D[3];          /* 0x0D */
} WMTorchState;

STATIC_ASSERT(offsetof(WMTorchState, linkedObject) == 0x00);
STATIC_ASSERT(offsetof(WMTorchState, motionRate) == 0x04);
STATIC_ASSERT(offsetof(WMTorchState, unknown08) == 0x08);
STATIC_ASSERT(offsetof(WMTorchState, colorIndex) == 0x0A);
STATIC_ASSERT(offsetof(WMTorchState, torchType) == 0x0C);
STATIC_ASSERT(offsetof(WMTorchState, unknown0D) == 0x0D);
STATIC_ASSERT(sizeof(WMTorchState) == 0x10);

int wmtorch_getExtraSize(void);
int wmtorch_getObjectTypeId(void);
void wmtorch_free(GameObject* obj, int mode);
void wmtorch_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void wmtorch_hitDetect(void);
void wmtorch_update(GameObject* obj);
void wmtorch_init(GameObject* obj, const WMTorchPlacementView* placement);
void wmtorch_release(void);
void wmtorch_initialise(void);

extern ObjectDescriptor gWM_TorchObjDescriptor;

#endif /* DLLS_OBJECTS_516_WM_TORCH_H_ */
