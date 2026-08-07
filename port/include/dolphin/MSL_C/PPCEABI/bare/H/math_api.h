#ifndef FOXHOLLOW_SHIM_MATH_API_H
#define FOXHOLLOW_SHIM_MATH_API_H

#include <math.h>
#include "types.h"
#include <dolphin/MSL_C/PPCEABI/bare/H/math_trig_api.h>
#include <dolphin/MSL_C/PPCEABI/bare/H/math_float_helpers.h>

f32 asinf(f32 x);
f32 powfCoreFast(f32 x, f32 y);
f32 powfCoreHighPrecision(f32 x, f32 y);
float powfBitEstimate(float base, float exponentValue);
void Vec_normalize(void* input, void* output);
void Vec_scale(void* input, void* output, float scale);
float Vec_lengthSquared(void* input);
float trigReduceQuadrant(u16* quadrant, float angle);
float acosf_fast(float x);
float atanf_fast(float x);
void mathSinCosf(float angle, float* sinOut, float* cosOut);
float mathSinfPrecise(float x);
float mathCosfPrecise(float x);
float mathTanf(float angle);
float log2fBitEstimate(float value);

#endif
