/*
 * DLL 108 / 0x6C - a no-op DLL.
 */
#include "main/dll/dll_006C_dummy6c.h"

int Dummy6C_init(void) {
    return 0;
}

void Dummy6C_release(void) {
}

void Dummy6C_initialise(void) {
}

RESOURCE_ACQUIRE_ADAPTER(gDummy6CDescriptorAcquire, Dummy6C_initialise)

Dummy6CDescriptor gDummy6CDescriptor = {
    { {0x00000000, 0x00000000, 0x00000000, 0x00030000}, gDummy6CDescriptorAcquire, Dummy6C_release },
    NULL,
    Dummy6C_init,
};
