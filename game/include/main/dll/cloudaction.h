#ifndef MAIN_DLL_CLOUDACTION_H_
#define MAIN_DLL_CLOUDACTION_H_

#include "global.h"
#include "main/dll/cloudaction_interface.h"

typedef struct CloudEnvTbl {
    s32 mainCloudAssetIds[5];
    s32 upperCloudAssetIds[4];
    s32 lowerCloudAssetIds[5];
} CloudEnvTbl;

STATIC_ASSERT(sizeof(CloudEnvTbl) == 0x38);

extern CloudEnvTbl gCloudActionEnvTbl;
extern f32 gCloudActionGlareQuadSize[2];
extern s32 lbl_803DB618[2];

void cloudaction_func08_nop(f32 x, f32 y, f32 z, int intensity);
void cloudaction_func09_nop(int enabled);
void cloudaction_free(void);
void cloudaction_scrollTexture(void);
void cloudaction_onMapSetup(void);
void cloudaction_update(void* p1, void* p2, void* state, int p4, u16 val);
void cloudaction_release(void);
void cloudaction_initialise(void);
void renderClouds(int a, int b, int c, int d);
void* cloudGetLayerTexture(f32* outOffsetS, f32* outOffsetT);

#endif /* MAIN_DLL_CLOUDACTION_H_ */
