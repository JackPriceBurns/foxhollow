/*
 * projtesla (DLL 190 / 0xBE) - defunct "tesla" projectile (behaviour cut).
 *
 * Its only non-trivial entry point reports the "no longer supported" string
 * via OSReport and returns 0. The release and initialise descriptor hooks are
 * empty stubs.
 */
#include "main/dll/dll_00BE_projtesla.h"
#include "dolphin/os/OSReport.h"

extern char sProjteslaDoNoLongerSupported[];

int projtesla_doUnsupported(void) {
    OSReport(sProjteslaDoNoLongerSupported);
    return 0;
}

void projtesla_release(void) {
}

void projtesla_initialise(void) {
}

ProjteslaResourceDescriptor gProjteslaResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000},
    projtesla_initialise,
    projtesla_release,
    NULL,
    projtesla_doUnsupported,
};

char sProjteslaDoNoLongerSupported[] = "<projtesla Do>No Longer supported \n";
