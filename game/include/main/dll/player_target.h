#ifndef MAIN_DLL_PLAYER_TARGET_H_
#define MAIN_DLL_PLAYER_TARGET_H_

#include "game/objects/object.h"
int playerGetTargetObject(GameObject* playerObj);

static inline int Player_GetTargetObject(int playerObj)
{
    return playerGetTargetObject((GameObject*)(playerObj));
}

#endif
