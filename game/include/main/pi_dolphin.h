#ifndef MAIN_PI_DOLPHIN_H_
#define MAIN_PI_DOLPHIN_H_

#include "types.h"
#include "dolphin/gx/GXStruct.h"
#include "dolphin/gx/GXFifo.h"
#include "dolphin/os/OSThread.h"
#include "dolphin/os/OSStopwatch.h"
#include "main/model_engine.h"

void piRomLoadSection(int romOffset, int mapIndex, void* destBuf);
void mapsLoadTabOffsets(int firstWord, s32* offsets, int count);

/* extern-cleanup: defining-file public prototypes */
void setDisplayCopyFilter(void);
void gxDisableGpuHangRecovery(void);
void allocSomething32bytes(void);
void initViewport(void);
void tvInit(void);
void loadModelsBin(int fileOffset, int* animCount, int* headerSize, int* amapFlag, int* dataLen, int id);
void* fileLoad(int id, int heap);
void videoInit(void* rmode, int arg);
int fileLoadToBuffer(int id, void* buffer);
u8 initLoadFiles(void);
void videoBlackScreenForFrames(int frameCount);
void checkLoadBlock(int a, int* compressedSize, int* decompressedSize);

extern void** gDvdFileInfoPool;
extern GXRenderModeObj* gRenderModeObj;
extern s32 gObjLevelLockSlots[2];

#include "main/mldf_fileid.h"

extern volatile int gAssetLoadInFlightFlags;
extern s16 gDefragDelayFrames;
extern int gPendingDvdReadCount;
extern u8 gVideoRetracePending;
extern int gModelsArchiveLoadCount;
extern void* lbl_803DCD10;
extern char* lbl_803DCD08;
extern s16 gForceNextLoadSync;
extern u8 gLoadFilesInitDone;

extern volatile int gAssetLoadCompletedFlags;
extern u8 lbl_803DCD00;
extern int lbl_803DCCFC;
extern u8 lbl_803DCCF8;
extern int lbl_803DCCF4;
extern void* externalFrameBuffer0;
extern void* externalFrameBuffer1;
extern u32 gGxFifoSize;
extern char* lbl_803DCCE0;
extern void* gGxFifoBase;
extern void* renderFrameBuffer;
extern void* displayFrameBuffer;
extern char gVideoFlipWaitQueue;
extern int gDispCopyYScaleLines;
extern GXColor gEfbCopyClearColor;
extern u8 gDispCopyFilterWeights[8];
extern char gVideoFlipQueueBuffer[10 * 3 * sizeof(void*)];
extern f32 gFrameElapsedMs;
extern f32 gFrameStepRemainder;
extern u8 gGpuHangRecoveryEnabled;
extern volatile int gGpuStallRetraceCount;
extern u8 gGxBreakPtEnabled;
extern u8 gVideoBlackScreenFrameCount;
extern u16 gGxDrawSyncToken;
extern u32 gRomListLoadInFlight;
extern int gForceLoadImmediately;
extern int sMapFileNameIndexRemapTable[];
extern GXFifoObj* gGxFifoObj;
extern OSThread* gVideoWaitThread;
extern OSStopwatch gFrameStopwatch;
extern RingBufferQueue gVideoFlipQueue;
extern u8 gLoadingScreenTextures[];
extern char lbl_8030C6A0[0x1B8];

#include "main/pi_frame.h"
#include "main/pi_dolphin_path.h"

double SeekTwiceBeforeRead(void);
void* loadAndDecompressDataFile(int fileId, void* destBuf, int offsetFlags, u32 length, int* sizeOut, int entryIndex,
                                u32 flagBits);
int mapGetDirIdx(int idx);
u8 isHeavyFogEnabled(void);
void disableHeavyFog(void);
void enableHeavyFog(f32 top, f32 bottom, f32 depthScale, f32 depthOffset, f32 worldScale, u8 mode);
void addWavyCausticTevStage(void);
void addTexModulateReg2Stage(void);
void addShadowFalloffTevStages(void);
void addSmallReflectionTevStage(void);
void videoSetEfbCopyClearColor(u8 r, u8 g, u8 b);
void addYUVVideoTevStages(void* yTexture, void* uTexture, void* vTexture, s16 width, s16 height);
void setHeatEffectParams(u8 alpha, f32 scale);
void* Shader_getLayer(void* shader, int layerIdx);
void setupCausticBaseTevStages(void* viewMtx);
void setupHeatShimmerTevStages(char* shader);
void addWarpedRingTevStages(void);
void addVertexAlphaDimStage(u8* color);
void renderHeavyFog(void* fogColor);
void addLightColorModulateStage(int* color);
void addAccumulatedLightBlendStages(void);
void addAccumulatedLightModulateStage(void);
void addPointLightAccumStages(f32 scale, int* color, f32* position);
void addFirstPointLightStages(f32 scale, int* color, f32* position, u8* chanColor);
void addPointLightDirectStages(f32 scale, int* color, f32* position);
void addSignedOverlayTexStage(u8* texture, void* texMtx, u8* color);
void addSphereMapLitStages(void* texture, f32* texMtx, void* color, int unused);
void addTexLayerStagesLit(void* texture, void* texMtx);
void addLitColorStage(u8 mode);
int addEnvMapBumpStages(void* texture, int stageCount, u8 mode, void* indirectTexture);
void addSphereMapTexStage(void* textureRef, u8 intensity);
void addLightTexReg2Stage(void* textureRef, u8 hasBaseTexture, u8 mode);
void addAlphaLitColorReg2Stage(u8 mode);
void addCastShadowTevStages(u8* objectInstance);
void addProjectedLightTevStage(u8* texture, void* texMtx, int stageMode, int componentMode, int variant);
void addEnvMapTexCoord(int scale);
void addWarpedNoiseTevStages(void* texture, void* texMtx);
void addRenderOpFadeStage(void* renderOp);

void mapsBinGetRomlistSize(int idx, int* out1, int* out2, int* out3, int p5);

extern s16 gObjMapBlockInfo[];
extern s16 sMapFileNameAdjacencyTable[];
extern char sAssetIndexOverflowError[];

extern uintptr_t gResourceFileBuffers[];
extern u32 gResourceFileSizes[];

#endif /* MAIN_PI_DOLPHIN_H_ */
