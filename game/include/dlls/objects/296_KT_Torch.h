#ifndef DLLS_OBJECTS_296_KT_TORCH_H_
#define DLLS_OBJECTS_296_KT_TORCH_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

/* Only the accessed placement prefix is recovered; the complete retail width is not established. */
typedef struct KTTorchPlacement {
    ObjPlacement base;           /* 0x00 */
    u8 modelBankIndex;           /* 0x18: clamped to the model count */
    u8 animationIndex;           /* 0x19: initial animation */
    u8 initialAnimationProgress; /* 0x1A: scaled by 1/256 */
    u8 animationSpeed;           /* 0x1B: scaled by 1/10000 per frame */
    u8 scaleMultiplier;          /* 0x1C: clamped to 10, then scaled by 1/64 */
    u8 initialYaw;               /* 0x1D: low six bits encode the initial yaw */
    u8 pad1E[2];                 /* 0x1E */
    s16 visibilityGameBit;       /* 0x20: -1 leaves the placement alpha unchanged */
} KTTorchPlacement;

STATIC_ASSERT(offsetof(KTTorchPlacement, base) == 0x0);
STATIC_ASSERT(offsetof(KTTorchPlacement, modelBankIndex) == 0x18);
STATIC_ASSERT(offsetof(KTTorchPlacement, animationIndex) == 0x19);
STATIC_ASSERT(offsetof(KTTorchPlacement, initialAnimationProgress) == 0x1A);
STATIC_ASSERT(offsetof(KTTorchPlacement, animationSpeed) == 0x1B);
STATIC_ASSERT(offsetof(KTTorchPlacement, scaleMultiplier) == 0x1C);
STATIC_ASSERT(offsetof(KTTorchPlacement, initialYaw) == 0x1D);
STATIC_ASSERT(offsetof(KTTorchPlacement, pad1E) == 0x1E);
STATIC_ASSERT(offsetof(KTTorchPlacement, visibilityGameBit) == 0x20);

int KT_Torch_getExtraSize(void);
int KT_Torch_getObjectTypeId(void);
void KT_Torch_free(void);
void KT_Torch_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void KT_Torch_hitDetect(void);
void KT_Torch_update(GameObject* obj);
void KT_Torch_init(GameObject* obj, KTTorchPlacement* placement);
void KT_Torch_release(void);
void KT_Torch_initialise(void);

extern ObjectDescriptor gKT_TorchObjDescriptor;

#endif /* DLLS_OBJECTS_296_KT_TORCH_H_ */
