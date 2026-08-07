#ifndef DLLS_OBJECTS_493_SB_FIREBALL_H_
#define DLLS_OBJECTS_493_SB_FIREBALL_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"

typedef struct SBFireBallState {
    GameObject* target; /* 0x00: copied from obj->userData2 */
    s16 age;            /* 0x04: frames since launch */
    u8 padding06[2];
    f32 velocityX; /* 0x08 */
    f32 velocityY; /* 0x0C */
    f32 velocityZ; /* 0x10 */
    s8 launched;   /* 0x14 */
    u8 padding15[3];
} SBFireBallState;

STATIC_ASSERT(offsetof(SBFireBallState, target) == 0x00);
STATIC_ASSERT(offsetof(SBFireBallState, age) == 0x04);
STATIC_ASSERT(offsetof(SBFireBallState, padding06) == 0x06);
STATIC_ASSERT(offsetof(SBFireBallState, velocityX) == 0x08);
STATIC_ASSERT(offsetof(SBFireBallState, velocityY) == 0x0C);
STATIC_ASSERT(offsetof(SBFireBallState, velocityZ) == 0x10);
STATIC_ASSERT(offsetof(SBFireBallState, launched) == 0x14);
STATIC_ASSERT(offsetof(SBFireBallState, padding15) == 0x15);
STATIC_ASSERT(sizeof(SBFireBallState) == 0x18);

int SB_FireBall_getExtraSize(void);
int SB_FireBall_getObjectTypeId(void);
void SB_FireBall_free(GameObject* obj);
void SB_FireBall_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void SB_FireBall_hitDetect(GameObject* obj);
void SB_FireBall_update(GameObject* obj);
void SB_FireBall_init(GameObject* obj);
void SB_FireBall_release(void);
void SB_FireBall_initialise(void);

extern ObjectDescriptor gSB_FireBallObjDescriptor;

#endif /* DLLS_OBJECTS_493_SB_FIREBALL_H_ */
