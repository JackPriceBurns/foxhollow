#include "global.h"

void mtx44Identity(f32* mat) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (i == j) {
                mat[j] = 1.0f;
            } else {
                mat[j] = 0.0f;
            }
        }
        mat += 4;
    }
}
