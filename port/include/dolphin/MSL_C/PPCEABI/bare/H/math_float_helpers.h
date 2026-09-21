#ifndef FOXHOLLOW_SHIM_MATH_FLOAT_HELPERS_H
#define FOXHOLLOW_SHIM_MATH_FLOAT_HELPERS_H

#include <math.h>
#include <dolphin/types.h>

static inline float fastCastS16ToFloat(const s16* input) { return (float)*input; }
static inline void fastCastFloatToS16(float value, s16* output) { *output = (s16)value; }
static inline float fastCastU16ToFloat(const u16* input) { return (float)*input; }
static inline void fastCastFloatToU16(float value, u16* output) { *output = (u16)value; }
static inline float fastFloorf(float value) { return floorf(value); }

#endif
