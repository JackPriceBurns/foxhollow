#ifndef GAME_OBJECTS_OBJECT_INTERFACE_H_
#define GAME_OBJECTS_OBJECT_INTERFACE_H_

#include "global.h"

typedef struct GameObject GameObject;

typedef void (*ObjectInterfaceCallback)(void);
typedef void (*ObjectInitCallback)(GameObject* obj, void* placement, int flags);
typedef void (*ObjectUpdateCallback)(GameObject* obj);
typedef void (*ObjectHitDetectCallback)(GameObject* obj);
typedef void (*ObjectRenderCallback)(GameObject* obj, int arg2, int arg3, int arg4, int arg5, s8 visible);
typedef void (*ObjectFreeCallback)(GameObject* obj, int flags);
typedef int (*ObjectGetObjectTypeIdCallback)(GameObject* obj);
typedef int (*ObjectGetExtraSizeCallback)(GameObject* obj);

#define OBJECT_INTERFACE_FIELDS                    \
    ObjectInterfaceCallback slot02;                \
    ObjectInitCallback init;                       \
    ObjectUpdateCallback update;                   \
    ObjectHitDetectCallback hitDetect;             \
    ObjectRenderCallback render;                   \
    ObjectFreeCallback free;                       \
    ObjectGetObjectTypeIdCallback getObjectTypeId; \
    ObjectGetExtraSizeCallback getExtraSize

typedef struct ObjectInterface {
    OBJECT_INTERFACE_FIELDS;
} ObjectInterface;

typedef ObjectInterface** ObjectInterfaceHandle;

STATIC_ASSERT(offsetof(ObjectInterface, slot02) == 0x00);
STATIC_ASSERT(offsetof(ObjectInterface, init) == 0x04);
STATIC_ASSERT(offsetof(ObjectInterface, update) == 0x08);
STATIC_ASSERT(offsetof(ObjectInterface, hitDetect) == 0x0C);
STATIC_ASSERT(offsetof(ObjectInterface, render) == 0x10);
STATIC_ASSERT(offsetof(ObjectInterface, free) == 0x14);
STATIC_ASSERT(offsetof(ObjectInterface, getObjectTypeId) == 0x18);
STATIC_ASSERT(offsetof(ObjectInterface, getExtraSize) == 0x1C);
STATIC_ASSERT(sizeof(ObjectInterface) == 0x20);

#endif /* GAME_OBJECTS_OBJECT_INTERFACE_H_ */
