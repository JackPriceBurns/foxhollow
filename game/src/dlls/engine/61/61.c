#include "dlls/object_descriptor.h"
#include "track/intersect_hud_api.h"
#include "main/dll/dll_003D_titlemenuitem.h"
#include "main/pad.h"
#include "main/texture.h"
#include "main/audio/sfx.h"
#include "main/audio/sfx_trigger_ids.h"
#include "main/textrender_api.h"
#include "dolphin/pad.h"
#include "main/audio/music_api.h"
#include "main/gametext_api.h"
#include "main/gametext_color_api.h"
#include "main/mm.h"
#include "main/audio/sfx_keep_alive_api.h"
#include "main/audio/sfx_play_api.h"

#define TITLE_MENU_FLAG_ENABLED          0x01
#define TITLE_MENU_FLAG_WRAP             0x02
#define TITLE_MENU_FLAG_MOVED_LEFT       0x04
#define TITLE_MENU_FLAG_MOVED_RIGHT      0x08
#define TITLE_MENU_FLAG_CHANGED          0x10
#define TITLE_MENU_FLAG_A_TOGGLE_DISABLED 0x20
#define TITLE_MENU_FLAG_VOLUME_PREVIEW   0x40
#define TITLE_MENU_FLAG_MUSIC_PREVIEW    0x80

#define TITLE_MENU_KIND_SLIDER 0
#define TITLE_MENU_KIND_TOGGLE 1
#define TITLE_MENU_KIND_WINDOW 2

#define TITLE_MENU_ITEM_TEXTURE_COUNT 6

s8 gTitleMenuPrevMove;
f32 gTitleMenuSliderValue;
s16 gTitleMenuSliderVelocity;
void* gTitleMenuItemTextures[TITLE_MENU_ITEM_TEXTURE_COUNT];
s16 gTitleMenuItemTextureIds[TITLE_MENU_ITEM_TEXTURE_COUNT] = {1558, 1557, 777, 778, 779, 780};

void TitleMenuItem_setAButtonToggle(TitleMenuItem* item, int enabled)
{
    if (enabled != 0)
    {
        item->flags = (u8)(item->flags & ~TITLE_MENU_FLAG_A_TOGGLE_DISABLED);
    }
    else
    {
        item->flags = (u8)(item->flags | TITLE_MENU_FLAG_A_TOGGLE_DISABLED);
    }
}

int TitleMenuItem_isChanged(TitleMenuItem* item)
{
    return item->flags & TITLE_MENU_FLAG_CHANGED;
}

void TitleMenuItem_setVal(TitleMenuItem* item, int val)
{
    item->value = val;
    item->frameDelay = 2;
}

int TitleMenuItem_getVal(TitleMenuItem* item)
{
    return item->value;
}

void TitleMenuItem_setEnabled(TitleMenuItem* item, int flag)
{
    if (flag != 0)
    {
        if ((item->flags & TITLE_MENU_FLAG_ENABLED) == 0)
        {
            gTitleMenuSliderVelocity = 0;
            gTitleMenuSliderValue = item->value;
        }
        item->flags = (u8)(item->flags | TITLE_MENU_FLAG_ENABLED);
    }
    else
    {
        item->flags = (u8)(item->flags & ~TITLE_MENU_FLAG_ENABLED);
    }
}

int TitleMenuItem_isEnabled(TitleMenuItem* item)
{
    return item->flags & TITLE_MENU_FLAG_ENABLED;
}

void TitleMenuItem_render(TitleMenuItem* item, int unused, int alpha)
{
    Texture* texture;
    void* phrase;
    int textureIndex;
    int drawAlpha;
    f32 markerX;

    switch (item->kind)
    {
    case TITLE_MENU_KIND_SLIDER:
        drawTexture(gTitleMenuItemTextures[1], item->x, item->y, (u8)(((u8)alpha * 0xb4) >> 8), 0x100);

        texture = gTitleMenuItemTextures[0];
        markerX = (f32)(int)((f32)item->extra.textId *
                                 ((f32)(item->value - item->minValue) / (f32)(item->maxValue - item->minValue)) +
                             item->x - (f32)(texture->width >> 1));
        drawTexture(texture, markerX, (f32)(item->y - 4), (u8)(((u8)alpha * 0xff) >> 8), 0x100);
        break;
    case TITLE_MENU_KIND_TOGGLE:
        if ((item->flags & TITLE_MENU_FLAG_ENABLED) != 0)
        {
            if (item->value != 0)
            {
                textureIndex = 2;
            }
            else
            {
                textureIndex = 4;
            }
        }
        else if (item->value != 0)
        {
            textureIndex = 3;
        }
        else
        {
            textureIndex = 5;
        }

        if ((item->flags & TITLE_MENU_FLAG_A_TOGGLE_DISABLED) != 0)
        {
            drawAlpha = (u8)alpha >> 1;
        }
        else
        {
            drawAlpha = (u8)alpha;
        }
        drawTexture(gTitleMenuItemTextures[textureIndex], item->x, item->y, (u8)drawAlpha, 0x100);
        break;
    case TITLE_MENU_KIND_WINDOW:
        phrase = gameTextGetPhrase(item->extra.window.phraseId,
                                   (item->flags & TITLE_MENU_FLAG_MUSIC_PREVIEW) != 0 ? 0 : item->value);
        gameTextSetColor(0, 0, 0, (u8)(((u8)alpha * 0x96) >> 8));
        gameTextSetWindowStrPos(item->extra.window.windowId, 2, 2);
        gameTextAppendStr(phrase, item->extra.window.windowId);
        gameTextSetColor(0xff, 0xff, 0xff, alpha);
        gameTextSetWindowStrPos(item->extra.window.windowId, 0, 0);
        gameTextAppendStr(phrase, item->extra.window.windowId);
        break;
    }

    if (--item->frameDelay < 0)
    {
        item->frameDelay = 0;
    }
}

void TitleMenuItem_update(TitleMenuItem* item)
{
    s16 oldValue;
    s8 stickX;
    s16 move;
    s16 gatedMove;
    int sliderDelta;
    s16 previewVolume;
    int clampedVolume;

    if ((item->flags & TITLE_MENU_FLAG_ENABLED) == 0)
    {
        return;
    }

    item->flags =
        (u8)(item->flags & ~(TITLE_MENU_FLAG_MOVED_LEFT | TITLE_MENU_FLAG_MOVED_RIGHT | TITLE_MENU_FLAG_CHANGED));
    oldValue = item->value;
    item->frameDelay = 4;

    switch (item->kind)
    {
    case TITLE_MENU_KIND_WINDOW:
        stickX = padGetStickX(0);
        if (stickX > 0x23)
        {
            move = 1;
        }
        else if (stickX < -0x23)
        {
            move = -1;
        }
        else
        {
            move = 0;
        }

        gatedMove = move;
        if (gTitleMenuPrevMove != 0)
        {
            gatedMove = 0;
        }
        gTitleMenuPrevMove = move;

        if (gatedMove < 0)
        {
            Sfx_PlayFromObject(0, SFXTRIG_sc_lockedon22);
            item->value--;
            item->flags = (u8)(item->flags | TITLE_MENU_FLAG_MOVED_LEFT);
        }
        else if (gatedMove > 0)
        {
            Sfx_PlayFromObject(0, SFXTRIG_sc_lockedon22);
            item->value++;
            item->flags = (u8)(item->flags | TITLE_MENU_FLAG_MOVED_RIGHT);
        }
        break;
    case TITLE_MENU_KIND_SLIDER:
        stickX = padGetStickX(0);
        sliderDelta = (s16)(stickX / 16) * 0xa0;

        if (((s16)sliderDelta != 0) && (!(gTitleMenuSliderValue < item->minValue) || ((s16)sliderDelta >= 0)) &&
            (!(gTitleMenuSliderValue > item->maxValue) || ((s16)sliderDelta <= 0)))
        {
            gTitleMenuSliderVelocity = (s16)(0.3f * (f32)(s16)(sliderDelta - gTitleMenuSliderVelocity) + gTitleMenuSliderVelocity);
            Sfx_KeepAliveLoopedObjectSound(0, SFXTRIG_pda_compassbeep);
        }
        else
        {
            gTitleMenuSliderVelocity = 0;
        }

        gTitleMenuSliderValue += gTitleMenuSliderVelocity / 150.0f;
        item->value = (s16)(0.5f + gTitleMenuSliderValue);

        if ((item->flags & TITLE_MENU_FLAG_VOLUME_PREVIEW) != 0)
        {
            clampedVolume = item->value > 0x7f ? 0x7f : item->value;
            previewVolume = item->value;
            Sfx_SetObjectSfxVolume(0, SFXTRIG_pda_compassbeep,
                                   clampedVolume < 0 ? 0 : (previewVolume > 0x7f ? 0x7f : previewVolume),
                                   0.5f);
        }
        break;
    default:
        if (((item->flags & TITLE_MENU_FLAG_A_TOGGLE_DISABLED) == 0) && ((getButtonsJustPressed(0) & PAD_BUTTON_A) != 0))
        {
            Sfx_PlayFromObject(0, SFXTRIG_sc_lockedon22_f4);
            item->value = (s16)(item->value ^ 1);
        }
        break;
    }

    if (item->value > item->maxValue)
    {
        if ((item->flags & TITLE_MENU_FLAG_WRAP) != 0)
        {
            item->value = 0;
        }
        else
        {
            item->value = item->maxValue;
        }
    }
    else if (item->value < item->minValue)
    {
        if ((item->flags & TITLE_MENU_FLAG_WRAP) != 0)
        {
            item->value = item->maxValue;
        }
        else
        {
            item->value = item->minValue;
        }
    }

    if (oldValue != item->value)
    {
        item->flags = (u8)(item->flags | TITLE_MENU_FLAG_CHANGED);
    }

    if (((item->flags & TITLE_MENU_FLAG_MUSIC_PREVIEW) != 0) && ((item->flags & TITLE_MENU_FLAG_CHANGED) != 0))
    {
        Music_PlayTrackByIndex(item->value);
    }
}

void TitleMenuItem_free(TitleMenuItem* item)
{
    mm_free(item);
}

TitleMenuItem* TitleMenuItem_createWithWindow(int phraseId, int windowId, s16 minValue, s16 maxValue, s16 value)
{
    TitleMenuItem* item;

    if (value < minValue)
    {
        value = minValue;
    }
    if (value > maxValue)
    {
        value = maxValue;
    }

    item = (TitleMenuItem*)mmAlloc(0x12, 5, 0);
    item->kind = TITLE_MENU_KIND_WINDOW;
    item->extra.window.phraseId = phraseId;
    item->extra.window.windowId = windowId;
    item->value = value;
    item->minValue = minValue;
    item->maxValue = maxValue;
    item->flags = TITLE_MENU_FLAG_WRAP;
    item->frameDelay = 4;
    return item;
}

TitleMenuItem* TitleMenuItem_create(s16 x, s16 y, s16 minValue, s16 maxValue, s16 value)
{
    TitleMenuItem* item;

    if (value < minValue)
    {
        value = minValue;
    }
    if (value > maxValue)
    {
        value = maxValue;
    }

    item = (TitleMenuItem*)mmAlloc(0xe, 5, 0);
    item->kind = TITLE_MENU_KIND_TOGGLE;
    item->value = value;
    item->minValue = minValue;
    item->maxValue = maxValue;
    item->x = x;
    item->y = y;
    item->flags = 0;
    item->frameDelay = 4;
    return item;
}

TitleMenuItem* TitleMenuItem_createWithText(s16 x, s16 y, s16 minValue, s16 maxValue, s16 value, int textId)
{
    TitleMenuItem* item;

    if (value < minValue)
    {
        value = minValue;
    }
    if (value > maxValue)
    {
        value = maxValue;
    }

    item = (TitleMenuItem*)mmAlloc(0x10, 5, 0);
    item->kind = TITLE_MENU_KIND_SLIDER;
    item->value = value;
    item->minValue = minValue;
    item->maxValue = maxValue;
    item->x = x;
    item->y = y;
    item->flags = 0;
    item->frameDelay = 4;
    item->extra.textId = textId;
    return item;
}

void TitleMenuItem_loadTextures(void)
{
    int i;

    for (i = 0; i < TITLE_MENU_ITEM_TEXTURE_COUNT; i++)
    {
        if (gTitleMenuItemTextures[i] == NULL)
        {
            gTitleMenuItemTextures[i] = textureLoadAsset(gTitleMenuItemTextureIds[i]);
        }
    }
}

void TitleMenuItem_release(void)
{
    int i;

    for (i = 0; i < TITLE_MENU_ITEM_TEXTURE_COUNT; i++)
    {
        textureFree((Texture*)(gTitleMenuItemTextures[i]));
        gTitleMenuItemTextures[i] = NULL;
    }
}

void TitleMenuItem_initialise(void)
{
    void** slots = gTitleMenuItemTextures;
    slots[0] = NULL;
    slots[1] = NULL;
    slots[2] = NULL;
    slots[3] = NULL;
    slots[4] = NULL;
    slots[5] = NULL;
}
typedef struct TitleMenuItemDllInterface {
    u32 reserved0;
    u32 reserved1;
    u32 reserved2;
    u32 slotCountAndFlags;
    ObjectDescriptorCallback initialise;
    ObjectDescriptorCallback release;
    ObjectDescriptorCallback slot02;
    ObjectDescriptorCallback createWithText;
    ObjectDescriptorCallback create;
    ObjectDescriptorCallback createWithWindow;
    ObjectDescriptorCallback free;
    ObjectDescriptorCallback update;
    ObjectDescriptorCallback render;
    ObjectDescriptorCallback isEnabled;
    ObjectDescriptorCallback setEnabled;
    ObjectDescriptorCallback getVal;
    ObjectDescriptorCallback setVal;
    ObjectDescriptorCallback isChanged;
    ObjectDescriptorCallback setAButtonToggle;
} TitleMenuItemDllInterface;

TitleMenuItemDllInterface TitleMenuItem_funcs = {
    0,
    0,
    0,
    0x000e0000,
    (ObjectDescriptorCallback)TitleMenuItem_initialise,
    (ObjectDescriptorCallback)TitleMenuItem_release,
    0,
    (ObjectDescriptorCallback)TitleMenuItem_createWithText,
    (ObjectDescriptorCallback)TitleMenuItem_create,
    (ObjectDescriptorCallback)TitleMenuItem_createWithWindow,
    (ObjectDescriptorCallback)TitleMenuItem_free,
    (ObjectDescriptorCallback)TitleMenuItem_update,
    (ObjectDescriptorCallback)TitleMenuItem_render,
    (ObjectDescriptorCallback)TitleMenuItem_isEnabled,
    (ObjectDescriptorCallback)TitleMenuItem_setEnabled,
    (ObjectDescriptorCallback)TitleMenuItem_getVal,
    (ObjectDescriptorCallback)TitleMenuItem_setVal,
    (ObjectDescriptorCallback)TitleMenuItem_isChanged,
    (ObjectDescriptorCallback)TitleMenuItem_setAButtonToggle,
};
