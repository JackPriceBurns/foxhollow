/*
 * projlightning3 (DLL 177 / 0x00B1) - a retired projectile object.
 *
 * The lightning-3 projectile was cut from the shipped game: its object
 * entry point (projlightning3_doUnsupported) only logs that it is "no longer supported"
 * and returns the unsupported sentinel. release/initialise are the empty
 * object lifecycle hooks that remain so the object descriptor stays valid.
 */
#include "main/dll/dll_00B1_projlightning3.h"
#include "dolphin/os/OSReport.h"

Projlightning3ResourceDescriptor gProjlightning3ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000},
    projlightning3_initialise,
    projlightning3_release,
    NULL,
    projlightning3_doUnsupported,
};

char sProjlightning3DoNoLongerSupported[] = "<projlightning3 Do>No Longer supported \n";

int projlightning3_doUnsupported(void) {
    OSReport(sProjlightning3DoNoLongerSupported);
    return -1;
}

void projlightning3_release(void) {
}

void projlightning3_initialise(void) {
}
