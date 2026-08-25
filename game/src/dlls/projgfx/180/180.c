/*
 * projenergise1 (DLL 180 / 0xB4) - retired "energise projectile" object DLL.
 *
 * The object itself is gone: its only real entry point logs a
 * "no longer supported" message and returns -1, while the standard DLL
 * lifecycle hooks (release/initialise) are empty stubs. Effectively a
 * placeholder that keeps the DLL id valid after the projectile behavior
 * was removed.
 */
#include "main/dll/dll_00B4_projenergise1.h"
#include "dolphin/os/OSReport.h"

int projenergise1_doUnsupported(void) {
    OSReport("<projenergise1 Do>No Longer supported \n");
    return -1;
}

void projenergise1_release(void) {
}

void projenergise1_initialise(void) {
}

RESOURCE_ACQUIRE_ADAPTER(gProjenergise1ResourceDescriptorAcquire, projenergise1_initialise)

Projenergise1ResourceDescriptor gProjenergise1ResourceDescriptor = {
    { {0x00000000, 0x00000000, 0x00000000, 0x00030000}, gProjenergise1ResourceDescriptorAcquire, projenergise1_release },
    NULL,
    projenergise1_doUnsupported,
};
