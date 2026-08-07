#ifndef MAIN_DLL_DR_DLL_0281_DREARTHCAL_H
#define MAIN_DLL_DR_DLL_0281_DREARTHCAL_H

#include "global.h"
#include "game/objects/object.h"
#include "dlls/object_descriptor.h"
#include "game/objects/object_setup.h"

typedef struct DREarthCalSetup
{
    ObjPlacement base;
    s8 yaw;
} DREarthCalSetup;

STATIC_ASSERT(offsetof(DREarthCalSetup, yaw) == 0x18);

extern ObjectDescriptor12 gDrEarthCalObjDescriptor;

int drearthcal_func0A(void);
int drearthcal_getExtraSize(void);
int drearthcal_getObjectTypeId(void);
void drearthcal_free(void);
void drearthcal_render(void);
void drearthcal_hitDetect(void);
void drearthcal_update(GameObject* obj);
void drearthcal_init(GameObject* obj, DREarthCalSetup* setup);
void drearthcal_release(void);
void drearthcal_initialise(void);

#endif /* MAIN_DLL_DR_DLL_0281_DREARTHCAL_H */
