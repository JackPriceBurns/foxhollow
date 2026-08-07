#ifndef MAIN_DLL_DOOR_H_
#define MAIN_DLL_DOOR_H_

#include "game/objects/object.h"
#include "game/objects/object_setup.h"

typedef struct DfpTargetBlockState DfpTargetBlockState;

typedef struct DfpTargetBlockCollisionPoints {
  u8 pointData[0x64];
  u8 pad64[0x68 - 0x64];
  s8 count;
} DfpTargetBlockCollisionPoints;

#endif /* MAIN_DLL_DOOR_H_ */
