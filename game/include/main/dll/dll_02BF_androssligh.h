#ifndef MAIN_DLL_DLL_02BF_ANDROSSLIGH_H
#define MAIN_DLL_DLL_02BF_ANDROSSLIGH_H

#include "game/objects/object_fwd.h"
#include "dlls/object_descriptor.h"

typedef enum AndrossLighMode
{
    ANDROSSLIGH_IDLE = 0,
    ANDROSSLIGH_ACTIVE = 1,
    ANDROSSLIGH_DONE = 2
} AndrossLighMode;

extern ObjectDescriptor gAndrossLighObjDescriptor;

void androssligh_setState(GameObject* obj, AndrossLighMode newState, u8 force);
int androssligh_getExtraSize(void);
int androssligh_getObjectTypeId(void);
void androssligh_free(void);
void androssligh_render(GameObject* obj);
void androssligh_hitDetect(void);
void androssligh_update(GameObject* obj);
void androssligh_init(void);

#endif
