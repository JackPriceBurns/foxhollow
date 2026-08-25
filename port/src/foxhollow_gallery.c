#include "foxhollow_compat.h"
#include "foxhollow_gallery.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dolphin/types.h>

#include "global.h"
#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/asset_load.h"
#include "main/loaded_file_flags.h"
#include "main/mldf_fileid.h"
#include "main/obj_list.h"
#include "main/objanim_internal.h"
#include "main/objtype.h"
#include "main/pi_data_file.h"
#include "main/pi_dolphin.h"
#include "main/rcp_dolphin.h"
#include "main/shader.h"
#include "sys/objects/lifecycle.h"

#define FH_GALLERY_IDENT_TAG 0x6a410000
#define FH_GALLERY_IDENT_MASK 0xffff0000
#define FH_GALLERY_MAX_PAGE 256
#define FH_GALLERY_DEF_HEADER 0xa0
#define FH_GALLERY_MODELS_TABLE_ENTRIES 0x800
#define FH_GALLERY_SEQ_SABRE 0x00
#define FH_GALLERY_SEQ_KRYSTAL 0x1f

#define FH_GALLERY_OPEN_AREA_WARP 3
#define FH_GALLERY_FREEZE_DELAY_FRAMES 4

extern int* gObjFileOffsetTable;
extern int gObjFileCount;
extern s16* gObjSeqToObjIdTable;
extern int gObjSeqToObjIdMax;
extern u8 gWarpRequested;
extern u8 gRcpWarpTransitionType;

typedef struct FhGalleryEntry {
  s16 seqId;
  s16 defId;
  s16 dllId;
  s16 modelCount;
  s16 mapLoadObjectId;
  s32 modelIdOffset;
  char name[16];
} FhGalleryEntry;

static int sChecked;
static int sEnabled;
static int sCatalogueBuilt;
static int sReady;

static FhGalleryEntry* sCatalogue;
static int sCatalogueCount;
static u8* sObjectsBlob;
static int sObjectsBlobSize;

static s16* sModelIndex;
static int sModelIndexCount;

static int* sCandidates;
static int sCandidateCount;

static int* sSkipList;
static int sSkipCount;
static int sSkipCapacity;

static FhGallerySpawn sSpawns[FH_GALLERY_MAX_PAGE];
static int sSpawnCount;

static int sPage;
static int sPageSize = 36;
static float sSpacing = 400.0f;
static int sFreeze = 1;
static int sShowLabels = 1;
static int sRespawnPending;
static int sClearPending;
static int sWarpPending;
static int sTeleportSlot = -1;
static int sFreezeDelay;
static int sDiagnostics;
static int sDiagnosticTick;
static int sOpenAreaWarp = FH_GALLERY_OPEN_AREA_WARP;

static char sSkipPath[1024];
static char sInflightPath[1024];

static const char* gallery_dir(void) {
  const char* dir = getenv("FOXHOLLOW_GALLERY_DIR");
  if (dir != NULL && dir[0] != '\0') {
    return dir;
  }
  return ".";
}

static void skip_list_add(int defId) {
  int i;
  for (i = 0; i < sSkipCount; i++) {
    if (sSkipList[i] == defId) {
      return;
    }
  }
  if (sSkipCount == sSkipCapacity) {
    int capacity = sSkipCapacity != 0 ? sSkipCapacity * 2 : 32;
    int* grown = (int*)realloc(sSkipList, (size_t)capacity * sizeof(*grown));
    if (grown == NULL) {
      return;
    }
    sSkipList = grown;
    sSkipCapacity = capacity;
  }
  sSkipList[sSkipCount++] = defId;
}

static int skip_list_contains(int defId) {
  int i;
  for (i = 0; i < sSkipCount; i++) {
    if (sSkipList[i] == defId) {
      return 1;
    }
  }
  return 0;
}

static void skip_list_append_file(int defId) {
  FILE* f = fopen(sSkipPath, "a");
  if (f == NULL) {
    return;
  }
  fprintf(f, "%d\n", defId);
  fclose(f);
}

static void skip_list_load(void) {
  FILE* f;
  int defId;

  snprintf(sSkipPath, sizeof(sSkipPath), "%s/foxhollow_gallery_skip.txt", gallery_dir());
  snprintf(sInflightPath, sizeof(sInflightPath), "%s/foxhollow_gallery_inflight.txt", gallery_dir());

  f = fopen(sSkipPath, "r");
  if (f != NULL) {
    while (fscanf(f, "%d", &defId) == 1) {
      skip_list_add(defId);
    }
    fclose(f);
  }

  f = fopen(sInflightPath, "r");
  if (f != NULL) {
    if (fscanf(f, "%d", &defId) == 1) {
      skip_list_add(defId);
      skip_list_append_file(defId);
      fprintf(stderr, "[gallery] object %d crashed the previous run; added to the skip list\n", defId);
    }
    fclose(f);
    remove(sInflightPath);
  }
}

static void inflight_set(int defId) {
  FILE* f = fopen(sInflightPath, "w");
  if (f == NULL) {
    return;
  }
  fprintf(f, "%d\n", defId);
  fflush(f);
  fclose(f);
}

static void inflight_clear(void) { remove(sInflightPath); }

int fhGalleryEnabled(void) {
  const char* value;
  if (sChecked) {
    return sEnabled;
  }
  sChecked = 1;
  value = getenv("FOXHOLLOW_GALLERY");
  sEnabled = value != NULL && value[0] != '\0' && strcmp(value, "0") != 0 && strcmp(value, "false") != 0 &&
             strcmp(value, "off") != 0;
  if (sEnabled) {
    const char* warp = getenv("FOXHOLLOW_GALLERY_WARP");
    const char* warpIndex = getenv("FOXHOLLOW_GALLERY_WARP_INDEX");
    if (warpIndex != NULL && warpIndex[0] != '\0') {
      sOpenAreaWarp = (int)strtol(warpIndex, NULL, 10);
    }
    const char* extraSkips = getenv("FOXHOLLOW_GALLERY_SKIP");
    sDiagnostics = getenv("FOXHOLLOW_GALLERY_DIAG") != NULL;
    skip_list_load();
    if (extraSkips != NULL) {
      const char* cursor = extraSkips;
      while (*cursor != '\0') {
        char* end = NULL;
        long parsed = strtol(cursor, &end, 10);
        if (end == cursor) {
          break;
        }
        skip_list_add((int)parsed);
        cursor = (*end != '\0') ? end + 1 : end;
      }
    }
    if (warp != NULL && warp[0] != '\0' && strcmp(warp, "0") != 0) {
      sWarpPending = 1;
    }
  }
  return sEnabled;
}

static int file_is_resident(int fileId) { return gResourceFileBuffers[fileId] != 0; }

static u32 read_be_u32(const u8* p) {
  return ((u32)p[0] << 24) | ((u32)p[1] << 16) | ((u32)p[2] << 8) | (u32)p[3];
}

static s16 read_be_s16(const u8* p) { return (s16)(((u16)p[0] << 8) | (u16)p[1]); }

static void build_catalogue(void) {
  int i;
  int total;

  if (sCatalogueBuilt) {
    return;
  }
  if (gObjFileOffsetTable == NULL || gObjFileCount <= 0) {
    return;
  }
  if (!file_is_resident(MLDF_FILEID_OBJECTS_BIN) && !file_is_resident(MLDF_FILEID_OBJECTS_TAB)) {
    return;
  }

  total = gObjFileOffsetTable[gObjFileCount];
  if (total <= 0) {
    return;
  }

  sObjectsBlob = (u8*)malloc((size_t)total);
  if (sObjectsBlob == NULL) {
    return;
  }
  sObjectsBlobSize = total;
  fileLoadToBufferOffset(MLDF_FILEID_OBJECTS_BIN, sObjectsBlob, 0, total);

  sCatalogue = (FhGalleryEntry*)calloc((size_t)gObjFileCount, sizeof(*sCatalogue));
  if (sCatalogue == NULL) {
    free(sObjectsBlob);
    sObjectsBlob = NULL;
    return;
  }

  for (i = 0; i < gObjFileCount; i++) {
    int base = gObjFileOffsetTable[i];
    int size = gObjFileOffsetTable[i + 1] - base;
    FhGalleryEntry* entry = &sCatalogue[i];
    const u8* def;

    entry->seqId = -1;
    entry->defId = (s16)i;
    entry->dllId = -1;
    entry->modelCount = 0;
    entry->mapLoadObjectId = -1;
    entry->modelIdOffset = 0;
    entry->name[0] = '\0';

    if (base < 0 || size < FH_GALLERY_DEF_HEADER || base + size > total) {
      continue;
    }
    def = sObjectsBlob + base;
    entry->modelIdOffset = (s32)read_be_u32(def + 0x08);
    entry->dllId = read_be_s16(def + 0x50);
    entry->modelCount = (s16)(s8)def[0x55];
    entry->mapLoadObjectId = read_be_s16(def + 0x78);
    memcpy(entry->name, def + 0x91, 11);
    entry->name[11] = '\0';
  }

  for (i = 0; i <= gObjSeqToObjIdMax && gObjSeqToObjIdTable != NULL; i++) {
    int defId;
    if (i == FH_GALLERY_SEQ_SABRE || i == FH_GALLERY_SEQ_KRYSTAL) {
      continue;
    }
    defId = gObjSeqToObjIdTable[i];
    if (defId <= 0 || defId >= gObjFileCount) {
      continue;
    }
    if (sCatalogue[defId].seqId < 0) {
      sCatalogue[defId].seqId = (s16)i;
    }
  }

  sCatalogueCount = gObjFileCount;
  sCatalogueBuilt = 1;
  fprintf(stderr, "[gallery] catalogued %d object definitions\n", sCatalogueCount);
}

static void refresh_model_index(void) {
  int size;

  free(sModelIndex);
  sModelIndex = NULL;
  sModelIndexCount = 0;

  if (!file_is_resident(MLDF_FILEID_MODELIND_BIN)) {
    return;
  }
  size = getDataFileSize(MLDF_FILEID_MODELIND_BIN);
  if (size <= 0) {
    return;
  }
  sModelIndex = (s16*)malloc((size_t)size);
  if (sModelIndex == NULL) {
    return;
  }
  fileLoadToBufferOffset(MLDF_FILEID_MODELIND_BIN, sModelIndex, 0, size);
  sModelIndexCount = size / 2;
}

static int model_is_present(int modelId) {
  const int* models = (const int*)getCurrentDataFile(MLDF_FILEID_MODELS_TAB_A);
  int realId;

  if (models == NULL) {
    return 0;
  }
  if (modelId < 0) {
    realId = -modelId;
  } else {
    if (sModelIndex == NULL || modelId >= sModelIndexCount) {
      return 0;
    }
    realId = read_be_s16((const u8*)&sModelIndex[modelId]);
  }
  if (realId < 0 || realId >= FH_GALLERY_MODELS_TABLE_ENTRIES) {
    return 0;
  }
  return models[realId] != 0;
}

static int entry_is_spawnable(const FhGalleryEntry* entry) {
  int i;

  if (entry->seqId < 0) {
    return 0;
  }
  if (entry->modelCount <= 0) {
    return 0;
  }
  if (entry->mapLoadObjectId > -1) {
    return 0;
  }
  if (skip_list_contains(entry->defId)) {
    return 0;
  }
  if (entry->modelIdOffset <= 0) {
    return 0;
  }
  for (i = 0; i < entry->modelCount; i++) {
    int base = gObjFileOffsetTable[entry->defId];
    int offset = base + entry->modelIdOffset + i * 4;
    if (offset + 4 > sObjectsBlobSize) {
      return 0;
    }
    if (!model_is_present((s32)read_be_u32(sObjectsBlob + offset))) {
      return 0;
    }
  }
  return 1;
}

static void rebuild_candidates(void) {
  int i;
  int count = 0;

  refresh_model_index();

  free(sCandidates);
  sCandidates = (int*)malloc((size_t)sCatalogueCount * sizeof(*sCandidates));
  sCandidateCount = 0;
  if (sCandidates == NULL) {
    return;
  }
  for (i = 0; i < sCatalogueCount; i++) {
    if (entry_is_spawnable(&sCatalogue[i])) {
      sCandidates[count++] = i;
    }
  }
  sCandidateCount = count;
}

static GameObject* player_object(void) {
  int count = 0;
  GameObject** all = objGetAllOfType(0, &count);
  if (all == NULL || count <= 0) {
    return NULL;
  }
  return all[0];
}

static int spawn_slot_of(GameObject* obj) {
  ObjPlacement* placement;
  if (obj == NULL || obj->anim.placementData == NULL) {
    return -1;
  }
  placement = obj->anim.placement;
  if (((u32)placement->ident & FH_GALLERY_IDENT_MASK) != FH_GALLERY_IDENT_TAG) {
    return -1;
  }
  return (int)((u32)placement->ident & 0xffff);
}

static void free_gallery_objects(void) {
  int start = 0;
  int count = 0;
  GameObject** list = ObjList_GetObjects(&start, &count);
  int i;

  if (list == NULL) {
    return;
  }
  for (i = count - 1; i >= 0; i--) {
    if (spawn_slot_of(list[i]) >= 0) {
      Obj_FreeObject(list[i]);
    }
  }
  sSpawnCount = 0;
  memset(sSpawns, 0, sizeof(sSpawns));
}

static void apply_freeze(GameObject* obj) {
  if (sFreeze && sFreezeDelay <= 0) {
    obj->objectFlags |= (u16)(OBJECT_OBJFLAG_UPDATE_DISABLED | OBJECT_OBJFLAG_HITDETECT_DISABLED);
  } else {
    obj->objectFlags &= (u16)~(OBJECT_OBJFLAG_UPDATE_DISABLED | OBJECT_OBJFLAG_HITDETECT_DISABLED);
  }
}

static void spawn_page(void) {
  GameObject* player;
  int pageStart;
  int columns;
  int rows;
  int available;
  int i;
  int placed = 0;
  f32 originX;
  f32 originY;
  f32 originZ;
  s8 mapEventSlot;
  void* parent;

  free_gallery_objects();

  player = player_object();
  if (player == NULL) {
    return;
  }
  if (sCandidateCount == 0) {
    return;
  }

  if (sPageSize < 1) {
    sPageSize = 1;
  }
  if (sPageSize > FH_GALLERY_MAX_PAGE) {
    sPageSize = FH_GALLERY_MAX_PAGE;
  }
  if (sPage < 0) {
    sPage = 0;
  }
  if (sPage * sPageSize >= sCandidateCount) {
    sPage = (sCandidateCount - 1) / sPageSize;
  }

  pageStart = sPage * sPageSize;
  available = sCandidateCount - pageStart;
  if (available > sPageSize) {
    available = sPageSize;
  }
  columns = 1;
  while (columns * columns < available) {
    columns++;
  }
  rows = (available + columns - 1) / columns;

  originX = player->anim.localPosX;
  originY = player->anim.localPosY;
  originZ = player->anim.localPosZ;
  mapEventSlot = player->anim.mapEventSlot;
  parent = player->anim.parent;

  for (i = 0; i < sPageSize && pageStart + i < sCandidateCount; i++) {
    int index = sCandidates[pageStart + i];
    const FhGalleryEntry* entry = &sCatalogue[index];
    ObjPlacement* placement;
    GameObject* obj;
    int row = i / columns;
    int column = i % columns;

    placement = Obj_AllocObjectSetup(0x40, entry->seqId);
    if (placement == NULL) {
      continue;
    }
    placement->loadFlags = 0x0a;
    placement->loadRange = 0xff;
    placement->unk07 = 0xff;
    placement->posX = originX + (f32)(column - columns / 2) * sSpacing;
    placement->posY = originY;
    placement->posZ = originZ + (f32)(row - rows / 2) * sSpacing;
    placement->ident = (s32)(FH_GALLERY_IDENT_TAG | (u32)placed);

    inflight_set(entry->defId);
    fprintf(stderr, "[gallery] spawning def %d seq %d (%s)\n", entry->defId, entry->seqId, entry->name);
    obj = objSetupObject(placement, 1 | 4, mapEventSlot, -1, parent);
    inflight_clear();

    if (obj == NULL) {
      fprintf(stderr, "[gallery] object %d (%s) failed to load\n", entry->defId, entry->name);
      continue;
    }

    obj->anim.loadDistance = 1.0e9f;
    obj->anim.cullDistance2 = 1.0e9f;
    apply_freeze(obj);

    sSpawns[placed].defId = entry->defId;
    sSpawns[placed].seqId = entry->seqId;
    sSpawns[placed].dllId = entry->dllId;
    sSpawns[placed].slot = placed;
    sSpawns[placed].name = sCatalogue[index].name;
    sSpawns[placed].alive = 1;
    sSpawns[placed].x = placement->posX;
    sSpawns[placed].y = placement->posY;
    sSpawns[placed].z = placement->posZ;
    placed++;
  }

  sSpawnCount = placed;
  sFreezeDelay = FH_GALLERY_FREEZE_DELAY_FRAMES;
  fprintf(stderr, "[gallery] page %d/%d: spawned %d of %d candidates\n", sPage + 1,
          (sCandidateCount + sPageSize - 1) / sPageSize, placed, sCandidateCount);
}

static void refresh_spawn_positions(void) {
  int start = 0;
  int count = 0;
  GameObject** list = ObjList_GetObjects(&start, &count);
  int i;

  for (i = 0; i < sSpawnCount; i++) {
    sSpawns[i].alive = 0;
  }
  if (list == NULL) {
    return;
  }
  for (i = 0; i < count; i++) {
    int slot = spawn_slot_of(list[i]);
    if (slot < 0 || slot >= sSpawnCount) {
      continue;
    }
    sSpawns[slot].x = list[i]->anim.worldPosX;
    sSpawns[slot].y = list[i]->anim.worldPosY;
    sSpawns[slot].z = list[i]->anim.worldPosZ;
    sSpawns[slot].alive = 1;
    apply_freeze(list[i]);
    if (sDiagnostics && sDiagnosticTick == 0) {
      fprintf(stderr, "[gallery] slot %2d %-12s hitboxScale=%.2f rootMotionScale=%.3f alpha=%d renderAlpha=%d flags=%04x\n",
              slot, sSpawns[slot].name, list[i]->anim.hitboxScale, list[i]->anim.rootMotionScale,
              list[i]->anim.alpha, list[i]->anim.renderAlpha, list[i]->objectFlags);
    }
  }
}

static void teleport_to_slot(int slot) {
  GameObject* player = player_object();
  if (player == NULL || slot < 0 || slot >= sSpawnCount || !sSpawns[slot].alive) {
    return;
  }
  player->anim.localPosX = sSpawns[slot].x;
  player->anim.localPosY = sSpawns[slot].y;
  player->anim.localPosZ = sSpawns[slot].z - sSpacing * 0.6f;
  player->anim.worldPosX = player->anim.localPosX;
  player->anim.worldPosY = player->anim.localPosY;
  player->anim.worldPosZ = player->anim.localPosZ;
  player->anim.velocityX = 0.0f;
  player->anim.velocityY = 0.0f;
  player->anim.velocityZ = 0.0f;
}

static void warp_to_flat_map(void) {
  free_gallery_objects();
  warpToMap(sOpenAreaWarp, 0);
}

void fhGalleryUpdate(void) {
  if (!fhGalleryEnabled()) {
    return;
  }

  build_catalogue();
  if (!sCatalogueBuilt) {
    sReady = 0;
    return;
  }
  if (player_object() == NULL) {
    sReady = 0;
    sSpawnCount = 0;
    return;
  }
  if (!sReady) {
    sReady = 1;
    rebuild_candidates();
  }

  if (sWarpPending) {
    sWarpPending = 0;
    warp_to_flat_map();
    return;
  }
  if (sClearPending) {
    sClearPending = 0;
    free_gallery_objects();
  }
  if (sRespawnPending) {
    sRespawnPending = 0;
    rebuild_candidates();
    spawn_page();
  }
  if (sTeleportSlot >= 0) {
    teleport_to_slot(sTeleportSlot);
    sTeleportSlot = -1;
  }

  if (sFreezeDelay > 0) {
    sFreezeDelay--;
  }
  if (sDiagnostics) {
    sDiagnosticTick = (sDiagnosticTick + 1) % 120;
  }
  refresh_spawn_positions();
}

void fhGalleryGetStatus(FhGalleryStatus* out) {
  if (out == NULL) {
    return;
  }
  out->enabled = sEnabled;
  out->ready = sReady;
  out->page = sPage;
  out->pageSize = sPageSize;
  out->pageCount = sPageSize > 0 ? (sCandidateCount + sPageSize - 1) / sPageSize : 0;
  out->candidateCount = sCandidateCount;
  out->catalogueCount = sCatalogueCount;
  out->skippedCount = sSkipCount;
  out->freeze = sFreeze;
  out->showLabels = sShowLabels;
  out->spacing = sSpacing;
}

const FhGallerySpawn* fhGallerySpawns(int* count) {
  if (count != NULL) {
    *count = sSpawnCount;
  }
  return sSpawns;
}

void fhGallerySetPage(int page) {
  sPage = page;
  sRespawnPending = 1;
}

void fhGallerySetPageSize(int size) {
  if (size < 1) {
    size = 1;
  }
  if (size > FH_GALLERY_MAX_PAGE) {
    size = FH_GALLERY_MAX_PAGE;
  }
  sPageSize = size;
}

void fhGallerySetSpacing(float spacing) {
  if (spacing < 40.0f) {
    spacing = 40.0f;
  }
  sSpacing = spacing;
}

void fhGallerySetFreeze(int freeze) { sFreeze = freeze != 0; }

void fhGallerySetShowLabels(int show) { sShowLabels = show != 0; }

void fhGalleryRespawn(void) { sRespawnPending = 1; }

void fhGalleryClear(void) { sClearPending = 1; }

void fhGalleryTeleportToSlot(int slot) { sTeleportSlot = slot; }

void fhGalleryWarpToFlatMap(void) { sWarpPending = 1; }

int fhGalleryCandidateCount(void) { return sCandidateCount; }

int fhGalleryCandidateInfo(int index, int* defId, int* seqId, int* dllId, const char** name) {
  const FhGalleryEntry* entry;
  if (index < 0 || index >= sCandidateCount) {
    return 0;
  }
  entry = &sCatalogue[sCandidates[index]];
  if (defId != NULL) {
    *defId = entry->defId;
  }
  if (seqId != NULL) {
    *seqId = entry->seqId;
  }
  if (dllId != NULL) {
    *dllId = entry->dllId;
  }
  if (name != NULL) {
    *name = entry->name;
  }
  return 1;
}

int fhGalleryFindCandidatePage(int index) {
  if (sPageSize < 1 || index < 0) {
    return 0;
  }
  return index / sPageSize;
}
