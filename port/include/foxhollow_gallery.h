#ifndef FOXHOLLOW_GALLERY_H_
#define FOXHOLLOW_GALLERY_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct FhGallerySpawn {
  float x;
  float y;
  float z;
  int defId;
  int seqId;
  int dllId;
  int slot;
  int alive;
  const char* name;
} FhGallerySpawn;

typedef struct FhGalleryStatus {
  int enabled;
  int ready;
  int page;
  int pageCount;
  int pageSize;
  int candidateCount;
  int catalogueCount;
  int skippedCount;
  int freeze;
  int showLabels;
  float spacing;
} FhGalleryStatus;

int fhGalleryEnabled(void);
void fhGalleryUpdate(void);

void fhGalleryGetStatus(FhGalleryStatus* out);
const FhGallerySpawn* fhGallerySpawns(int* count);

void fhGallerySetPage(int page);
void fhGallerySetPageSize(int size);
void fhGallerySetSpacing(float spacing);
void fhGallerySetFreeze(int freeze);
void fhGallerySetShowLabels(int show);
void fhGalleryRespawn(void);
void fhGalleryClear(void);
void fhGalleryTeleportToSlot(int slot);
void fhGalleryWarpToFlatMap(void);

int fhGalleryCandidateCount(void);
int fhGalleryCandidateInfo(int index, int* defId, int* seqId, int* dllId, const char** name);
int fhGalleryFindCandidatePage(int index);

void fhGalleryDrawOverlay(void);

#ifdef __cplusplus
}
#endif

#endif
