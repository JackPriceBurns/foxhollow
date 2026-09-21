#include "main/gametext.h"
#include "main/gametext_shared_internal.h"
#include "PowerPC_EABI_Support/Msl/MSL_C/MSL_Common/printf.h"
#include "main/frame_timing.h"
#include "main/textrender_internal.h"
#include "main/rcp_dolphin.h"
#include "main/dll/dll_0015_save_settings.h"
#include "main/lightmap.h"
#include "main/textrender.h"

void* gameTextGetPhrase(int textId, int phraseIndex) {
    GameTextDef* entry;

    if (gameTextFonts->status != 2) {
        gGameTextBufferIndex = gGameTextBufferIndex + 1;
        if (gGameTextBufferIndex >= 8) {
            gGameTextBufferIndex = 0;
        }
        gGameTextLastEntry = gGameTextRuntime.fallbackDefs + gGameTextBufferIndex;
        gCurTextBuffer = *gGameTextLastEntry->strings;
        gGameTextLastEntry->identifier = 0xffff;
        gGameTextFallbackBuf = gGameTextRuntime.fadeTimers + gGameTextBufferIndex;
        switch (gameTextFonts->status) {
        case 0:
            sprintf(gCurTextBuffer, sGameTextParserMessages.uninitialised);
            break;
        case 1:
            sprintf(gCurTextBuffer, sGameTextParserMessages.loading);
            break;
        case 3:
            sprintf(gCurTextBuffer, sGameTextParserMessages.fileEmpty);
            break;
        case 4:
            sprintf(gCurTextBuffer, sGameTextParserMessages.noFile);
            break;
        }
        return gGameTextLastEntry;
    }

    entry = gameTextGet(textId);
    if (entry->identifier == 0xffff) {
        gGameTextBufferIndex = gGameTextBufferIndex + 1;
        if (gGameTextBufferIndex >= 8) {
            gGameTextBufferIndex = 0;
        }
        gGameTextLastEntry = gGameTextRuntime.fallbackDefs + gGameTextBufferIndex;
        gCurTextBuffer = *gGameTextLastEntry->strings;
        gGameTextLastEntry->identifier = 0xffff;
        gGameTextFallbackBuf = gGameTextRuntime.fadeTimers + gGameTextBufferIndex;
        sprintf(gCurTextBuffer, sGameTextParserMessages.notInFile, textId, sMapDirectoryNameTable[curGameTextDir]);
        return gGameTextLastEntry;
    }

    if (phraseIndex >= entry->count) {
        gGameTextBufferIndex = gGameTextBufferIndex + 1;
        if (gGameTextBufferIndex >= 8) {
            gGameTextBufferIndex = 0;
        }
        gGameTextLastEntry = gGameTextRuntime.fallbackDefs + gGameTextBufferIndex;
        gCurTextBuffer = *gGameTextLastEntry->strings;
        gGameTextLastEntry->identifier = 0xffff;
        gGameTextFallbackBuf = gGameTextRuntime.fadeTimers + gGameTextBufferIndex;
        sprintf(gCurTextBuffer, sGameTextParserMessages.noPhrase, textId, phraseIndex);
        return gGameTextLastEntry;
    }

    return entry->strings[phraseIndex];
}

void* gameTextGetStr(int textId) {
    GameTextDef* textEntry;

    if (gameTextFonts->status != 2) {
        gGameTextBufferIndex = gGameTextBufferIndex + 1;
        if (gGameTextBufferIndex >= 8) {
            gGameTextBufferIndex = 0;
        }
        gGameTextLastEntry = gGameTextRuntime.fallbackDefs + gGameTextBufferIndex;
        gCurTextBuffer = *gGameTextLastEntry->strings;
        gGameTextLastEntry->identifier = 0xffff;
        gGameTextFallbackBuf = gGameTextRuntime.fadeTimers + gGameTextBufferIndex;
        switch (gameTextFonts->status) {
        case 0:
            sprintf(gCurTextBuffer, sGameTextParserMessages.uninitialised);
            break;
        case 1:
            sprintf(gCurTextBuffer, sGameTextParserMessages.loading);
            break;
        case 3:
            sprintf(gCurTextBuffer, sGameTextParserMessages.fileEmpty);
            break;
        case 4:
            sprintf(gCurTextBuffer, sGameTextParserMessages.noFile);
            break;
        }
        return gGameTextLastEntry;
    }
    textEntry = gameTextGet(textId);
    return *textEntry->strings;
}

void* gameTextGet(int textId) {
    TextFont* fonts;
    GameTextDef* entry;
    int count;
    int slotIndex;
    GameTextDef* cachedEntry;
    f32 zero;
    f32* cachedAlpha;

    fonts = gameTextFonts;

    if (fonts->status != 2) {
        gGameTextBufferIndex++;
        if (gGameTextBufferIndex >= 8) {
            gGameTextBufferIndex = 0;
        }
        gGameTextLastEntry = gGameTextRuntime.fallbackDefs + gGameTextBufferIndex;
        gCurTextBuffer = *gGameTextLastEntry->strings;
        gGameTextLastEntry->identifier = 0xffff;
        gGameTextFallbackBuf = gGameTextRuntime.fadeTimers + gGameTextBufferIndex;

        switch (gameTextFonts->status) {
        case 0:
            sprintf(gCurTextBuffer, sGameTextParserMessages.uninitialised);
            break;
        case 1:
            sprintf(gCurTextBuffer, sGameTextParserMessages.loading);
            break;
        case 3:
            sprintf(gCurTextBuffer, sGameTextParserMessages.fileEmpty);
            break;
        case 4:
            sprintf(gCurTextBuffer, sGameTextParserMessages.noFile);
            break;
        }
        return gGameTextLastEntry;
    }

    entry = fonts->entries;
    count = fonts->entryCount;
    while (count != 0) {
        if (entry->identifier == textId) {
            return entry;
        }
        entry++;
        count--;
    }

    slotIndex = 8;
    cachedEntry = gGameTextRuntime.fallbackDefs + 8;
    while (cachedEntry--, slotIndex-- != 0) {
        if (cachedEntry->identifier == textId) {
            zero = lbl_803DE704;
            gGameTextRuntime.fadeElapsed[slotIndex] = zero;
            cachedAlpha = &gGameTextRuntime.fadeTimers[slotIndex];
            if (zero < gGameTextFadeLimit) {
                f32 av = zero + timeDelta;
                *cachedAlpha = av;
                if (av >= gGameTextFadeLimit) {
                    sprintf(*cachedEntry->strings, sGameTextParserMessages.notInFile, textId,
                            sMapDirectoryNameTable[curGameTextDir]);
                }
            }
            return cachedEntry;
        }
    }

    gGameTextBufferIndex++;
    if (gGameTextBufferIndex >= 8) {
        gGameTextBufferIndex = 0;
    }
    gGameTextLastEntry = gGameTextRuntime.fallbackDefs + gGameTextBufferIndex;
    gCurTextBuffer = *gGameTextLastEntry->strings;
    gGameTextLastEntry->identifier = 0xffff;
    gGameTextFallbackBuf = gGameTextRuntime.fadeTimers + gGameTextBufferIndex;
    sprintf(gCurTextBuffer, sGameTextBlankFormat, textId, sMapDirectoryNameTable[curGameTextDir]);
    gGameTextLastEntry->identifier = textId;
    *gGameTextFallbackBuf = lbl_803DE704;
    return gGameTextLastEntry;
}

void gameTextResetCursor(int flags) {
    if (flags & 1) {
        gGameTextCursorX = 0;
        gGameTextCursorY = 0;
    }
    if (flags & 2) {
        int* p = &gGameTextCommandSlots[gGameTextCommandCount++].opcode;
        *p = 0xb;
    }
}

void gameTextSetCursor(u16 x, u16 y, int flags) {
    if (flags & 1) {
        gGameTextCursorX = x;
        gGameTextCursorY = y;
    }
    if (flags & 2) {
        int i = gGameTextCommandCount;
        GameTextSlot* cmd;
        gGameTextCommandCount = i + 1;
        cmd = &gGameTextCommandSlots[i];
        cmd->opcode = 0xa;
        cmd->arg0 = x;
        cmd->arg1 = y;
    }
}

void gameTextSetWindowStrPos(int idx, int x, int y) {
    if (gameTextDrawFunc != NULL) {
        gTextBoxes[idx].cursorX = x;
        gTextBoxes[idx].cursorY = y;
    } else {
        int i = gGameTextCommandCount;
        GameTextSlot* cmd;
        gGameTextCommandCount = i + 1;
        cmd = &gGameTextCommandSlots[i];
        cmd->opcode = 4;
        cmd->arg0 = idx;
        cmd->arg1 = x;
        cmd->arg2 = y;
    }
}

void gameTextSetColor(u8 r, u8 g, u8 b, u8 a) {
    if (gameTextDrawFunc != NULL) {
        gGameTextColorR = r;
        gGameTextColorG = g;
        gGameTextColorB = b;
        gGameTextColorA = a;
    } else {
        int i = gGameTextCommandCount;
        GameTextSlot* cmd;
        gGameTextCommandCount = i + 1;
        cmd = &gGameTextCommandSlots[i];
        cmd->opcode = 3;
        cmd->arg0 = r;
        cmd->arg1 = g;
        cmd->arg2 = b;
        cmd->arg3 = a;
    }
}
