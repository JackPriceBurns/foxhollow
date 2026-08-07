/*
 * projroombeam (DLL 174 / 0xAE) - retired projectile object.
 *
 * One of the stubbed-out projectile DLLs. The object has
 * no behaviour left: release/initialise are empty and doUnsupported just
 * logs the "no longer supported" string and returns the failure sentinel.
 * The slot is kept so the DLL id stays valid.
 */
#include "main/dll/dll_00AE_projroombeam.h"
#include "dolphin/os/OSReport.h"

extern char sProjroombeamDoNoLongerSupported[];

int projroombeam_doUnsupported(void) {
    OSReport(sProjroombeamDoNoLongerSupported);
    return -1;
}

void projroombeam_release(void) {
}

void projroombeam_initialise(void) {
}

ProjroombeamResourceDescriptor gProjroombeamResourceDescriptor = {
    {0x00000000, 0x00000000, 0x00000000, 0x00030000},
    projroombeam_initialise,
    projroombeam_release,
    NULL,
    projroombeam_doUnsupported,
};

char sProjroombeamDoNoLongerSupported[] = "<projroombeam Do>No Longer supported \n";
