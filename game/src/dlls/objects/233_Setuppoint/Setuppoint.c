/*
 * Setuppoint object (DLL slot 233 / 0xE9).
 *
 * This marker object exposes only an empty initialization callback.
 */
#include "dlls/objects/233_Setuppoint.h"

void setuppoint_init(void) {
}

ObjectDescriptor gSetuppointObjDescriptor = {
    0,                                         /* reserved0 */
    0,                                         /* reserved1 */
    0,                                         /* reserved2 */
    OBJECT_DESCRIPTOR_FLAGS_10_SLOTS,          /* slotCountAndFlags */
    0,                                         /* initialise */
    0,                                         /* release */
    0,                                         /* slot02 */
    (ObjectDescriptorCallback)setuppoint_init, /* init */
    0,                                         /* update */
    0,                                         /* hitDetect */
    0,                                         /* render */
    0,                                         /* free */
    0,                                         /* getObjectTypeId */
    0,                                         /* getExtraSize */
};
