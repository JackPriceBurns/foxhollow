#include <dolphin/types.h>
#include "foxhollow_compat.h"
#include <string.h>
#include <stdio.h>

extern uintptr_t gResourceFileBuffers[];
extern u32 gResourceFileSizes[];
extern char* sResourceFileNameTable[];

static void* sSwappedTabs[96];
static int sSwappedTabCount;

static int alreadySwapped(void* p) {
  int i;
  for (i = 0; i < sSwappedTabCount; i++) {
    if (sSwappedTabs[i] == p) {
      return 1;
    }
  }
  return 0;
}

void fhSwapTabBufferOnce(void* buf, unsigned int words) {
  if (buf && words && !alreadySwapped(buf)) {
    fhSwapU32Array(buf, words);
    if (sSwappedTabCount < 96) {
      sSwappedTabs[sSwappedTabCount++] = buf;
    }
  }
}

void fhSwapResidentTabs(void) {
  int i;
  for (i = 0; i < 0x58; i++) {
    char* name = sResourceFileNameTable[i];
    void* buf = (void*)gResourceFileBuffers[i];
    int len = name ? (int)strlen(name) : 0;
    if (buf && len > 4 &&
        (strcmp(name + len - 4, ".tab") == 0 || strcmp(name + len - 4, ".TAB") == 0) &&
        !alreadySwapped(buf)) {
      fhSwapU32Array(buf, gResourceFileSizes[i] / 4);
      if (sSwappedTabCount < 96) {
        sSwappedTabs[sSwappedTabCount++] = buf;
      }
    }
  }
}
