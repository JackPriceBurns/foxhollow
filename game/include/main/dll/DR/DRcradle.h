#ifndef MAIN_DLL_DR_DRCRADLE_H_
#define MAIN_DLL_DR_DRCRADLE_H_

#include "types.h"
#include "game/objects/object.h"
#include "dlls/object_descriptor.h"

extern ObjectDescriptor24 gSnowBikeObjDescriptor;

void SnowBike_func17(void);
void SnowBike_func16(void);
int SnowBike_getDismountSide(void);
int SnowBike_canDismount(void);
int SnowBike_getExtraSize(void);
int SnowBike_getObjectTypeId(void);
u8 SnowBike_getMountSide(GameObject *obj);
s32 SnowBike_getRacePosition(GameObject *obj);
s32 SnowBike_getMountState(GameObject *obj);

#endif /* MAIN_DLL_DR_DRCRADLE_H_ */
