#include "dlls/object_descriptor.h"
#include "main/gamebits.h"
#include "main/frame_timing.h"
#include "dolphin/pad.h"
#include "main/hud_visibility.h"
#include "main/pad.h"
#include "track/intersect_screen.h"
#include "main/dll/dll_003B_menu.h"

s8 gMenuSelectedId;
s8 gMenuCancelId;
s16 gMenuTotalWidth;
s8 gMenuItemCount;
f32 gMenuScrollTimer;
s8 gMenuArmed;

s32 Menu_getItemCount(void) {
    return gMenuItemCount;
}

void Menu_setArmed(int v) {
    gMenuArmed = v;
}

void Menu_func09_nop(void) {
}

int Menu_poll(int* sel) {
    s8 xInput;
    s8 yInput;

    if (getHudHiddenFrameCount() != 0) {
        return -1;
    }

    gMenuScrollTimer += timeDelta;
    if (gMenuScrollTimer > 200.0f) {
        gMenuScrollTimer -= 200.0f;
    }

    padGetAnalogInput(0, &xInput, &yInput);
    if (yInput < 0) {
        *sel++;
    } else if (yInput > 0) {
        *sel--;
    }

    if (*sel < 0) {
        *sel = gMenuItemCount - 1;
    }
    if (*sel >= gMenuItemCount) {
        *sel = 0;
    }

    if (gMenuArmed != 0) {
        int input = getButtonsJustPressed(0);
        if ((input & (PAD_BUTTON_A | PAD_BUTTON_START)) != 0 && mainGetBit(GAMEBIT_MenuRelated044F) == 0) {
            return gMenuSelectedId;
        }
        if ((input & PAD_BUTTON_B) != 0) {
            return gMenuCancelId;
        }
    }

    gMenuArmed = 1;
    return -1;
}
void Menu_setCancelId(int v) {
    gMenuCancelId = v;
}
void Menu_addItemEx(int resultId, int unused2, int unused3, int itemWidth, int defaultIndex) {
    if (defaultIndex == gMenuItemCount) {
        gMenuSelectedId = resultId;
    }

    gMenuTotalWidth += itemWidth;
    gMenuItemCount++;
}

void Menu_addItem(int resultId, int unused2, int itemWidth, int defaultIndex) {
    if (defaultIndex == gMenuItemCount) {
        gMenuSelectedId = resultId;
    }

    gMenuTotalWidth += itemWidth;
    gMenuItemCount++;
}
void Menu_open(int unused, int v) {
    getScreenResolution();
    gMenuTotalWidth = v;
    gMenuItemCount = 0;
    gMenuCancelId = -1;
}
void Menu_reset(int v) {
    gMenuTotalWidth = v;
    gMenuItemCount = 0;
    gMenuCancelId = -1;
}
void Menu_release(void) {
}
void Menu_initialise(void) {
    gMenuItemCount = 0;
    gMenuTotalWidth = 0;
    gMenuCancelId = 0;
    gMenuSelectedId = 0;
    gMenuArmed = 0;
}
typedef struct MenuDllInterfaceCallbacks {
    void* slot02;
    __typeof__(Menu_reset)* reset;
    __typeof__(Menu_open)* open;
    __typeof__(Menu_addItem)* addItem;
    __typeof__(Menu_addItemEx)* addItemEx;
    __typeof__(Menu_setCancelId)* setCancelId;
    __typeof__(Menu_poll)* poll;
    __typeof__(Menu_func09_nop)* slot09;
    __typeof__(Menu_setArmed)* setArmed;
    __typeof__(Menu_getItemCount)* getItemCount;
} MenuDllInterfaceCallbacks;

typedef struct MenuDllInterface {
    ResourceDescriptorHeader header;
    MenuDllInterfaceCallbacks interface;
} MenuDllInterface;

RESOURCE_ACQUIRE_ADAPTER(gMenuResourceAcquire, Menu_initialise)

MenuDllInterface Menu_funcs = {
    {
        {0, 0, 0, 0x000b0000},
        gMenuResourceAcquire,
        Menu_release,
    },
    {
        NULL,
        Menu_reset,
        Menu_open,
        Menu_addItem,
        Menu_addItemEx,
        Menu_setCancelId,
        Menu_poll,
        Menu_func09_nop,
        Menu_setArmed,
        Menu_getItemCount,
    },
};
