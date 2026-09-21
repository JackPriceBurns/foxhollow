/*
 * projdummy (DLL 171 / 0xAB) - retired projectile object.
 *
 * One of the stubbed-out projectile DLLs. The object has
 * no behaviour left: release/initialise are empty and doUnsupported just
 * logs the "no longer supported" string and returns the failure sentinel.
 * The slot is kept so the DLL id stays valid.
 */
#include "main/dll/dll_00AB_projdummy.h"
#include "dolphin/os/OSReport.h"

int projdummy_doUnsupported(void) {
    OSReport("<projdummy Do>No Longer supported \n");
    return -1;
}

void projdummy_release(void) {
}

void projdummy_initialise(void) {
}

RESOURCE_ACQUIRE_ADAPTER(gProjdummyResourceDescriptorAcquire, projdummy_initialise)

ProjdummyResourceDescriptor gProjdummyResourceDescriptor = {
    { {0x00000000, 0x00000000, 0x00000000, 0x00030000}, gProjdummyResourceDescriptorAcquire, projdummy_release },
    NULL,
    projdummy_doUnsupported,
};
