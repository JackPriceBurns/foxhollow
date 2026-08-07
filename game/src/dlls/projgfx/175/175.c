/*
 * projlightning1 (DLL 175 / 0x00AF) - retired "lightning 1" projectile object.
 *
 * The object's behaviour was cut from the shipping game: its "do" entry
 * point now only logs a "no longer supported" message and returns the
 * unsupported sentinel (-1). release/initialise are empty stubs kept so the
 * DLL still exports the standard projectile lifecycle entry points. This is
 * one of a family of identical retired projectile DLLs.
 */
#include "main/dll/dll_00AF_projlightning1.h"
#include "dolphin/os/OSReport.h"

extern char sProjlightning1DoNoLongerSupported[];

int projlightning1_doUnsupported(void) {
    OSReport(sProjlightning1DoNoLongerSupported);
    return -1;
}

void projlightning1_release(void) {
}

void projlightning1_initialise(void) {
}

Projlightning1ResourceDescriptor gProjlightning1ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000},
    projlightning1_initialise,
    projlightning1_release,
    NULL,
    projlightning1_doUnsupported,
};

char sProjlightning1DoNoLongerSupported[] = "<projlightning1 Do>No Longer supported \n";
