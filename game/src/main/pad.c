#include <stdio.h>
#include <stdlib.h>
#include "global.h"
#include "main/fileio.h"
#include "main/frame_timing.h"
#include "main/gameloop.h"
#include "main/pad.h"

#include "dolphin/pad.h"
#include "string.h"

static FILE* gFhInputRecordFile;
static FILE* gFhInputReplayFile;
static u32 gFhInputTraceFrame;
static int gFhInputTraceInitialized;
static int gFhInputReplayEnded;

static const u8 gFhInputTraceHeader[8] = {'F', 'H', 'P', 'A', 'D', 1, sizeof(PADStatus), 4};

static void fhInputTraceInit(void) {
    const char* recordPath;
    const char* replayPath;

    if (gFhInputTraceInitialized != 0) {
        return;
    }
    gFhInputTraceInitialized = 1;
    recordPath = getenv("FOXHOLLOW_INPUT_RECORD");
    replayPath = getenv("FOXHOLLOW_INPUT_REPLAY");
    if (recordPath != NULL && recordPath[0] != '\0' && replayPath != NULL && replayPath[0] != '\0') {
        fprintf(stderr, "[FH] INPUT trace disabled: record and replay cannot be enabled together\n");
        return;
    }
    if (recordPath != NULL && recordPath[0] != '\0') {
        gFhInputRecordFile = fopen(recordPath, "wb");
        if (gFhInputRecordFile == NULL) {
            fprintf(stderr, "[FH] INPUT record could not open %s\n", recordPath);
            return;
        }
        setvbuf(gFhInputRecordFile, NULL, _IONBF, 0);
        if (fwrite(gFhInputTraceHeader, sizeof(gFhInputTraceHeader), 1, gFhInputRecordFile) != 1) {
            fprintf(stderr, "[FH] INPUT record could not write %s\n", recordPath);
            fclose(gFhInputRecordFile);
            gFhInputRecordFile = NULL;
            return;
        }
        fprintf(stderr, "[FH] INPUT recording to %s\n", recordPath);
    } else if (replayPath != NULL && replayPath[0] != '\0') {
        u8 header[sizeof(gFhInputTraceHeader)];

        gFhInputReplayFile = fopen(replayPath, "rb");
        if (gFhInputReplayFile == NULL) {
            fprintf(stderr, "[FH] INPUT replay could not open %s\n", replayPath);
            return;
        }
        if (fread(header, sizeof(header), 1, gFhInputReplayFile) != 1 ||
            memcmp(header, gFhInputTraceHeader, sizeof(header)) != 0) {
            fprintf(stderr, "[FH] INPUT replay rejected invalid trace %s\n", replayPath);
            fclose(gFhInputReplayFile);
            gFhInputReplayFile = NULL;
            return;
        }
        fprintf(stderr, "[FH] INPUT replaying %s\n", replayPath);
    }
}

static int fhInputTraceReplay(PADStatus* statuses) {
    int i;

    if (gFhInputReplayFile == NULL) {
        return 0;
    }
    if (fread(statuses, sizeof(PADStatus), 4, gFhInputReplayFile) == 4) {
        gFhInputTraceFrame++;
        return 1;
    }
    memset(statuses, 0, sizeof(PADStatus) * 4);
    statuses[0].err = PAD_ERR_NONE;
    for (i = 1; i < 4; i++) {
        statuses[i].err = PAD_ERR_NO_CONTROLLER;
    }
    if (gFhInputReplayEnded == 0) {
        fprintf(stderr, "[FH] INPUT replay ended after %u frames\n", gFhInputTraceFrame);
        gFhInputReplayEnded = 1;
    }
    return 1;
}

static void fhInputTraceRecord(const PADStatus* statuses) {
    if (gFhInputRecordFile == NULL) {
        return;
    }
    if (fwrite(statuses, sizeof(PADStatus), 4, gFhInputRecordFile) != 4) {
        fprintf(stderr, "[FH] INPUT recording stopped after %u frames\n", gFhInputTraceFrame);
        fclose(gFhInputRecordFile);
        gFhInputRecordFile = NULL;
        return;
    }
    gFhInputTraceFrame++;
}

u8 gPadMenuStickRepeatDelay = 5;

/* Synthesized C-stick-as-direction bits OR'd into the extended button word. */
#define PAD_BUTTON_CSTICK_UP    0x10000
#define PAD_BUTTON_CSTICK_DOWN  0x20000
#define PAD_BUTTON_CSTICK_LEFT  0x40000
#define PAD_BUTTON_CSTICK_RIGHT 0x80000

/* Synthesized digital-trigger bits (analog trigger past threshold). */
#define PAD_ANALOG_TRIGGER_R 0x20
#define PAD_ANALOG_TRIGGER_L 0x40

extern PADStatus gPadStatuses[2][4];

u8 gPadStatusBufferIndex;
s8 gPadLastStickX[4];
s8 gPadLastStickY[4];
s8 gPadMenuStickXHoldTimer[4];
s8 gPadMenuStickYHoldTimer[4];
s8 gPadMenuStickXSign[4];
s8 gPadMenuStickYSign[4];
u16 gPadTriggersPressed[4];
u16 gPadTriggersReleased[4];
u16 gPadTriggers[4];
u16 gPadPrevTriggers[4];
u32 gPadResetMask;
f32 gRumbleTimer;
u8 rumbleEnabled;
u8 joypadDisabled;

void stopRumble2(void) {
    if (rumbleEnabled != 0) {
        PADControlMotor(0, PAD_MOTOR_STOP_HARD);
        gRumbleTimer = 0.0f;
    }
}

void stopRumble(void) {
    if (rumbleEnabled != 0) {
        PADControlMotor(0, PAD_MOTOR_STOP);
        gRumbleTimer = 0.0f;
    }
}

void doRumble(f32 duration) {
    if (rumbleEnabled != 0 && getGameState() == 1) {
        f32 rumbleTimer;

        PADControlMotor(0, PAD_MOTOR_RUMBLE);
        rumbleTimer = gRumbleTimer;
        gRumbleTimer = rumbleTimer > duration ? rumbleTimer : duration;
    }
}

void setJoypadDisabled(void) {
    joypadDisabled = 1;
}

void padSetStickRepeatDelay(int delay) {
    gPadMenuStickRepeatDelay = delay;
}

u32 buttonGetDisabled(int port) {
    return ~gPadButtonMask[port];
}

void buttonDisable(int port, u32 mask) {
    gPadButtonMask[port] &= ~mask;
}

void padClearAnalogInputY(int port) {
    gPadMenuStickYSign[port] = 0;
}

void padClearAnalogInputX(int port) {
    gPadMenuStickXSign[port] = 0;
}

void padGetAnalogInput(int port, s8* x, s8* y) {
    if (joypadDisabled != 0 || port > 0 || gDvdErrorPauseActive != 0) {
        *x = 0;
        *y = 0;
        return;
    }
    *x = gPadMenuStickXSign[port];
    *y = gPadMenuStickYSign[port];
}

s8 padGetCY(int port) {
    PADStatus* statuses;

    if (port > 0) {
        return 0;
    }
    if (joypadDisabled != 0 || gDvdErrorPauseActive != 0) {
        return 0;
    }
    statuses = gPadStatuses[0];
    return statuses[gPadStatusBufferIndex * 4 + port].substickY;
}

s8 padGetCX(int port) {
    PADStatus* statuses;

    if (port > 0) {
        return 0;
    }
    if (joypadDisabled != 0 || gDvdErrorPauseActive != 0) {
        return 0;
    }
    statuses = gPadStatuses[0];
    return statuses[gPadStatusBufferIndex * 4 + port].substickX;
}

s8 padGetStickY(int port) {
    PADStatus* statuses;

    if (port > 0) {
        return 0;
    }
    if (joypadDisabled != 0 || gDvdErrorPauseActive != 0) {
        return 0;
    }
    statuses = gPadStatuses[0];
    return statuses[gPadStatusBufferIndex * 4 + port].stickY;
}

s8 padGetStickX(int port) {
    PADStatus* statuses;

    if (port > 0) {
        return 0;
    }
    if (joypadDisabled != 0 || gDvdErrorPauseActive != 0) {
        return 0;
    }
    statuses = gPadStatuses[0];
    return statuses[gPadStatusBufferIndex * 4 + port].stickX;
}

u8 padGetLTrigger(int port) {
    PADStatus* statuses;

    if (joypadDisabled != 0 || gDvdErrorPauseActive != 0) {
        return 0;
    }
    statuses = gPadStatuses[0];
    return statuses[gPadStatusBufferIndex * 4 + port].triggerLeft;
}

u8 padGetRTrigger(int port) {
    PADStatus* statuses;

    if (joypadDisabled != 0 || gDvdErrorPauseActive != 0) {
        return 0;
    }
    statuses = gPadStatuses[0];
    return statuses[gPadStatusBufferIndex * 4 + port].triggerRight;
}

u16 padGetTriggersPressed(int port) {
    if (port > 0) {
        port = 0;
    }
    if (joypadDisabled != 0 || gDvdErrorPauseActive != 0) {
        return 0;
    }
    return gPadTriggersPressed[port];
}

u16 padGetTriggers(int port) {
    if (port > 0) {
        port = 0;
    }
    if (joypadDisabled != 0 || gDvdErrorPauseActive != 0) {
        return 0;
    }
    return gPadTriggers[port];
}

u32 getButtonsJustPressedIfNotBusy(int port) {
    if (port > 0) {
        return 0;
    }
    if (gDvdErrorPauseActive != 0) {
        return 0;
    }
    if (joypadDisabled != 0) {
        return -1;
    }
    return gPadButtonsReleased[port] & gPadButtonMask[port];
}

u32 getButtonsJustPressed(int port) {
    if (port > 0) {
        return 0;
    }
    if (joypadDisabled != 0 || gDvdErrorPauseActive != 0) {
        return 0;
    }
    return gPadButtonsJustPressed[port] & gPadButtonMask[port];
}

u32 getNewInputs(int port) {
    if (port > 0) {
        return 0;
    }
    return gPadButtonsHeld[port];
}

u32 getButtonsHeld(int port) {
    if (port > 0) {
        return 0;
    }
    if (joypadDisabled != 0 || gDvdErrorPauseActive != 0) {
        return 0;
    }
    return gPadButtonsHeld[port] & gPadButtonMask[port];
}

void doNothing_endOfFrame(void) {
}
void padUpdate(void) {
    PADStatus* currentStatus;
    s8* prevStickY;
    s8* prevStickX;
    s8* repeatY;
    s8* repeatX;
    s8* analogY;
    s8* analogX;
    u32* previousButtons;
    u32* currentButtons;
    u32* releasedButtons;
    u32* pressedButtons;
    u16* prevTriggers;
    u16* triggers;
    u16* triggersReleased;
    u16* triggersPressed;
    const PADStatus* prevPad;
    PADStatus* readPad;
    s32 i;
    PADStatus* statuses;
    PADStatus* secondStatus;
    int sx;
    int sy;
    u8 useprev;

    prevPad = gPadStatuses[gPadStatusBufferIndex];
    gPadStatusBufferIndex ^= 1;
    readPad = gPadStatuses[gPadStatusBufferIndex];
    fhInputTraceInit();
    if (PADRead(readPad) == PAD_ERR_TRANSFER && gFhInputReplayFile == NULL) {
        return;
    }
    fhInputTraceReplay(readPad);
    PADClamp(readPad);
    fhInputTraceRecord(readPad);
    if (rumbleEnabled != 0) {
        if (gRumbleTimer > 0.0f) {
            gRumbleTimer = gRumbleTimer - timeDelta;
            if (gRumbleTimer <= 0.0f) {
                if (rumbleEnabled != 0) {
                    PADControlMotor(0, PAD_MOTOR_STOP);
                    gRumbleTimer = 0.0f;
                }
            }
        }
    }
    useprev = 0;
    joypadDisabled = 0;

    i = 0;
    currentStatus = readPad;
    prevStickY = gPadLastStickY;
    prevStickX = gPadLastStickX;
    repeatY = gPadMenuStickYHoldTimer;
    repeatX = gPadMenuStickXHoldTimer;
    analogY = gPadMenuStickYSign;
    analogX = gPadMenuStickXSign;
    previousButtons = gPadButtonsPrevious;
    currentButtons = gPadButtonsHeld;
    releasedButtons = gPadButtonsReleased;
    pressedButtons = gPadButtonsJustPressed;
    prevTriggers = gPadPrevTriggers;
    triggers = gPadTriggers;
    triggersReleased = gPadTriggersReleased;
    triggersPressed = gPadTriggersPressed;
    statuses = gPadStatuses[0];
    secondStatus = gPadStatuses[1];

    for (; i < 4; i++) {
        if (currentStatus->err == PAD_ERR_NO_CONTROLLER) {
            *prevStickY = 0;
            *prevStickX = 0;
            *repeatY = 0;
            *repeatX = 0;
            *analogY = 0;
            *analogX = 0;
            *previousButtons = 0;
            *currentButtons = 0;
            *releasedButtons = 0;
            *pressedButtons = 0;
            *prevTriggers = 0;
            *triggers = 0;
            *triggersReleased = 0;
            *triggersPressed = 0;
            memset(statuses, 0, sizeof(PADStatus));
            memset(secondStatus, 0, sizeof(PADStatus));
            gPadResetMask |= PAD_CHAN0_BIT >> i;
            currentStatus->err = PAD_ERR_NO_CONTROLLER;
        } else if ((u8)(currentStatus->err + 3) <= 1 || gPadReadReady == 0) {
            memcpy(currentStatus, prevPad, sizeof(PADStatus));
            useprev = 1;
        } else {
            *currentButtons = currentStatus->button;
            if (currentStatus->substickY < -40) {
                *currentButtons |= (u64)PAD_BUTTON_CSTICK_DOWN;
            }
            if (currentStatus->substickY > 40) {
                *currentButtons |= (u64)PAD_BUTTON_CSTICK_UP;
            }
            if (currentStatus->substickX < -40) {
                *currentButtons |= (u64)PAD_BUTTON_CSTICK_LEFT;
            }
            if (currentStatus->substickX > 40) {
                *currentButtons |= (u64)PAD_BUTTON_CSTICK_RIGHT;
            }
            *pressedButtons = *currentButtons & (*currentButtons ^ *previousButtons);
            *releasedButtons = *previousButtons & (*currentButtons ^ *previousButtons);
            *previousButtons = *currentButtons;

            *triggers = 0;
            if (currentStatus->triggerRight > 10) {
                *triggers |= PAD_ANALOG_TRIGGER_R;
            }
            if (currentStatus->triggerLeft > 10) {
                *triggers |= PAD_ANALOG_TRIGGER_L;
            }
            *triggersPressed = *triggers & (*triggers ^ *prevTriggers);
            *triggersReleased = *prevTriggers & (*triggers ^ *prevTriggers);
            *prevTriggers = *triggers;

            sx = currentStatus->stickX;
            sy = currentStatus->stickY;
            *analogX = 0;
            *analogY = 0;
            if (sx < -35 && *prevStickX >= -35) {
                *analogX = -1;
                *repeatX = 0;
            }
            if (sx > 35 && *prevStickX <= 35) {
                *analogX = 1;
                *repeatX = 0;
            }
            if (sy < -35 && *prevStickY >= -35) {
                *analogY = -1;
                *repeatY = 0;
            }
            if (sy > 35 && *prevStickY <= 35) {
                *analogY = 1;
                *repeatY = 0;
            }
            *prevStickY = sy;
            sy = *prevStickY;
            if (sy < -35) {
                (*repeatY)++;
            } else if (sy > 35) {
                (*repeatY)++;
            } else {
                *repeatY = 0;
            }
            if (*repeatY > gPadMenuStickRepeatDelay) {
                *prevStickY = 0;
                *repeatY = 0;
            }
            *prevStickX = sx;
            sx = *prevStickX;
            if (sx < -35) {
                (*repeatX)++;
            } else if (sx > 35) {
                (*repeatX)++;
            } else {
                *repeatX = 0;
            }
            if (*repeatX > gPadMenuStickRepeatDelay) {
                *prevStickX = 0;
                *repeatX = 0;
            }
            gPadButtonMask[i] = -1;
        }

        currentStatus++;
        prevStickY++;
        prevStickX++;
        repeatY++;
        repeatX++;
        analogY++;
        analogX++;
        previousButtons++;
        currentButtons++;
        releasedButtons++;
        pressedButtons++;
        prevTriggers++;
        triggers++;
        triggersReleased++;
        triggersPressed++;
        statuses++;
        secondStatus++;
        prevPad++;
    }

    if (gPadResetMask != 0) {
        if (PADReset(gPadResetMask) != 0) {
            gPadResetMask = 0;
        }
    }
    if (useprev != 0) {
        gPadStatusBufferIndex ^= 1;
    }
    gPadReadReady = 0;
}

void setRumbleEnabled(u8 enabled) {
    rumbleEnabled = enabled;
}

enum {
    HOST_KEY_A = 4,
    HOST_KEY_D = 7,
    HOST_KEY_E = 8,
    HOST_KEY_I = 12,
    HOST_KEY_J = 13,
    HOST_KEY_K = 14,
    HOST_KEY_L = 15,
    HOST_KEY_Q = 20,
    HOST_KEY_R = 21,
    HOST_KEY_S = 22,
    HOST_KEY_W = 26,
    HOST_KEY_RETURN = 40,
    HOST_KEY_SPACE = 44,
    HOST_KEY_RIGHT = 79,
    HOST_KEY_LEFT = 80,
    HOST_KEY_DOWN = 81,
    HOST_KEY_UP = 82,
    HOST_KEY_LEFT_CONTROL = 224,
    HOST_KEY_LEFT_SHIFT = 225,
    HOST_KEY_LEFT_ALT = 226,
};

static void initKeyboardControls(void) {
    u32 bindingCount;
    PADKeyButtonBinding buttonBindings[PAD_BUTTON_COUNT] = {
        {HOST_KEY_SPACE, PAD_BUTTON_A},
        {HOST_KEY_LEFT_SHIFT, PAD_BUTTON_B},
        {HOST_KEY_E, PAD_BUTTON_X},
        {HOST_KEY_R, PAD_BUTTON_Y},
        {HOST_KEY_RETURN, PAD_BUTTON_START},
        {HOST_KEY_Q, PAD_TRIGGER_Z},
        {HOST_KEY_LEFT_CONTROL, PAD_TRIGGER_L},
        {HOST_KEY_LEFT_ALT, PAD_TRIGGER_R},
        {HOST_KEY_UP, PAD_BUTTON_UP},
        {HOST_KEY_DOWN, PAD_BUTTON_DOWN},
        {HOST_KEY_LEFT, PAD_BUTTON_LEFT},
        {HOST_KEY_RIGHT, PAD_BUTTON_RIGHT},
    };
    PADKeyAxisBinding axisBindings[PAD_AXIS_COUNT] = {
        {HOST_KEY_D, PAD_AXIS_LEFT_X_POS, 0},           {HOST_KEY_A, PAD_AXIS_LEFT_X_NEG, 0},
        {HOST_KEY_W, PAD_AXIS_LEFT_Y_POS, 0},           {HOST_KEY_S, PAD_AXIS_LEFT_Y_NEG, 0},
        {HOST_KEY_L, PAD_AXIS_RIGHT_X_POS, 0},          {HOST_KEY_J, PAD_AXIS_RIGHT_X_NEG, 0},
        {HOST_KEY_I, PAD_AXIS_RIGHT_Y_POS, 0},          {HOST_KEY_K, PAD_AXIS_RIGHT_Y_NEG, 0},
        {HOST_KEY_LEFT_CONTROL, PAD_AXIS_TRIGGER_L, 0}, {HOST_KEY_LEFT_ALT, PAD_AXIS_TRIGGER_R, 0},
    };

    if (PADGetKeyButtonBindings(PAD_CHAN0, &bindingCount) == NULL) {
        PADSetKeyButtonBindings(PAD_CHAN0, buttonBindings);
        PADSetKeyAxisBindings(PAD_CHAN0, axisBindings);
        PADSetKeyboardActive(PAD_CHAN0, TRUE);
    }
}

int initControllers(void) {
    s8* prevStickY;
    s8* prevStickX;
    s8* repeatY;
    s8* repeatX;
    s8* analogY;
    s8* analogX;
    u32* previousButtons;
    u32* currentButtons;
    u32* buttonsReleased;
    u32* buttonsPressed;
    u16* prevTriggers;
    u16* triggers;
    u16* triggersReleased;
    u16* triggersPressed;
    PADStatus* statuses;
    PADStatus* secondStatus;
    s32 i;

    gPadResetMask = 0xF0000000;
    PADInit();
    initKeyboardControls();
    PADRecalibrate(gPadResetMask);
    if (PADReset(gPadResetMask) != 0) {
        gPadResetMask = 0;
    }

    i = 0;
    prevStickY = gPadLastStickY;
    prevStickX = gPadLastStickX;
    repeatY = gPadMenuStickYHoldTimer;
    repeatX = gPadMenuStickXHoldTimer;
    analogY = gPadMenuStickYSign;
    analogX = gPadMenuStickXSign;
    previousButtons = gPadButtonsPrevious;
    currentButtons = gPadButtonsHeld;
    buttonsReleased = gPadButtonsReleased;
    buttonsPressed = gPadButtonsJustPressed;
    prevTriggers = gPadPrevTriggers;
    triggers = gPadTriggers;
    triggersReleased = gPadTriggersReleased;
    triggersPressed = gPadTriggersPressed;
    statuses = gPadStatuses[0];
    secondStatus = gPadStatuses[1];

    for (; i < 4; i++) {
        *prevStickY = 0;
        *prevStickX = 0;
        *repeatY = 0;
        *repeatX = 0;
        *analogY = 0;
        *analogX = 0;
        *previousButtons = 0;
        *currentButtons = 0;
        *buttonsReleased = 0;
        *buttonsPressed = 0;
        *prevTriggers = 0;
        *triggers = 0;
        *triggersReleased = 0;
        *triggersPressed = 0;
        memset(statuses, 0, sizeof(PADStatus));
        memset(secondStatus, 0, sizeof(PADStatus));

        prevStickY++;
        prevStickX++;
        repeatY++;
        repeatX++;
        analogY++;
        analogX++;
        previousButtons++;
        currentButtons++;
        buttonsReleased++;
        buttonsPressed++;
        prevTriggers++;
        triggers++;
        triggersReleased++;
        triggersPressed++;
        statuses++;
        secondStatus++;
    }

    gPadStatusBufferIndex = 0;
    rumbleEnabled = 1;
    PADControlMotor(0, PAD_MOTOR_STOP_HARD);
    gRumbleTimer = 0.0f;
    return 0;
}

u32 gPadButtonMask[4] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};

PADStatus gPadStatuses[2][4];
u32 gPadButtonsJustPressed[4];
u32 gPadButtonsReleased[4];
u32 gPadButtonsHeld[4];
u32 gPadButtonsPrevious[4];
