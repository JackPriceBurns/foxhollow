#include "dlls/object_descriptor.h"
#include "main/gamebits.h"
#include "main/frame_timing.h"
#include "dolphin/pad.h"
#include "main/hud_visibility_api.h"
#include "main/pad.h"
#include "track/intersect_screen_api.h"
#include "main/dll/dll_003B_menu.h"

#define PAD_ACCEPT_MASK  (PAD_BUTTON_A | PAD_BUTTON_START)

s8 gMenuSelectedId;
s8 gMenuCancelId;
s16 gMenuTotalWidth;
s8 gMenuItemCount;
f32 gMenuScrollTimer;
s8 gMenuArmed;
s32 Menu_getItemCount(void)
{
    return gMenuItemCount;
}
void Menu_setArmed(int v)
{
    gMenuArmed = v;
}
void Menu_func09_nop(void)
{
}
int Menu_poll(int* sel)
{
    s8 xInput;
    s8 yInput;
    int input;
    f32 timer;

    if (getHudHiddenFrameCount() != 0)
    {
        return -1;
    }
    timer = gMenuScrollTimer + timeDelta;
    gMenuScrollTimer = timer;
    if (timer > 2e+02f)
    {
        gMenuScrollTimer = timer - 2e+02f;
    }
    padGetAnalogInput(0, &xInput, &yInput);
    if (yInput < 0)
    {
        *sel = *sel + 1;
    }
    else if (yInput > 0)
    {
        *sel = *sel - 1;
    }
    if (*sel < 0)
    {
        *sel = gMenuItemCount - 1;
    }
    if (*sel >= gMenuItemCount)
    {
        *sel = 0;
    }
    if (gMenuArmed != 0)
    {
        input = getButtonsJustPressed(0);
        if (((input & PAD_ACCEPT_MASK) != 0) && (mainGetBit(GAMEBIT_MenuRelated044F) == 0))
        {
            return gMenuSelectedId;
        }
        if ((input & PAD_BUTTON_B) != 0)
        {
            return gMenuCancelId;
        }
    }
    gMenuArmed = 1;
    return -1;
}
void Menu_setCancelId(int v)
{
    gMenuCancelId = v;
}
void Menu_addItemEx(int resultId, int unused2, int unused3, int itemWidth, int defaultIndex)
{
    if (defaultIndex == gMenuItemCount)
    {
        gMenuSelectedId = resultId;
    }
    gMenuTotalWidth = (s32)gMenuTotalWidth + itemWidth;
    gMenuItemCount++;
}

void Menu_addItem(int resultId, int unused2, int itemWidth, int defaultIndex)
{
    if (defaultIndex == gMenuItemCount)
    {
        gMenuSelectedId = resultId;
    }
    gMenuTotalWidth = (s32)gMenuTotalWidth + itemWidth;
    gMenuItemCount++;
}
void Menu_open(int unused, int v)
{
    getScreenResolution();
    gMenuTotalWidth = v;
    gMenuItemCount = 0;
    gMenuCancelId = -1;
}
void Menu_reset(int v)
{
    gMenuTotalWidth = v;
    gMenuItemCount = 0;
    gMenuCancelId = -1;
}
void Menu_release(void)
{
}
void Menu_initialise(void)
{
    gMenuItemCount = 0;
    gMenuTotalWidth = 0;
    gMenuCancelId = 0;
    gMenuSelectedId = 0;
    gMenuArmed = 0;
}
typedef struct MenuDllInterface {
    u32 reserved0;
    u32 reserved1;
    u32 reserved2;
    u32 slotCountAndFlags;
    ObjectDescriptorCallback initialise;
    ObjectDescriptorCallback release;
    ObjectDescriptorCallback slot02;
    ObjectDescriptorCallback reset;
    ObjectDescriptorCallback open;
    ObjectDescriptorCallback addItem;
    ObjectDescriptorCallback addItemEx;
    ObjectDescriptorCallback setCancelId;
    ObjectDescriptorCallback poll;
    ObjectDescriptorCallback slot09;
    ObjectDescriptorCallback setArmed;
    ObjectDescriptorCallback getItemCount;
} MenuDllInterface;

MenuDllInterface Menu_funcs = {
    0,
    0,
    0,
    0x000b0000,
    (ObjectDescriptorCallback)Menu_initialise,
    (ObjectDescriptorCallback)Menu_release,
    0,
    (ObjectDescriptorCallback)Menu_reset,
    (ObjectDescriptorCallback)Menu_open,
    (ObjectDescriptorCallback)Menu_addItem,
    (ObjectDescriptorCallback)Menu_addItemEx,
    (ObjectDescriptorCallback)Menu_setCancelId,
    (ObjectDescriptorCallback)Menu_poll,
    (ObjectDescriptorCallback)Menu_func09_nop,
    (ObjectDescriptorCallback)Menu_setArmed,
    (ObjectDescriptorCallback)Menu_getItemCount,
};
