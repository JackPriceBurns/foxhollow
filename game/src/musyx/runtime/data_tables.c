/*
 * MusyX synthdata.c -- sound data table management.
 * Matches the public MusyX runtime source (see PrimeDecomp/mariopartyrd
 * synthdata.c); SFA's build uses sndBegin/sndEnd for the IRQ guard.
 */
#include "musyx/data_ref.h"
#include "musyx/dsp_voice.h"
#include "musyx/hw_aram.h"
#include "musyx/hw_samplemem.h"
#include "musyx/sal_dsp.h"
#include "musyx/data_tables.h"
#include "musyx/snd_service.h"
#include "musyx/synth_jobs.h"
#include "musyx/endian.h"
#include <string.h>

LAYER_TAB* dataGetLayer_result;
DATA_TAB* dataGetKeymap_result;
DATA_TAB dataGetKeymap_key;
DATA_TAB* dataGetCurve_result;
DATA_TAB dataGetCurve_key;
SAMPLE_HEADER* dataGetSample_sheader;
SDIR_DATA* dataGetSample_result;
MAC_SUBTAB* dataGetMacro_result;
MAC_SUBTAB dataGetMacro_key;
s32 dataGetMacro_bucket;
s32 dataGetMacro_main;
u16 dataFXGroupNum;
u16 dataMacTotal;
u16 dataLayerNum;
u16 dataKeymapNum;
u16 dataCurveNum;
u16 dataSmpSDirNum;

DataFXSearchKey dataGetFXSearchKey;
LAYER_TAB dataGetLayerSearchKey;
SDIR_DATA dataGetSampleSearchKey;

static SDIR_TAB dataSmpSDirs[128];
static DATA_TAB dataCurveTable[2048];
static DATA_TAB dataKeymapTable[256];
static LAYER_TAB dataLayerTable[256];
static MAC_MAINTAB dataMacroBucketTable[512];
static MAC_SUBTAB dataMacroTable[2048];
static FX_GROUP dataFXGroupTable[128];

typedef struct DataKeymapEntry
{
    u16 id;
    s8 transpose;
    u8 panning;
    s16 prioOffset;
    u8 reserved[2];
} DataKeymapEntry;

typedef struct DataLayerEntry
{
    u16 id;
    u8 keyLow;
    u8 keyHigh;
    s8 transpose;
    u8 volume;
    s16 prioOffset;
    u8 panning;
    u8 reserved[3];
} DataLayerEntry;

static const void* dataSDirRaw[128];
static SDIR_DATA* dataSDirNative[128];
static u16 dataSDirNativeCount;

static SDIR_DATA* dataConvertSDir(const void* rawData)
{
    const u8* raw = rawData;
    SDIR_DATA* converted;
    u32 count;
    u32 i;

    for (i = 0; i < dataSDirNativeCount; i++)
    {
        if (dataSDirRaw[i] == rawData)
        {
            return dataSDirNative[i];
        }
    }

    for (count = 0; musyxReadBE16(raw + count * 0x20) != 0xFFFF; count++)
    {
    }

    converted = salMalloc((count + 1) * sizeof(SDIR_DATA));
    if (converted == NULL)
    {
        return NULL;
    }

    for (i = 0; i < count; i++)
    {
        const u8* source = raw + i * 0x20;
        u32 extraOffset = musyxReadBE32(source + 0x1c);
        converted[i].id = musyxReadBE16(source);
        converted[i].ref_cnt = musyxReadBE16(source + 2);
        converted[i].offset = musyxReadBE32(source + 4);
        converted[i].addr = NULL;
        converted[i].header.info = musyxReadBE32(source + 0xc);
        converted[i].header.length = musyxReadBE32(source + 0x10);
        converted[i].header.loopOffset = musyxReadBE32(source + 0x14);
        converted[i].header.loopLength = musyxReadBE32(source + 0x18);
        converted[i].extraData = extraOffset != 0 ? (void*)(raw + extraOffset) : NULL;
    }
    converted[count].id = 0xFFFF;
    converted[count].ref_cnt = 0;
    converted[count].offset = 0;
    converted[count].addr = NULL;
    converted[count].header.info = 0;
    converted[count].header.length = 0;
    converted[count].header.loopOffset = 0;
    converted[count].header.loopLength = 0;
    converted[count].extraData = NULL;

    dataSDirRaw[dataSDirNativeCount] = rawData;
    dataSDirNative[dataSDirNativeCount] = converted;
    dataSDirNativeCount++;
    return converted;
}

int dataInsertKeymap(u16 cid, void* keymapData)
{
    long i;
    long j;
    DataKeymapEntry* converted;
    DataKeymapEntry* source;
    DATA_TAB* c;

    sndBegin();

    c = dataKeymapTable;
    for (i = 0; i < dataKeymapNum && c->id < cid; ++c, ++i)
        ;

    if (i < dataKeymapNum)
    {
        if (cid != dataKeymapTable[i].id)
        {
            if (dataKeymapNum < 256)
            {
                c = dataKeymapTable;
                for (j = dataKeymapNum - 1; j >= i; --j)
                    c[j + 1] = c[j];
                ++dataKeymapNum;
            }
            else
            {
                sndEnd();
                return 0;
            }
        }
        else
        {
            dataKeymapTable[i].refCount++;
            sndEnd();
            return 0;
        }
    }
    else if (dataKeymapNum < 256)
    {
        ++dataKeymapNum;
    }
    else
    {
        sndEnd();
        return 0;
    }

    converted = salMalloc(128 * sizeof(DataKeymapEntry));
    if (converted == NULL)
    {
        for (j = i; j + 1 < dataKeymapNum; j++)
        {
            dataKeymapTable[j] = dataKeymapTable[j + 1];
        }
        dataKeymapNum--;
        sndEnd();
        return 0;
    }
    source = keymapData;
    for (j = 0; j < 128; j++)
    {
        converted[j].id = musyxReadBE16(&source[j].id);
        converted[j].transpose = source[j].transpose;
        converted[j].panning = source[j].panning;
        converted[j].prioOffset = (s16)musyxReadBE16(&source[j].prioOffset);
        memcpy(converted[j].reserved, source[j].reserved, sizeof(converted[j].reserved));
    }
    dataKeymapTable[i].id = cid;
    dataKeymapTable[i].data = converted;
    dataKeymapTable[i].refCount = 1;
    sndEnd();
    return 1;
}

int dataRemoveKeymap(u16 sid)
{
    long i;
    long j;
    long num;

    sndBegin();
    num = dataKeymapNum;
    {
        DATA_TAB* c = dataKeymapTable;
        for (i = 0; i < num && sid != c->id; ++c, ++i)
            ;
    }

    if (i != num && --dataKeymapTable[i].refCount == 0)
    {
        salFree(dataKeymapTable[i].data);
        {
            DATA_TAB* keymap = dataKeymapTable;
            DATA_TAB* p = &keymap[i + 1];
            for (j = i + 1; j < num; j++)
            {
                p[-1] = p[0];
                p++;
            }
        }
        --dataKeymapNum;
        sndEnd();
        return 1;
    }
    sndEnd();
    return 0;
}

s32 dataInsertLayer(u16 cid, void* layerdata, u16 size)
{
    long i;
    long j;
    DataLayerEntry* converted;
    DataLayerEntry* source;

    sndBegin();

    {
        LAYER_TAB* c = dataLayerTable;
        for (i = 0; i < dataLayerNum && c->id < cid; ++c, ++i)
            ;
    }

    if (i < dataLayerNum)
    {
        if (cid != dataLayerTable[i].id)
        {
            if (dataLayerNum < 256)
            {
                {
                    LAYER_TAB* layer = dataLayerTable;
                    for (j = dataLayerNum - 1; j >= i; --j)
                        layer[j + 1] = layer[j];
                }
                ++dataLayerNum;
            }
            else
            {
                sndEnd();
                return 0;
            }
        }
        else
        {
            dataLayerTable[i].refCount++;
            sndEnd();
            return 0;
        }
    }
    else if (dataLayerNum < 256)
    {
        ++dataLayerNum;
    }
    else
    {
        sndEnd();
        return 0;
    }

    converted = salMalloc(size * sizeof(DataLayerEntry));
    if (converted == NULL)
    {
        for (j = i; j + 1 < dataLayerNum; j++)
        {
            dataLayerTable[j] = dataLayerTable[j + 1];
        }
        dataLayerNum--;
        sndEnd();
        return 0;
    }
    source = layerdata;
    for (j = 0; j < size; j++)
    {
        converted[j].id = musyxReadBE16(&source[j].id);
        converted[j].keyLow = source[j].keyLow;
        converted[j].keyHigh = source[j].keyHigh;
        converted[j].transpose = source[j].transpose;
        converted[j].volume = source[j].volume;
        converted[j].prioOffset = (s16)musyxReadBE16(&source[j].prioOffset);
        converted[j].panning = source[j].panning;
        memcpy(converted[j].reserved, source[j].reserved, sizeof(converted[j].reserved));
    }
    dataLayerTable[i].id = cid;
    dataLayerTable[i].data = converted;
    dataLayerTable[i].num = size;
    dataLayerTable[i].refCount = 1;
    sndEnd();
    return 1;
}

s32 dataRemoveLayer(u16 sid)
{
    long i;
    long j;
    long num;

    sndBegin();
    num = dataLayerNum;
    {
        LAYER_TAB* c = dataLayerTable;
        for (i = 0; i < num && sid != c->id; ++c, ++i)
            ;
    }

    if (i != num && --dataLayerTable[i].refCount == 0)
    {
        salFree(dataLayerTable[i].data);
        {
            LAYER_TAB* layer = dataLayerTable;
            LAYER_TAB* p = &layer[i + 1];
            for (j = i + 1; j < num; j++)
            {
                p[-1] = p[0];
                p++;
            }
        }

        --dataLayerNum;
        sndEnd();
        return 1;
    }

    sndEnd();
    return 0;
}

s32 dataInsertCurve(u16 cid, void* curvedata)
{
    long i;
    long j;

    sndBegin();

    for (i = 0; i < dataCurveNum && dataCurveTable[i].id < cid; ++i)
        ;

    if (i < dataCurveNum)
    {
        if (cid != dataCurveTable[i].id)
        {
            if (dataCurveNum < 2048)
            {
                for (j = dataCurveNum - 1; j >= i; --j)
                    dataCurveTable[j + 1] = dataCurveTable[j];
                ++dataCurveNum;
            }
            else
            {
                sndEnd();
                return 0;
            }
        }
        else
        {
            sndEnd();
            dataCurveTable[i].refCount++;
            return 0;
        }
    }
    else if (dataCurveNum < 2048)
    {
        ++dataCurveNum;
    }
    else
    {
        sndEnd();
        return 0;
    }

    dataCurveTable[i].id = cid;
    dataCurveTable[i].data = curvedata;
    dataCurveTable[i].refCount = 1;
    sndEnd();
    return 1;
}

s32 dataRemoveCurve(u16 sid)
{
    long i;
    long j;
    long num;

    sndBegin();
    num = dataCurveNum;
    {
        DATA_TAB* c = dataCurveTable;
        for (i = 0; i < num && sid != c->id; ++c, ++i)
            ;
    }

    if (i != num && --dataCurveTable[i].refCount == 0)
    {
        {
            DATA_TAB* curve = dataCurveTable;
            DATA_TAB* p = &curve[i + 1];
            for (j = i + 1; j < num; j++)
            {
                p[-1] = p[0];
                p++;
            }
        }

        --dataCurveNum;
        sndEnd();
        return 1;
    }

    sndEnd();
    return 0;
}

static inline void dataFindSampleDir(SDIR_DATA* sample, s32* dirIndex, u16* k)
{
    for (*dirIndex = 0; *dirIndex < dataSmpSDirNum; ++*dirIndex)
    {
        for (*k = 0; *k < dataSmpSDirs[*dirIndex].numSmp; ++*k)
        {
            if (sample->id == dataSmpSDirs[*dirIndex].data[*k].id)
                return;
        }
    }
}

u32 dataInsertSDir(SDIR_DATA* sdir, void* smp_data)
{
    s32 i;
    SDIR_DATA* s;
    u16 n;
    u16 j;
    u16 k;

    sdir = dataConvertSDir(sdir);
    if (sdir == NULL)
    {
        return 0;
    }

    for (i = 0; i < dataSmpSDirNum && dataSmpSDirs[i].data != sdir; ++i)
        ;

    if (i == dataSmpSDirNum)
    {
        if (dataSmpSDirNum < 128)
        {
            n = 0;
            for (s = sdir; s->id != 0xFFFF; ++s)
            {
                ++n;
            }

            sndBegin();
            for (j = 0; j < n; ++j)
            {
                dataFindSampleDir(&sdir[j], &i, &k);
                if (i != dataSmpSDirNum)
                {
                    sdir[j].ref_cnt = 0xFFFF;
                }
                else
                {
                    sdir[j].ref_cnt = 0;
                }
            }

            dataSmpSDirs[dataSmpSDirNum].data = sdir;
            dataSmpSDirs[dataSmpSDirNum].numSmp = n;
            dataSmpSDirs[dataSmpSDirNum].base = smp_data;
            ++dataSmpSDirNum;
            sndEnd();
            return 1;
        }
        else
        {
            return 0;
        }
    }

    return 1;
}

static inline void dataFindSampleReference(u16 sid, u32* dirIndex, SDIR_DATA** cursor,
                                           SDIR_DATA** result)
{
    for (*dirIndex = 0; *dirIndex < dataSmpSDirNum; ++*dirIndex)
    {
        for (*cursor = dataSmpSDirs[*dirIndex].data; (*cursor)->id != 0xFFFF; ++*cursor)
        {
            if ((*cursor)->id == sid && (*cursor)->ref_cnt != 0xFFFF)
            {
                *result = *cursor;
                return;
            }
        }
    }
}

s32 dataAddSampleReference(u16 sid)
{
    u32 i;
    SDIR_TAB* tab;
    SAMPLE_HEADER* header;
    SDIR_DATA* data;
    SDIR_DATA* sdir;

    data = NULL;
    sdir = NULL;
    dataFindSampleReference(sid, &i, &data, &sdir);

    if (sdir->ref_cnt == 0)
    {
        tab = dataSmpSDirs;
        sdir->addr = (void*)(sdir->offset + (uintptr_t)tab[i].base);
        header = &sdir->header;
        hwSaveSample(&header, &sdir->addr);
    }

    ++sdir->ref_cnt;
    return 1;
}

s32 dataRemoveSampleReference(u16 sid)
{
    u32 i;
    SDIR_DATA* sdir;

    for (i = 0; i < dataSmpSDirNum; ++i)
    {
        for (sdir = dataSmpSDirs[i].data; sdir->id != 0xFFFF; ++sdir)
        {
            if (sdir->id == sid && sdir->ref_cnt != 0xFFFF)
            {
                --sdir->ref_cnt;

                if (sdir->ref_cnt == 0)
                {
                    hwRemoveSample(&sdir->header, sdir->addr);
                }

                return 1;
            }
        }
    }
    return 0;
}

u32 dataInsertFX(u16 gid, FX_TAB* fx, u16 fxNum)
{
    long i;
    FX_GROUP* g;
    FX_TAB* source = fx;
    fx = salMalloc(fxNum * sizeof(FX_TAB));
    if (fx == NULL)
    {
        return 0;
    }
    for (i = 0; i < fxNum; i++)
    {
        fx[i].id = musyxReadBE16(&source[i].id);
        fx[i].macro = musyxReadBE16(&source[i].macro);
        fx[i].maxVoices = source[i].maxVoices;
        fx[i].priority = source[i].priority;
        fx[i].volume = source[i].volume;
        fx[i].panning = source[i].panning;
        fx[i].key = source[i].key;
        fx[i].vGroup = source[i].vGroup;
    }

    g = dataFXGroupTable;
    for (i = 0; i < dataFXGroupNum && gid != g[i].gid; ++i)
    {
    }

    if (i == dataFXGroupNum && dataFXGroupNum < 128)
    {
        sndBegin();
        i = dataFXGroupNum;
        dataFXGroupTable[i].gid = gid;
        dataFXGroupTable[i].fxNum = fxNum;
        dataFXGroupTable[i].fxTab = fx;

        for (i = 0; i < fxNum; ++i, ++fx)
        {
            fx->vGroup = 31;
        }

        dataFXGroupNum++;
        sndEnd();
        return 1;
    }
    salFree(fx);
    return 0;
}

s32 dataInsertMacro(u16 mid, void* macroaddr)
{
    u32 main;
    long pos;
    long base;
    long i;

    sndBegin();

    main = mid >> 6;

    if (dataMacroBucketTable[main].num == 0)
    {
        pos = base = dataMacroBucketTable[main].subTabIndex = dataMacTotal;
    }
    else
    {
        base = dataMacroBucketTable[main].subTabIndex;
        for (i = 0; i < dataMacroBucketTable[main].num && dataMacroTable[base + i].id < mid; ++i)
        {
        }

        if (i < dataMacroBucketTable[main].num)
        {
            pos = base + i;
            if (mid == dataMacroTable[pos].id)
            {
                dataMacroTable[pos].refCount++;
                sndEnd();
                return 0;
            }
        }
        else
        {
            pos = base + i;
        }
    }

    if (dataMacTotal < 2048)
    {
        for (i = 0; i < 512; ++i)
        {
            if (dataMacroBucketTable[i].subTabIndex > base)
            {
                dataMacroBucketTable[i].subTabIndex++;
            }
        }

        for (i = dataMacTotal - 1; i >= pos; --i)
            dataMacroTable[i + 1] = dataMacroTable[i];

        dataMacroTable[pos].id = mid;
        dataMacroTable[pos].data = macroaddr;
        dataMacroTable[pos].refCount = 1;
        dataMacroBucketTable[main].num++;
        dataMacTotal++;
        sndEnd();
        return 1;
    }
    sndEnd();
    return 0;
}

s32 dataRemoveMacro(u16 mid)
{
    s32 main;
    s32 base;
    s32 i;

    sndBegin();
    main = (mid >> 6) & 0x3ff;

    if (dataMacroBucketTable[main].num != 0)
    {
        base = dataMacroBucketTable[main].subTabIndex;
        for (i = 0; i < dataMacroBucketTable[main].num && mid != dataMacroTable[base + i].id; ++i)
        {
        }

        if (i < dataMacroBucketTable[main].num)
        {
            if (--dataMacroTable[base + i].refCount == 0)
            {
                for (i = base + i + 1; i < dataMacTotal; ++i)
                    dataMacroTable[i - 1] = dataMacroTable[i];

                for (i = 0; i < 512; ++i)
                {
                    if (dataMacroBucketTable[i].subTabIndex > base)
                    {
                        --dataMacroBucketTable[i].subTabIndex;
                    }
                }

                --dataMacroBucketTable[main].num;
                --dataMacTotal;
            }
        }
    }

    sndEnd();
    return 0;
}

static s32 maccmp(void* p1, void* p2)
{
    return ((MAC_SUBTAB*)p1)->id - ((MAC_SUBTAB*)p2)->id;
}

void* dataGetMacro(u16 mid)
{
    u16 num;

    dataGetMacro_bucket = mid >> 6;
    if (dataGetMacro_bucket >= 512)
    {
        return NULL;
    }
    num = dataMacroBucketTable[dataGetMacro_bucket].num;

    if (num != 0)
    {
        dataGetMacro_main = dataMacroBucketTable[dataGetMacro_bucket].subTabIndex;
        dataGetMacro_key.id = mid;
        if ((dataGetMacro_result =
                 (MAC_SUBTAB*)sndBSearch(&dataGetMacro_key, &dataMacroTable[dataGetMacro_main], num,
                                         sizeof(MAC_SUBTAB), maccmp)) != NULL)
        {
            return dataGetMacro_result->data;
        }
    }

    return NULL;
}

static s32 smpcmp(void* p1, void* p2)
{
    return ((SDIR_DATA*)p1)->id - ((SDIR_DATA*)p2)->id;
}

s32 dataGetSample(u16 sid, SAMPLE_INFO* newsmp)
{
    long i;
    dataGetSampleSearchKey.id = sid;

    for (i = 0; i < dataSmpSDirNum; ++i)
    {
        if ((dataGetSample_result =
                 (SDIR_DATA*)sndBSearch(&dataGetSampleSearchKey, dataSmpSDirs[i].data,
                                        dataSmpSDirs[i].numSmp, sizeof(SDIR_DATA), smpcmp)) != NULL)
        {
            if (dataGetSample_result->ref_cnt != 0xFFFF)
            {
                dataGetSample_sheader = &dataGetSample_result->header;
                newsmp->info = dataGetSample_sheader->info;
                newsmp->addr = dataGetSample_result->addr;
                newsmp->offset = 0;
                newsmp->loop = dataGetSample_sheader->loopOffset;
                newsmp->length = dataGetSample_sheader->length & 0xFFFFFF;
                newsmp->loopLength = dataGetSample_sheader->loopLength;
                newsmp->compType = dataGetSample_sheader->length >> 24;

                if (dataGetSample_result->extraData)
                {
                    newsmp->extraData = dataGetSample_result->extraData;
                }
                return 0;
            }
        }
    }

    return -1;
}

static s32 curvecmp(void* p1, void* p2)
{
    return ((DATA_TAB*)p1)->id - ((DATA_TAB*)p2)->id;
}

void* dataGetCurve(u16 cid)
{
    dataGetCurve_key.id = cid;
    if ((dataGetCurve_result =
             (DATA_TAB*)sndBSearch(&dataGetCurve_key, dataCurveTable, dataCurveNum, sizeof(DATA_TAB), curvecmp)))
    {
        return dataGetCurve_result->data;
    }
    return NULL;
}

void* dataGetKeymap(u16 cid)
{
    dataGetKeymap_key.id = cid;
    if ((dataGetKeymap_result =
             (DATA_TAB*)sndBSearch(&dataGetKeymap_key, dataKeymapTable, dataKeymapNum, sizeof(DATA_TAB), curvecmp)))
    {
        return dataGetKeymap_result->data;
    }
    return NULL;
}

static s32 layercmp(void* p1, void* p2)
{
    return ((LAYER_TAB*)p1)->id - ((LAYER_TAB*)p2)->id;
}

void* dataGetLayer(u16 cid, u16* count)
{
    dataGetLayerSearchKey.id = cid;
    if ((dataGetLayer_result =
             (LAYER_TAB*)sndBSearch(&dataGetLayerSearchKey, dataLayerTable, dataLayerNum,
                                    sizeof(LAYER_TAB), layercmp)))
    {
        *count = dataGetLayer_result->num;
        return dataGetLayer_result->data;
    }
    return NULL;
}

static s32 fxcmp(void* p1, void* p2)
{
    return ((FX_TAB*)p1)->id - ((FX_TAB*)p2)->id;
}

FX_TAB* dataGetFX(u16 fid)
{
    FX_TAB* ret;
    long i;
    FX_TAB* tab;
    FX_GROUP* g;

    dataGetFXSearchKey.key.id = fid;
    g = dataFXGroupTable;
    for (i = 0; i < dataFXGroupNum; ++i)
    {
        tab = g[i].fxTab;
        if ((ret = (FX_TAB*)sndBSearch(&dataGetFXSearchKey.key, tab, g[i].fxNum,
                                       sizeof(FX_TAB), fxcmp)))
        {
            return ret;
        }
    }

    return NULL;
}

void dataInit(u32 smpBase, u32 smpLength)
{
    long i;

    dataSmpSDirNum = 0;
    dataCurveNum = 0;
    dataKeymapNum = 0;
    dataLayerNum = 0;
    dataFXGroupNum = 0;
    dataMacTotal = 0;
    dataSDirNativeCount = 0;
    for (i = 0; i < 512; ++i)
    {
        dataMacroBucketTable[i].num = 0;
        dataMacroBucketTable[i].subTabIndex = 0;
    }
    hwInitSampleMem(smpBase, smpLength);
}

void dataExit(void)
{
    hwExitSampleMem();
}
