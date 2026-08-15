#ifndef MAIN_DLL_DUSTER_H_
#define MAIN_DLL_DUSTER_H_

#include "main/dll/dll_00C9_enemy.h"
#include "game/objects/object.h"
#include "main/dll/duster_api.h"

void rachnopUpdateApproach(GameObject* obj, void* state);
void rachnopUpdateAttack(GameObject* obj, void* state);
void rachnopUpdateIdle(GameObject* obj, void* state);
void spittingEbaUpdateIdle(GameObject* obj, void* state);
void spittingEbaUpdateEngaged(GameObject* obj, void* state);

void rachnopInit(GameObject* unused, void* state);
void spittingEbaSpawnPollen(GameObject* obj,void* state);
void spittingEbaUpdateTimeOfDay(GameObject* obj,void* state);
void spittingEbaInit(GameObject* unused, void* state);
void wbInit(GameObject* unused, void* state);

enum
{
    DUSTER_WALL_PLANE_OFFSET = offsetof(EnemyState, wallPlane),
    DUSTER_WALL_NORMAL_X_OFFSET = offsetof(EnemyState, wallPlane.normal[0]),
    DUSTER_WALL_NORMAL_Y_OFFSET = offsetof(EnemyState, wallPlane.normal[1]),
    DUSTER_WALL_NORMAL_Z_OFFSET = offsetof(EnemyState, wallPlane.normal[2])
};

#endif /* MAIN_DLL_DUSTER_H_ */
