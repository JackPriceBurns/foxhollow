#include "dolphin.h"

static u32 sRandState;

u32 sfaRand(void) {
    sRandState = sRandState * 0x19660D + 0x3C6EF35F;
    return sRandState;
}

void sfaSrand(u32 seed) {
    sRandState = seed;
}
