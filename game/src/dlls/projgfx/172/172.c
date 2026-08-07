/*
 * projmagicstream (DLL 172 / 0xAC) - retired projectile object.
 *
 * One of the stubbed-out projectile DLLs. The object has
 * no behaviour left: release/initialise are empty and doUnsupported just
 * logs the "no longer supported" string and returns the failure sentinel.
 * The slot is kept so the DLL id stays valid.
 */
#include "main/dll/dll_00AC_projmagicstream.h"
#include "dolphin/os/OSReport.h"

ProjmagicstreamResourceDescriptor gProjmagicstreamResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000},
    projmagicstream_initialise,
    projmagicstream_release,
    NULL,
    projmagicstream_doUnsupported,
};

char sProjmagicstreamDoNoLongerSupported[] = "<projmagicstream Do>No Longer supported \n";

int projmagicstream_doUnsupported(void) {
    OSReport(sProjmagicstreamDoNoLongerSupported);
    return -1;
}

void projmagicstream_release(void) {
}

void projmagicstream_initialise(void) {
}
