#ifndef MAIN_DLL_SHIP_BATTLE_API_H_
#define MAIN_DLL_SHIP_BATTLE_API_H_

#include "game/objects/object.h"

int SB_CloudRunner_getTargetMode(GameObject* obj);
void SB_CloudRunner_getSpawnPos(GameObject* obj, f32* x, f32* y, f32* z);

#endif /* MAIN_DLL_SHIP_BATTLE_API_H_ */
