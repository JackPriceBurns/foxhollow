#include <dolphin/types.h>
#include "shim_log.h"

typedef void (*AIDCallback)(void);
typedef void (*AISCallback)(u32 count);

static AIDCallback sDmaCallback;
static u32 sDmaStartAddr;
static u32 sDmaLength;
static u32 sDspSampleRate;
static u32 sStreamPlayState;
static u32 sStreamVolLeft;
static u32 sStreamVolRight;

void AIInit(u8* stack) { (void)stack; }
void AIReset(void) {}

AIDCallback AIRegisterDMACallback(AIDCallback callback) {
  AIDCallback prev = sDmaCallback;
  sDmaCallback = callback;
  return prev;
}

void AIInitDMA(u32 startAddr, u32 length) {
  sDmaStartAddr = startAddr;
  sDmaLength = length;
}

void AIStartDMA(void) { FH_STUB_ONCE("AIStartDMA (audio output not wired yet)"); }
void AIStopDMA(void) {}
u32 AIGetDMAStartAddr(void) { return sDmaStartAddr; }
u32 AIGetDMALength(void) { return sDmaLength; }
AIDCallback AIGetDMACallback(void) { return sDmaCallback; }
void AISetDSPSampleRate(u32 rate) { sDspSampleRate = rate; }
u32 AIGetDSPSampleRate(void) { return sDspSampleRate; }
void AISetStreamPlayState(u32 state) { sStreamPlayState = state; }
u32 AIGetStreamPlayState(void) { return sStreamPlayState; }
void AISetStreamVolLeft(u8 vol) { sStreamVolLeft = vol; }
void AISetStreamVolRight(u8 vol) { sStreamVolRight = vol; }
u8 AIGetStreamVolLeft(void) { return (u8)sStreamVolLeft; }
u8 AIGetStreamVolRight(void) { return (u8)sStreamVolRight; }
