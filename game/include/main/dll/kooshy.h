#ifndef H_MAIN_DLL_KOOSHY_H
#define H_MAIN_DLL_KOOSHY_H

#include "game/objects/object.h"

void kooshy_updateIdle(GameObject* obj, int state);
void kooshy_updateEngaged(GameObject* obj, int state);
void kooshy_init(int unused, int state);

#endif /* H_MAIN_DLL_KOOSHY_H */
