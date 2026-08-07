/*
 * projsquirt1 (DLL 182 / 0xB6) - retired "squirt" projectile object.
 *
 * The object's behaviour has been removed: its single live entry point
 * just logs that it is no longer supported and returns failure. The
 * release/initialise descriptor hooks are empty stubs.
 */
#include "main/dll/dll_00B6_projsquirt1.h"
#include "dolphin/os/OSReport.h"

Projsquirt1ResourceDescriptor gProjsquirt1ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000},
    projsquirt1_initialise,
    projsquirt1_release,
    NULL,
    projsquirt1_doUnsupported,
};

char sProjsquirt1DoNoLongerSupported[] = "<projsquirt1 Do>No Longer supported \n";

int projsquirt1_doUnsupported(void) {
    OSReport(sProjsquirt1DoNoLongerSupported);
    return -1;
}

void projsquirt1_release(void) {
}

void projsquirt1_initialise(void) {
}
