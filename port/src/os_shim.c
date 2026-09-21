#include <dolphin/os.h>
#include <aurora/dvd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shim_log.h"

static BOOL sInterruptsEnabled = 1;
static u32 sSoundMode = 1;
static u32 sProgressiveMode;
static u8 sLanguage;

BOOL OSDisableInterrupts(void) {
  BOOL prev = sInterruptsEnabled;
  sInterruptsEnabled = 0;
  return prev;
}

BOOL OSEnableInterrupts(void) {
  BOOL prev = sInterruptsEnabled;
  sInterruptsEnabled = 0;
  aurora_dvd_process_callbacks();
  sInterruptsEnabled = 1;
  return prev;
}

BOOL OSRestoreInterrupts(BOOL level) {
  BOOL prev = sInterruptsEnabled;
  if (level) {
    sInterruptsEnabled = 0;
    aurora_dvd_process_callbacks();
  }
  sInterruptsEnabled = level;
  return prev;
}

void OSPanic(const char* file, int line, const char* msg, ...) {
  fprintf(stderr, "[foxhollow] OSPanic %s:%d: %s\n", file, line, msg);
  abort();
}

u32 OSGetSoundMode(void) { return sSoundMode; }
void OSSetSoundMode(u32 mode) { sSoundMode = mode; }
u32 OSGetProgressiveMode(void) { return sProgressiveMode; }
void OSSetProgressiveMode(u32 mode) { sProgressiveMode = mode; }
u8 OSGetLanguage(void) { return sLanguage; }
void OSSetLanguage(u8 language) { sLanguage = language; }
u32 OSGetResetCode(void) { return 0; }
BOOL OSGetResetButtonState(void) { return 0; }

static OSErrorHandler sErrorHandlers[17];

OSErrorHandler OSSetErrorHandler(OSError error, OSErrorHandler handler) {
  OSErrorHandler prev = 0;
  if (error < 17) {
    prev = sErrorHandlers[error];
    sErrorHandlers[error] = handler;
  }
  return prev;
}
void OSSetSaveRegion(void* start, void* end) { (void)start; (void)end; }

void OSResetSystem(int reset, u32 resetCode, BOOL forceMenu) {
  (void)reset; (void)resetCode; (void)forceMenu;
  fprintf(stderr, "[foxhollow] OSResetSystem requested; exiting\n");
  exit(0);
}
