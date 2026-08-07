#ifndef MAIN_DLL_FIRE_H_
#define MAIN_DLL_FIRE_H_

#include "types.h"
#include "game/objects/object.h"
#include "main/mapEventTypes.h"
#include "dlls/object_descriptor.h"
#include "main/objseq.h"

#define LINKA_LEVCONTROL_OBJECT_DEF_ID 0x0342
#define LINKA_LEVCONTROL_DLL_ID 0x0238
#define LINKA_LEVCONTROL_CLASS_ID 0x0030
#define LINKA_LEVCONTROL_OBJECT_DEF_SIZE 0xC0
#define LINKA_LEVCONTROL_PLACEMENT_SIZE 0x24

extern ObjectDescriptor gFireObjDescriptor;

int LinkALevControl_seqFn(GameObject *obj, int unused, ObjSeqState *animUpdate);
int LinkALevControl_getExtraSize(void);
int LinkALevControl_getObjectTypeId(void);
void LinkALevControl_free(void);
void LinkALevControl_render(GameObject* obj, int p2, int p3, int p4, int p5, s8 visible);
void LinkALevControl_hitDetect(void);
void LinkALevControl_update(GameObject *obj);
void LinkALevControl_init(GameObject *obj);
void LinkALevControl_release(void);
void LinkALevControl_initialise(void);

#endif /* MAIN_DLL_FIRE_H_ */
