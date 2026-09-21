#ifndef H_MAIN_DLL_WISPBADDIE_BADDIE_H
#define H_MAIN_DLL_WISPBADDIE_BADDIE_H

#include "main/dll/dll_00C9_enemy.h"

void battleDroidUpdate(GameObject* obj, EnemyState* state);
void battleDroidUpdateAttack(GameObject* obj, EnemyState* state);
void battleDroidInit(GameObject* obj, EnemyState* state);

#endif /* H_MAIN_DLL_WISPBADDIE_BADDIE_H */
