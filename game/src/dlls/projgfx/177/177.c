/*
 * projlightning3 (DLL 177 / 0x00B1) - a retired projectile object.
 *
 * The lightning-3 projectile was cut from the shipped game: its object
 * entry point (projlightning3_doUnsupported) only logs that it is "no longer supported"
 * and returns the unsupported sentinel. release/initialise are the empty
 * object lifecycle hooks that remain so the object descriptor stays valid.
 */
#include "main/dll/dll_00B1_projlightning3.h"
#include "dolphin/os/OSReport.h"

int projlightning3_doUnsupported(void) {
    OSReport("<projlightning3 Do>No Longer supported \n");
    return -1;
}

void projlightning3_release(void) {
}

void projlightning3_initialise(void) {
}

RESOURCE_ACQUIRE_ADAPTER(gProjlightning3ResourceDescriptorAcquire, projlightning3_initialise)

Projlightning3ResourceDescriptor gProjlightning3ResourceDescriptor = {
    { {0x00000000, 0x00000000, 0x00000000, 0x00030000}, gProjlightning3ResourceDescriptorAcquire, projlightning3_release },
    NULL,
    projlightning3_doUnsupported,
};
