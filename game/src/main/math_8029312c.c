#include "main/math_8029312c.h"

float sqrtfHighPrecision(float value) {
    if (value == 0.0f) {
        return 0.0f;
    }

    float reciprocalSqrt = (float)__frsqrte(value);
    float halfValue = 0.5f * value;
    reciprocalSqrt = reciprocalSqrt * (1.5f - reciprocalSqrt * (halfValue * reciprocalSqrt));
    reciprocalSqrt = reciprocalSqrt * (1.5f - reciprocalSqrt * (halfValue * reciprocalSqrt));
    reciprocalSqrt = reciprocalSqrt * (1.5f - reciprocalSqrt * (halfValue * reciprocalSqrt));
    return reciprocalSqrt * value;
}

float invSqrt(float value) {
    float reciprocalSqrt = (float)__frsqrte(value);
    float halfValue = 0.5f * value;
    reciprocalSqrt = reciprocalSqrt * (1.5f - reciprocalSqrt * (halfValue * reciprocalSqrt));
    return reciprocalSqrt;
}
