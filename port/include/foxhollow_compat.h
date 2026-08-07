#ifndef FOXHOLLOW_COMPAT_H
#define FOXHOLLOW_COMPAT_H

#include <math.h>
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

unsigned int sfaRand(void);
void sfaSrand(unsigned int seed);

#endif
