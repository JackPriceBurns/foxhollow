#ifndef MAIN_GAME_UI_INTERFACE_H_
#define MAIN_GAME_UI_INTERFACE_H_

#include "global.h"

typedef struct GameUIInterface {
    void (*pad00_slots[1])(void);
    void (*frameStart)(void);
    void (*frameEnd)(void);
    void (*render)(void *context, int arg1, int arg2);
    void (*pad10_slots[3])(void);
    int (*isAnyItemBeingUsed)(void);
    int (*isItemBeingUsed)(int itemId);
    int (*isOneOfItemsBeingUsed)(s32 *items, int count);
    void (*pad28_slots[4])(void);
    void (*showNpcDialogue)(s32 id, s32 unusedA, s32 unusedB, s32 disableInput);
    void (*pad3C_slots[1])(void);
    void (*setCMenuShouldClose)(u8 shouldClose);
    void (*setInputOverride)(s32 buttons, s16 stickX, s16 stickY);
    void (*pad48_slots[2])(void);
    void (*setUnusedHudSetting)(u8 value);
    void (*pad54_slots[1])(void);
    void (*initAirMeter)(s32 maxValue, s32 textureId);
    void (*runAirMeter)(s32 value);
    void (*airMeterShutdown)(void);
    void (*airMeterSetShutdown)(void);
    void (*airMeterSetField24)(f32 value);
} GameUIInterface;

STATIC_ASSERT(offsetof(GameUIInterface, frameStart) == 0x04);
STATIC_ASSERT(offsetof(GameUIInterface, frameEnd) == 0x08);
STATIC_ASSERT(offsetof(GameUIInterface, render) == 0x0C);
STATIC_ASSERT(offsetof(GameUIInterface, isAnyItemBeingUsed) == 0x1C);
STATIC_ASSERT(offsetof(GameUIInterface, isItemBeingUsed) == 0x20);
STATIC_ASSERT(offsetof(GameUIInterface, isOneOfItemsBeingUsed) == 0x24);
STATIC_ASSERT(offsetof(GameUIInterface, showNpcDialogue) == 0x38);
STATIC_ASSERT(offsetof(GameUIInterface, setCMenuShouldClose) == 0x40);
STATIC_ASSERT(offsetof(GameUIInterface, setInputOverride) == 0x44);
STATIC_ASSERT(offsetof(GameUIInterface, setUnusedHudSetting) == 0x50);
STATIC_ASSERT(offsetof(GameUIInterface, initAirMeter) == 0x58);
STATIC_ASSERT(offsetof(GameUIInterface, runAirMeter) == 0x5C);
STATIC_ASSERT(offsetof(GameUIInterface, airMeterShutdown) == 0x60);
STATIC_ASSERT(offsetof(GameUIInterface, airMeterSetShutdown) == 0x64);
STATIC_ASSERT(offsetof(GameUIInterface, airMeterSetField24) == 0x68);

extern GameUIInterface **gGameUIInterface;

#endif /* MAIN_GAME_UI_INTERFACE_H_ */
