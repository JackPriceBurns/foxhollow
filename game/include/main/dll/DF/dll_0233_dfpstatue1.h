#ifndef MAIN_DLL_DF_DLL_0233_DFPSTATUE1_H_
#define MAIN_DLL_DF_DLL_0233_DFPSTATUE1_H_

#include "types.h"
#include "main/objseq.h"
#include "game/objects/object_setup.h"
#include "dlls/object_descriptor.h"
#include "game/objects/object.h"

typedef struct DfpStatue1State {
  s16 triggerSfxId;
  s16 loopSfxId;
  s16 loopSfxStopTimer;
  u8 loopActive;
  u8 effectPairCount;
  u8 stateFlags;
} DfpStatue1State;

typedef struct DfpStatue1MapData {
  ObjPlacement base;
  s8 yawByte;
  u8 effectPairCount;
  u8 pad1A[0x1E - 0x1A];
  s16 triggerSfxId;
  s16 loopSfxId;
} DfpStatue1MapData;

STATIC_ASSERT(offsetof(DfpStatue1MapData, yawByte) == 0x18);
STATIC_ASSERT(offsetof(DfpStatue1MapData, effectPairCount) == 0x19);
STATIC_ASSERT(offsetof(DfpStatue1MapData, triggerSfxId) == 0x1E);
STATIC_ASSERT(offsetof(DfpStatue1MapData, loopSfxId) == 0x20);

u32 dfpstatue1_SeqFn(GameObject* obj, u32 unused, ObjSeqState *animUpdate);
void dfpstatue1_updateState(GameObject *obj);

extern ObjectDescriptor gDfpstatue1ObjDescriptor;

int DFP_Statue1_getExtraSize(void);
int DFP_Statue1_getObjectTypeId(void);
void DFP_Statue1_free(void);
void DFP_Statue1_render(void);
void DFP_Statue1_hitDetect(void);
void DFP_Statue1_update(GameObject *obj);
void DFP_Statue1_init(GameObject *obj, DfpStatue1MapData *mapData);
void DFP_Statue1_release(void);
void DFP_Statue1_initialise(void);

#endif /* MAIN_DLL_DF_DLL_0233_DFPSTATUE1_H_ */
