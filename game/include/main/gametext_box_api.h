#ifndef MAIN_GAMETEXT_BOX_API_H_
#define MAIN_GAMETEXT_BOX_API_H_

#include "types.h"

typedef struct GameTextBox
{
    u16 unk00;
    u16 maxWidth;
    u16 unk04;
    u16 maxHeight;
    u16 width;
    u16 height;
    f32 scale;
    u8 alignH;
    u8 alignV;
    u8 alignment;
    u8 style;
    s16 x;
    s16 y;
    s16 cursorX;
    s16 cursorY;
    u16 flags;
    u8 alpha;
    u8 unk1F;
} GameTextBox;

typedef GameTextBox TextSlot;

#define GAMETEXT_BOX_COUNT 148

void* gameTextGetBox(int box);

#endif /* MAIN_GAMETEXT_BOX_API_H_ */
