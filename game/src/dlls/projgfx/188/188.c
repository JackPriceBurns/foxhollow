/*
 * projquakeshock (DLL 188 / 0xBC) - retired "quake shock" projectile object.
 *
 * The DLL's lifecycle hooks (release/initialise) are empty and its single
 * entry point logs a "no longer supported" message and returns a failure
 * code, so this projectile type has been disabled in retail.
 */
#include "main/dll/dll_00BC_projquakeshock.h"
#include "dolphin/os/OSReport.h"

extern char sProjquakeshockDoNoLongerSupported[];

int projquakeshock_doUnsupported(void) {
    OSReport(sProjquakeshockDoNoLongerSupported);
    return -1;
}

void projquakeshock_release(void) {
}

void projquakeshock_initialise(void) {
}

ProjquakeshockResourceDescriptor gProjquakeshockResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000},
    projquakeshock_initialise,
    projquakeshock_release,
    NULL,
    projquakeshock_doUnsupported,
};

char sProjquakeshockDoNoLongerSupported[] = "<projquakeshock Do>No Longer supported \n";
