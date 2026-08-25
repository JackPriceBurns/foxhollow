/*
 * projlightning7 (DLL 185 / 0x00B9) - retired "lightning 7" projectile object.
 *
 * The object's behaviour was cut from the shipping game: its "do" entry
 * point now only logs a "no longer supported" message and returns the
 * unsupported sentinel (-1). release/initialise are empty stubs kept so the
 * DLL still exports the standard projectile lifecycle entry points. This is
 * one of a family of identical retired projectile DLLs.
 */
#include "main/dll/dll_00B9_projlightning7.h"
#include "dolphin/os/OSReport.h"

int projlightning7_doUnsupported(void) {
    OSReport("<projlightning7 Do>No Longer supported \n");
    return -1;
}

void projlightning7_release(void) {
}

void projlightning7_initialise(void) {
}

RESOURCE_ACQUIRE_ADAPTER(gProjlightning7ResourceDescriptorAcquire, projlightning7_initialise)

Projlightning7ResourceDescriptor gProjlightning7ResourceDescriptor = {
    { {0x00000000, 0x00000000, 0x00000000, 0x00030000}, gProjlightning7ResourceDescriptorAcquire, projlightning7_release },
    NULL,
    projlightning7_doUnsupported,
};
