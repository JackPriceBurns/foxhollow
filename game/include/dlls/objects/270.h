#ifndef DLLS_OBJECTS_270_H_
#define DLLS_OBJECTS_270_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"

#define DEATH_SEQ_STATE_SIZE 0x24

typedef struct DeathSeqStateFlags {
    u8 menuShown : 1;         /* bit 7 */
    u8 cameraActive : 1;      /* bit 6 */
    u8 transitionStarted : 1; /* bit 5 */
    u8 unused : 5;
} DeathSeqStateFlags;

typedef struct DeathSeqState {
    f32 menuDelay;            /* 0x00 */
    f32 savedCameraX;         /* 0x04 */
    f32 savedCameraY;         /* 0x08 */
    f32 savedCameraZ;         /* 0x0C */
    f32 cameraDistance;       /* 0x10 */
    f32 cameraDistanceTarget; /* 0x14 */
    s32 savedCameraYaw;       /* 0x18: sign-extended Camera yaw */
    s32 savedCameraPitch;     /* 0x1C: sign-extended Camera pitch */
    DeathSeqStateFlags flags; /* 0x20 */
    u8 pad21[3];              /* 0x21 */
} DeathSeqState;

STATIC_ASSERT(sizeof(DeathSeqStateFlags) == 0x1);
STATIC_ASSERT(offsetof(DeathSeqState, menuDelay) == 0x0);
STATIC_ASSERT(offsetof(DeathSeqState, savedCameraX) == 0x4);
STATIC_ASSERT(offsetof(DeathSeqState, savedCameraY) == 0x8);
STATIC_ASSERT(offsetof(DeathSeqState, savedCameraZ) == 0xC);
STATIC_ASSERT(offsetof(DeathSeqState, cameraDistance) == 0x10);
STATIC_ASSERT(offsetof(DeathSeqState, cameraDistanceTarget) == 0x14);
STATIC_ASSERT(offsetof(DeathSeqState, savedCameraYaw) == 0x18);
STATIC_ASSERT(offsetof(DeathSeqState, savedCameraPitch) == 0x1C);
STATIC_ASSERT(offsetof(DeathSeqState, flags) == 0x20);
STATIC_ASSERT(offsetof(DeathSeqState, pad21) == 0x21);
STATIC_ASSERT(sizeof(DeathSeqState) == DEATH_SEQ_STATE_SIZE);

int DeathSeq_getExtraSize(void);
int DeathSeq_getObjectTypeId(void);
void DeathSeq_free(GameObject* obj);
void DeathSeq_render(void);
void DeathSeq_hitDetect(void);
void DeathSeq_update(GameObject* obj);
void DeathSeq_init(GameObject* obj);
void DeathSeq_release(void);
void DeathSeq_initialise(void);

extern ObjectDescriptor gDeathSeqObjDescriptor;

#endif /* DLLS_OBJECTS_270_H_ */
