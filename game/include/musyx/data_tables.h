#ifndef MUSYX_DATA_TABLES_H_
#define MUSYX_DATA_TABLES_H_

#include "types.h"
#include "musyx/data_ref.h"
#include "musyx/dsp_voice.h"

/* MusyX synthdata.c - see src/musyx/runtime/data_tables.c */

s32 dataInsertLayer(u16 cid, void *layerdata, u16 size);
s32 dataRemoveLayer(u16 sid);
s32 dataInsertCurve(u16 cid, void *curvedata);
s32 dataRemoveCurve(u16 sid);
s32 dataAddSampleReference(u16 sid);
s32 dataRemoveSampleReference(u16 sid);
u32 dataInsertSDir(SDIR_DATA *sdir, void *smp_data);
u32 dataInsertFX(u16 gid, FX_TAB *fx, u16 fxNum);
s32 dataInsertMacro(u16 mid, void *macroaddr);
s32 dataRemoveMacro(u16 mid);
void *dataGetMacro(u16 mid);
void *dataGetCurve(u16 cid);
void *dataGetKeymap(u16 cid);
void *dataGetLayer(u16 cid, u16 *n);
FX_TAB *dataGetFX(u16 fid);
s32 dataGetSample(u16 sampleId, SAMPLE_INFO *sampleInfo);
void dataInit(u32 smpBase, u32 smpLength);
void dataExit(void);

#endif /* MUSYX_DATA_TABLES_H_ */
