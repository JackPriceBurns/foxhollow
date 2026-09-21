/*
 * projcore1 (DLL 191 / 0xBF) - retired projectile-core DLL.
 *
 * The only live entry point reports that projectiles are no longer
 * supported and returns the unsupported sentinel (-1); the DLL's
 * release/initialise lifecycle hooks are empty stubs.
 */
#include "main/dll/dll_00BF_projcore1.h"
#include "dolphin/os/OSReport.h"

int projcore1_doUnsupported(void) {
    OSReport("<projcore1 Do>No Longer supported \n");
    return -1;
}

void projcore1_release(void) {
}

void projcore1_initialise(void) {
}

RESOURCE_ACQUIRE_ADAPTER(gProjcore1ResourceDescriptorAcquire, projcore1_initialise)

Projcore1ResourceDescriptor gProjcore1ResourceDescriptor = {
    { {0x00000000, 0x00000000, 0x00000000, 0x00030000}, gProjcore1ResourceDescriptorAcquire, projcore1_release },
    NULL,
    projcore1_doUnsupported,
};
