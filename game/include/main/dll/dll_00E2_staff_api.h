#ifndef MAIN_DLL_DLL_00E2_STAFF_API_H_
#define MAIN_DLL_DLL_00E2_STAFF_API_H_

#include "game/objects/object.h"
#include "dlls/object_descriptor.h"

extern ObjectDescriptor23 gStaffObjDescriptor;

/* The retail structures contain 32-bit pointers, so their native offsets and
 * strides differ. Code shared with the renderer must use these fields rather
 * than the original byte offsets. */
typedef struct StaffSwipeSlot {
    u8* vertexData;
    f32 unk4;
    f32 lengthScale;
    u16 startIndex;
    u16 endIndex;
    s16 idx;
    s16 vertexCount;
    u8 flags;
    u8 pad15[0x18 - 0x15];
} StaffSwipeSlot;

typedef struct StaffState {
    StaffSwipeSlot slots[3];
    StaffSwipeSlot* activeSlot;
    u8 pad4C[4];
    f32 moveSpeed;
    f32 geometryPointAX[2];
    f32 geometryPointAY[2];
    f32 geometryPointAZ[2];
    f32 geometryPointBX[2];
    f32 geometryPointBY[2];
    f32 geometryPointBZ[2];
    u8 pad84[4];
    s16 hitReactValue;
    u8 pad8A[2];
    f32 anchorX;
    f32 anchorY;
    f32 anchorZ;
    f32 progress;
    u8 pad9C[0xAA - 0x9C];
    u8 unkAA;
    u8 padAB[0xB0 - 0xAB];
    s16 unkB0;
    s16 fieldB2;
    u8 padB4[5];
    s8 swipeTextureIndex;
    u8 glowEnable;
    u8 glowAttackType;
    u8 hudSuppressed;
} StaffState;

/* gStaffObjDescriptor from slot02 onwards: the export table other objects
   reach through the player's staff child object (childObjs[0])->anim.dll. */
typedef struct StaffInterface
{
    void* pad00[8];
    void (*func0A)(void);
    void (*func0B)(void);
    void (*updateSwipe)(GameObject* staff, GameObject* player, void* context);
    void (*hitDetectGeometry)(GameObject* staff);
    void (*func0E)(void);
    void (*func0F)(void);
    void (*func10)(GameObject* staff, s32 value);
    void (*setHitReactValue)(GameObject* staff, s32 value);
    void (*addHitReactValue)(GameObject* staff, s32 delta);
    int (*getHitReactValue)(GameObject* staff);
    void (*getHitGeometryPoints)(GameObject* staff, f32* outA, f32* outB);
    void (*startSwipe)(GameObject* staff, f32 start, f32 lengthScale);
    s32 (*getSwipeTextureIndex)(GameObject* staff);
} StaffInterface;

#define STAFF_INTERFACE(staff) ((StaffInterface*)*((GameObject*)(staff))->anim.dll)

STATIC_ASSERT(offsetof(StaffInterface, updateSwipe) == 0x28);
STATIC_ASSERT(offsetof(StaffInterface, hitDetectGeometry) == 0x2C);
STATIC_ASSERT(offsetof(StaffInterface, func10) == 0x38);
STATIC_ASSERT(offsetof(StaffInterface, setHitReactValue) == 0x3C);
STATIC_ASSERT(offsetof(StaffInterface, addHitReactValue) == 0x40);
STATIC_ASSERT(offsetof(StaffInterface, getHitReactValue) == 0x44);
STATIC_ASSERT(offsetof(StaffInterface, getHitGeometryPoints) == 0x48);
STATIC_ASSERT(offsetof(StaffInterface, startSwipe) == 0x4C);
STATIC_ASSERT(offsetof(StaffInterface, getSwipeTextureIndex) == 0x50);
STATIC_ASSERT(sizeof(StaffInterface) == 0x54);

void objSetAnimField48to0(GameObject* obj);
void staffUpdateWhileTimeStopped(GameObject* obj);
void staffUpdateAttackEffects(GameObject* obj, GameObject* player);
void staffDrawQuakeSpellRing(void);
void staff_addHitReactValue(GameObject* obj, s32 delta);
void staffDoGrowShrinkAnim(GameObject* obj, u8 grow, u8 alternateRate, int unused);
void staff_free(GameObject* obj);
void staff_func0B(void);
void staff_func0E(void);
void staff_func0F(void);
void staff_func10(GameObject* obj, s32 value);
int staff_getExtraSize(void);
void staff_getHitGeometryPoints(GameObject* obj, f32* outA, f32* outB);
s16 staff_getHitReactValue(GameObject* obj);
int staff_getObjectTypeId(void);
s32 staff_getSwipeTextureIndex(GameObject* obj);
void staff_hitDetect(void);
void staff_hitDetectGeometry(GameObject* obj);
void staff_init(GameObject* obj);
void staff_initialise(void);
void staff_updateSwipe(GameObject* obj, GameObject* player, void* context);
void staff_release(void);
void staff_render(void);
void staffSetGlow(GameObject* obj, u8 attackType, u8 enable);
void staff_func0A(void);
void staff_setHitReactValue(GameObject* obj, s32 value);
void staff_setupSwipe(GameObject* unused, StaffState* swipe, void* context, GameObject* player);
void staff_startSwipe(GameObject* obj, f32 start, f32 lengthScale);
void staff_update(GameObject* obj);
void staffStartQuakeSpell(f32* position);

#endif /* MAIN_DLL_DLL_00E2_STAFF_API_H_ */
