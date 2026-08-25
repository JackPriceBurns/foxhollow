/*
 * projrobotfire (DLL 178 / 0x00B2) - the robot fire projectile object.
 *
 * The entire retail DLL is a stub: doUnsupported logs "no longer supported"
 * via OSReport and returns -1; release/initialise are empty.
 */
#include "main/dll/dll_00B2_projrobotfire.h"
#include "dolphin/os/OSReport.h"

int projrobotfire_doUnsupported(void) {
    OSReport("<projrobotfire Do>No Longer supported \n");
    return -1;
}

void projrobotfire_release(void) {
}

void projrobotfire_initialise(void) {
}

RESOURCE_ACQUIRE_ADAPTER(gProjrobotfireResourceDescriptorAcquire, projrobotfire_initialise)

ProjrobotfireResourceDescriptor gProjrobotfireResourceDescriptor = {
    { {0x00000000, 0x00000000, 0x00000000, 0x00030000}, gProjrobotfireResourceDescriptorAcquire, projrobotfire_release },
    NULL,
    projrobotfire_doUnsupported,
};
