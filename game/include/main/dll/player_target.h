#ifndef MAIN_DLL_PLAYER_TARGET_H_
#define MAIN_DLL_PLAYER_TARGET_H_

#include "game/objects/object.h"
GameObject* playerGetTargetObject(GameObject* playerObj);

static inline uintptr_t Player_GetTargetObject(int playerObj)
{
    return (uintptr_t)playerGetTargetObject((GameObject*)(playerObj));
}

#endif
