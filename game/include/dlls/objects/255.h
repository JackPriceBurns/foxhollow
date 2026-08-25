#ifndef DLLS_OBJECTS_255_H_
#define DLLS_OBJECTS_255_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "dlls/objects/237.h"

#define MAGICGEM_DEF_GREEN  0x2C4
#define MAGICGEM_DEF_RED    0x2CD
#define MAGICGEM_DEF_YELLOW 0x2CE
#define MAGICGEM_DEF_BLUE   0x2CF

#define MAGICGEM_FLAG_BURST1        0x01 /* First timed particle-burst phase. */
#define MAGICGEM_FLAG_SETTLED       0x02 /* At rest after repeated bounces. */
#define MAGICGEM_FLAG_BURST2        0x04 /* Second timed particle-burst phase. */
#define MAGICGEM_FLAG_COLLECTED     0x08 /* Collected/despawning. */
#define MAGICGEM_FLAG_AMBIENT_FX    0x10 /* Proximity effects are active. */
#define MAGICGEM_FLAG_CLAIMED       0x20 /* Pickup message and game-bit claim sent. */
#define MAGICGEM_FLAG_COLLECT_LATCH 0x40 /* Collection path has been taken. */

#define MAGICGEM_FLAG_MOTION_MASK (MAGICGEM_FLAG_BURST1 | MAGICGEM_FLAG_SETTLED)
#define MAGICGEM_FLAG_BURST_MASK  (MAGICGEM_FLAG_BURST1 | MAGICGEM_FLAG_BURST2)

typedef struct MagicGemState {
    CurvesCollisionState path;
    f32 collectRadius;
    f32 burstTimer;
    u16 burstEffectId;
    u16 ambientEffectId;
    s16 sfxId;
    s16 unk276;
    s16 ambientTimer;
    u8 flags;
    u8 bounceCount;
    u8 mode;
    u8 pad27D[3];
    s16 pickupMsgArg;
    u8 pad282[6];
} MagicGemState;

int MagicDust_getExtraSize(void);
void MagicDust_free(GameObject* obj);
void MagicDust_render(GameObject* obj, int fwdArg2, int fwdArg3, int fwdArg4, int fwdArg5, s8 unusedVisible);
void MagicDust_update(GameObject* obj);
void MagicDust_init(GameObject* obj, CollectibleSetup* placement);

extern ObjectDescriptor gMagicGemObjDescriptor;

#endif /* DLLS_OBJECTS_255_H_ */
