#ifndef MAIN_DLL_HUD_TEXTURES_H_
#define MAIN_DLL_HUD_TEXTURES_H_

#include "global.h"
#include "main/texture.h"

/*
 * HudTextures - the 0x198 pause-menu HUD work record at hudTextures
 * (.bss 0x803A89B0). Field widths mirror the deref widths observed in
 * maybetemplate.c and dll_0000_gameui.c; unobserved ranges padded.
 */
typedef struct HudTextures {
    void* pad0[0x28 / 4];
    void * tex28;
    void * tex2C;
    void * tex30;
    void * tex34;
    void * tex38;
    void * tex3C;
    void * tex40;
    void* pad44[(0x5C - 0x44) / 4];
    void * tex5C;
    void* pad60[(0x80 - 0x60) / 4];
    void * tex80;
    void* pad84[(0xB8 - 0x84) / 4];
    void * texB8;
    void * texBC;
    void * texC0;
    void* padC4[(0xF8 - 0xC4) / 4];
    void * texF8;
    void * texFC;
    void * tex100;
    void * tex104;
    void* pad108[(0x10C - 0x108) / 4];
    void * tex10C;
    void * tex110;
    void * tex114;
    void * tex118;
    void * tex11C;
    void* pad120[(0x134 - 0x120) / 4];
    void * tex134;
    void* pad138[(0x13C - 0x138) / 4];
    Texture* textBoxFrameTex[5]; /* 0x13C: the five game-text box frame textures
        copied into gGameTextBoxFrameTextures */
    void * tex150;
    void* pad154[(0x170 - 0x154) / 4];
    void * tex170;
    void* pad174[(0x17C - 0x174) / 4];
    void * tex17C;
    void * tex180;
    void * tex184;
    void* pad188[(0x198 - 0x188) / 4];
} HudTextures;

STATIC_ASSERT(sizeof(HudTextures) == sizeof(void*) * (0x198 / 4));
STATIC_ASSERT(offsetof(HudTextures, tex28) == sizeof(void*) * (0x28 / 4));
STATIC_ASSERT(offsetof(HudTextures, tex5C) == sizeof(void*) * (0x5C / 4));
STATIC_ASSERT(offsetof(HudTextures, tex80) == sizeof(void*) * (0x80 / 4));
STATIC_ASSERT(offsetof(HudTextures, texBC) == sizeof(void*) * (0xBC / 4));
STATIC_ASSERT(offsetof(HudTextures, texF8) == sizeof(void*) * (0xF8 / 4));
STATIC_ASSERT(offsetof(HudTextures, tex134) == sizeof(void*) * (0x134 / 4));
STATIC_ASSERT(offsetof(HudTextures, textBoxFrameTex) == sizeof(void*) * (0x13C / 4));
STATIC_ASSERT(offsetof(HudTextures, tex170) == sizeof(void*) * (0x170 / 4));

#endif
