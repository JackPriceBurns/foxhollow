#include "PowerPC_EABI_Support/Runtime/runtime.h"
#include "main/math_80292d3c.h"


double trigReduceQuadrantHighPrecision(int* quadrant, float angle)
{
    unsigned int roundedQuadrant;
    double absoluteAngle;
    double scaledAngle;

    absoluteAngle = __fabsf(angle);
    scaledAngle = 1.2732395447351628 * absoluteAngle;
    roundedQuadrant = (__cvt_fp2unsigned(scaledAngle) + 1) & ~1U;
    *quadrant = roundedQuadrant;
    return absoluteAngle - 0.7853981633974483 * (double)roundedQuadrant;
}
