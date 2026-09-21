#ifndef MAIN_DLL_WC_DLL_028C_WCBOUNCYCRA_H
#define MAIN_DLL_WC_DLL_028C_WCBOUNCYCRA_H

#include "game/objects/object.h"
#include "game/objects/object_setup.h"
#include "main/dll/WC/wc_block_state.h"

typedef struct WCBlockGridInterface {
    void (*pad0_slots[8])(void);
    void (*getCellWorldA)(GameObject* obj, s16 cellX, s16 cellZ, f32* worldX, f32* worldZ,
                          struct WCBlockGridInterface* self);
    void (*pad24_slots[3])(void);
    void (*getCellXYA)(u8 tileIndex, s16* cellX, s16* cellZ, struct WCBlockGridInterface* self);
    void (*pad34_slots[2])(void);
    void (*getCellWorldB)(GameObject* obj, s16 cellX, s16 cellZ, f32* worldX, f32* worldZ,
                          struct WCBlockGridInterface* self);
    void (*pad40_slots[3])(void);
    void (*getCellXYB)(u8 tileIndex, s16* cellX, s16* cellZ, struct WCBlockGridInterface* self);
} WCBlockGridInterface;

typedef struct WCBouncyCrateState {
    f32 homeY;
    u8 pad04[4];
    s16 cooldown;
    u8 flags;
    u8 bounceCount;
} WCBouncyCrateState;

STATIC_ASSERT(sizeof(WCBouncyCrateState) == 0x0C);
STATIC_ASSERT(offsetof(WCBouncyCrateState, cooldown) == 0x08);
STATIC_ASSERT(offsetof(WCBouncyCrateState, flags) == 0x0A);
STATIC_ASSERT(offsetof(WCBouncyCrateState, bounceCount) == 0x0B);

extern f32 gBouncyCrateTriggerSearchRadius;
extern f32 lbl_803E6D24;
extern f32 gBouncyCrateNearDistance;
extern f32 lbl_803E6D2C;
extern f32 gBouncyCrateFarDistance;
extern f32 lbl_803E6D34;
extern f32 gBouncyCrateGravity;
extern f32 gBouncyCrateRestitution;
extern f32 lbl_803E6D38;
extern f32 WCBLOCK_PLAYER_CELL_MARGIN;

int WCBouncyCra_getExtraSize(void);
int WCBouncyCra_getObjectTypeId(void);
void WCBouncyCra_free(void);
void WCBouncyCra_render(GameObject* obj, int p2, int p3, int p4, int p5, s8 visible);
void WCBouncyCra_hitDetect(void);
void WCBouncyCra_update(GameObject* obj);
void WCBouncyCra_init(GameObject* obj, ObjPlacement* setup);
void WCBouncyCra_release(void);
void WCBouncyCra_initialise(void);

#endif
