#include "main/dll/modgfx.h"
#include "dolphin/os/OSReport.h"

int projgfx_rayhit_doUnsupported(void) {
    OSReport("<projgfx rayhit Do>No Longer supported \n");
    return 0;
}

int projgfx_setzscale_doUnsupported(void) {
    OSReport("<projgfx setzscale  Do>No Longer supported \n");
    return 0;
}

int projgfx_getObjectTypeId(void) {
    return 0x0;
}

void projgfx_func07_nop(void) {
}

void projgfx_func06_nop(void) {
}

void projgfx_func05_nop(void) {
}

int projgfx_func04_ret_m1(void) {
    return -0x1;
}

void projgfx_onMapSetup(void) {
}

void projgfx_release_doUnsupported(void) {
    OSReport("<projgfx release Do>No Longer supported \n");
}

void projgfx_initialise(void) {
}

RESOURCE_ACQUIRE_ADAPTER(gProjgfxResourceAcquire, projgfx_initialise)

ProjgfxDllInterface projgfx_funcs = {
    {
        {0, 0, 0, OBJECT_DESCRIPTOR_FLAGS_11_SLOTS},
        gProjgfxResourceAcquire,
        projgfx_release_doUnsupported,
    },
    {
        {0},
        projgfx_onMapSetup,
        projgfx_func04_ret_m1,
        projgfx_func05_nop,
        projgfx_func06_nop,
        projgfx_func07_nop,
        projgfx_getObjectTypeId,
        projgfx_setzscale_doUnsupported,
        projgfx_rayhit_doUnsupported,
    },
};
