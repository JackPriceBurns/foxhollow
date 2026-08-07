#ifndef MAIN_DLL_DLL_0024_EFFECT11_H_
#define MAIN_DLL_DLL_0024_EFFECT11_H_

#include "types.h"
#include "main/dll/partfx_interface.h"

int Effect11_spawnObject(s16* obj, int id, PartFxSpawnParams* src, u32 flags, u8 srcByte);
void Effect11_func05_nop(void);
void Effect11_func03_nop(void);
void Effect11_release(void);
void Effect11_initialise(void);

#endif /* MAIN_DLL_DLL_0024_EFFECT11_H_ */
