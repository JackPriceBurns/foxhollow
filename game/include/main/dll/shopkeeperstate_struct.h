#ifndef MAIN_DLL_SHOPKEEPERSTATE_STRUCT_H_
#define MAIN_DLL_SHOPKEEPERSTATE_STRUCT_H_

#include "types.h"
#include "main/dll/baddie_state.h"
#include "main/dll/dll_002E_moveLib.h"
#include "main/model_engine.h"
#include "main/objprint_character_api.h"

typedef struct ShopkeeperState
{
    BaddieState baddie;
    MoveLibState moveLib; /* 0x35c: dll_2E look-controller block */
    CharacterEyeAnimState eyeAnimState; /* 0x980: head-aim / eye-blink record (characterDoEyeAnims) */
    u8 pad9A8[0x9B0 - 0x9A8];
    RingBufferQueue* msgStack; /* 0x9b0: queued state messages */
    struct GameObject* vendorObj; /* 0x9b4: nearest group-9 shop manager */
    f32 bobAmplitude; /* 0x9b8 */
    f32 bobBaseY; /* 0x9bc */
    f32 actionTimer; /* 0x9c0 */
    f32 textTimer; /* 0x9c4: gameTextShow 0x433 countdown */
    s16 playerMoney; /* 0x9c8 */
    u16 bobPhase; /* 0x9ca */
    s16 price; /* 0x9cc */
    s16 minPrice; /* 0x9ce */
    s16 priceShown; /* 0x9d0 */
    u8 haggleCount; /* 0x9d2 */
    u8 curveNodeType; /* 0x9d3 */
    u8 flags9D4; /* 0x9d4: 2 purchased-event, 4 facing, 8 idle-anim active, 0x10 leave, 0x20 tick */
    u8 amount; /* 0x9d5 */
    u8 opacity; /* 0x9d6: copied to obj alpha */
    u8 pad9D7;
} ShopkeeperState;

#endif
