/*
 * projship1 (DLL 183 / 0xB7) - retired "ship projectile 1" object.
 *
 * The object's behaviour was removed during development: its only live
 * entry point, projship1_doUnsupported, just logs a "no longer supported"
 * message and returns -1. release/initialise are empty lifecycle hooks
 * kept so the object descriptor / DLL loader still resolves.
 */
#include "main/dll/dll_00B7_projship1.h"
#include "dolphin/os/OSReport.h"

extern char sProjship1DoNoLongerSupported[];

int projship1_doUnsupported(void) {
    OSReport(sProjship1DoNoLongerSupported);
    return -1;
}

void projship1_release(void) {
}

void projship1_initialise(void) {
}

Projship1ResourceDescriptor gProjship1ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000},
    projship1_initialise,
    projship1_release,
    NULL,
    projship1_doUnsupported,
};

char sProjship1DoNoLongerSupported[] = "<projship1 Do>No Longer supported \n";
