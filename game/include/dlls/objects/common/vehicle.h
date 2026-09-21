#ifndef DLLS_OBJECTS_COMMON_VEHICLE_H_
#define DLLS_OBJECTS_COMMON_VEHICLE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object.h"

enum VehicleMountState {
    VEHICLE_NoRider = 0,
    VEHICLE_Mounting = 1,
    VEHICLE_Mounted = 2,
    VEHICLE_Dismounting = 3,
};

/*
 * Shared export table of every rideable object DLL, reached through
 * obj->anim.dll. The layout is the object descriptor from slot02 onwards and
 * is identical across SnowBike, DIMSnowHorn1, DR_EarthWarrior, DR_CloudRunner,
 * SB_CloudRunner, HighTop and DrakorHoverPad.
 */
typedef struct VehicleInterface {
    OBJECT_INTERFACE_FIELDS;
    int (*canMount)(GameObject* obj, GameObject* player);
    int (*getMountSide)(GameObject* obj);
    void (*getRiderPosition)(GameObject* obj, f32* outX, f32* outY, f32* outZ);
    int (*canDismount)(GameObject* obj, GameObject* player);
    int (*getDismountSide)(GameObject* obj);
    void (*getCameraPosition)(GameObject* obj, f32* outX, f32* outY, f32* outZ);
    int (*getMountState)(GameObject* obj);
    void (*setMountState)(GameObject* obj, int mountState);
    void (*getPlayerAnim)(GameObject* obj, f32* outBlend, int* outAnim);
    f32 (*getNormalizedSpeed)(GameObject* obj, f32* out);
    int (*getRacePosition)(GameObject* obj);
    void (*resetToRomListPosition)(GameObject* obj);
    void (*handleRiderScale)(GameObject* obj, f32 rootMotionScaleBase);
    void (*getLookTargetYaw)(GameObject* obj, int mode, int* out);
} VehicleInterface;

OBJECT_DESCRIPTOR_TYPE(VehicleDescriptor, VehicleInterface);

typedef struct VehicleDescriptorWithPadding {
    VehicleDescriptor descriptor;
    u32 padding;
} VehicleDescriptorWithPadding;

#define VEHICLE_CAN_MOUNT_ADAPTER(adapter, callback, ...)                                                              \
    static int adapter(GameObject* obj, GameObject* player) {                                                          \
        return callback(__VA_ARGS__);                                                                                  \
    }
#define VEHICLE_MOUNT_SIDE_ADAPTER(adapter, callback, ...)                                                             \
    static int adapter(GameObject* obj) {                                                                              \
        return callback(__VA_ARGS__);                                                                                  \
    }
#define VEHICLE_RIDER_POSITION_ADAPTER(adapter, callback, ...)                                                         \
    static void adapter(GameObject* obj, f32* x, f32* y, f32* z) {                                                     \
        callback(__VA_ARGS__);                                                                                         \
    }
#define VEHICLE_CAN_DISMOUNT_ADAPTER(adapter, callback, ...)                                                           \
    static int adapter(GameObject* obj, GameObject* player) {                                                          \
        return callback(__VA_ARGS__);                                                                                  \
    }
#define VEHICLE_DISMOUNT_SIDE_ADAPTER(adapter, callback, ...)                                                          \
    static int adapter(GameObject* obj) {                                                                              \
        return callback(__VA_ARGS__);                                                                                  \
    }
#define VEHICLE_CAMERA_POSITION_ADAPTER(adapter, callback, ...)                                                        \
    static void adapter(GameObject* obj, f32* x, f32* y, f32* z) {                                                     \
        callback(__VA_ARGS__);                                                                                         \
    }
#define VEHICLE_MOUNT_STATE_ADAPTER(adapter, callback, ...)                                                            \
    static int adapter(GameObject* obj) {                                                                              \
        return callback(__VA_ARGS__);                                                                                  \
    }
#define VEHICLE_SET_MOUNT_STATE_ADAPTER(adapter, callback, ...)                                                        \
    static void adapter(GameObject* obj, int mountState) {                                                             \
        callback(__VA_ARGS__);                                                                                         \
    }
#define VEHICLE_PLAYER_ANIM_ADAPTER(adapter, callback, ...)                                                            \
    static void adapter(GameObject* obj, f32* blend, int* anim) {                                                      \
        callback(__VA_ARGS__);                                                                                         \
    }
#define VEHICLE_NORMALIZED_SPEED_ADAPTER(adapter, callback, ...)                                                       \
    static f32 adapter(GameObject* obj, f32* speed) {                                                                  \
        return callback(__VA_ARGS__);                                                                                  \
    }
#define VEHICLE_RACE_POSITION_ADAPTER(adapter, callback, ...)                                                          \
    static int adapter(GameObject* obj) {                                                                              \
        return callback(__VA_ARGS__);                                                                                  \
    }
#define VEHICLE_RESET_POSITION_ADAPTER(adapter, callback, ...)                                                         \
    static void adapter(GameObject* obj) {                                                                             \
        callback(__VA_ARGS__);                                                                                         \
    }
#define VEHICLE_RIDER_SCALE_ADAPTER(adapter, callback, ...)                                                            \
    static void adapter(GameObject* obj, f32 scale) {                                                                  \
        callback(__VA_ARGS__);                                                                                         \
    }
#define VEHICLE_LOOK_TARGET_ADAPTER(adapter, callback, ...)                                                            \
    static void adapter(GameObject* obj, int mode, int* out) {                                                         \
        callback(__VA_ARGS__);                                                                                         \
    }

#define VEHICLE_INTERFACE(vehicle) ((VehicleInterface*)*((GameObject*)(vehicle))->anim.dll)

STATIC_ASSERT(offsetof(VehicleInterface, render) == 0x10);
STATIC_ASSERT(offsetof(VehicleInterface, canMount) == 0x20);
STATIC_ASSERT(offsetof(VehicleInterface, getMountSide) == 0x24);
STATIC_ASSERT(offsetof(VehicleInterface, getRiderPosition) == 0x28);
STATIC_ASSERT(offsetof(VehicleInterface, canDismount) == 0x2C);
STATIC_ASSERT(offsetof(VehicleInterface, getDismountSide) == 0x30);
STATIC_ASSERT(offsetof(VehicleInterface, getCameraPosition) == 0x34);
STATIC_ASSERT(offsetof(VehicleInterface, getMountState) == 0x38);
STATIC_ASSERT(offsetof(VehicleInterface, setMountState) == 0x3C);
STATIC_ASSERT(offsetof(VehicleInterface, getPlayerAnim) == 0x40);
STATIC_ASSERT(offsetof(VehicleInterface, getNormalizedSpeed) == 0x44);
STATIC_ASSERT(offsetof(VehicleInterface, getRacePosition) == 0x48);
STATIC_ASSERT(offsetof(VehicleInterface, resetToRomListPosition) == 0x4C);
STATIC_ASSERT(offsetof(VehicleInterface, handleRiderScale) == 0x50);
STATIC_ASSERT(offsetof(VehicleInterface, getLookTargetYaw) == 0x54);
STATIC_ASSERT(sizeof(VehicleInterface) == 0x58);

#endif /* DLLS_OBJECTS_COMMON_VEHICLE_H_ */
