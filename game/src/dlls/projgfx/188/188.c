/*
 * projquakeshock (DLL 188 / 0xBC) - retired "quake shock" projectile object.
 *
 * The DLL's lifecycle hooks (release/initialise) are empty and its single
 * entry point logs a "no longer supported" message and returns a failure
 * code, so this projectile type has been disabled in retail.
 */
#include "main/dll/dll_00BC_projquakeshock.h"
#include "dolphin/os/OSReport.h"

int projquakeshock_doUnsupported(void) {
    OSReport("<projquakeshock Do>No Longer supported \n");
    return -1;
}

void projquakeshock_release(void) {
}

void projquakeshock_initialise(void) {
}

RESOURCE_ACQUIRE_ADAPTER(gProjquakeshockResourceDescriptorAcquire, projquakeshock_initialise)

ProjquakeshockResourceDescriptor gProjquakeshockResourceDescriptor = {
    {{0x00000000, 0x00000000, 0x00000000, 0x00030000},
     gProjquakeshockResourceDescriptorAcquire,
     projquakeshock_release},
    NULL,
    projquakeshock_doUnsupported,
};
