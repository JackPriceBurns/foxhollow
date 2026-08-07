#ifndef MAIN_OBJ_TRIGGER_H_
#define MAIN_OBJ_TRIGGER_H_

#include "global.h"
#include "types.h"
#include "game/objects/object_fwd.h"

bool ObjTrigger_UpdateIdBlockFlag(GameObject* obj);
int ObjTrigger_IsSetById(GameObject* obj, int triggerId);
int ObjTrigger_IsSet(GameObject* obj);

#endif /* MAIN_OBJ_TRIGGER_H_ */
