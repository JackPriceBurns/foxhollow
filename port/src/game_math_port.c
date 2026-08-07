#include <math.h>
#include <dolphin/types.h>

float mathSinf(float angle) { return sinf(angle); }
float mathSinfFast(float angle) { return sinf(angle); }
float mathSinfHighPrecision(float angle) { return sinf(angle); }
float mathSinfPrecise(float x) { return sinf(x); }
float mathCosf(float angle) { return cosf(angle); }
float mathCosfHighPrecision(float angle) { return cosf(angle); }
float mathCosfPrecise(float x) { return cosf(x); }
float mathTanf(float angle) { return tanf(angle); }

float trigReduceQuadrant(u16* quadrant, float angle) {
  float absoluteAngle = fabsf(angle);
  double scaledAngle = 1.2732395447351628 * absoluteAngle;
  u32 roundedQuadrant;
  if (scaledAngle <= 0.0) {
    roundedQuadrant = 0;
  } else if (scaledAngle >= 4294967295.0) {
    roundedQuadrant = 0xffffffffu;
  } else {
    roundedQuadrant = (u32)scaledAngle;
  }
  roundedQuadrant = (roundedQuadrant + 1) & ~1u;
  *quadrant = (u16)roundedQuadrant;
  return (float)(absoluteAngle - 0.7853981633974483 * (double)roundedQuadrant);
}

f32 powfCoreFast(f32 x, f32 y) { return powf(x, y); }
f32 powfCoreHighPrecision(f32 x, f32 y) { return powf(x, y); }
float powfBitEstimate(float base, float exponentValue) { return powf(base, exponentValue); }
float log2fBitEstimate(float value) { return log2f(value); }

void Vec_normalize(void* input, void* output) {
  f32* in = input;
  f32* out = output;
  f32 len = sqrtf(in[0] * in[0] + in[1] * in[1] + in[2] * in[2]);
  if (len != 0.0f) {
    f32 inv = 1.0f / len;
    out[0] = in[0] * inv;
    out[1] = in[1] * inv;
    out[2] = in[2] * inv;
  } else {
    out[0] = 0.0f;
    out[1] = 0.0f;
    out[2] = 0.0f;
  }
}
