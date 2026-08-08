#include "main/gametext_api.h"
#include "main/gametext_internal.h"
#include "main/gametext_shared_internal.h"
#include "main/textrender_api.h"
#include "main/rcp_dolphin_api.h"
#include "main/camera.h"

char sMapDirectoryNameArwing[] = "Arwing";
char sMapDirectoryNameBoot[] = "Boot";
char sMapDirectoryNameCRFort[] = "CRFort";
char sMapDirectoryNameDFPTop[] = "DFPTop";
char sMapDirectoryNameDesert[] = "Desert";
char sMapDirectoryNameLINKG[] = "LINKG";
char sMapDirectoryNameLink[] = "Link";
char sMapDirectoryNameLinkB[] = "LinkB";
char sMapDirectoryNameLinkC[] = "LinkC";
char sMapDirectoryNameLinkD[] = "LinkD";
char sMapDirectoryNameLinkE[] = "LinkE";
char sMapDirectoryNameLinkF[] = "LinkF";
char sMapDirectoryNameLinkH[] = "LinkH";
char sMapDirectoryNameLinkJ[] = "LinkJ";
char sMapDirectoryNameMMPass[] = "MMPass";
char sMapDirectoryNameNWastes[] = "NWastes";
char sMapDirectoryNameShop[] = "Shop";
char sMapDirectoryNameSwapHol[] = "SwapHol";
char sMapDirectoryNameVolcano[] = "Volcano";
char sMapDirectoryNameWarlock[] = "Warlock";
char sLanguageNameEnglish[] = "English";
char sLanguageNameFrench[] = "French";
char sLanguageNameGerman[] = "German";
char sLanguageNameItalian[] = "Italian";
char sLanguageNameSpanish[] = "Spanish";

int isSpace(u32 c);
static inline int gameTextIdExists(int id);
static inline int textCountChars(char* lineStr);

int isSpace(u32 c) {
    int result = 0;

    if (c == 0x20 || c == 0x3000 || c == 0x303F) {
        result = 1;
    }
    return result;
}
static inline int gameTextIdExists(int id) {
    GameTextDef* e;
    int count;
    int i;

    if (gameTextFonts->status != 2) {
        return 0;
    }
    e = gameTextFonts->entries;
    count = gameTextFonts->entryCount;
    for (i = 0; i != count; i++) {
        if (e->identifier == id) {
            return 1;
        }
        e++;
    }
    return 0;
}

static inline int textCountChars(char* lineStr) {
    int charCount;
    int byteOffset;
    u32 ch;
    int charLen;

    charCount = 0;
    byteOffset = 0;
    if (lineStr == NULL) {
        return 0;
    }
    while ((ch = utf8GetNextChar((u8*)(lineStr + byteOffset), &charLen)) != 0) {
        byteOffset += charLen;
        if (ch >= 0xe000 && ch <= 0xf8ff) {
            CtrlCharEntry* g = gGameTextCtrlCodeArgCounts;
            int n;
            int val = 0;
            for (n = 46; n-- != 0;) {
                if (g->key == ch) {
                    val = g->len;
                    break;
                }
                g++;
            }
            byteOffset += val * 2;
        } else {
            charCount++;
        }
    }
    return charCount;
}

static char* gameStrcpy(char* dst, char* src) {
    u32 ch;
    int len;
    do {
        ch = utf8GetNextChar((u8*)src, &len);
        while (len-- != 0) {
            *dst++ = *src++;
        }
        if (ch >= 0xe000 && ch <= 0xf8ff) {
            len = getControlCharLen(ch) * 2;
            while (len-- != 0) {
                *dst++ = *src++;
            }
        }
    } while (ch != 0);
    return dst - 1;
}

int utf8GetNextChar(u8* str, int* outLen) {
    u8 first = *str;
    int cls = gUtf8CharClassTable[first];
    u32 acc = 0;
    switch (cls) {
    case 5:
        str++;
        acc = first << 6;
    case 4:
        acc += *str++;
        acc <<= 6;
    case 3:
        acc += *str++;
        acc <<= 6;
    case 2:
        acc += *str++;
        acc <<= 6;
    case 1:
        acc += *str++;
        acc <<= 6;
    case 0:
        acc += *str;
    default:
        break;
    }
    *outLen = cls + 1;
    return acc - gUtf8ClassOffsetTable[cls];
}
int gameTextGetTaskText(int id, int* outTextSeqId, int* outDirId) {
    int i;
    TaskTextEntry* e = gTaskTextTable;
    for (i = 0; i < 0x7a; i++) {
        if (e->objSeqId == id) {
            if (outTextSeqId != NULL) {
                *outTextSeqId = e->textSeqId;
            }
            if (outDirId != NULL) {
                *outDirId = e->dirId;
            }
            return 1;
        }
        e++;
    }
    return 0;
}

void gameTextShowStr(char* text, int box, int cursorX, int cursorY) {
    int i;
    GameTextSlot* e;
    char* buf;
    if (gameTextDrawFunc != NULL) {
        TextSlot* slot = (TextSlot*)gTextBoxes + box;
        slot->cursorX = cursorX;
        slot->cursorY = cursorY;
        gameTextRenderStrs(text, box);
    } else {
        i = gGameTextCommandCount++;
        e = &gGameTextCommandSlots[i];
        e->opcode = 7;
        buf = gGameTextCommandStringCursor;
        gGameTextCommandStringCursor = gameStrcpy(buf, text) + 1;
        e->arg0 = (intptr_t)buf;
        e->arg1 = box;
        e->arg2 = cursorX;
        e->arg3 = cursorY;
    }
}

void gameTextRenderStrs(char* str, int boxIdx) {
    TextSlot* slot = (TextSlot*)gTextBoxes + boxIdx;
    char** lines;
    int count;
    f32 lineH;
    int i;
    int closeAtEnd = 0;

    if (gGameTextRenderingById != 1) {
        slot->alignment = slot->alignH;
        if (gGameTextMeasureOnly == 0) {
            gameTextDrawBox(NULL, (intptr_t)str, slot);
        }
    }
    lines = gameTextWrapLines(str, (f32)(u32)slot->width, slot->scale, &count, &lineH);
    if (lines == NULL) {
        slot->cursorY = (s16)(lineH * count + slot->cursorY);
        return;
    }
    if (gameTextDrawFunc != NULL) {
        gxSetScissorRect(0, 0, 0, 0, 0x280, 0x1e0);
    } else if (gGameTextMeasureOnly == 0) {
        gxSetScissorRect(0, 0, slot->x, slot->y, slot->x + slot->width, slot->y + slot->height);
    }
    gGameTextScale = slot->scale;
    for (i = 0; i < count; i++) {
        if (i == count - 1 && slot->alignment == 3) {
            slot->alignment = 0;
            closeAtEnd = 1;
        }
        if (gGameTextShadowEnabled == 1 && gGameTextMeasureOnly == 0) {
            u8 save7 = gGameTextColorR;
            u8 save6 = gGameTextColorG;
            u8 save5 = gGameTextColorB;
            f32 saveColor = gGameTextScale;
            gGameTextColorR = gGameTextShadowColorR;
            gGameTextColorG = gGameTextShadowColorG;
            gGameTextColorB = gGameTextShadowColorB;
            textRenderStr(lines[i], slot, slot->cursorX, slot->cursorY, lineH, 1);
            gGameTextColorR = save7;
            gGameTextColorG = save6;
            gGameTextColorB = save5;
            gGameTextScale = saveColor;
        }
        textRenderStr(lines[i], slot, slot->cursorX, slot->cursorY, lineH, 0);
        slot->cursorY = (s16)((f32)slot->cursorY + lineH);
        if (closeAtEnd) {
            slot->alignment = 3;
        }
    }
    if (gGameTextMeasureOnly == 0) {
        Camera_ApplyCurrentViewport(NULL);
    }
}

void gameTextAppendStr(char* str, int box) {
    int i;
    GameTextSlot* e;
    char* buf;
    if (gameTextDrawFunc != NULL) {
        gameTextRenderStrs(str, box);
    } else {
        i = gGameTextCommandCount++;
        e = &gGameTextCommandSlots[i];
        e->opcode = 6;
        buf = gGameTextCommandStringCursor;
        gGameTextCommandStringCursor = gameStrcpy(buf, str) + 1;
        e->arg0 = (intptr_t)buf;
        e->arg1 = box;
    }
}

void gameTextShowTimeStr(char* str) {
    int i;
    GameTextSlot* e;
    char* buf;
    i = gGameTextCommandCount++;
    e = &gGameTextCommandSlots[i];
    e->opcode = 5;
    buf = gGameTextCommandStringCursor;
    gGameTextCommandStringCursor = gameStrcpy(buf, str) + 1;
    e->arg0 = (intptr_t)buf;
}

u8 gUtf8CharClassTable[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5,
};

int gUtf8ClassOffsetTable[6] = {0, 12416, 925824, 63447168, -100130688, -2113396608};

char sMapDirectoryNameAnimtest[] = "Animtest";

char sMapDirectoryNameBOSSAndross[] = "BOSSAndross";

char sMapDirectoryNameBossDrakor[] = "BossDrakor";

char sMapDirectoryNameBossGaldon[] = "BossGaldon";

char sMapDirectoryNameBossTrex[] = "BossTrex";

char sMapDirectoryNameCapeClaw[] = "CapeClaw";

char sMapDirectoryNameCloudDungeon[] = "CloudDungeon";

char sMapDirectoryNameCloudRace[] = "CloudRace";

char sMapDirectoryNameCommunicator[] = "Communicator";

char sMapDirectoryNameDBShrine[] = "DBShrine";

char sMapDirectoryNameDFShrine[] = "DFShrine";

char sMapDirectoryNameDarkIceMines[] = "DarkIceMines";

char sMapDirectoryNameDarkIceMines2[] = "DarkIceMines2";

char sMapDirectoryNameDragRock[] = "DragRock";

char sMapDirectoryNameDragRockBot[] = "DragRockBot";

char sMapDirectoryNameECShrine[] = "ECShrine";

char sMapDirectoryNameFrontEnd[] = "FrontEnd";

char sMapDirectoryNameGPShrine[] = "GPShrine";

char sMapDirectoryNameGameMaze[] = "GameMaze";

char sMapDirectoryNameIceMountain[] = "IceMountain";

char sMapDirectoryNameInsideGal[] = "InsideGal";

char sMapDirectoryNameLightFoot[] = "LightFoot";

char sMapDirectoryNameMMShrine[] = "MMShrine";

char sMapDirectoryNameMagicCave[] = "MagicCave";

char sMapDirectoryNameNWShrine[] = "NWShrine";

char sMapDirectoryNameSequences[] = "Sequences";

char sMapDirectoryNameShipBattle[] = "ShipBattle";

char sMapDirectoryNameTaskTexts000[] = "TaskTexts000";

char sMapDirectoryNameTaskTexts001[] = "TaskTexts001";

char sMapDirectoryNameTaskTexts002[] = "TaskTexts002";

char sMapDirectoryNameTaskTexts003[] = "TaskTexts003";

char sMapDirectoryNameTaskTexts004[] = "TaskTexts004";

char sMapDirectoryNameTaskTexts005[] = "TaskTexts005";

char sMapDirectoryNameTaskTexts006[] = "TaskTexts006";

char sMapDirectoryNameTaskTexts007[] = "TaskTexts007";

char sMapDirectoryNameTaskTexts008[] = "TaskTexts008";

char sMapDirectoryNameTaskTexts009[] = "TaskTexts009";

char sMapDirectoryNameTaskTexts010[] = "TaskTexts010";

char sMapDirectoryNameTaskTexts011[] = "TaskTexts011";

char sMapDirectoryNameTaskTexts012[] = "TaskTexts012";

char sMapDirectoryNameTaskTexts013[] = "TaskTexts013";

char sMapDirectoryNameTaskTexts014[] = "TaskTexts014";

char sMapDirectoryNameTaskTexts015[] = "TaskTexts015";

char sMapDirectoryNameTaskTexts016[] = "TaskTexts016";

char sMapDirectoryNameTaskTexts017[] = "TaskTexts017";

char sMapDirectoryNameTaskTexts018[] = "TaskTexts018";

char sMapDirectoryNameTaskTexts019[] = "TaskTexts019";

char sMapDirectoryNameTaskTexts021[] = "TaskTexts021";

char sMapDirectoryNameTaskTexts022[] = "TaskTexts022";

char sMapDirectoryNameTaskTexts023[] = "TaskTexts023";

char sMapDirectoryNameTaskTexts024[] = "TaskTexts024";

char sMapDirectoryNameWallCity[] = "WallCity";

char sMapDirectoryNameWorldMap[] = "WorldMap";

char* sMapDirectoryNameTable[74] = {
    sMapDirectoryNameAnimtest,      sMapDirectoryNameArwing,
    sMapDirectoryNameBOSSAndross,   sMapDirectoryNameBoot,
    sMapDirectoryNameBossDrakor,    sMapDirectoryNameBossGaldon,
    sMapDirectoryNameBossTrex,      sMapDirectoryNameCRFort,
    sMapDirectoryNameCapeClaw,      sMapDirectoryNameCloudDungeon,
    sMapDirectoryNameCloudRace,     sMapDirectoryNameCommunicator,
    sMapDirectoryNameDBShrine,      sMapDirectoryNameDFPTop,
    sMapDirectoryNameDFShrine,      sMapDirectoryNameDarkIceMines,
    sMapDirectoryNameDarkIceMines2, sMapDirectoryNameDesert,
    sMapDirectoryNameDragRock,      sMapDirectoryNameDragRockBot,
    sMapDirectoryNameECShrine,      sMapDirectoryNameFrontEnd,
    sMapDirectoryNameGPShrine,      sMapDirectoryNameGameMaze,
    sMapDirectoryNameIceMountain,   sMapDirectoryNameInsideGal,
    sMapDirectoryNameLINKG,         sMapDirectoryNameLightFoot,
    sMapDirectoryNameLink,          sMapDirectoryNameLinkB,
    sMapDirectoryNameLinkC,         sMapDirectoryNameLinkD,
    sMapDirectoryNameLinkE,         sMapDirectoryNameLinkF,
    sMapDirectoryNameLinkH,         sMapDirectoryNameLinkJ,
    sMapDirectoryNameMMPass,        sMapDirectoryNameMMShrine,
    sMapDirectoryNameMagicCave,     sMapDirectoryNameNWShrine,
    sMapDirectoryNameNWastes,       sMapDirectoryNameSequences,
    sMapDirectoryNameShipBattle,    sMapDirectoryNameShop,
    sMapDirectoryNameSwapHol,       sMapDirectoryNameTaskTexts000,
    sMapDirectoryNameTaskTexts001,  sMapDirectoryNameTaskTexts002,
    sMapDirectoryNameTaskTexts003,  sMapDirectoryNameTaskTexts004,
    sMapDirectoryNameTaskTexts005,  sMapDirectoryNameTaskTexts006,
    sMapDirectoryNameTaskTexts007,  sMapDirectoryNameTaskTexts008,
    sMapDirectoryNameTaskTexts009,  sMapDirectoryNameTaskTexts010,
    sMapDirectoryNameTaskTexts011,  sMapDirectoryNameTaskTexts012,
    sMapDirectoryNameTaskTexts013,  sMapDirectoryNameTaskTexts014,
    sMapDirectoryNameTaskTexts015,  sMapDirectoryNameTaskTexts016,
    sMapDirectoryNameTaskTexts017,  sMapDirectoryNameTaskTexts018,
    sMapDirectoryNameTaskTexts019,  sMapDirectoryNameTaskTexts021,
    sMapDirectoryNameTaskTexts022,  sMapDirectoryNameTaskTexts023,
    sMapDirectoryNameTaskTexts024,  sMapDirectoryNameVolcano,
    sMapDirectoryNameWallCity,      sMapDirectoryNameWarlock,
    sMapDirectoryNameWorldMap,      NULL,
};

char sLanguageNameJapanese[] = "Japanese";

LanguageName sLanguageNameTable[6] = {
    {sLanguageNameEnglish, 4, {0, 0, 0}}, {sLanguageNameFrench, 4, {0, 0, 0}},   {sLanguageNameGerman, 4, {0, 0, 0}},
    {sLanguageNameItalian, 4, {0, 0, 0}}, {sLanguageNameJapanese, 0, {0, 0, 0}}, {sLanguageNameSpanish, 4, {0, 0, 0}},
};

GameTextBox gTextBoxes[GAMETEXT_BOX_COUNT] = {
    {560, 560, 400, 400, 560, 400, 1.0f, 2, 0, 2, 5, 40, 40, 0, 0, 0, 0, 0},
    {256, 256, 96, 96, 256, 96, 1.0f, 3, 0, 3, 6, 30, 30, 0, 0, 0, 0, 0},
    {580, 580, 400, 400, 580, 400, 1.0f, 2, 1, 2, 5, 30, 40, 0, 0, 0, 0, 0},
    {16, 320, 16, 110, 320, 110, 1.0f, 0, 1, 0, 7, 40, 40, 0, 0, 0, 0, 0},
    {330, 330, 256, 256, 330, 256, 1.0f, 0, 0, 0, 5, 30, 100, 0, 0, 0, 0, 0},
    {330, 330, 330, 330, 330, 330, 1.0f, 0, 0, 0, 5, 30, 240, 0, 0, 0, 0, 0},
    {230, 230, 256, 256, 230, 256, 1.0f, 2, 0, 2, 5, 380, 100, 0, 0, 0, 0, 0},
    {230, 230, 256, 256, 230, 256, 1.0f, 2, 0, 2, 5, 380, 240, 0, 0, 0, 0, 0},
    {16, 200, 100, 256, 200, 256, 1.0f, 1, 0, 1, 5, 361, 63, 0, 0, 0, 0, 0},
    {16, 200, 16, 256, 200, 256, 1.0f, 1, 0, 1, 5, 346, 88, 0, 0, 0, 0, 0},
    {580, 580, 25, 25, 580, 25, 1.0f, 2, 0, 2, 5, 30, 415, 0, 0, 0, 0, 0},
    {580, 580, 480, 480, 580, 480, 1.0f, 2, 0, 2, 5, 30, 0, 0, 0, 0, 0, 0},
    {390, 390, 200, 200, 390, 200, 1.0f, 2, 0, 2, 7, 40, 50, 0, 0, 0, 0, 0},
    {150, 150, 16, 40, 150, 40, 1.2f, 0, 1, 0, 5, 54, 300, 0, 0, 0, 0, 0},
    {16, 502, 16, 32, 502, 32, 1.0f, 2, 0, 2, 3, 69, 263, 0, 0, 0, 0, 0},
    {16, 502, 16, 32, 502, 32, 1.0f, 2, 0, 2, 3, 69, 314, 0, 0, 0, 0, 0},
    {260, 260, 16, 32, 260, 32, 1.0f, 1, 0, 1, 5, 56, 0, 0, 0, 0, 0, 0},
    {260, 260, 16, 32, 260, 32, 1.0f, 1, 0, 1, 5, 56, -34, 0, 0, 0, 0, 0},
    {16, 502, 16, 32, 502, 32, 1.0f, 2, 0, 2, 3, 69, 161, 0, 0, 0, 0, 0},
    {16, 502, 16, 32, 502, 32, 1.0f, 2, 0, 2, 3, 69, 215, 0, 0, 0, 0, 0},
    {16, 502, 16, 32, 502, 32, 1.0f, 2, 0, 2, 3, 69, 269, 0, 0, 0, 0, 0},
    {640, 640, 16, 32, 640, 32, 1.0f, 2, 0, 2, 5, 0, 416, 0, 0, 0, 0, 0},
    {260, 260, 16, 32, 260, 32, 1.0f, 0, 0, 0, 5, 56, 0, 0, 0, 0, 0, 0},
    {260, 260, 16, 32, 260, 32, 1.0f, 0, 0, 0, 5, 56, 26, 0, 0, 0, 0, 0},
    {260, 260, 16, 32, 260, 32, 1.0f, 0, 0, 0, 5, 56, 52, 0, 0, 0, 0, 0},
    {260, 260, 16, 52, 260, 52, 1.0f, 0, 0, 0, 5, 56, 78, 0, 0, 0, 0, 0},
    {260, 260, 16, 32, 260, 32, 1.0f, 0, 0, 0, 5, 56, 104, 0, 0, 0, 0, 0},
    {260, 260, 16, 52, 260, 52, 1.0f, 0, 0, 0, 5, 56, 130, 0, 0, 0, 0, 0},
    {260, 260, 16, 32, 260, 32, 1.0f, 0, 0, 0, 5, 56, 156, 0, 0, 0, 0, 0},
    {260, 260, 16, 52, 260, 52, 1.0f, 0, 0, 0, 5, 56, 182, 0, 0, 0, 0, 0},
    {260, 260, 16, 52, 260, 52, 1.0f, 0, 0, 0, 5, 56, 208, 0, 0, 0, 0, 0},
    {240, 240, 16, 32, 240, 32, 1.0f, 0, 0, 0, 5, 76, 52, 0, 0, 0, 0, 0},
    {240, 240, 16, 32, 240, 32, 1.0f, 0, 0, 0, 5, 76, 94, 0, 0, 0, 0, 0},
    {240, 240, 16, 32, 240, 32, 1.0f, 0, 0, 0, 5, 76, 136, 0, 0, 0, 0, 0},
    {260, 260, 128, 128, 260, 128, 1.0f, 1, 0, 1, 5, 56, 26, 0, 0, 0, 0, 0},
    {260, 260, 128, 128, 260, 128, 1.0f, 1, 0, 1, 5, 56, 52, 0, 0, 0, 0, 0},
    {260, 260, 128, 128, 260, 128, 1.0f, 1, 0, 1, 5, 56, 78, 0, 0, 0, 0, 0},
    {260, 260, 128, 128, 260, 128, 1.0f, 1, 0, 1, 5, 56, 104, 0, 0, 0, 0, 0},
    {260, 260, 128, 128, 260, 128, 1.0f, 1, 0, 1, 5, 56, 130, 0, 0, 0, 0, 0},
    {260, 260, 128, 128, 260, 128, 1.0f, 1, 0, 1, 5, 56, 156, 0, 0, 0, 0, 0},
    {260, 260, 128, 128, 260, 128, 1.0f, 1, 0, 1, 5, 56, 182, 0, 0, 0, 0, 0},
    {260, 260, 128, 128, 260, 128, 1.0f, 1, 0, 1, 5, 56, 208, 0, 0, 0, 0, 0},
    {32, 32, 16, 32, 32, 32, 1.0f, 2, 0, 2, 5, 142, 26, 0, 0, 0, 0, 0},
    {32, 32, 16, 32, 32, 32, 1.0f, 2, 0, 2, 5, 169, 26, 0, 0, 0, 0, 0},
    {32, 32, 16, 32, 32, 32, 1.0f, 2, 0, 2, 5, 196, 26, 0, 0, 0, 0, 0},
    {260, 260, 32, 32, 260, 32, 1.0f, 0, 0, 0, 5, 56, 0, 0, 0, 0, 0, 0},
    {260, 260, 32, 32, 260, 32, 1.0f, 0, 0, 0, 5, 56, 234, 0, 0, 0, 0, 0},
    {16, 256, 32, 32, 256, 32, 1.0f, 2, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0},
    {16, 256, 32, 32, 256, 32, 1.0f, 2, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0},
    {16, 256, 32, 32, 256, 32, 1.0f, 2, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0},
    {16, 256, 32, 32, 256, 32, 1.0f, 2, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0},
    {16, 256, 32, 32, 256, 32, 1.0f, 2, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0},
    {16, 256, 32, 32, 256, 32, 1.0f, 2, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0},
    {16, 260, 32, 32, 260, 32, 1.0f, 2, 0, 2, 3, 56, 397, 0, 0, 0, 0, 0},
    {400, 400, 300, 300, 400, 300, 1.0f, 2, 0, 2, 5, 120, 90, 0, 0, 0, 0, 0},
    {16, 160, 24, 24, 160, 24, 1.0f, 0, 2, 0, 5, 450, 263, 0, 0, 0, 0, 0},
    {16, 187, 24, 24, 187, 24, 1.0f, 0, 2, 0, 5, 423, 292, 0, 0, 0, 0, 0},
    {16, 256, 24, 24, 256, 24, 1.0f, 0, 2, 0, 5, 64, 97, 0, 0, 0, 0, 0},
    {16, 256, 24, 24, 256, 24, 1.0f, 0, 2, 0, 5, 353, 113, 0, 0, 0, 0, 0},
    {16, 190, 24, 24, 190, 24, 1.0f, 2, 2, 2, 5, 111, 125, 0, 0, 0, 0, 0},
    {16, 244, 24, 24, 244, 24, 1.0f, 0, 2, 0, 5, 366, 219, 0, 0, 0, 0, 0},
    {16, 208, 24, 24, 208, 24, 1.0f, 0, 2, 0, 5, 402, 180, 0, 0, 0, 0, 0},
    {16, 189, 24, 24, 189, 24, 1.0f, 0, 2, 0, 5, 421, 152, 0, 0, 0, 0, 0},
    {16, 256, 24, 24, 256, 24, 1.0f, 0, 2, 0, 5, 67, 359, 0, 0, 0, 0, 0},
    {16, 225, 24, 24, 225, 24, 1.0f, 0, 2, 0, 5, 385, 324, 0, 0, 0, 0, 0},
    {260, 260, 16, 32, 260, 32, 1.0f, 0, 0, 0, 5, 56, 0, 0, 0, 0, 0, 0},
    {260, 260, 128, 128, 260, 128, 1.0f, 1, 0, 1, 5, 56, 0, 0, 0, 0, 0, 0},
    {260, 260, 16, 32, 260, 32, 1.0f, 0, 0, 0, 5, 56, 26, 0, 0, 0, 0, 0},
    {200, 200, 128, 128, 200, 128, 1.0f, 1, 0, 1, 5, 121, 26, 0, 0, 0, 0, 0},
    {200, 160, 128, 128, 160, 128, 1.0f, 1, 0, 1, 5, 121, 26, 0, 0, 0, 0, 0},
    {200, 200, 24, 24, 200, 24, 1.0f, 2, 0, 2, 7, 370, 300, 0, 0, 0, 0, 0},
    {200, 200, 24, 24, 200, 24, 1.0f, 2, 0, 2, 7, 70, 300, 0, 0, 0, 0, 0},
    {200, 200, 24, 24, 200, 24, 1.0f, 2, 0, 2, 7, 220, 260, 0, 0, 0, 0, 0},
    {0, 500, 46, 46, 500, 46, 1.0f, 2, 1, 2, 2, 60, 52, 0, 0, 0, 0, 0},
    {100, 100, 16, 32, 100, 32, 1.0f, 0, 0, 0, 5, 130, 178, 0, 0, 0, 0, 0},
    {100, 100, 16, 32, 100, 32, 1.0f, 0, 0, 0, 5, 130, 204, 0, 0, 0, 0, 0},
    {100, 100, 16, 32, 100, 32, 1.0f, 0, 0, 0, 5, 130, 230, 0, 0, 0, 0, 0},
    {100, 100, 16, 32, 100, 32, 1.0f, 0, 0, 0, 5, 130, 256, 0, 0, 0, 0, 0},
    {100, 100, 16, 32, 100, 32, 1.0f, 0, 0, 0, 5, 130, 282, 0, 0, 0, 0, 0},
    {100, 100, 16, 32, 100, 32, 1.0f, 0, 0, 0, 5, 401, 178, 0, 0, 0, 0, 0},
    {100, 100, 16, 32, 100, 32, 1.0f, 0, 0, 0, 5, 401, 204, 0, 0, 0, 0, 0},
    {100, 100, 16, 32, 100, 32, 1.0f, 0, 0, 0, 5, 401, 230, 0, 0, 0, 0, 0},
    {100, 100, 16, 32, 100, 32, 1.0f, 0, 0, 0, 5, 401, 256, 0, 0, 0, 0, 0},
    {100, 100, 16, 32, 100, 32, 1.0f, 0, 0, 0, 5, 401, 282, 0, 0, 0, 0, 0},
    {200, 200, 16, 32, 200, 32, 1.0f, 2, 0, 2, 5, 70, 110, 0, 0, 0, 0, 0},
    {200, 200, 16, 32, 200, 32, 1.0f, 2, 0, 2, 5, 370, 110, 0, 0, 0, 0, 0},
    {1600, 1600, 24, 24, 1600, 24, 1.0f, 0, 0, 0, 5, 50, 78, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 152, 200, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 200, 200, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 248, 200, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 296, 200, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 344, 200, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 392, 200, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 440, 200, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 488, 200, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 128, 248, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 176, 248, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 224, 248, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 272, 248, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 320, 248, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 368, 248, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 416, 248, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 464, 248, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 104, 296, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 152, 296, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 200, 296, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 248, 296, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 296, 296, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 344, 296, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 392, 296, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 440, 296, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 488, 296, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 200, 344, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 248, 344, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 296, 344, 0, 0, 0, 0, 0},
    {48, 48, 24, 24, 48, 24, 1.0f, 2, 0, 2, 7, 344, 344, 0, 0, 0, 0, 0},
    {400, 400, 24, 24, 400, 24, 1.0f, 2, 0, 2, 5, 120, 228, 0, 0, 0, 0, 0},
    {400, 400, 24, 24, 400, 24, 1.0f, 2, 0, 2, 5, 120, 254, 0, 0, 0, 0, 0},
    {400, 400, 24, 24, 400, 24, 1.0f, 2, 0, 2, 5, 120, 280, 0, 0, 0, 0, 0},
    {400, 400, 24, 24, 400, 24, 1.0f, 2, 0, 2, 5, 120, 306, 0, 0, 0, 0, 0},
    {400, 400, 24, 24, 400, 24, 1.0f, 2, 0, 2, 5, 120, 332, 0, 0, 0, 0, 0},
    {360, 360, 16, 420, 360, 420, 1.0f, 2, 1, 2, 5, 140, 60, 0, 0, 0, 0, 0},
    {560, 560, 45, 45, 560, 45, 1.0f, 3, 0, 3, 5, 40, 395, 0, 0, 0, 0, 0},
    {560, 560, 480, 480, 560, 480, 1.0f, 2, 0, 2, 5, 40, 0, 0, 0, 0, 0, 0},
    {512, 512, 25, 25, 512, 25, 1.0f, 2, 0, 2, 5, 84, 415, 0, 0, 0, 0, 0},
    {512, 512, 480, 480, 512, 480, 1.0f, 3, 0, 3, 5, 84, 0, 0, 0, 0, 0, 0},
    {48, 48, 56, 56, 48, 56, 1.0f, 2, 1, 2, 5, 32, 415, 0, 0, 0, 0, 0},
    {160, 160, 16, 256, 160, 256, 1.0f, 1, 0, 1, 5, 140, 60, 0, 0, 0, 0, 0},
    {160, 160, 16, 256, 160, 256, 1.0f, 0, 0, 0, 5, 340, 60, 0, 0, 0, 0, 0},
    {340, 340, 300, 300, 340, 300, 1.0f, 2, 0, 2, 7, 150, 60, 0, 0, 0, 0, 0},
    {240, 240, 256, 256, 240, 256, 1.0f, 2, 0, 2, 7, 360, 60, 0, 0, 0, 0, 0},
    {112, 192, 100, 100, 192, 100, 1.0f, 2, 1, 2, 5, 54, 340, 0, 0, 0, 0, 0},
    {640, 640, 100, 100, 640, 100, 1.7f, 2, 0, 2, 5, 0, 230, 0, 0, 0, 0, 0},
    {640, 640, 350, 350, 640, 350, 1.7f, 2, 0, 2, 5, 0, 100, 0, 0, 0, 0, 0},
    {180, 180, 300, 300, 180, 300, 1.0f, 1, 0, 1, 5, 120, 90, 0, 0, 0, 0, 0},
    {180, 180, 300, 300, 180, 300, 1.0f, 0, 0, 0, 5, 340, 90, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 128, 248, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 176, 248, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 224, 248, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 272, 248, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 320, 248, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 0, 248, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 0, 248, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 0, 248, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 0, 248, 0, 0, 0, 0, 0},
    {24, 24, 24, 24, 24, 24, 1.0f, 2, 0, 2, 7, 0, 248, 0, 0, 0, 0, 0},
    {16, 320, 16, 110, 320, 110, 1.0f, 0, 1, 0, 7, 250, 150, 0, 0, 0, 0, 0},
    {640, 640, 480, 480, 640, 480, 1.0f, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0},
};

FontMetrics gGameTextFontMetrics[7] = {
    {0, {14, 170}, 21, 10, 2, 0, 21, 21, {0, 0, 0, 0}},
    {0, {0, 1}, 14, 7, 1, 0, 14, 21, {0, 0, 0, 0}},
    {0, {0, 11}, 30, 15, 1, 0, 30, 22, {0, 0, 0, 0}},
    {0, {0, 6}, 32, 16, 1, 0, 32, 24, {0, 0, 0, 0}},
    {0, {0, 136}, 21, 10, 2, 0, 21, 21, {0, 0, 0, 0}},
    {0, {0, 8}, 46, 23, 1, 0, 46, 55, {0, 0, 0, 0}},
    {0, {0, 0}, 0, 0, 0, 0, 0, 0, {0, 0, 0, 0}},
};

CtrlCharEntry gGameTextCtrlCodeArgCounts[46] = {
    {0x0000F8F2, 0x00000002}, {0x0000F8F3, 0x00000000}, {0x0000F8F4, 0x00000001}, {0x0000F8F5, 0x00000001},
    {0x0000F8F6, 0x00000001}, {0x0000F8F7, 0x00000001}, {0x0000F8F8, 0x00000000}, {0x0000F8F9, 0x00000000},
    {0x0000F8FA, 0x00000000}, {0x0000F8FB, 0x00000000}, {0x0000F8FC, 0x00000000}, {0x0000F8FD, 0x00000000},
    {0x0000F8FE, 0x00000000}, {0x0000F8FF, 0x00000004}, {0x0000E000, 0x00000001}, {0x0000E018, 0x00000003},
    {0x0000E020, 0x00000001}, {0x0000F8FF, 0x00000004}, {0x0000F8FF, 0x00000004}, {0x0000F8FF, 0x00000004},
    {0x0000F8FF, 0x00000004}, {0x0000F8FF, 0x00000004}, {0x0000F8FF, 0x00000004}, {0x0000F8FF, 0x00000004},
    {0x0000F8FF, 0x00000004}, {0x0000F8FF, 0x00000004}, {0x0000F8FF, 0x00000004}, {0x0000F8FF, 0x00000004},
    {0x0000F8FF, 0x00000004}, {0x0000F8FF, 0x00000004}, {0x0000F8FF, 0x00000004}, {0x0000F8FF, 0x00000004},
    {0x0000F8FF, 0x00000004}, {0x0000F8FF, 0x00000004}, {0x0000F8FF, 0x00000004}, {0x0000F8FF, 0x00000004},
    {0x0000F8FF, 0x00000004}, {0x0000F8FF, 0x00000004}, {0x0000F8FF, 0x00000004}, {0x0000F8FF, 0x00000004},
    {0x0000F8FF, 0x00000004}, {0x0000F8FF, 0x00000004}, {0x0000F8FF, 0x00000004}, {0x0000F8FF, 0x00000004},
    {0x0000F8FF, 0x00000004}, {0x0000F8FF, 0x00000004},
};

TaskTextEntry gTaskTextTable[208] = {
    {0x0004, 0x0029, 0x00D1}, {0x0006, 0x0029, 0x04F7}, {0x0009, 0x0029, 0x017C}, {0x000B, 0x0029, 0x004B},
    {0x000C, 0x0029, 0x0285}, {0x000E, 0x0029, 0x04EA}, {0x0010, 0x0029, 0x0041}, {0x0011, 0x0029, 0x047A},
    {0x0013, 0x0029, 0x046C}, {0x0015, 0x0029, 0x01D7}, {0x0016, 0x0029, 0x0477}, {0x0031, 0x0029, 0x0205},
    {0x0037, 0x0029, 0x01B0}, {0x0038, 0x0029, 0x0075}, {0x003C, 0x0029, 0x02E5}, {0x003D, 0x0029, 0x0078},
    {0x003F, 0x0029, 0x0499}, {0x0042, 0x0029, 0x001E}, {0x0043, 0x0029, 0x000C}, {0x0048, 0x0029, 0x0027},
    {0x004B, 0x0029, 0x00A7}, {0x0056, 0x0029, 0x00AD}, {0x005A, 0x0029, 0x020F}, {0x005F, 0x0029, 0x0023},
    {0x0092, 0x0029, 0x04C3}, {0x00A6, 0x0029, 0x00E4}, {0x00A7, 0x0029, 0x001C}, {0x00AA, 0x0029, 0x00FE},
    {0x00AB, 0x0029, 0x0105}, {0x00AD, 0x0029, 0x00FF}, {0x00AE, 0x0029, 0x0121}, {0x00AF, 0x0029, 0x056A},
    {0x00B1, 0x0029, 0x00FA}, {0x00B2, 0x0029, 0x00FB}, {0x00B3, 0x0029, 0x00FC}, {0x00B8, 0x0029, 0x01AA},
    {0x00B9, 0x0029, 0x01AB}, {0x00CA, 0x0029, 0x016E}, {0x00CB, 0x0029, 0x01A4}, {0x00E6, 0x0029, 0x007A},
    {0x00F0, 0x0029, 0x0324}, {0x01F8, 0x0029, 0x0338}, {0x01FE, 0x0029, 0x035A}, {0x0203, 0x0029, 0x049C},
    {0x0205, 0x0029, 0x053E}, {0x020A, 0x0029, 0x0510}, {0x020B, 0x0029, 0x0544}, {0x0265, 0x0029, 0x0462},
    {0x0288, 0x0029, 0x0532}, {0x0289, 0x0029, 0x008E}, {0x028A, 0x0029, 0x0282}, {0x028C, 0x0029, 0x01DB},
    {0x028E, 0x0029, 0x0045}, {0x02A0, 0x0029, 0x00E3}, {0x02B4, 0x0029, 0x001F}, {0x02B9, 0x0029, 0x04E8},
    {0x02BA, 0x0029, 0x04E9}, {0x02F1, 0x0029, 0x0127}, {0x02F2, 0x0029, 0x0128}, {0x02F3, 0x0029, 0x0487},
    {0x02F4, 0x0029, 0x03C4}, {0x02F5, 0x0029, 0x03C8}, {0x4E21, 0x0029, 0x0464}, {0x4E22, 0x0029, 0x0481},
    {0x4E23, 0x0029, 0x0483}, {0x4E24, 0x0029, 0x053D}, {0x4E25, 0x0029, 0x02D8}, {0x4E26, 0x0029, 0x04FB},
    {0x4E27, 0x0029, 0x04FE}, {0x4E28, 0x0029, 0x0505}, {0x4E29, 0x0029, 0x0503}, {0x4E2A, 0x0029, 0x0052},
    {0x4E2B, 0x0029, 0x004F}, {0x4E2C, 0x0029, 0x0050}, {0x4E2D, 0x0029, 0x011B}, {0x4E2E, 0x0029, 0x0571},
    {0x4E2F, 0x0029, 0x0074}, {0x4E30, 0x0029, 0x007B}, {0x4E31, 0x0029, 0x0383}, {0x4E32, 0x0029, 0x0384},
    {0x4E34, 0x0029, 0x0515}, {0x4E35, 0x0029, 0x0549}, {0x4E36, 0x0029, 0x0148}, {0x4E37, 0x0029, 0x014A},
    {0x4E38, 0x0029, 0x033A}, {0x4E3D, 0x0029, 0x001D}, {0x4E40, 0x0029, 0x0020}, {0x4E41, 0x0029, 0x0388},
    {0x4E42, 0x0029, 0x0395}, {0x4E43, 0x0029, 0x015C}, {0x4E44, 0x0029, 0x058B}, {0x4E45, 0x0029, 0x0283},
    {0x4E46, 0x0029, 0x02AA}, {0x4E84, 0x0029, 0x0064}, {0x4E89, 0x0029, 0x0069}, {0x4E8B, 0x0029, 0x0083},
    {0x4E8C, 0x0029, 0x0490}, {0x4EAB, 0x0029, 0x008B}, {0x4EAC, 0x0029, 0x0598}, {0x4EB6, 0x0029, 0x059A},
    {0x4EE9, 0x0029, 0x00C9}, {0x4EEA, 0x0029, 0x00CA}, {0x4EEB, 0x0029, 0x00CB}, {0x4EF2, 0x0029, 0x00D2},
    {0x4EF5, 0x0029, 0x00D5}, {0x4F0A, 0x0029, 0x00EA}, {0x4F35, 0x0029, 0x0115}, {0x4F38, 0x0029, 0x0118},
    {0x4F3E, 0x0029, 0x011E}, {0x501A, 0x0029, 0x01FA}, {0x501C, 0x0029, 0x01FC}, {0x5078, 0x0029, 0x0080},
    {0x509B, 0x0029, 0x0271}, {0x50B5, 0x0029, 0x0493}, {0x50D7, 0x0029, 0x006D}, {0x50D8, 0x0029, 0x0180},
    {0x50DC, 0x0029, 0x059C}, {0x517F, 0x0029, 0x035F}, {0x529F, 0x0029, 0x047F}, {0x52B2, 0x0029, 0x0492},
    {0x52BD, 0x0029, 0x049D}, {0x5368, 0x0029, 0x0548}, {0xFFFF, 0x0000, 0x0000}, {0x0000, 0x0000, 0xFFFF},
    {0xFFFF, 0x0000, 0x0000}, {0x0006, 0x0006, 0x0006}, {0x0006, 0x0006, 0x0006}, {0xFFFF, 0xFFFF, 0x0002},
    {0x0002, 0x0005, 0x0005}, {0x0005, 0x0005, 0x0006}, {0xFFFF, 0xFFFF, 0x0006}, {0x0006, 0x0006, 0x0006},
    {0x0006, 0x0006, 0xFFFF}, {0x0005, 0x0005, 0x0005}, {0x0006, 0x0007, 0xFFFF}, {0x0007, 0x0007, 0x0007},
    {0x0007, 0x0007, 0x0007}, {0x0007, 0x0007, 0x0007}, {0x0007, 0xFFFF, 0xFFFF}, {0xFFFF, 0xFFFF, 0x0007},
    {0x0006, 0x0009, 0x0009}, {0x000A, 0x000A, 0x000A}, {0x000A, 0x000A, 0xFFFF}, {0x0009, 0x0009, 0x0009},
    {0x0009, 0x0009, 0x0009}, {0x0006, 0xFFFF, 0x0000}, {0x0000, 0x000C, 0xFFFF}, {0x000C, 0xFFFF, 0xFFFF},
    {0x000C, 0x0006, 0x000B}, {0xFFFF, 0x000B, 0x000B}, {0x000B, 0x000B, 0x000B}, {0x000B, 0xFFFF, 0xFFFF},
    {0xFFFF, 0xFFFF, 0x000B}, {0xFFFF, 0x000C, 0x0008}, {0x0008, 0x0008, 0x0008}, {0xFFFF, 0xFFFF, 0xFFFF},
    {0x0006, 0x0004, 0x0004}, {0x0004, 0xFFFF, 0x0004}, {0xFFFF, 0xFFFF, 0xFFFF}, {0x0004, 0xFFFF, 0x0000},
    {0x0006, 0x0006, 0x0003}, {0xFFFF, 0x0003, 0x0003}, {0x0003, 0x0003, 0xFFFF}, {0xFFFF, 0xFFFF, 0x0003},
    {0x000A, 0x000A, 0x000A}, {0x000A, 0xFFFF, 0x0006}, {0xFFFF, 0x0006, 0x0005}, {0x0005, 0x0005, 0xFFFF},
    {0x0000, 0xFFFF, 0x0006}, {0x0001, 0xFFFF, 0xFFFF}, {0x0001, 0x0001, 0x0001}, {0x0001, 0xFFFF, 0x0001},
    {0x0001, 0xFFFF, 0xFFFF}, {0xFFFF, 0xFFFF, 0xFFFF}, {0xFFFF, 0x0001, 0x000C}, {0x0008, 0xFFFF, 0x0008},
    {0xFFFF, 0xFFFF, 0x0006}, {0x0006, 0x0003, 0x0003}, {0xFFFF, 0x0003, 0xFFFF}, {0xFFFF, 0xFFFF, 0x0003},
    {0x0003, 0x0000, 0x0000}, {0x0000, 0x0000, 0x0000}, {0xFFFF, 0xFFFF, 0xFFFF}, {0xFFFF, 0xFFFF, 0xFFFF},
    {0x0000, 0x0000, 0xFFFF}, {0xFFFF, 0xFFFF, 0xFFFF}, {0xFFFF, 0xFFFF, 0xFFFF}, {0xFFFF, 0xFFFF, 0xFFFF},
    {0xFFFF, 0xFFFF, 0xFFFF}, {0xFFFF, 0xFFFF, 0xFFFF}, {0xFFFF, 0xFFFF, 0xFFFF}, {0xFFFF, 0xFFFF, 0xFFFF},
    {0xFFFF, 0xFFFF, 0xFFFF}, {0xFFFF, 0xFFFF, 0xFFFF}, {0xFFFF, 0xFFFF, 0xFFFF}, {0xFFFF, 0xFFFF, 0xFFFF},
    {0xFFFF, 0xFFFF, 0xFFFF}, {0xFFFF, 0xFFFF, 0xFFFF}, {0xFFFF, 0xFFFF, 0xFFFF}, {0xFFFF, 0xFFFF, 0xFFFF},
    {0xFFFF, 0xFFFF, 0xFFFF}, {0xFFFF, 0xFFFF, 0xFFFF}, {0xFFFF, 0xFFFF, 0xFFFF}, {0xFFFF, 0xFFFF, 0xFFFF},
    {0xFFFF, 0xFFFF, 0xFFFF}, {0xFFFF, 0xFFFF, 0xFFFF}, {0xFFFF, 0xFFFF, 0xFFFF}, {0x0000, 0x0000, 0x0000},
};
