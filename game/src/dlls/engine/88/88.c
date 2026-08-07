/*
 * DLL 88 / 0x58 - a no-op DLL.
 */
#include "main/dll/dll_0058_dummy58.h"

int Dummy58_init(void) {
    return 0;
}

void Dummy58_release(void) {
}

void Dummy58_initialise(void) {
}

Dummy58Descriptor gDummy58Descriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000}, Dummy58_initialise, Dummy58_release, NULL, Dummy58_init,
};
