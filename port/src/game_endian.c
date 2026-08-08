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

int fhTabIs16Bit(const char* name) {
  int len = name ? (int)strlen(name) : 0;
  if (len >= 12 && strcmp(name + len - 12, "OBJSEQ2C.tab") == 0) {
    return 1;
  }
  if (len >= 10 && strcmp(name + len - 10, "OBJSEQ.tab") == 0) {
    return 1;
  }
  if (len >= 11 && strcmp(name + len - 11, "MODANIM.tab") == 0) {
    return 1;
  }
  return 0;
}

void fhSwapObjDef(void* def) {
  u8* d = (u8*)def;
  static const u8 u16Offs[] = {0x48, 0x4a, 0x4e, 0x50, 0x52, 0x68, 0x6a, 0x6c,
                               0x6e, 0x78, 0x7c, 0x7e, 0x80, 0x82, 0x84, 0x86};
  u32 i;
  for (i = 0; i <= 0x44; i += 4) {
    *(u32*)(d + i) = fhSwap32(*(u32*)(d + i));
  }
  for (i = 0; i < sizeof(u16Offs); i++) {
    *(u16*)(d + u16Offs[i]) = fhSwap16(*(u16*)(d + u16Offs[i]));
  }
  *(u32*)(d + 0x88) = fhSwap32(*(u32*)(d + 0x88));
}

void fhSwapRomListSection(void* buf, unsigned int size) {
  u8* p = (u8*)buf;
  unsigned int pos = 0;
  if (!buf) {
    return;
  }
  while (pos + 0x18 <= size) {
    unsigned int step = (unsigned int)p[2] * 4;
    *(u16*)p = fhSwap16(*(u16*)p);
    *(u32*)(p + 0x08) = fhSwap32(*(u32*)(p + 0x08));
    *(u32*)(p + 0x0c) = fhSwap32(*(u32*)(p + 0x0c));
    *(u32*)(p + 0x10) = fhSwap32(*(u32*)(p + 0x10));
    *(u32*)(p + 0x14) = fhSwap32(*(u32*)(p + 0x14));
    if (step < 0x18) {
      return;
    }
    pos += step;
    p += step;
  }
}

void fhSwapTab16BufferOnce(void* buf, unsigned int halves) {
  TabSwapRecord* rec;
  if (!buf || !halves) {
    return;
  }
  rec = findRecord(buf);
  if (rec && rec->fingerprint == fingerprintOf(buf, halves / 2)) {
    return;
  }
  fhSwapU16Array(buf, halves);
  if (!rec && sSwappedTabCount < 96) {
    rec = &sSwappedTabs[sSwappedTabCount++];
    rec->buf = buf;
  }
  if (rec) {
    rec->fingerprint = fingerprintOf(buf, halves / 2);
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
      if (fhTabIs16Bit(name)) {
        fhSwapTab16BufferOnce(buf, gResourceFileSizes[i] / 2);
      } else {
        fhSwapTabBufferOnce(buf, gResourceFileSizes[i] / 4);
      }
    }
  }
}
