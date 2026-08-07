#ifndef MAIN_DLL_BADDIEWHIRLPOOL_H_
#define MAIN_DLL_BADDIEWHIRLPOOL_H_

#include "main/dll/dll_00C9_enemy.h"

typedef struct GameObject GameObject;

void iceBaddie_enterWhirlpoolGroup(GameObject* obj, EnemyState* state);
void iceBaddie_leaveWhirlpoolGroup(GameObject* obj, EnemyState* state);
void baddie_initWhirlpoolState(int* obj, EnemyState* state);

#endif /* MAIN_DLL_BADDIEWHIRLPOOL_H_ */
