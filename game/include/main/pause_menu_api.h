#ifndef MAIN_PAUSE_MENU_API_H_
#define MAIN_PAUSE_MENU_API_H_

#include "types.h"

extern u8 pauseMenuState;

u8 pauseMenuGetState(void);
void pauseMenuUpdateFadeAndBack(void);
void pauseMenuRunSubmenu(int submenu);
void pauseMenuDrawText(int unused1, int unused2, int unused3);
void gameTextFadeOut(void);
void pauseMenuSetupTitle(s32 fadeTarget, u8 index, u8 flags, u8 arg3);

extern u8 gTimeListPromptSelection;
extern s16 gMinimapAreaNameAlpha;
extern u8 gMinimapHelpTextActive;
extern s16 gMinimapAreaNameId;

extern u8 gDummy39Countdown;
extern u8* gDummy39Texture;

#endif /* MAIN_PAUSE_MENU_API_H_ */
