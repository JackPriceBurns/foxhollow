#ifndef MAIN_NEWSHADOWS_H_
#define MAIN_NEWSHADOWS_H_

#include "types.h"
#include "main/texture.h"

void updateReflectionTextures(void);
void selectReflectionTexture(int id);
void* textureAlloc512(void);
void findSomething(void* needle);


/* extern-cleanup: defining-file public prototypes */
void newShadowsInitProceduralTextures(void);
f32 getNewShadowDistortionWaveOffset(void);
void drawReflectionTexture(void);
void newShadowsBeginFrame(void);
void freeNewShadowDistortionTexture(void);
void createNewShadowDistortionTexture(void);

uintptr_t getReflectionTexture1(void);
void getReflectionTexture2(uintptr_t* out);
void getNewShadowCausticTexture(uintptr_t* out);
void getNewShadowDiskTexture(uintptr_t* out);
void newshadows_getReflectionScrollOffsets(f32* outScrollX, f32* outScrollY);
uintptr_t getNewShadowReflectionGradientTexture(void);
void getNewShadowSnowFlashTexture(uintptr_t* out);
void getNewShadowHeatHazeTexture(Texture** out);
void getNewShadowRingTexture(Texture** out);
void getNewShadowLightningTexture(Texture** out);
void getNewShadowHeavyFogTexture(Texture** out);
void getNewShadowDistortionTexture(Texture** out);
void getNewShadowRadialTexture(Texture** out);
void getNewShadowRampTexture(uintptr_t* out);
void loadNewShadowBumpTexture(int texMapId);
void selectWhirlpoolTexture(int id);
void getNewShadowNoiseTextureFrames(Texture*** tableOut, int* frameCountOut);
void loadNewShadowSmallReflectionTexture(int id);

#endif /* MAIN_NEWSHADOWS_H_ */
