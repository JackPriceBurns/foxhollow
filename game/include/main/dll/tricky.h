#ifndef MAIN_DLL_TRICKY_H_
#define MAIN_DLL_TRICKY_H_

void gameUiLoadResources(void);
void pauseMenuTextDrawFn(int x0, int y0, int x1, int y1, f32 u0, f32 v0, f32 u1, f32 v1);
void hudDrawAirMeter(void);
void fearTestMeterDraw(void);
void gameUiSetupTexturedQuadTev(void* this, u8 a, s16 b, int c);
void pauseMenuSetHoloTransform(f32 f1, f32 f2, f32 f3, f32 f4, s16 a, s16 b, s16 c);
void arwingHudSetVisible(u32 mode);

#include "global.h"

enum AButtonIcon {
    A_BUTTON_ICON_THROW_CARRYABLE = 4,
    A_BUTTON_ICON_PLACE_CARRYABLE = 5,
    A_BUTTON_ICON_HINT = 7,
    A_BUTTON_ICON_TALK_NPC = 8,
    A_BUTTON_ICON_TALK_OBJECT = 9,
    A_BUTTON_ICON_CONTEXT_B = 0x0F
};

void setAButtonIcon(int icon);
void setBButtonIcon(int icon);
void showFuelCellTokenConfirmMenu(void);
void drawViewFinderHud(void);
void hudSetMagicCostPreview(u8 value);
void fearTestMeterSetFadeIn(u32 value);
void setHudForceShowMask(u8 value);
void showDeathMenu(void);
void resetYbutton(void);
int getYButtonItem(s16* out);
void gameUiResetMenuState(void);
void setTrickyHudShowNearestInfo(u8 value);

#endif /* MAIN_DLL_TRICKY_H_ */
