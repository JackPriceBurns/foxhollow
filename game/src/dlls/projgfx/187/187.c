/*
 * projwallpower (DLL 187 / 0x00BB) - retired projectile "wall power" object.
 *
 * The object's behavior has been removed: the only live entry point,
 * projwallpower_doUnsupported, just reports that the feature is no longer
 * supported and returns 0 (false). release/initialise are empty lifecycle
 * stubs.
 *
 * Note: unlike the rest of the retired-projectile family (projlightning1,
 * projquakeshock, etc.) which return the -1 unsupported sentinel, wallpower
 * returns 0.
 */
#include "main/dll/dll_00BB_projwallpower.h"
#include "dolphin/os/OSReport.h"

extern char sProjwallpowerDoNoLongerSupported[];

int projwallpower_doUnsupported(void) {
    OSReport(sProjwallpowerDoNoLongerSupported);
    return 0;
}

void projwallpower_release(void) {
}

void projwallpower_initialise(void) {
}

ProjwallpowerResourceDescriptor gProjwallpowerResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000},
    projwallpower_initialise,
    projwallpower_release,
    NULL,
    projwallpower_doUnsupported,
};

char sProjwallpowerDoNoLongerSupported[] = "<projwallpower Do>No Longer supported \n";
