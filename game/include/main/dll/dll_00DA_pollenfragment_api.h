#ifndef MAIN_DLL_DLL_00DA_POLLENFRAGMENT_API_H_
#define MAIN_DLL_DLL_00DA_POLLENFRAGMENT_API_H_

#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "dlls/object_descriptor.h"

typedef enum PollenFragmentObjectId
{
    POLLEN_FRAGMENT_OBJECT_ID = 0x482
} PollenFragmentObjectId;

typedef struct PollenFragmentPlacement
{
    ObjPlacement base;
    u8 unk18;
    s8 pollenType;
    u8 unk1A[10];
} PollenFragmentPlacement;

STATIC_ASSERT(offsetof(PollenFragmentPlacement, pollenType) == 0x19);
STATIC_ASSERT(sizeof(PollenFragmentPlacement) == 0x24);

extern ObjectDescriptor gPollenFragmentObjDescriptor;

int pollenfragment_getExtraSize(void);
int pollenfragment_getObjectTypeId(void);
void pollenfragment_free(GameObject* obj);
void pollenfragment_render(GameObject* obj, int p2, int p3, int p4, int p5);
void pollenfragment_hitDetect(GameObject* obj);
void pollenfragment_update(GameObject* obj);
void pollenfragment_init(GameObject* obj, PollenFragmentPlacement* setup);
void pollenfragment_release(void);
void pollenfragment_initialise(void);

#endif /* MAIN_DLL_DLL_00DA_POLLENFRAGMENT_API_H_ */
