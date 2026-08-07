#ifndef MAIN_DLL_DLL_00DA_POLLENFRAGMENT_API_H_
#define MAIN_DLL_DLL_00DA_POLLENFRAGMENT_API_H_

#include "game/objects/object.h"
#include "dlls/object_descriptor.h"

#define POLLEN_FRAGMENT_OBJECT_ID 0x482

typedef struct PollenFragmentConfig
{
    s16 spawnSfxId;
    s16 loopSfxId;
    s16 explodeSfxId;
    s16 initFxId;
    s16 burstFxId;
    s16 auraFxId;
    f32 steerSpeed;
    s16 targetGroup;
    u8 noVertical : 1;
    u8 timed : 1;
    u8 smoothTurn : 1;
    u8 usePath : 1;
} PollenFragmentConfig;

extern PollenFragmentConfig gPollenFragmentConfig0;
extern PollenFragmentConfig gPollenFragmentConfig1;
extern PollenFragmentConfig gPollenFragmentConfig2;
extern PollenFragmentConfig gPollenFragmentConfig3;
extern PollenFragmentConfig gPollenFragmentConfig4;
extern PollenFragmentConfig* gPollenFragmentConfigs[];
extern ObjectDescriptor gPollenFragmentObjDescriptor;

typedef struct PollenFragmentPlacement PollenFragmentPlacement;

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
