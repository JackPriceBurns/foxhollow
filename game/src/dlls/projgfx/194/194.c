/*
 * projdfp1r (DLL 194 / 0xC2) - retired "dfp1r" projectile object.
 *
 * The object is no longer supported: its single behavior entry point just
 * prints the "projdfp1r ... No Longer supported" banner and returns -1, and
 * the load/unload hooks are empty stubs.
 */
#include "main/dll/dll_00C2_projdfp1r.h"
#include "dolphin/os/OSReport.h"

extern char sProjdfp1rDoNoLongerSupported[];

int projdfp1r_doUnsupported(void) {
    OSReport(sProjdfp1rDoNoLongerSupported);
    return -1;
}

void projdfp1r_release(void) {
}

void projdfp1r_initialise(void) {
}

Projdfp1rResourceDescriptor gProjdfp1rResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000},
    projdfp1r_initialise,
    projdfp1r_release,
    NULL,
    projdfp1r_doUnsupported,
};

char sProjdfp1rDoNoLongerSupported[] = "<projdfp1r Do>No Longer supported \n";
