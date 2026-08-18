/*
 * projcore2 (DLL 192 / 0xC0) - retired projectile-core DLL.
 *
 * The only live entry point reports that projectiles are no longer
 * supported and returns the unsupported sentinel (-1); the DLL's
 * release/initialise lifecycle hooks are empty stubs.
 */
#include "main/dll/dll_00C0_projcore2.h"
#include "dolphin/os/OSReport.h"

int projcore2_doUnsupported(void) {
    OSReport("<projcore2 Do>No Longer supported \n");
    return -1;
}

void projcore2_release(void) {
}

void projcore2_initialise(void) {
}

Projcore2ResourceDescriptor gProjcore2ResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000},
    projcore2_initialise,
    projcore2_release,
    NULL,
    projcore2_doUnsupported,
};
