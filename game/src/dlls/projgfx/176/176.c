/*
 * projlightning2 (DLL 176 / 0x00B0) - retired "lightning 2" projectile object.
 *
 * The object's behaviour was cut from the shipping game: its "do" entry
 * point now only logs a "no longer supported" message and returns the
 * unsupported sentinel (-1). release/initialise are empty stubs kept so the
 * DLL still exports the standard projectile lifecycle entry points. This is
 * one of a family of identical retired projectile DLLs.
 */
#include "main/dll/dll_00B0_projlightning2.h"
#include "dolphin/os/OSReport.h"

extern char sProjlightning2DoNoLongerSupported[];

int projlightning2_doUnsupported(void) {
    OSReport(sProjlightning2DoNoLongerSupported);
    return -1;
}

void projlightning2_release(void) {
}

void projlightning2_initialise(void) {
}

Projlightning2ResourceDescriptor gProjlightning2ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000},
    projlightning2_initialise,
    projlightning2_release,
    NULL,
    projlightning2_doUnsupported,
};

char sProjlightning2DoNoLongerSupported[] = "<projlightning2 Do>No Longer supported \n";
