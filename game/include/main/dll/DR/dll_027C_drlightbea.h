#ifndef MAIN_DLL_DR_DLL_027C_DRLIGHTBEA_H_
#define MAIN_DLL_DR_DLL_027C_DRLIGHTBEA_H_

#include "global.h"
#include "game/objects/object_fwd.h"

int DR_LightBea_getExtraSize(void);
int DR_LightBea_getObjectTypeId(void);
void DR_LightBea_free(GameObject* obj);
void DR_LightBea_render(GameObject* obj, int p2, int p3, int p4, int p5);
void DR_LightBea_hitDetect(void);
void DR_LightBea_update(GameObject* obj);
void DR_LightBea_init(GameObject* obj);
void DR_LightBea_release(void);
void DR_LightBea_initialise(void);

#endif
