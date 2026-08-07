/*
 * projlightning6 (DLL 186 / 0xBA) - retired lightning-projectile object.
 *
 * Only a deprecation stub survives: the object's main entry point logs
 * "no longer supported" and returns the unsupported sentinel; release and
 * initialise are empty. The behaviour was removed before retail, so the
 * DLL exists purely to keep the object id slot wired up.
 */
#include "main/dll/dll_00BA_projlightning6.h"
#include "dolphin/os/OSReport.h"

extern char sProjlightning6DoNoLongerSupported[];

int projlightning6_doUnsupported(void) {
    OSReport(sProjlightning6DoNoLongerSupported);
    return -1;
}

void projlightning6_release(void) {
}

void projlightning6_initialise(void) {
}

Projlightning6ResourceDescriptor gProjlightning6ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000},
    projlightning6_initialise,
    projlightning6_release,
    NULL,
    projlightning6_doUnsupported,
};

char sProjlightning6DoNoLongerSupported[] = "<projlightning6 Do>No Longer supported \n";
