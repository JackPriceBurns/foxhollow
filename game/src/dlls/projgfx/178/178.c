/*
 * projrobotfire (DLL 178 / 0x00B2) - the robot fire projectile object.
 *
 * The entire retail DLL is a stub: doUnsupported logs "no longer supported"
 * via OSReport and returns -1; release/initialise are empty.
 */
#include "main/dll/dll_00B2_projrobotfire.h"
#include "dolphin/os/OSReport.h"

extern char sProjrobotfireDoNoLongerSupported[];

int projrobotfire_doUnsupported(void) {
    OSReport(sProjrobotfireDoNoLongerSupported);
    return -1;
}

void projrobotfire_release(void) {
}

void projrobotfire_initialise(void) {
}

ProjrobotfireResourceDescriptor gProjrobotfireResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000},
    projrobotfire_initialise,
    projrobotfire_release,
    NULL,
    projrobotfire_doUnsupported,
};

char sProjrobotfireDoNoLongerSupported[] = "<projrobotfire Do>No Longer supported \n";
