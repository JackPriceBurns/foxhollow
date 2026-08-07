#ifndef MAIN_DLL_FRONT_FRONTEND_CONTROL_H_
#define MAIN_DLL_FRONT_FRONTEND_CONTROL_H_

#include "types.h"

typedef struct FrontendSaveSlot {
    char name[4];
    u8 completionPercent;
    u8 rankA;
    u8 rankB;
    u8 pad07;
    u32 playTimeSeconds;
    char* taskTexts[5];
    u8 isOccupied;
    u8 chaptersUnlocked;
    u8 pad22[0x24 - 0x22];
} FrontendSaveSlot;

void saveFileSelect_checkCheatCodes(void);
void saveSelect_drawText(int unused, int alpha);

#endif /* MAIN_DLL_FRONT_FRONTEND_CONTROL_H_ */
