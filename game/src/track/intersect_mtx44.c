#include "global.h"


/* 4x4 identity fill. */
void mtx44Identity(f32* mat)
{
    int i = 0, j;
    f32 zero, one;
    one = 1.0f;
    zero = 0.0f;
    for (; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (i == j)
                mat[j] = one;
            else
                mat[j] = zero;
        }
        mat += 4;
    }
}
