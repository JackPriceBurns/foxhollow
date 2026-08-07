#ifndef MAIN_DLL_PAUSEMENU_H_
#define MAIN_DLL_PAUSEMENU_H_

#include "types.h"
#include "game/objects/object.h"

void pauseMenuDraw(int arg1, int arg2, int arg3);
void pauseMenuDrawStatus(void);
void pauseMenuDrawStatusPage(GameObject* player);
void pauseMenuUpdate(void);
void pauseMenuDrawSideRails(s32 alpha);

#endif /* MAIN_DLL_PAUSEMENU_H_ */
