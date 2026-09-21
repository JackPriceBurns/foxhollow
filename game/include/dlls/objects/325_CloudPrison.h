#ifndef DLLS_OBJECTS_325_CLOUDPRISON_H_
#define DLLS_OBJECTS_325_CLOUDPRISON_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"

#define CLOUD_PRISON_CONTROL_CONFIG_WORD_COUNT 16

/*
 * The descriptor symbol owns a 0x40-byte configuration tail. Its individual
 * fields have no established consumers yet, so preserve the proven layout
 * without assigning speculative meanings.
 */
typedef struct CloudPrisonControlDescriptor {
    ObjectDescriptor descriptor;
    u32 configWords[CLOUD_PRISON_CONTROL_CONFIG_WORD_COUNT];
} CloudPrisonControlDescriptor;

STATIC_ASSERT(offsetof(CloudPrisonControlDescriptor, descriptor) == 0x00);
STATIC_ASSERT(offsetof(CloudPrisonControlDescriptor, configWords) == 0x38);
STATIC_ASSERT(sizeof(CloudPrisonControlDescriptor) == 0x78);

int CloudPrisonControl_getExtraSize(void);
int CloudPrisonControl_getObjectTypeId(void);
void CloudPrisonControl_free(void);
void CloudPrisonControl_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                               s8 visible);
void CloudPrisonControl_hitDetect(void);
void CloudPrisonControl_update(GameObject* obj);
void CloudPrisonControl_init(GameObject* obj);
void CloudPrisonControl_release(void);
void CloudPrisonControl_initialise(void);

extern CloudPrisonControlDescriptor gCloudPrisonControlObjDescriptor;

#endif /* DLLS_OBJECTS_325_CLOUDPRISON_H_ */
