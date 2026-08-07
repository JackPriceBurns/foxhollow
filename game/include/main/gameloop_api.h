#ifndef MAIN_GAMELOOP_API_H_
#define MAIN_GAMELOOP_API_H_

#include "game/objects/object_fwd.h"
#include "types.h"
#include "main/gameloop_gamebit_api.h"
#include "main/hud_visibility_api.h"

extern u8 gSaveGameEnabled;
typedef struct LinkInterface LinkInterface;
extern LinkInterface* gTitleMenuLinkInterface;

int main(int argc, char** argv);
int getGameState(void);
int getScreenBlankFrameCount(void);
int TriggSetpShouldUnload(void);
void requestGalleonBattleMusic(void);
void checkReset(void);
void setShouldResetNextFrame(int reset);
void mapReload(void);
void mapLoadByCoords(f32 x, f32 y, f32 z, int layer);
void setGameState(int state);
void cutsceneExit(void);
void cutsceneEnterExit(int entering, int affectSounds);
void cutsceneFadeInOut(int mode);
void setTimeStop(int frames);
void doNothing_onSaveSelectScreenExit(void);
void requestKrazoaShrineMusic(void);
int getButtonObjects(GameObject*** objectsOut);
int cacheAllocAndCopy(u8* srcAddress, u32 size, u32* cacheCursor, u32* outEnd, u32 limit);
void nop_onUnloadMap(int a, int b);
void doNothing_startOfFrame(void);
void crash(int a, int b, int c, int d, int e, int f, int g, int h);

void addButtonObject(GameObject* obj);

void removeButtonObject(GameObject* object);

void blankScreen(int frames);

#endif /* MAIN_GAMELOOP_API_H_ */
