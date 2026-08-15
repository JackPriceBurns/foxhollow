#include <dolphin/ai.h>
#include <dolphin/dvd.h>
#include <SDL3/SDL.h>
#include <stdio.h>

#define AI_DMA_SAMPLE_RATE 32000
#define AI_STREAM_SAMPLE_RATE 48000
#define AI_STREAM_BLOCK_SIZE 32
#define AI_STREAM_BLOCK_FRAMES 28
#define AI_OUTPUT_QUEUE_MS 40
#define AI_DMA_QUEUE_BYTES ((AI_DMA_SAMPLE_RATE * 4 * AI_OUTPUT_QUEUE_MS) / 1000)
#define AI_STREAM_QUEUE_BYTES ((AI_STREAM_SAMPLE_RATE * 4 * AI_OUTPUT_QUEUE_MS) / 1000)

static AIDCallback sDmaCallback;
static AISCallback sStreamCallback;
static uintptr_t sDmaStartAddr;
static u32 sDmaLength;
static u32 sDspSampleRate;
static u32 sStreamPlayState;
static u32 sStreamVolLeft;
static u32 sStreamVolRight;
static u32 sDmaRunning;
static u32 sStreamSampleCount;
static u32 sStreamTrigger;
#if defined(FOXHOLLOW_DEBUG_SHORTCUTS)
static u32 sMuted = 1;
static u32 sMuteKeyWasDown;
#endif
static SDL_AudioStream* sOutputStream;
static SDL_AudioStream* sStreamOutput;
static u8* sStreamData;
static u32 sStreamDataLength;
static u32 sStreamDataOffset;
static s32 sStreamHistLeft1;
static s32 sStreamHistLeft2;
static s32 sStreamHistRight1;
static s32 sStreamHistRight2;

#if defined(FOXHOLLOW_DEBUG_SHORTCUTS)
static void aiSetMuted(u32 muted) {
  float gain = muted != 0 ? 0.0f : 1.0f;

  sMuted = muted;
  if (sOutputStream != NULL) {
    SDL_SetAudioStreamGain(sOutputStream, gain);
  }
  if (sStreamOutput != NULL) {
    SDL_SetAudioStreamGain(sStreamOutput, gain);
  }
  fprintf(stderr, "[foxhollow] audio %s\n", muted != 0 ? "muted" : "unmuted");
}

static void aiUpdateMuteToggle(void) {
  int keyCount;
  const bool* keys = SDL_GetKeyboardState(&keyCount);
  u32 muteKeyDown = keyCount > SDL_SCANCODE_M && keys[SDL_SCANCODE_M];

  if (muteKeyDown != 0 && sMuteKeyWasDown == 0) {
    aiSetMuted(sMuted == 0);
  }
  sMuteKeyWasDown = muteKeyDown;
}
#endif

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
#if defined(FOXHOLLOW_DEBUG_SHORTCUTS)
  else {
    SDL_SetAudioStreamGain(sOutputStream, sMuted != 0 ? 0.0f : 1.0f);
  }
#endif
}

static void aiOpenStreamOutput(void) {
  SDL_AudioSpec spec;

  if (sStreamOutput != NULL) {
    return;
  }
  if (!SDL_InitSubSystem(SDL_INIT_AUDIO)) {
    fprintf(stderr, "[foxhollow] SDL stream audio init failed: %s\n", SDL_GetError());
    return;
  }
  SDL_zero(spec);
  spec.format = SDL_AUDIO_S16;
  spec.channels = 2;
  spec.freq = AI_STREAM_SAMPLE_RATE;
  sStreamOutput = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
  if (sStreamOutput == NULL) {
    fprintf(stderr, "[foxhollow] SDL stream audio output failed: %s\n", SDL_GetError());
  }
#if defined(FOXHOLLOW_DEBUG_SHORTCUTS)
  else {
    SDL_SetAudioStreamGain(sStreamOutput, sMuted != 0 ? 0.0f : 1.0f);
  }
#endif
}

static s16 aiDecodeStreamSample(s32 bits, s32 predictorScale, s32* hist1, s32* hist2) {
  s32 hist;
  s32 sample;

  switch (predictorScale >> 4) {
  case 1:
    hist = *hist1 * 0x3c;
    break;
  case 2:
    hist = (*hist1 * 0x73) - (*hist2 * 0x34);
    break;
  case 3:
    hist = (*hist1 * 0x62) - (*hist2 * 0x37);
    break;
  default:
    hist = 0;
    break;
  }
  hist = SDL_clamp((hist + 0x20) >> 6, -0x200000, 0x1fffff);
  sample = (((s16)(bits << 12) >> (predictorScale & 0xf)) << 6) + hist;
  *hist2 = *hist1;
  *hist1 = sample;
  return (s16)SDL_clamp(sample >> 6, -0x8000, 0x7fff);
}

static void aiPumpStream(void) {
  int queued;

  if (sStreamPlayState != AI_STREAM_START || sStreamData == NULL) {
    return;
  }

  queued = sStreamOutput != NULL ? SDL_GetAudioStreamQueued(sStreamOutput) : 0;
  while (sStreamOutput != NULL && queued < AI_STREAM_QUEUE_BYTES &&
         sStreamDataOffset + AI_STREAM_BLOCK_SIZE <= sStreamDataLength) {
    const u8* block = sStreamData + sStreamDataOffset;
    s16 pcm[AI_STREAM_BLOCK_FRAMES * 2];
    int i;

    for (i = 0; i < AI_STREAM_BLOCK_FRAMES; i++) {
      const u8 sample = block[i + 4];
      s32 left = aiDecodeStreamSample(sample & 0xf, block[0], &sStreamHistLeft1, &sStreamHistLeft2);
      s32 right = aiDecodeStreamSample(sample >> 4, block[1], &sStreamHistRight1, &sStreamHistRight2);
      pcm[i * 2] = (s16)((left * (s32)sStreamVolLeft) / 255);
      pcm[i * 2 + 1] = (s16)((right * (s32)sStreamVolRight) / 255);
    }
    if (sStreamOutput != NULL) {
      SDL_PutAudioStreamData(sStreamOutput, pcm, sizeof(pcm));
      queued += sizeof(pcm);
    }
    sStreamDataOffset += AI_STREAM_BLOCK_SIZE;
    sStreamSampleCount += AI_STREAM_BLOCK_FRAMES;
    if (sStreamCallback != NULL && sStreamTrigger != 0 && sStreamSampleCount >= sStreamTrigger) {
      sStreamCallback(sStreamSampleCount);
    }
  }

  if (sStreamDataOffset + AI_STREAM_BLOCK_SIZE > sStreamDataLength) {
    sStreamPlayState = AI_STREAM_STOP;
  }
}

void AIInit(u8* stack) { (void)stack; }
void AIReset(void) { AIStopDMA(); }

int fhAIPrepareStream(void* fileInfoPtr) {
  DVDFileInfo* fileInfo = fileInfoPtr;
  u8* data;
  s32 bytesRead;

  if (fileInfo == NULL || fileInfo->length == 0 || (fileInfo->length & (AI_STREAM_BLOCK_SIZE - 1)) != 0) {
    return 0;
  }
  data = SDL_aligned_alloc(32, fileInfo->length);
  if (data == NULL) {
    fprintf(stderr, "[foxhollow] Unable to allocate %u bytes for stream audio\n", fileInfo->length);
    return 0;
  }
  bytesRead = DVDReadPrio(fileInfo, data, (s32)fileInfo->length, 0, 2);
  if (bytesRead != (s32)fileInfo->length) {
    fprintf(stderr, "[foxhollow] Unable to read stream audio (%d/%u bytes)\n", bytesRead,
            fileInfo->length);
    SDL_aligned_free(data);
    return 0;
  }

  if (sStreamData != NULL) {
    SDL_aligned_free(sStreamData);
  }
  sStreamData = data;
  sStreamDataLength = fileInfo->length;
  sStreamDataOffset = 0;
  sStreamSampleCount = 0;
  sStreamHistLeft1 = 0;
  sStreamHistLeft2 = 0;
  sStreamHistRight1 = 0;
  sStreamHistRight2 = 0;
  sStreamPlayState = AI_STREAM_STOP;
  aiOpenStreamOutput();
  if (sStreamOutput != NULL) {
    SDL_PauseAudioStreamDevice(sStreamOutput);
    SDL_ClearAudioStream(sStreamOutput);
  }
  return 1;
}

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
  if (sOutputStream != NULL) {
    SDL_PauseAudioStreamDevice(sOutputStream);
    SDL_ClearAudioStream(sOutputStream);
  }
}

void fhAIPump(void) {
  int dmaQueued = sOutputStream != NULL ? SDL_GetAudioStreamQueued(sOutputStream) : 0;

#if defined(FOXHOLLOW_DEBUG_SHORTCUTS)
  aiUpdateMuteToggle();
#endif
  aiPumpStream();
  while (sDmaRunning != 0 && sOutputStream != NULL && dmaQueued < AI_DMA_QUEUE_BYTES) {
    AIDCallback callback;

    if (sOutputStream != NULL && sDmaStartAddr != 0 && sDmaLength != 0) {
      SDL_PutAudioStreamData(sOutputStream, (const void*)sDmaStartAddr, (int)sDmaLength);
      dmaQueued += sDmaLength;
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
void AISetStreamPlayState(u32 state) {
  sStreamPlayState = state;
  if (sStreamOutput == NULL) {
    return;
  }
  if (state == AI_STREAM_START) {
    SDL_ResumeAudioStreamDevice(sStreamOutput);
  } else {
    SDL_PauseAudioStreamDevice(sStreamOutput);
    SDL_ClearAudioStream(sStreamOutput);
  }
}
u32 AIGetStreamPlayState(void) { return sStreamPlayState; }
void AISetStreamSampleRate(u32 rate) { (void)rate; }
u32 AIGetStreamSampleRate(void) { return AI_SAMPLERATE_48KHZ; }
void AISetStreamVolLeft(u8 vol) { sStreamVolLeft = vol; }
void AISetStreamVolRight(u8 vol) { sStreamVolRight = vol; }
u8 AIGetStreamVolLeft(void) { return (u8)sStreamVolLeft; }
u8 AIGetStreamVolRight(void) { return (u8)sStreamVolRight; }
