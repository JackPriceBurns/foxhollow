/*
 * projcore3 (DLL 193 / 0x00C1) - retired projectile-core variant.
 *
 * Every functional entry point has been stubbed out: the lone behavioural
 * hook, projcore3_doUnsupported, just logs the "no longer supported"
 * message and reports failure (-1). release/initialise are empty no-ops.
 * The DLL is kept as a placeholder so its slot/id remains valid.
 */
#include "main/dll/dll_00C1_projcore3.h"
#include "dolphin/os/OSReport.h"

int projcore3_doUnsupported(void) {
    OSReport("<projcore3 Do>No Longer supported \n");
    return -1;
}

void projcore3_release(void) {
}

void projcore3_initialise(void) {
}

RESOURCE_ACQUIRE_ADAPTER(gProjcore3ResourceDescriptorAcquire, projcore3_initialise)

Projcore3ResourceDescriptor gProjcore3ResourceDescriptor = {
    { {0x00000000, 0x00000000, 0x00000000, 0x00030000}, gProjcore3ResourceDescriptorAcquire, projcore3_release },
    NULL,
    projcore3_doUnsupported,
};
