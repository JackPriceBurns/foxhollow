#include <dolphin/types.h>
#include "foxhollow_compat.h"
#include <string.h>
#include <stdio.h>

extern uintptr_t gResourceFileBuffers[];
extern u32 gResourceFileSizes[];
extern char* sResourceFileNameTable[];

typedef struct {
  void* buf;
  u32 fingerprint;
} TabSwapRecord;

static TabSwapRecord sSwappedTabs[96];
static int sSwappedTabCount;

static u32 fingerprintOf(void* p, u32 words) {
  u32* w = (u32*)p;
  u32 i;
  u32 n = words < 8 ? words : 8;
  for (i = 0; i < n; i++) {
    if (w[i] != 0 && w[i] != 0xffffffff) {
      return w[i];
    }
  }
  return 0;
}

static TabSwapRecord* findRecord(void* p) {
  int i;
  for (i = 0; i < sSwappedTabCount; i++) {
    if (sSwappedTabs[i].buf == p) {
      return &sSwappedTabs[i];
    }
  }
  return 0;
}

void fhSwapTabTableInPlace(void* table) {
  u32* q = (u32*)table;
  if (!q) {
    return;
  }
  if ((q[0] >> 24) != 0 || q[0] == 0xffffffff) {
    return;
  }
  while (*q != 0xffffffff) {
    *q = fhSwap32(*q);
    q++;
  }
}

void fhSwapTabBufferOnce(void* buf, unsigned int words) {
  TabSwapRecord* rec;
  if (!buf || !words) {
    return;
  }
  rec = findRecord(buf);
  if (rec && rec->fingerprint == fingerprintOf(buf, words)) {
    return;
  }
  fhSwapU32Array(buf, words);
  if (!rec && sSwappedTabCount < 96) {
    rec = &sSwappedTabs[sSwappedTabCount++];
    rec->buf = buf;
  }
  if (rec) {
    rec->fingerprint = fingerprintOf(buf, words);
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
        strncmp((char*)buf, "DIR", 3) != 0 && strncmp((char*)buf, "ZLB", 3) != 0) {
      fhSwapTabBufferOnce(buf, gResourceFileSizes[i] / 4);
    }
  }
}
