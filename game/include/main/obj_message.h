#ifndef MAIN_OBJ_MESSAGE_H_
#define MAIN_OBJ_MESSAGE_H_

#include "game/objects/object.h"

extern char sObjMsgOverflowInObjectWarning[];

int ObjMsg_Peek(GameObject* obj, u32* outMessage, uintptr_t* outSender, u32* outParam);
int ObjMsg_Pop(GameObject* obj, u32* outMessage, uintptr_t* outSender, u32* outParam);
int ObjMsg_PopNative(GameObject* obj, u32* outMessage, uintptr_t* outSender, uintptr_t* outParam);
void ObjMsg_SendToNearbyObjects(int targetId, f32 radius, u32 flags, void* sender, u32 message, uintptr_t param);
void ObjMsg_SendToObjects(int targetId, u32 flags, void* sender, u32 message, uintptr_t param);
u32 ObjMsg_SendToObject(GameObject* obj, u32 message, void* sender, uintptr_t param);
void ObjMsg_AllocQueue(GameObject* obj, int capacity);

#endif /* MAIN_OBJ_MESSAGE_H_ */
