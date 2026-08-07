#ifndef MAIN_DLL_DLL_47_H_
#define MAIN_DLL_DLL_47_H_

#include "types.h"
#include "main/dll/FRONT/frontend_control.h"

/* extern-cleanup: defining-file public prototypes */
void saveSelectGoToChooseSlot(int arg);

extern u8 saveFileSelect_debugCheatProgress;
extern u8 saveFileSelect_saveCheatProgress;
extern u8 saveFileSelect_cheatInputTimer;
extern s8 saveFileSelect_currentSlotIndex;
extern u8 saveFileSelect_saveDirty;
extern FrontendSaveSlot* saveFileSelect_saveSlotsBase;
extern FrontendSaveSlot* saveFileSelect_saveSlots;
extern u16 saveFileSelect_debugCheatSequence[6];
extern u16 saveFileSelect_slotCheatSequence[6];
extern char sFrontendCompletionPercentFormat[5];
extern char sFrontendSingleDigitFormat[4];
extern char sFrontendTimeFormat[14];

#endif /* MAIN_DLL_DLL_47_H_ */
