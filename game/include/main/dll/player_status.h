#ifndef MAIN_DLL_PLAYER_STATUS_H_
#define MAIN_DLL_PLAYER_STATUS_H_

#include "game/objects/object.h"
int playerGetCurMagic(GameObject* playerObj);
int playerGetMaxMagic(GameObject* playerObj);
int playerGetMaxHealth(GameObject* playerObj);
int playerGetCurHealth(GameObject* playerObj);
int playerStatusIsPositive(GameObject* playerObj);
int playerHasRevived(GameObject* playerObj);
void playerSetIsDead(GameObject* playerObj, int isDead);
void saveSetOverrideHealth(int health);

static inline int Player_GetCurrentMagic(uintptr_t playerObj)
{
    return playerGetCurMagic((GameObject*)playerObj);
}

static inline int Player_GetMaxMagic(uintptr_t playerObj)
{
    return playerGetMaxMagic((GameObject*)(playerObj));
}

static inline int Player_GetMaxHealth(uintptr_t playerObj)
{
    return playerGetMaxHealth((GameObject*)(playerObj));
}

static inline int Player_GetCurrentHealth(uintptr_t playerObj)
{
    return playerGetCurHealth((GameObject*)(playerObj));
}

#endif
