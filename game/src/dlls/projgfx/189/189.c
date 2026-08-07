/*
 * projsunshock (DLL 189 / 0xBD) - a deprecated projectile object DLL.
 *
 * Only the standard DLL lifecycle entry points survive. The object is
 * no longer functional: doUnsupported logs a "no longer supported"
 * message and returns the failure sentinel, while release/initialise are
 * empty stubs that keep the DLL loadable.
 */
#include "main/dll/dll_00BD_projsunshock.h"
#include "dolphin/os/OSReport.h"

extern char sProjsunshockDoNoLongerSupported[];

int projsunshock_doUnsupported(void) {
    OSReport(sProjsunshockDoNoLongerSupported);
    return -1;
}

void projsunshock_release(void) {
}

void projsunshock_initialise(void) {
}

ProjsunshockResourceDescriptor gProjsunshockResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000},
    projsunshock_initialise,
    projsunshock_release,
    NULL,
    projsunshock_doUnsupported,
};

char sProjsunshockDoNoLongerSupported[] = "<projsunshock Do>No Longer supported \n";
