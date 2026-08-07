#ifndef MAIN_DLL_DLL_0050_CAMERAMODECRAWL_H_
#define MAIN_DLL_DLL_0050_CAMERAMODECRAWL_H_

#include "global.h"
#include "main/camera_object.h"
#include "main/resource.h"

typedef struct CameraModeCrawlFlags {
    u8 useDefaultHandler : 1;
    u8 unknown : 7;
} CameraModeCrawlFlags;

STATIC_ASSERT(sizeof(CameraModeCrawlFlags) == 0x01);

typedef struct CameraModeCrawlState {
    u8 unk00[0x08];
    CameraModeCrawlFlags flags;
    u8 unk09[0x0C - 0x09];
} CameraModeCrawlState;

STATIC_ASSERT(offsetof(CameraModeCrawlState, unk00) == 0x00);
STATIC_ASSERT(offsetof(CameraModeCrawlState, flags) == 0x08);
STATIC_ASSERT(offsetof(CameraModeCrawlState, unk09) == 0x09);
STATIC_ASSERT(sizeof(CameraModeCrawlState) == 0x0C);

typedef struct CameraModeCrawlDescriptor {
    u32 metadata[4];
    void (*initialise)(void);
    void (*release)(void);
    ResourceDescriptorCallback reserved18;
    void (*init)(void);
    void (*update)(CameraObject* camera);
    void (*free)(void);
    void (*copyToCurrent)(void* actionData, int recordSize);
    ResourceDescriptorCallback reserved2C;
} CameraModeCrawlDescriptor;

STATIC_ASSERT(offsetof(CameraModeCrawlDescriptor, metadata) == 0x00);
STATIC_ASSERT(offsetof(CameraModeCrawlDescriptor, initialise) == 0x10);
STATIC_ASSERT(offsetof(CameraModeCrawlDescriptor, release) == 0x14);
STATIC_ASSERT(offsetof(CameraModeCrawlDescriptor, reserved18) == 0x18);
STATIC_ASSERT(offsetof(CameraModeCrawlDescriptor, init) == 0x1C);
STATIC_ASSERT(offsetof(CameraModeCrawlDescriptor, update) == 0x20);
STATIC_ASSERT(offsetof(CameraModeCrawlDescriptor, free) == 0x24);
STATIC_ASSERT(offsetof(CameraModeCrawlDescriptor, copyToCurrent) == 0x28);
STATIC_ASSERT(offsetof(CameraModeCrawlDescriptor, reserved2C) == 0x2C);
STATIC_ASSERT(sizeof(CameraModeCrawlDescriptor) == 0x30);

extern CameraModeCrawlDescriptor gCameraModeCrawlDescriptor;

void CameraModeCrawl_copyToCurrent(void* actionData, int recordSize);
void CameraModeCrawl_free(void);
void CameraModeCrawl_update(CameraObject* camera);
void CameraModeCrawl_init(void);
void CameraModeCrawl_release(void);
void CameraModeCrawl_initialise(void);

#endif /* MAIN_DLL_DLL_0050_CAMERAMODECRAWL_H_ */
