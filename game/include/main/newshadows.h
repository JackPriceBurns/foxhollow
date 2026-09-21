#ifndef MAIN_NEWSHADOWS_H_
#define MAIN_NEWSHADOWS_H_

#include "types.h"
#include "main/texture.h"

void updateReflectionTextures(void);
void selectReflectionTexture(int id);
Texture* textureAlloc512(void);
void findSomething(void* needle);

/* extern-cleanup: defining-file public prototypes */
void newShadowsInitProceduralTextures(void);
f32 getNewShadowDistortionWaveOffset(void);
void drawReflectionTexture(void);
void newShadowsBeginFrame(void);
void freeNewShadowDistortionTexture(void);
void createNewShadowDistortionTexture(void);

Texture* getReflectionTexture1(void);
void getReflectionTexture2(Texture** out);
void getNewShadowCausticTexture(Texture** out);
void getNewShadowDiskTexture(Texture** out);
void newshadows_getReflectionScrollOffsets(f32* outScrollX, f32* outScrollY);
Texture* getNewShadowReflectionGradientTexture(void);
void getNewShadowSnowFlashTexture(Texture** out);
void getNewShadowHeatHazeTexture(Texture** out);
void getNewShadowRingTexture(Texture** out);
void getNewShadowLightningTexture(Texture** out);
void getNewShadowHeavyFogTexture(Texture** out);
void getNewShadowDistortionTexture(Texture** out);
void getNewShadowRadialTexture(Texture** out);
void getNewShadowRampTexture(Texture** out);
void loadNewShadowBumpTexture(int texMapId);
void selectWhirlpoolTexture(int id);
void getNewShadowNoiseTextureFrames(Texture*** tableOut, int* frameCountOut);
void loadNewShadowSmallReflectionTexture(int id);

#endif /* MAIN_NEWSHADOWS_H_ */
