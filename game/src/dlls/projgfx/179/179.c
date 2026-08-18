/*
 * projlightning4 (DLL 179 / 0x00B3) - a retired projectile object.
 *
 * The lightning-4 projectile was cut from the shipped game: its object
 * entry point (projlightning4_doUnsupported) only logs that it is "no longer
 * supported" and returns the unsupported sentinel. release/initialise are the
 * empty object lifecycle hooks that remain so the object descriptor stays valid.
 */
#include "main/dll/dll_00B3_projlightning4.h"
#include "dolphin/os/OSReport.h"

int projlightning4_doUnsupported(void) {
    OSReport("<projlightning4 Do>No Longer supported \n");
    return -1;
}

void projlightning4_release(void) {
}

void projlightning4_initialise(void) {
}

Projlightning4ResourceDescriptor gProjlightning4ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000},
    projlightning4_initialise,
    projlightning4_release,
    NULL,
    projlightning4_doUnsupported,
};
