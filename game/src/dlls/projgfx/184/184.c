/*
 * projlightning5 (DLL 184 / 0x00B8) - retired "lightning 5" projectile object.
 *
 * The object's behaviour was cut from the shipping game: its "do" entry
 * point now only logs a "no longer supported" message and returns the
 * unsupported sentinel (-1). release/initialise are empty stubs kept so the
 * DLL still exports the standard projectile lifecycle entry points. One of
 * a family of identical retired projectile DLLs.
 */
#include "main/dll/dll_00B8_projlightning5.h"
#include "dolphin/os/OSReport.h"

extern char sProjlightning5DoNoLongerSupported[];

int projlightning5_doUnsupported(void) {
    OSReport(sProjlightning5DoNoLongerSupported);
    return -1;
}

void projlightning5_release(void) {
}

void projlightning5_initialise(void) {
}

Projlightning5ResourceDescriptor gProjlightning5ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000},
    projlightning5_initialise,
    projlightning5_release,
    NULL,
    projlightning5_doUnsupported,
};

char sProjlightning5DoNoLongerSupported[] = "<projlightning5 Do>No Longer supported \n";
