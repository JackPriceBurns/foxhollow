#ifndef MUSYX_DATA_REF_H_
#define MUSYX_DATA_REF_H_

#include "global.h"

typedef struct SAMPLE_HEADER {
    u32 info;
    u32 length;
    u32 loopOffset;
    u32 loopLength;
} SAMPLE_HEADER;

typedef struct SDIR_DATA {
    u16 id;
    u16 ref_cnt;
    u32 offset;
    void* addr;
    SAMPLE_HEADER header;
    void* extraData;
} SDIR_DATA;

typedef struct SDIR_TAB {
    SDIR_DATA* data;
    void* base;
    u16 numSmp;
    u16 res;
} SDIR_TAB;

typedef struct DATA_TAB {
    void* data;
    union {
        u16 id;
        u16 key;
    };
    u16 refCount;
} DATA_TAB;

typedef struct LAYER_TAB {
    void* data;
    u16 id;
    u16 num;
    u16 refCount;
    u16 reserved;
} LAYER_TAB;

typedef struct MAC_MAINTAB {
    u16 num;
    u16 subTabIndex;
} MAC_MAINTAB;

typedef DATA_TAB MAC_SUBTAB;

typedef struct FX_TAB {
    u16 id;
    u16 macro;
    u8 maxVoices;
    u8 priority;
    u8 volume;
    u8 panning;
    u8 key;
    u8 vGroup;
} FX_TAB;

typedef struct FX_GROUP {
    u16 gid;
    u16 fxNum;
    FX_TAB* fxTab;
} FX_GROUP;

typedef DATA_TAB DataRefEntry;
typedef LAYER_TAB DataLayerRef;
typedef SDIR_DATA DataSampleDirEntry;
typedef SDIR_TAB DataSampleDirBucket;
typedef FX_GROUP DataFXGroupRef;
typedef MAC_MAINTAB DataMacroBucket;
typedef FX_TAB DataFXEntry;
typedef struct {
    FX_TAB key;
    u8 pad[2];
} DataFXSearchKey;

extern u16 dataSmpSDirNum;
extern u16 dataCurveNum;
extern u16 dataKeymapNum;
extern u16 dataLayerNum;
extern u16 dataMacTotal;
extern u16 dataFXGroupNum;

#endif /* MUSYX_DATA_REF_H_ */
