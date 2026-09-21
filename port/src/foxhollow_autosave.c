#include "foxhollow_autosave.h"
#include "foxhollow_compat.h"
#include "foxhollow_config.h"
#include "main/dll/dll_0017_savegame.h"
#include "main/dll/player_state.h"
#include "main/dll/savegame_load.h"

#include <stdio.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif

#define FH_AUTOSAVE_INTERVAL_SECONDS 30
#define FH_AUTOSAVE_VERSION 1
#define FH_AUTOSAVE_HEADER_SIZE 0x20
#define FH_AUTOSAVE_SLOT_SIZE 0x6ec
#define FH_AUTOSAVE_LIVE_COPY_SIZE 0x564
#define FH_AUTOSAVE_NEW_FILE_FLAG_OFFSET 0x21
#define FH_AUTOSAVE_SAVE_LOCK_OFFSET 0x22
#define FH_AUTOSAVE_SLOT_COUNT 3

static const char sMagic[8] = {'F', 'H', 'A', 'U', 'T', 'O', 'S', 'V'};

extern s8 gSaveGameCurrentSlot;

static time_t sNextCheck;
static u8 sLastImage[FH_AUTOSAVE_SLOT_SIZE];
static int sHaveLastImage;
static int sReportedFailure;

static void write_be32(u8* p, u32 value) {
  p[0] = (u8)(value >> 24);
  p[1] = (u8)(value >> 16);
  p[2] = (u8)(value >> 8);
  p[3] = (u8)value;
}

static void compose_slot_image(u8* image) {
  PlayerStatus* status = (PlayerStatus*)image;

  memcpy(image, gSaveGameWorkBuffer, FH_AUTOSAVE_SLOT_SIZE);
  if (gSaveGameData[FH_AUTOSAVE_SAVE_LOCK_OFFSET] == 0) {
    memcpy(image, gSaveGameData, FH_AUTOSAVE_LIVE_COPY_SIZE);
  }
  if (status[0].health < 1) {
    status[0].health = 1;
  }
  if (status[1].health < 1) {
    status[1].health = 1;
  }
}

static int replace_file(const char* temporary, const char* path) {
#ifdef _WIN32
  return MoveFileExA(temporary, path, MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) != 0;
#else
  return rename(temporary, path) == 0;
#endif
}

static int write_snapshot(const char* path, const u8* file, size_t size) {
  char temporary[1040];
  FILE* out;
  int ok;

  if (snprintf(temporary, sizeof(temporary), "%s.tmp", path) >= (int)sizeof(temporary)) {
    return 0;
  }
  out = fopen(temporary, "wb");
  if (out == NULL) {
    return 0;
  }
  ok = fwrite(file, 1, size, out) == size;
  ok = fflush(out) == 0 && ok;
  ok = fclose(out) == 0 && ok;
  if (!ok) {
    remove(temporary);
    return 0;
  }
  if (!replace_file(temporary, path)) {
    remove(temporary);
    return 0;
  }
  return 1;
}

void fhAutosaveUpdate(void) {
  const char* path = fhConfigAutosavePath();
  u8 file[FH_AUTOSAVE_HEADER_SIZE + FH_AUTOSAVE_SLOT_SIZE];
  u8* image = file + FH_AUTOSAVE_HEADER_SIZE;
  time_t now;

  if (path == NULL) {
    return;
  }
  now = time(NULL);
  if (now < sNextCheck) {
    return;
  }
  sNextCheck = now + FH_AUTOSAVE_INTERVAL_SECONDS;

  if (gSaveGameCurrentSlot < 0 || gSaveGameCurrentSlot >= FH_AUTOSAVE_SLOT_COUNT) {
    return;
  }
  if (gSaveGameWorkBuffer == NULL || getSaveGameLoadStatus() != 0) {
    return;
  }

  compose_slot_image(image);
  if (image[FH_AUTOSAVE_NEW_FILE_FLAG_OFFSET] == 0) {
    return;
  }
  if (sHaveLastImage && memcmp(image, sLastImage, FH_AUTOSAVE_SLOT_SIZE) == 0) {
    return;
  }
  memcpy(sLastImage, image, FH_AUTOSAVE_SLOT_SIZE);
  sHaveLastImage = 1;

  memcpy(file, sMagic, sizeof(sMagic));
  write_be32(file + 0x08, FH_AUTOSAVE_VERSION);
  write_be32(file + 0x0c, (u32)gSaveGameCurrentSlot);
  fhWriteBE64(file + 0x10, (uint64_t)now);
  write_be32(file + 0x18, FH_AUTOSAVE_SLOT_SIZE);
  write_be32(file + 0x1c, 0);
  fhSwapSaveGameSlot(image);

  if (!write_snapshot(path, file, sizeof(file))) {
    sHaveLastImage = 0;
    if (!sReportedFailure) {
      sReportedFailure = 1;
      fprintf(stderr, "[foxhollow] autosave snapshot could not be written to %s\n", path);
    }
  }
}

void fhAutosaveShutdown(void) {
  const char* path = fhConfigAutosavePath();
  char temporary[1040];

  if (path == NULL) {
    return;
  }
  remove(path);
  if (snprintf(temporary, sizeof(temporary), "%s.tmp", path) < (int)sizeof(temporary)) {
    remove(temporary);
  }
}
