#include <dolphin/ai.h>
#include <SDL3/SDL.h>
#include <stdio.h>

#define AI_DMA_SAMPLE_RATE 32000
#define AI_DMA_CHUNK_FRAMES 160
#define AI_RETRACE_RATE 60

static AIDCallback sDmaCallback;
static AISCallback sStreamCallback;
static uintptr_t sDmaStartAddr;
static u32 sDmaLength;
static u32 sDspSampleRate;
static u32 sStreamPlayState;
static u32 sStreamVolLeft;
static u32 sStreamVolRight;
static u32 sDmaRunning;
static u32 sDmaChunkAccumulator;
static u32 sStreamSampleCount;
static u32 sStreamTrigger;
static SDL_AudioStream* sOutputStream;

static void aiOpenOutput(void) {
  SDL_AudioSpec spec;

  if (sOutputStream != NULL) {
    return;
  }
  if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
    fprintf(stderr, "[foxhollow] SDL audio init failed: %s\n", SDL_GetError());
    return;
  }
  SDL_zero(spec);
  spec.format = SDL_AUDIO_S16;
  spec.channels = 2;
  spec.freq = AI_DMA_SAMPLE_RATE;
  sOutputStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
  if (sOutputStream == NULL) {
    fprintf(stderr, "[foxhollow] SDL audio output failed: %s\n", SDL_GetError());
  }
}

void AIInit(u8* stack) { (void)stack; }
void AIReset(void) { AIStopDMA(); }

AIDCallback AIRegisterDMACallback(AIDCallback callback) {
  AIDCallback previous = sDmaCallback;
  sDmaCallback = callback;
  return previous;
}

void AIInitDMA(uintptr_t startAddr, u32 length) {
  sDmaStartAddr = startAddr;
  sDmaLength = length;
}

void AIStartDMA(void) {
  aiOpenOutput();
  sDmaRunning = 1;
  if (sOutputStream != NULL) {
    SDL_ResumeAudioStreamDevice(sOutputStream);
  }
}

void AIStopDMA(void) {
  sDmaRunning = 0;
  sDmaChunkAccumulator = 0;
  if (sOutputStream != NULL) {
    SDL_PauseAudioStreamDevice(sOutputStream);
    SDL_ClearAudioStream(sOutputStream);
  }
}

void fhAIPump(void) {
  if (sDmaRunning == 0) {
    return;
  }
  sDmaChunkAccumulator += AI_DMA_SAMPLE_RATE;
  while (sDmaChunkAccumulator >= AI_RETRACE_RATE * AI_DMA_CHUNK_FRAMES) {
    AIDCallback callback;

    sDmaChunkAccumulator -= AI_RETRACE_RATE * AI_DMA_CHUNK_FRAMES;
    if (sOutputStream != NULL && sDmaStartAddr != 0 && sDmaLength != 0) {
      SDL_PutAudioStreamData(sOutputStream, (const void*)sDmaStartAddr, (int)sDmaLength);
    }
    callback = sDmaCallback;
    if (callback != NULL) {
      callback();
    }
  }
}

uintptr_t fhAIGetDMAStartAddr(void) { return sDmaStartAddr; }
u32 AIGetDMAStartAddr(void) { return (u32)sDmaStartAddr; }
u32 AIGetDMALength(void) { return sDmaLength; }
AIDCallback AIGetDMACallback(void) { return sDmaCallback; }
BOOL AIGetDMAEnableFlag(void) { return sDmaRunning != 0; }
u32 AIGetDMABytesLeft(void) { return sDmaLength; }
BOOL AICheckInit(void) { return TRUE; }
void AISetDSPSampleRate(u32 rate) { sDspSampleRate = rate; }
u32 AIGetDSPSampleRate(void) { return sDspSampleRate; }
AISCallback AIRegisterStreamCallback(AISCallback callback) {
  AISCallback previous = sStreamCallback;
  sStreamCallback = callback;
  return previous;
}
u32 AIGetStreamSampleCount(void) { return sStreamSampleCount; }
void AIResetStreamSampleCount(void) { sStreamSampleCount = 0; }
void AISetStreamTrigger(u32 trigger) { sStreamTrigger = trigger; }
u32 AIGetStreamTrigger(void) { return sStreamTrigger; }
void AISetStreamPlayState(u32 state) { sStreamPlayState = state; }
u32 AIGetStreamPlayState(void) { return sStreamPlayState; }
void AISetStreamSampleRate(u32 rate) { (void)rate; }
u32 AIGetStreamSampleRate(void) { return AI_SAMPLERATE_48KHZ; }
void AISetStreamVolLeft(u8 vol) { sStreamVolLeft = vol; }
void AISetStreamVolRight(u8 vol) { sStreamVolRight = vol; }
u8 AIGetStreamVolLeft(void) { return (u8)sStreamVolLeft; }
u8 AIGetStreamVolRight(void) { return (u8)sStreamVolRight; }
