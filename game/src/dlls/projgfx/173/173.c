/*
 * projmagicemmit1 (DLL 173 / 0xAD) - retired projectile object.
 *
 * One of the stubbed-out projectile DLLs. The object has
 * no behaviour left: release/initialise are empty and doUnsupported just
 * logs the "no longer supported" string and returns the failure sentinel.
 * The slot is kept so the DLL id stays valid.
 */
#include "main/dll/dll_00AD_projmagicemmit1.h"
#include "dolphin/os/OSReport.h"

extern char sProjmagicemmit1DoNoLongerSupported[];

int projmagicemmit1_doUnsupported(void) {
    OSReport(sProjmagicemmit1DoNoLongerSupported);
    return -1;
}

void projmagicemmit1_release(void) {
}

void projmagicemmit1_initialise(void) {
}

Projmagicemmit1ResourceDescriptor gProjmagicemmit1ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000},
    projmagicemmit1_initialise,
    projmagicemmit1_release,
    NULL,
    projmagicemmit1_doUnsupported,
};

char sProjmagicemmit1DoNoLongerSupported[] = "<projmagicemmit1 Do>No Longer supported \n";
