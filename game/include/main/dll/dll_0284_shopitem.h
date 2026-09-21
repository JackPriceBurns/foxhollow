#ifndef MAIN_DLL_DLL_0284_SHOPITEM_H_
#define MAIN_DLL_DLL_0284_SHOPITEM_H_

#include "types.h"
#include "main/lightningeffect.h"
#include "main/dll_000A_expgfx.h"
#include "main/dll/firefly_flight_state.h"
#include "main/dll/shopkeeperstate_struct.h"
#include "game/objects/object.h"
#include "main/objseq.h"

typedef struct ShopItemFlags {
    u8 flag_80 : 1;
    u8 flag_40 : 1;
    u8 _rest : 6;
} ShopItemFlags;

typedef struct ShopItemState {
    u8 pad00[4];
    FireFlyFlightState flight;
    u8 pad7C[0x88 - 0x7C];
    s16 msgParam;
    u8 pad8A[6];
    GameObject* vendorObj;
    s16 helpTextId;
    u8 pad96;
    ShopItemFlags flags97;
    LightningEffect* lightningHandles[10];
    f32 lightningTimers[10];
    ShopItemFlags flagsE8;
    u8 padE9[0xEC - 0xE9];
} ShopItemState;

STATIC_ASSERT(offsetof(ShopItemState, flight) == 0x04);
STATIC_ASSERT(offsetof(ShopItemState, msgParam) == 0x88);

void shopitem_onSeqFree(GameObject* obj);

#endif
