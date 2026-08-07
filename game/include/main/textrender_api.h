#ifndef MAIN_TEXTRENDER_API_H_
#define MAIN_TEXTRENDER_API_H_

#include "types.h"

struct GameTextDef;
struct GameTextBox;

typedef struct GameTextSlot {
    int opcode;
    int arg0;
    int arg1;
    int arg2;
    int arg3;
} GameTextSlot;

extern int lbl_803DC9C8;
extern u8 gGameTextFontIsSjis;
extern GameTextSlot lbl_8033A540[];

#define gGameTextCommandCount lbl_803DC9C8
#define gGameTextCommandSlots lbl_8033A540

void subtitleStop(void);
void subtitleStart(int x);
void gameTextMeasureString(u8* str, f32 scale, f32* outW, f32* outZero, f32* outMaxAdv, f32* outMaxH, int glyphLang);
void subtitleUpdateAndDraw(int mode);
int subtitleIsActive(void);
int setSubtitlesEnabled(int enabled);
void* gameTextGetPhrase(int textId, int phraseIndex);
void* gameTextGetStr(int textId);
void gameTextResetCursor(int flags);
void gameTextSetCursor(u16 x, u16 y, int flags);
void gameTextSetDrawFunc(void* drawFunc);
void gameTextSetWindow(u8* textBox);
void gameTextSetWindowById(int boxId);
f32 gameTextGetTimer(void);
void gameTextRun(void);
void* gameTextGet(int textId);
void mainLoopDoGameText(void);
void gameTextLoadDir(int dirId);
int gameTextSaveDir(int x);
void gameTextLoadForCurMap(int sourceId);
void gameTextLoadTaskText(int taskId);
int getCurGameText(void);
int getCurLanguage(void);
void gameTextInit(void);
void gameTextInitRendererState(void);
void subtitleInit(void);
void gameTextInitBoxTextures(void);
void gameTextBuildSystemFontAtlas(void);
void subtitleFreeBoxTextures(int mode);
void subtitleLoadBoxTextures(int mode);
void gameTextDrawBox(struct GameTextDef* def, int box, struct GameTextBox* slot);
void textRenderStr(char* str, struct GameTextBox* slot, f32 x, f32 y, f32 lineH, int mode);

void gameTextSetWindowStrPos(int idx, int x, int y);

#endif /* MAIN_TEXTRENDER_API_H_ */
