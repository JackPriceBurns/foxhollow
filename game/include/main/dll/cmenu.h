#ifndef MAIN_DLL_CMENU_H_
#define MAIN_DLL_CMENU_H_

#include "types.h"

struct CMenuItemDef;
struct GameObject;
int cMenuSetItems(struct CMenuItemDef* items, char useTricky);
int cMenuRingModelRenderFn(struct GameObject* obj, int block, int idx);
void drawTrickyHudOverlay(int obj, int unused1, int unused2);
int cMenuRingIconRenderFn(struct GameObject* obj, int block, int idx);
void hudDrawCMenu(int p1, int p2, int p3);
void cMenuUpdateRingRotation(void);

#endif /* MAIN_DLL_CMENU_H_ */
