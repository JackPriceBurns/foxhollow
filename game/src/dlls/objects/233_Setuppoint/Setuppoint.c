/*
 * Setuppoint object (DLL slot 233 / 0xE9).
 *
 * This marker object exposes only an empty initialization callback.
 */
#include "dlls/objects/233_Setuppoint.h"

void setuppoint_init(void) {
}

OBJECT_INIT_ADAPTER(gSetuppointObjDescriptorInitAdapter, setuppoint_init)

ObjectDescriptor gSetuppointObjDescriptor = {
    {
        {
            0,
            0,
            0,
            OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,
        },
        0,
        0,
    },
    0,
    gSetuppointObjDescriptorInitAdapter,
    0,
    0,
    0,
    0,
    0,
    0,
};
