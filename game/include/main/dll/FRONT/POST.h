#ifndef MAIN_DLL_FRONT_POST_H_
#define MAIN_DLL_FRONT_POST_H_

#include "types.h"
#include "game/objects/object.h"

typedef struct PostControl {
  u8 pad0[0x10];
  f32 primary[3];
  u8 secondary[0x5a0];
  s16 events[0x1e];
  int blocked;
  u8 pad5fc[0x10];
  s16 eventState;
  s16 yawLimit;
  u8 contactAnim;
  u8 flags;
} PostControl;

int moveLibTurnToFaceTarget(GameObject* obj, GameObject* targetObj, int* turning, PostControl* control, float* turnSpeed,
                       short* moves, float* targetPos);

#endif /* MAIN_DLL_FRONT_POST_H_ */
