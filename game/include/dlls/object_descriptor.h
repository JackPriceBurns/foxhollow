#ifndef DLLS_OBJECT_DESCRIPTOR_H_
#define DLLS_OBJECT_DESCRIPTOR_H_

#include "game/objects/object_interface.h"
#include "global.h"
#include "main/resource.h"

typedef void* ObjectDescriptorReservedSlot;
typedef ResourceDescriptorHeader ObjectDescriptorHeader;
#define OBJECT_RELEASE_ADAPTER(adapter, callback, ...) \
    static void adapter(void) { callback(__VA_ARGS__); }
#define OBJECT_INIT_ADAPTER(adapter, callback, ...) \
    static void adapter(GameObject* obj, void* placement, int flags) { callback(__VA_ARGS__); }
#define OBJECT_UPDATE_ADAPTER(adapter, callback, ...) \
    static void adapter(GameObject* obj) { callback(__VA_ARGS__); }
#define OBJECT_HIT_DETECT_ADAPTER(adapter, callback, ...) \
    static void adapter(GameObject* obj) { callback(__VA_ARGS__); }
#define OBJECT_RENDER_ADAPTER(adapter, callback, ...) \
    static void adapter(GameObject* obj, int arg2, int arg3, int arg4, int arg5, s8 visible) { callback(__VA_ARGS__); }
#define OBJECT_FREE_ADAPTER(adapter, callback, ...) \
    static void adapter(GameObject* obj, int flags) { callback(__VA_ARGS__); }
#define OBJECT_TYPE_ID_ADAPTER(adapter, callback, ...) \
    static int adapter(GameObject* obj) { return callback(__VA_ARGS__); }
#define OBJECT_EXTRA_SIZE_ADAPTER(adapter, callback, ...) \
    static int adapter(GameObject* obj) { return callback(__VA_ARGS__); }

#define OBJECT_DESCRIPTOR_TYPE(name, interfaceType) \
    typedef struct name {                           \
        ObjectDescriptorHeader header;              \
        interfaceType interface;                    \
    } name

#define OBJECT_DESCRIPTOR_WITH_PADDING_TYPE(name, descriptorType) \
    typedef struct name {                                      \
        descriptorType descriptor;                             \
        u32 padding;                                           \
    } name

typedef struct ObjectDescriptor4 {
    ObjectDescriptorHeader header;
    ObjectDescriptorReservedSlot slot02;
    ObjectDescriptorReservedSlot slot03;
} ObjectDescriptor4;

typedef struct ObjectDescriptor4WithPadding {
    ObjectDescriptor4 descriptor;
    u32 padding;
} ObjectDescriptor4WithPadding;

typedef struct ObjectDescriptor6 {
    ObjectDescriptorHeader header;
    ObjectDescriptorReservedSlot slot02;
    ObjectDescriptorReservedSlot slot03;
    ObjectDescriptorReservedSlot slot04;
    ObjectDescriptorReservedSlot slot05;
} ObjectDescriptor6;

typedef struct ObjectDescriptor {
    ObjectDescriptorHeader header;
    ObjectDescriptorReservedSlot slot02;
    ObjectInitCallback init;
    ObjectUpdateCallback update;
    ObjectHitDetectCallback hitDetect;
    ObjectRenderCallback render;
    ObjectFreeCallback free;
    ObjectGetObjectTypeIdCallback getObjectTypeId;
    ObjectGetExtraSizeCallback getExtraSize;
} ObjectDescriptor;

typedef struct ObjectDescriptor10WithPadding {
    ObjectDescriptor descriptor;
    u32 padding;
} ObjectDescriptor10WithPadding;

typedef struct ObjectDescriptor11ExtraSize {
    ObjectDescriptorHeader header;
    ObjectDescriptorReservedSlot slot02;
    ObjectInitCallback init;
    ObjectUpdateCallback update;
    ObjectHitDetectCallback hitDetect;
    ObjectRenderCallback render;
    ObjectFreeCallback free;
    ObjectGetObjectTypeIdCallback getObjectTypeId;
    ObjectGetExtraSizeCallback getExtraSize;
    ObjectDescriptorReservedSlot slot0A;
} ObjectDescriptor11ExtraSize;

typedef struct ObjectDescriptor11WithPadding {
    ObjectDescriptor11ExtraSize descriptor;
    u32 padding;
} ObjectDescriptor11WithPadding;

typedef struct ObjectDescriptor12 {
    ObjectDescriptorHeader header;
    ObjectDescriptorReservedSlot slot02;
    ObjectInitCallback init;
    ObjectUpdateCallback update;
    ObjectHitDetectCallback hitDetect;
    ObjectRenderCallback render;
    ObjectFreeCallback free;
    ObjectGetObjectTypeIdCallback getObjectTypeId;
    ObjectGetExtraSizeCallback getExtraSize;
    ObjectDescriptorReservedSlot slot0A;
    ObjectDescriptorReservedSlot slot0B;
} ObjectDescriptor12;

typedef struct ObjectDescriptor12WithPadding {
    ObjectDescriptor12 descriptor;
    u32 padding;
} ObjectDescriptor12WithPadding;

typedef struct ObjectDescriptor13 {
    ObjectDescriptorHeader header;
    ObjectDescriptorReservedSlot slot02;
    ObjectInitCallback init;
    ObjectUpdateCallback update;
    ObjectHitDetectCallback hitDetect;
    ObjectRenderCallback render;
    ObjectFreeCallback free;
    ObjectGetObjectTypeIdCallback getObjectTypeId;
    ObjectGetExtraSizeCallback getExtraSize;
    ObjectDescriptorReservedSlot slot0A;
    ObjectDescriptorReservedSlot slot0B;
    ObjectDescriptorReservedSlot slot0C;
} ObjectDescriptor13;

typedef struct ObjectDescriptor13WithPadding {
    ObjectDescriptor13 descriptor;
    u32 padding;
} ObjectDescriptor13WithPadding;

typedef struct ObjectDescriptor14 {
    ObjectDescriptorHeader header;
    ObjectDescriptorReservedSlot slot02;
    ObjectInitCallback init;
    ObjectUpdateCallback update;
    ObjectHitDetectCallback hitDetect;
    ObjectRenderCallback render;
    ObjectFreeCallback free;
    ObjectGetObjectTypeIdCallback getObjectTypeId;
    ObjectGetExtraSizeCallback getExtraSize;
    ObjectDescriptorReservedSlot slot0A;
    ObjectDescriptorReservedSlot slot0B;
    ObjectDescriptorReservedSlot slot0C;
    ObjectDescriptorReservedSlot slot0D;
} ObjectDescriptor14;

typedef struct ObjectDescriptor15 {
    ObjectDescriptorHeader header;
    ObjectDescriptorReservedSlot slot02;
    ObjectInitCallback init;
    ObjectUpdateCallback update;
    ObjectHitDetectCallback hitDetect;
    ObjectRenderCallback render;
    ObjectFreeCallback free;
    ObjectGetObjectTypeIdCallback getObjectTypeId;
    ObjectGetExtraSizeCallback getExtraSize;
    ObjectDescriptorReservedSlot slot0A;
    ObjectDescriptorReservedSlot slot0B;
    ObjectDescriptorReservedSlot slot0C;
    ObjectDescriptorReservedSlot slot0D;
    ObjectDescriptorReservedSlot slot0E;
} ObjectDescriptor15;

typedef struct ObjectDescriptor16 {
    ObjectDescriptorHeader header;
    ObjectDescriptorReservedSlot slot02;
    ObjectInitCallback init;
    ObjectUpdateCallback update;
    ObjectHitDetectCallback hitDetect;
    ObjectRenderCallback render;
    ObjectFreeCallback free;
    ObjectGetObjectTypeIdCallback getObjectTypeId;
    ObjectGetExtraSizeCallback getExtraSize;
    ObjectDescriptorReservedSlot slot0A;
    ObjectDescriptorReservedSlot slot0B;
    ObjectDescriptorReservedSlot slot0C;
    ObjectDescriptorReservedSlot slot0D;
    ObjectDescriptorReservedSlot slot0E;
    ObjectDescriptorReservedSlot slot0F;
} ObjectDescriptor16;

typedef struct ObjectDescriptor16WithPadding {
    ObjectDescriptor16 descriptor;
    u32 padding;
} ObjectDescriptor16WithPadding;

typedef struct ObjectDescriptor17 {
    ObjectDescriptorHeader header;
    ObjectDescriptorReservedSlot slot02;
    ObjectInitCallback init;
    ObjectUpdateCallback update;
    ObjectHitDetectCallback hitDetect;
    ObjectRenderCallback render;
    ObjectFreeCallback free;
    ObjectGetObjectTypeIdCallback getObjectTypeId;
    ObjectGetExtraSizeCallback getExtraSize;
    ObjectDescriptorReservedSlot slot0A;
    ObjectDescriptorReservedSlot slot0B;
    ObjectDescriptorReservedSlot slot0C;
    ObjectDescriptorReservedSlot slot0D;
    ObjectDescriptorReservedSlot slot0E;
    ObjectDescriptorReservedSlot slot0F;
    ObjectDescriptorReservedSlot slot10;
} ObjectDescriptor17;

typedef struct ObjectDescriptor20 {
    ObjectDescriptorHeader header;
    ObjectDescriptorReservedSlot slot02;
    ObjectInitCallback init;
    ObjectUpdateCallback update;
    ObjectHitDetectCallback hitDetect;
    ObjectRenderCallback render;
    ObjectFreeCallback free;
    ObjectGetObjectTypeIdCallback getObjectTypeId;
    ObjectGetExtraSizeCallback getExtraSize;
    ObjectDescriptorReservedSlot slot0A;
    ObjectDescriptorReservedSlot slot0B;
    ObjectDescriptorReservedSlot slot0C;
    ObjectDescriptorReservedSlot slot0D;
    ObjectDescriptorReservedSlot slot0E;
    ObjectDescriptorReservedSlot slot0F;
    ObjectDescriptorReservedSlot slot10;
    ObjectDescriptorReservedSlot slot11;
    ObjectDescriptorReservedSlot slot12;
    ObjectDescriptorReservedSlot slot13;
} ObjectDescriptor20;

typedef struct ObjectDescriptor21 {
    ObjectDescriptor20 descriptor;
    ObjectDescriptorReservedSlot slot14;
} ObjectDescriptor21;

typedef struct ObjectDescriptor23 {
    ObjectDescriptorHeader header;
    ObjectDescriptorReservedSlot slot02;
    ObjectInitCallback init;
    ObjectUpdateCallback update;
    ObjectHitDetectCallback hitDetect;
    ObjectRenderCallback render;
    ObjectFreeCallback free;
    ObjectGetObjectTypeIdCallback getObjectTypeId;
    ObjectGetExtraSizeCallback getExtraSize;
    ObjectDescriptorReservedSlot slot0A;
    ObjectDescriptorReservedSlot slot0B;
    ObjectDescriptorReservedSlot slot0C;
    ObjectDescriptorReservedSlot slot0D;
    ObjectDescriptorReservedSlot slot0E;
    ObjectDescriptorReservedSlot slot0F;
    ObjectDescriptorReservedSlot slot10;
    ObjectDescriptorReservedSlot slot11;
    ObjectDescriptorReservedSlot slot12;
    ObjectDescriptorReservedSlot slot13;
    ObjectDescriptorReservedSlot slot14;
    ObjectDescriptorReservedSlot slot15;
    ObjectDescriptorReservedSlot slot16;
} ObjectDescriptor23;

typedef struct ObjectDescriptor24 {
    ObjectDescriptorHeader header;
    ObjectDescriptorReservedSlot slot02;
    ObjectInitCallback init;
    ObjectUpdateCallback update;
    ObjectHitDetectCallback hitDetect;
    ObjectRenderCallback render;
    ObjectFreeCallback free;
    ObjectGetObjectTypeIdCallback getObjectTypeId;
    ObjectGetExtraSizeCallback getExtraSize;
    ObjectDescriptorReservedSlot slot0A;
    ObjectDescriptorReservedSlot slot0B;
    ObjectDescriptorReservedSlot slot0C;
    ObjectDescriptorReservedSlot slot0D;
    ObjectDescriptorReservedSlot slot0E;
    ObjectDescriptorReservedSlot slot0F;
    ObjectDescriptorReservedSlot slot10;
    ObjectDescriptorReservedSlot slot11;
    ObjectDescriptorReservedSlot slot12;
    ObjectDescriptorReservedSlot slot13;
    ObjectDescriptorReservedSlot slot14;
    ObjectDescriptorReservedSlot slot15;
    ObjectDescriptorReservedSlot slot16;
    ObjectDescriptorReservedSlot slot17;
} ObjectDescriptor24;

typedef struct ObjectDescriptor24WithPadding {
    ObjectDescriptor24 descriptor;
    u32 padding;
} ObjectDescriptor24WithPadding;

#define OBJECT_DESCRIPTOR_FLAGS_4_SLOTS  0x00030000
#define OBJECT_DESCRIPTOR_FLAGS_5_SLOTS  0x00040000
#define OBJECT_DESCRIPTOR_FLAGS_6_SLOTS  0x00050000
#define OBJECT_DESCRIPTOR_FLAGS_10_SLOTS 0x00090000
#define OBJECT_DESCRIPTOR_FLAGS_11_SLOTS 0x000A0000
#define OBJECT_DESCRIPTOR_FLAGS_12_SLOTS 0x000B0000
#define OBJECT_DESCRIPTOR_FLAGS_13_SLOTS 0x000C0000
#define OBJECT_DESCRIPTOR_FLAGS_14_SLOTS 0x000D0000
#define OBJECT_DESCRIPTOR_FLAGS_15_SLOTS 0x000E0000
#define OBJECT_DESCRIPTOR_FLAGS_16_SLOTS 0x000F0000
#define OBJECT_DESCRIPTOR_FLAGS_17_SLOTS 0x00100000
#define OBJECT_DESCRIPTOR_FLAGS_20_SLOTS 0x00130000
#define OBJECT_DESCRIPTOR_FLAGS_23_SLOTS 0x00160000
#define OBJECT_DESCRIPTOR_FLAGS_24_SLOTS 0x00170000
#define EMPTY_OBJECT_DESCRIPTOR(reserved0Value) { .header = { .metadata = { (reserved0Value) } } }

#endif /* DLLS_OBJECT_DESCRIPTOR_H_ */
