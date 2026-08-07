#ifndef MAIN_DLL_DLL_0027_EFFECT14_H_
#define MAIN_DLL_DLL_0027_EFFECT14_H_

#include "types.h"
#include "game/objects/object.h"
#include "main/dll/partfx_interface.h"

int Effect14_spawnObject(GameObject* obj, int id, PartFxSpawnParams* src, u32 flags, u8 srcByte, u16* extraArgs);
void Effect14_func05_nop(void);
void Effect14_func03_nop(void);
void Effect14_release(void);
void Effect14_initialise(void);

#endif /* MAIN_DLL_DLL_0027_EFFECT14_H_ */
