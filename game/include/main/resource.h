#ifndef MAIN_RESOURCE_H_
#define MAIN_RESOURCE_H_

#include "global.h"
#include "types.h"

typedef struct ResourceDescriptor ResourceDescriptor;

typedef void (*ResourceAcquireCallback)(ResourceDescriptor* descriptor);
typedef void (*ResourceReleaseCallback)(void);

typedef struct ResourceDescriptorHeader {
    u32 metadata[4];
    ResourceAcquireCallback acquire;
    ResourceReleaseCallback release;
} ResourceDescriptorHeader;

struct ResourceDescriptor {
    u32 metadata[4];
    ResourceAcquireCallback acquire;
    ResourceReleaseCallback release;
    u8 data[0];
};

#define RESOURCE_ACQUIRE_ADAPTER(adapter, callback, ...)                                                               \
    static void adapter(ResourceDescriptor* descriptor) {                                                              \
        callback(__VA_ARGS__);                                                                                         \
    }

#define RESOURCE_DESCRIPTOR_TYPE(name, interfaceType)                                                                  \
    typedef struct name {                                                                                              \
        ResourceDescriptorHeader header;                                                                               \
        interfaceType interface;                                                                                       \
    } name

#define RESOURCE_DESCRIPTOR_REF(descriptor)                                                                            \
    _Generic(&(descriptor), ResourceDescriptor*: &(descriptor), default: (void*)&(descriptor))

extern ResourceDescriptor* gResourceDescriptors[];
extern void* gResourceLoadedHandles[];
extern u16 gResourceRefCounts[];

BOOL Resource_Release(void* handleSlot);
void* Resource_Acquire(u16 id, int unused);
void Resource_ResetRefCounts(void);

#endif /* MAIN_RESOURCE_H_ */
