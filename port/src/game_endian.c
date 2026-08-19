#include <dolphin/types.h>
#include "foxhollow_compat.h"
#include <string.h>

int fhTabIs16Bit(const char* name) {
  int len = name ? (int)strlen(name) : 0;
  if (len >= 12 && strcmp(name + len - 12, "OBJSEQ2C.tab") == 0) {
    return 1;
  }
  if (len >= 10 && strcmp(name + len - 10, "OBJSEQ.tab") == 0) {
    return 1;
  }
  if (len >= 11 &&
      (strcmp(name + len - 11, "MODANIM.tab") == 0 ||
       strcmp(name + len - 11, "MODANIM.TAB") == 0)) {
    return 1;
  }
  if (len >= 10 && strcmp(name + len - 10, "TRKBLK.tab") == 0) {
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

void fhSwapLoadedTabFile(void* buf, unsigned int size, const char* name) {
  int len = name ? (int)strlen(name) : 0;
  if (!buf || len <= 4 ||
      (strcmp(name + len - 4, ".tab") != 0 && strcmp(name + len - 4, ".TAB") != 0) ||
      strncmp((char*)buf, "DIR", 3) == 0 || strncmp((char*)buf, "ZLB", 3) == 0) {
    return;
  }
  if (fhTabIs16Bit(name)) {
    fhSwapU16Array(buf, size / 2);
  } else {
    fhSwapU32Array(buf, size / 4);
  }
}

void fhSwapSaveGameSlot(void* data) {
  u8* p = (u8*)data;
  u32 i;
  u32 j;

  for (i = 0; i < 2; i++) {
    u32 base = i * 0xc;
    *(u16*)(p + base + 4) = fhSwap16(*(u16*)(p + base + 4));
    *(u16*)(p + base + 6) = fhSwap16(*(u16*)(p + base + 6));
  }
  for (i = 0; i < 0x3f; i++) {
    u32 base = 0x168 + i * 0x10;
    for (j = 0; j < 4; j++) {
      *(u32*)(p + base + j * 4) = fhSwap32(*(u32*)(p + base + j * 4));
    }
  }
  *(u32*)(p + 0x560) = fhSwap32(*(u32*)(p + 0x560));
  for (i = 0; i < 2; i++) {
    u32 base = 0x684 + i * 0x10;
    for (j = 0; j < 3; j++) {
      *(u32*)(p + base + j * 4) = fhSwap32(*(u32*)(p + base + j * 4));
    }
  }
  *(u16*)(p + 0x6a4) = fhSwap16(*(u16*)(p + 0x6a4));
  *(u32*)(p + 0x6a8) = fhSwap32(*(u32*)(p + 0x6a8));
  for (i = 0x6ac; i <= 0x6ba; i += 2) {
    if (i != 0x6b0) {
      *(u16*)(p + i) = fhSwap16(*(u16*)(p + i));
    }
  }
  for (i = 0x6bc; i < 0x6e0; i += 4) {
    *(u32*)(p + i) = fhSwap32(*(u32*)(p + i));
  }
}

void fhSwapSaveData(void* data) {
  u8* p = (u8*)data;
  u32 i;

  *(u32*)(p + 0x10) = fhSwap32(*(u32*)(p + 0x10));
  *(u32*)(p + 0x14) = fhSwap32(*(u32*)(p + 0x14));
  for (i = 0; i < 25; i++) {
    u32 offset = 0x1c + i * 8;
    *(u32*)(p + offset) = fhSwap32(*(u32*)(p + offset));
  }
}
