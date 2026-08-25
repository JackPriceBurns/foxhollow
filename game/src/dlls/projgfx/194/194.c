/*
 * projdfp1r (DLL 194 / 0xC2) - retired "dfp1r" projectile object.
 *
 * The object is no longer supported: its single behavior entry point just
 * prints the "projdfp1r ... No Longer supported" banner and returns -1, and
 * the load/unload hooks are empty stubs.
 */
#include "main/dll/dll_00C2_projdfp1r.h"
#include "dolphin/os/OSReport.h"

int projdfp1r_doUnsupported(void) {
    OSReport("<projdfp1r Do>No Longer supported \n");
    return -1;
}

void projdfp1r_release(void) {
}

void projdfp1r_initialise(void) {
}

RESOURCE_ACQUIRE_ADAPTER(gProjdfp1rResourceDescriptorAcquire, projdfp1r_initialise)

Projdfp1rResourceDescriptor gProjdfp1rResourceDescriptor = {
    { {0x00000000, 0x00000000, 0x00000000, 0x00030000}, gProjdfp1rResourceDescriptorAcquire, projdfp1r_release },
    NULL,
    projdfp1r_doUnsupported,
};
