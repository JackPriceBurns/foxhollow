#ifndef FOXHOLLOW_COMPAT_H
#define FOXHOLLOW_COMPAT_H

#include <math.h>
#include <stdint.h>
#include <string.h>

static inline double __frsqrte(double x) { return 1.0 / sqrt(x); }
static inline float __fres(float x) { return 1.0f / x; }
static inline float __fabsf(float x) { return fabsf(x); }
static inline double __fabs(double x) { return fabs(x); }
static inline int __cntlzw(unsigned int x) { return x ? __builtin_clz(x) : 32; }
static inline unsigned int __cvt_fp2unsigned(double x) {
  if (x <= 0.0) return 0;
  if (x >= 4294967295.0) return 0xffffffffu;
  return (unsigned int)x;
}

static inline unsigned int fhSwap32(unsigned int v) { return __builtin_bswap32(v); }
static inline unsigned short fhSwap16(unsigned short v) { return __builtin_bswap16(v); }
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

void fhSwapResidentTabs(void);
void fhSwapTabBufferOnce(void* buf, unsigned int words);
void fhSwapTab16BufferOnce(void* buf, unsigned int halves);
void fhSwapTabTableInPlace(void* table);
void fhSwapObjDef(void* def);
void fhSwapRomListSection(void* buf, unsigned int size);
int fhTabIs16Bit(const char* name);
unsigned int sfaRand(void);
void sfaSrand(unsigned int seed);

#define GX_PNMTX_IDENTITY 27
void fhLoadIdentityPosMtx(void);
void fhAIPump(void);
uintptr_t fhAIGetDMAStartAddr(void);
int fhAIPrepareStream(void* fileInfo);
void fhMusyxMix(short* destination);

#endif
