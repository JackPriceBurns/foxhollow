/*
 * projenergise2 (DLL 181 / 0x00B5) - retired "energise" projectile object.
 *
 * Its entry point reports that the projectile is no longer supported and
 * returns the unsupported sentinel; release/initialise are empty stubs.
 */
#include "main/dll/dll_00B5_projenergise2.h"
#include "dolphin/os/OSReport.h"

Projenergise2ResourceDescriptor gProjenergise2ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000},
    projenergise2_initialise,
    projenergise2_release,
    NULL,
    projenergise2_doUnsupported,
};

char sProjenergise2DoNoLongerSupported[] = "<projenergise2 Do>No Longer supported \n";

int projenergise2_doUnsupported(void) {
    OSReport(sProjenergise2DoNoLongerSupported);
    return -1;
}

void projenergise2_release(void) {
}

void projenergise2_initialise(void) {
}
