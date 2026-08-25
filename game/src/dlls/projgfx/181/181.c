/*
 * projenergise2 (DLL 181 / 0x00B5) - retired "energise" projectile object.
 *
 * Its entry point reports that the projectile is no longer supported and
 * returns the unsupported sentinel; release/initialise are empty stubs.
 */
#include "main/dll/dll_00B5_projenergise2.h"
#include "dolphin/os/OSReport.h"

int projenergise2_doUnsupported(void) {
    OSReport("<projenergise2 Do>No Longer supported \n");
    return -1;
}

void projenergise2_release(void) {
}

void projenergise2_initialise(void) {
}

RESOURCE_ACQUIRE_ADAPTER(gProjenergise2ResourceDescriptorAcquire, projenergise2_initialise)

Projenergise2ResourceDescriptor gProjenergise2ResourceDescriptor = {
    { {0x00000000, 0x00000000, 0x00000000, 0x00030000}, gProjenergise2ResourceDescriptorAcquire, projenergise2_release },
    NULL,
    projenergise2_doUnsupported,
};
