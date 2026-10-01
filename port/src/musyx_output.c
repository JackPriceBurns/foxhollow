#include <dolphin/ai.h>
#include <musyx/musyx.h>
#include <musyx/pc.h>
#include <SDL3/SDL.h>
#include <stdio.h>
#include <string.h>

#define FH_MUSYX_MIX_RATE 32000
#define FH_MUSYX_CHANNELS 2
#define FH_MUSYX_BUFFER_COUNT 4
#define FH_MUSYX_CHUNK_FRAMES 160
#define FH_MUSYX_CHUNK_BYTES (FH_MUSYX_CHUNK_FRAMES * FH_MUSYX_CHANNELS * sizeof(s16))

static s16 sMusyxBuffers[FH_MUSYX_BUFFER_COUNT][FH_MUSYX_CHUNK_FRAMES * FH_MUSYX_CHANNELS];
static u32 sMusyxBufferIndex;
static SDL_Mutex* sMusyxLock;

static void fhMusyxEnter(void) { SDL_LockMutex(sMusyxLock); }

static void fhMusyxLeave(void) { SDL_UnlockMutex(sMusyxLock); }

static void fhMusyxDmaCallback(void) {
  s16* next;

  sMusyxBufferIndex = (sMusyxBufferIndex + 1) % FH_MUSYX_BUFFER_COUNT;
  AIInitDMA((uintptr_t)sMusyxBuffers[sMusyxBufferIndex], FH_MUSYX_CHUNK_BYTES);
  next = sMusyxBuffers[(sMusyxBufferIndex + 2) % FH_MUSYX_BUFFER_COUNT];
  if (!sndPCRender(next, FH_MUSYX_CHUNK_FRAMES)) {
    memset(next, 0, FH_MUSYX_CHUNK_BYTES);
  }
}

void fhMusyxConfigure(void) {
  SND_PC_CONFIG config = {FH_MUSYX_MIX_RATE, FH_MUSYX_CHANNELS};

  if (sMusyxLock == NULL) {
    sMusyxLock = SDL_CreateMutex();
  }
  if (sMusyxLock != NULL) {
    sndPCSetSynchronization(fhMusyxEnter, fhMusyxLeave);
  }
  if (!sndPCConfigure(&config)) {
    fprintf(stderr, "[foxhollow] MusyX PC configuration rejected\n");
  }
}

void fhMusyxStartOutput(void) {
  memset(sMusyxBuffers, 0, sizeof(sMusyxBuffers));
  sMusyxBufferIndex = 1;
  AIRegisterDMACallback(fhMusyxDmaCallback);
  AIInitDMA((uintptr_t)sMusyxBuffers[sMusyxBufferIndex], FH_MUSYX_CHUNK_BYTES);
  AIStartDMA();
}

void fhMusyxStopOutput(void) {
  AIRegisterDMACallback(NULL);
  AIStopDMA();
}
