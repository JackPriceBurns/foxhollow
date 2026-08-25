#include "main/shader.h"
#include "main/pi_dolphin.h"
#include "dolphin/gx/GXLighting.h"
#include "main/track_dolphin.h"
#include "main/mm.h"
#include "main/asset_load.h"
#include "main/render_flags.h"
#include "main/lightmap_lifecycle.h"

extern u32 gVisibleObjectSortKeys[];

#define MAP_BLOCK_LAYER_COUNT 5

void initMapBlocks(void) {

    renderFlags = 0;
    gMapBlocks = mmAlloc(64 * sizeof(MapBlockData*), 5, 0);
    gMapBlockIds = mmAlloc(0x80, 5, 0);
    gMapBlockRefCounts = mmAlloc(0x40, 5, 0);
    gMapInfoBuffer = mmAlloc(0xd48, 5, 0);
    gMapBlockLayerTables[0] = mmAlloc(0x500, 5, 0);
    gMapBlockCellEntryTables[0] = mmAlloc(0x3c00, 5, 0);
    gMapBlockCellStateTables[0] = mmAlloc(0x500, 5, 0);

    for (int i = 0; i < 4; i++) {
        gMapBlockLayerTables[i + 1] = gMapBlockLayerTables[i] + 0x100;
        gMapBlockCellEntryTables[i + 1] = gMapBlockCellEntryTables[i] + 0x100;
        gMapBlockCellStateTables[i + 1] = gMapBlockCellStateTables[i] + 0x100;
    }

    loadAssetFileById(&gMapsTab, MLDF_FILEID_MAPS_TAB);
    loadAssetFileById(&gHitsTab, MLDF_FILEID_HITS_TAB);

    memset(gLoadedRomListPages, 0, sizeof(gLoadedRomListPages));

    loadAssetFileById(&gTrkBlkTab, MLDF_FILEID_TRKBLK_TAB);

    gTrkBlkTabCount = 0;
    u16* p = gTrkBlkTab;
    while (*p != 0xffff) {
        p++;
        gTrkBlkTabCount++;
    }
    gTrkBlkTabCount--;
    gPendingWarpIndex = -1;
    gArrivedWarpIndex = -2;

    void* tmp = mmAlloc(80 * sizeof(MapTextureOverride), 5, 0);
    gMapTextureOverrides = tmp;
    memset(tmp, 0, 80 * sizeof(MapTextureOverride));

    tmp = mmAlloc(0x3a0, 5, 0);
    gMapTextureScrolls = tmp;
    memset(tmp, 0, 0x3a0);

    extern u32 gVisibleObjectSortKeys[];
    memset(gVisibleObjectSortKeys, 0, 0xfa0);
    gVisibleObjectSortKeys[0] = -1;
}
