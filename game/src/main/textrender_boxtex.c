#include "main/rcp_dolphin_api.h"
#include "main/textrender_api.h"
#include "main/textrender_internal.h"
#include "dolphin/os/OSCache.h"

Texture* gGameTextBoxBgTexture;
Texture* gGameTextBoxCornerTexture;
Texture* gGameTextBoxEdgeTexture;

void gameTextInitBoxTextures(void) {
    Texture** textureSlot;
    s16* textureAsset;
    int column0Offset;
    int rowOffset;
    int column3Offset;
    int column2Offset;
    int column1Offset;
    int column;
    Texture* texture;
    int columnBase;
    int row;
    u8* sourceRow;
    u16* cornerDst;
    int assetCount;
    int cornerBlockRow;
    u16* edgeDst;
    int edgeBlockRow;

    assetCount = 1;
    textureAsset = &gGameTextBoxTexAssets + 1;
    textureSlot = &gGameTextBoxBgTexture + 1;
    while (textureAsset--, textureSlot--, assetCount-- != 0) {
        *textureSlot = textureLoadAsset(*textureAsset);
    }

    texture = textureAlloc(0x10, 0x10, 5, 0, 0, 0, 0, 1, 1);
    gGameTextBoxCornerTexture = texture;
    cornerDst = (u16*)(texture + 1);
    cornerBlockRow = 0;
    row = 0;
    for (; cornerBlockRow < 4; cornerBlockRow++) {
        column = 0;
        column0Offset = 0;
        columnBase = 0;
        while (column++ < 2) {
            column1Offset = (columnBase + 1) * 2;
            column2Offset = (columnBase + 2) * 2;
            column3Offset = (columnBase + 3) * 2;
            rowOffset = row * 32;
            sourceRow = (u8*)gGameTextBoxCornerTexSrc + rowOffset;
            cornerDst[0] = *(u16*)(sourceRow + column0Offset);
            cornerDst[1] = *(u16*)(sourceRow + column1Offset);
            cornerDst[2] = *(u16*)(sourceRow + column2Offset);
            cornerDst[3] = *(u16*)(sourceRow + column3Offset);
            rowOffset += 32;
            sourceRow = (u8*)gGameTextBoxCornerTexSrc + rowOffset;
            cornerDst[4] = *(u16*)(sourceRow + column0Offset);
            cornerDst[5] = *(u16*)(sourceRow + column1Offset);
            cornerDst[6] = *(u16*)(sourceRow + column2Offset);
            cornerDst[7] = *(u16*)(sourceRow + column3Offset);
            rowOffset += 32;
            sourceRow = (u8*)gGameTextBoxCornerTexSrc + rowOffset;
            cornerDst[8] = *(u16*)(sourceRow + column0Offset);
            cornerDst[9] = *(u16*)(sourceRow + column1Offset);
            cornerDst[10] = *(u16*)(sourceRow + column2Offset);
            cornerDst[11] = *(u16*)(sourceRow + column3Offset);
            rowOffset += 32;
            sourceRow = (u8*)gGameTextBoxCornerTexSrc + rowOffset;
            cornerDst[12] = *(u16*)(sourceRow + column0Offset);
            cornerDst[13] = *(u16*)(sourceRow + column1Offset);
            cornerDst[14] = *(u16*)(sourceRow + column2Offset);
            cornerDst[15] = *(u16*)(sourceRow + column3Offset);
            column0Offset += 8;
            column1Offset = (columnBase + 5) * 2;
            column2Offset = (columnBase + 6) * 2;
            column3Offset = (columnBase + 7) * 2;
            rowOffset = row * 32;
            sourceRow = (u8*)gGameTextBoxCornerTexSrc + rowOffset;
            cornerDst[16] = *(u16*)(sourceRow + column0Offset);
            cornerDst[17] = *(u16*)(sourceRow + column1Offset);
            cornerDst[18] = *(u16*)(sourceRow + column2Offset);
            cornerDst[19] = *(u16*)(sourceRow + column3Offset);
            rowOffset += 32;
            sourceRow = (u8*)gGameTextBoxCornerTexSrc + rowOffset;
            cornerDst[20] = *(u16*)(sourceRow + column0Offset);
            cornerDst[21] = *(u16*)(sourceRow + column1Offset);
            cornerDst[22] = *(u16*)(sourceRow + column2Offset);
            cornerDst[23] = *(u16*)(sourceRow + column3Offset);
            rowOffset += 32;
            sourceRow = (u8*)gGameTextBoxCornerTexSrc + rowOffset;
            cornerDst[24] = *(u16*)(sourceRow + column0Offset);
            cornerDst[25] = *(u16*)(sourceRow + column1Offset);
            cornerDst[26] = *(u16*)(sourceRow + column2Offset);
            cornerDst[27] = *(u16*)(sourceRow + column3Offset);
            rowOffset += 32;
            sourceRow = (u8*)gGameTextBoxCornerTexSrc + rowOffset;
            cornerDst[28] = *(u16*)(sourceRow + column0Offset);
            cornerDst[29] = *(u16*)(sourceRow + column1Offset);
            cornerDst[30] = *(u16*)(sourceRow + column2Offset);
            cornerDst[31] = *(u16*)(sourceRow + column3Offset);
            cornerDst += 32;
            columnBase += 8;
            column0Offset += 8;
        }
        row += 4;
    }
    DCFlushRange(gGameTextBoxCornerTexture + 1, 0x200);

    texture = textureAlloc(0x14, 0x14, 5, 0, 0, 0, 0, 1, 1);
    gGameTextBoxEdgeTexture = texture;
    edgeDst = (u16*)(texture + 1);
    edgeBlockRow = 0;
    row = edgeBlockRow;
    for (; edgeBlockRow < 5; edgeBlockRow++) {
        column = 0;
        column0Offset = 0;
        for (; column < 20;) {
            column1Offset = (column + 1) * 2;
            column2Offset = (column + 2) * 2;
            column3Offset = (column + 3) * 2;
            rowOffset = row * 40;
            sourceRow = (u8*)&gGameTextBoxEdgeTexSrc + rowOffset;
            edgeDst[0] = *(u16*)(sourceRow + column0Offset);
            edgeDst[1] = *(u16*)(sourceRow + column1Offset);
            edgeDst[2] = *(u16*)(sourceRow + column2Offset);
            edgeDst[3] = *(u16*)(sourceRow + column3Offset);
            rowOffset += 40;
            sourceRow = (u8*)&gGameTextBoxEdgeTexSrc + rowOffset;
            edgeDst[4] = *(u16*)(sourceRow + column0Offset);
            edgeDst[5] = *(u16*)(sourceRow + column1Offset);
            edgeDst[6] = *(u16*)(sourceRow + column2Offset);
            edgeDst[7] = *(u16*)(sourceRow + column3Offset);
            rowOffset += 40;
            sourceRow = (u8*)&gGameTextBoxEdgeTexSrc + rowOffset;
            edgeDst[8] = *(u16*)(sourceRow + column0Offset);
            edgeDst[9] = *(u16*)(sourceRow + column1Offset);
            edgeDst[10] = *(u16*)(sourceRow + column2Offset);
            edgeDst[11] = *(u16*)(sourceRow + column3Offset);
            rowOffset += 40;
            sourceRow = (u8*)&gGameTextBoxEdgeTexSrc + rowOffset;
            edgeDst[12] = *(u16*)(sourceRow + column0Offset);
            edgeDst[13] = *(u16*)(sourceRow + column1Offset);
            edgeDst[14] = *(u16*)(sourceRow + column2Offset);
            edgeDst[15] = *(u16*)(sourceRow + column3Offset);
            edgeDst += 16;
            column += 4;
            column0Offset += 8;
        }
        row += 4;
    }
    DCFlushRange(gGameTextBoxEdgeTexture + 1, 800);
}
