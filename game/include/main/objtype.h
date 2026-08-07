#ifndef MAIN_OBJTYPE_H_
#define MAIN_OBJTYPE_H_

#include "global.h"

struct GameObject;

int objIsObjectType(u32 obj, int group);
int objGetNearestType(int group, f32* point, f32* maxDistance);
struct GameObject* objGetNearestTypeToExcludingSelf(int group, struct GameObject* obj, f32* maxDistance);
struct GameObject* objGetNearestTypeTo(int group, struct GameObject* obj, f32* maxDistance);
struct GameObject** objGetAllOfType(int group, int* countOut);
void objFreeObjectType(int obj, int group);
int objGetObjectType(u32 obj);
void objAddObjectType(int obj, int group);
void objTypeInit(void);

#endif /* MAIN_OBJTYPE_H_ */
