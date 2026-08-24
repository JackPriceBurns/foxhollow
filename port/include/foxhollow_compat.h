#ifndef FOXHOLLOW_COMPAT_H
#define FOXHOLLOW_COMPAT_H

#include <math.h>
#include <stdint.h>
#include <string.h>

static inline double __frsqrte(double x) { return 1.0 / sqrt(x); }
static inline float __fres(float x) { return 1.0f / x; }
#undef __fabsf
#undef __fabs
#define __fabsf(x) __builtin_fabsf(x)
#define __fabs(x) __builtin_fabs(x)
static inline int __cntlzw(unsigned int x) { return x ? __builtin_clz(x) : 32; }
static inline unsigned int __cvt_fp2unsigned(double x) {
  if (x <= 0.0) return 0;
  if (x >= 4294967295.0) return 0xffffffffu;
  return (unsigned int)x;
}

static inline unsigned int fhSwap32(unsigned int v) { return __builtin_bswap32(v); }
static inline unsigned short fhSwap16(unsigned short v) { return __builtin_bswap16(v); }
static inline unsigned long long fhSwap64(unsigned long long v) { return __builtin_bswap64(v); }
static inline int fhAboveRetailMem1Watermark(const void* p) { (void)p; return 0; }
static inline uint32_t fhReadBE32(const void* p) {
  const uint8_t* bytes = (const uint8_t*)p;
  return ((uint32_t)bytes[0] << 24) | ((uint32_t)bytes[1] << 16) |
         ((uint32_t)bytes[2] << 8) | bytes[3];
}
static inline uint64_t fhReadBE64(const void* p) {
  const uint8_t* bytes = (const uint8_t*)p;
  return ((uint64_t)bytes[0] << 56) | ((uint64_t)bytes[1] << 48) |
         ((uint64_t)bytes[2] << 40) | ((uint64_t)bytes[3] << 32) |
         ((uint64_t)bytes[4] << 24) | ((uint64_t)bytes[5] << 16) |
         ((uint64_t)bytes[6] << 8) | bytes[7];
}
static inline void fhWriteBE64(void* p, uint64_t value) {
  uint8_t* bytes = (uint8_t*)p;
  bytes[0] = (uint8_t)(value >> 56);
  bytes[1] = (uint8_t)(value >> 48);
  bytes[2] = (uint8_t)(value >> 40);
  bytes[3] = (uint8_t)(value >> 32);
  bytes[4] = (uint8_t)(value >> 24);
  bytes[5] = (uint8_t)(value >> 16);
  bytes[6] = (uint8_t)(value >> 8);
  bytes[7] = (uint8_t)value;
}
static inline float fhReadBEF32(const void* p) {
  uint32_t bits = fhReadBE32(p);
  float value;
  memcpy(&value, &bits, sizeof(value));
  return value;
}
static inline uint16_t fhReadBE16(const void* p) {
  const uint8_t* bytes = (const uint8_t*)p;
  return ((uint16_t)bytes[0] << 8) | bytes[1];
}
static inline int16_t fhReadBES16(const void* p) {
  uint16_t bits = fhReadBE16(p);
  int16_t value;
  memcpy(&value, &bits, sizeof(value));
  return value;
}
static inline void fhSwapU16Array(void* p, unsigned long halves) {
  unsigned short* h = (unsigned short*)p;
  while (halves--) {
    *h = __builtin_bswap16(*h);
    h++;
  }
}
static inline void fhSwapU32Array(void* p, unsigned long words) {
  unsigned int* w = (unsigned int*)p;
  while (words--) {
    *w = __builtin_bswap32(*w);
    w++;
  }
}

void fhSwapLoadedTabFile(void* buf, unsigned int size, const char* name);
void fhSwapObjDef(void* def);
void fhSwapRomListSection(void* buf, unsigned int size);
void fhSwapSaveGameSlot(void* data);
void fhSwapSaveData(void* data);
int fhTabIs16Bit(const char* name);
int fhConfigRevision(void);
unsigned int sfaRand(void);
void sfaSrand(unsigned int seed);

#define GX_PNMTX_IDENTITY 27

#define GXSetCullMode fhGXSetCullMode
void fhLoadIdentityPosMtx(void);
void fhAIPump(void);
uintptr_t fhAIGetDMAStartAddr(void);
int fhAIPrepareStream(void* fileInfo);
void fhMusyxMix(short* destination);

#endif
