/*
 * DummyA4 (DLL 164 / 0xA4) - an empty placeholder resource DLL.
 */
#include "main/dll/dll_00A4_dummya4.h"

int DummyA4_returnZero(void) {
    return 0;
}

void DummyA4_release(void) {
}

void DummyA4_initialise(void) {
}

DummyA4ResourceDescriptor gDummyA4ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000}, DummyA4_initialise, DummyA4_release, NULL, DummyA4_returnZero,
};
