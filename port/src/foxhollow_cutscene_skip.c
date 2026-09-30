#include "foxhollow_compat.h"
#include "foxhollow_cutscene_skip.h"

#include <SDL3/SDL.h>
#include <string.h>

#include <dolphin/types.h>

#include "global.h"
#include "game/objects/object.h"
#include "main/dll/dll_0000_gameui.h"
#include "main/frame_timing.h"
#include "main/gameloop.h"
#include "main/gameloop_internal.h"
#include "main/gametext_shared_internal.h"
#include "main/model_engine.h"
#include "main/objseq.h"
#include "main/textrender.h"
#include "sys/objects.h"
#include "sys/objects/lifecycle.h"

#define FH_SKIP_SEQ_CLASS_ID          0x10
#define FH_SKIP_FIRST_RUNTIME_SLOT    0x19
#define FH_SKIP_SLOT_LIMIT            0x55
#define FH_SKIP_MAX_ACTORS            32
#define FH_SKIP_CONDITION_SLOTS       10
#define FH_SKIP_COND_BUTTON_A         0x12
#define FH_SKIP_COND_BUTTON_B         0x13
#define FH_SKIP_COND_DIALOGUE_CLOSED  0x1a
#define FH_SKIP_STEPS_PER_FRAME       120
#define FH_SKIP_STALLED_STEPS_PER_FRAME 8
#define FH_SKIP_STALL_FRAMES          180
#define FH_SKIP_MAX_TOTAL_STEPS       36000
#define FH_SKIP_REQUEST_MAX_AGE       4
#define FH_SKIP_UI_GAMEPLAY           1
#define FH_SKIP_GAME_STATE_RUNNING    1
#define FH_SKIP_RUNSTATE_RUNNING      1
#define FH_SKIP_INACTIVE_FLAGS        (OBJECT_OBJFLAG_FREED | OBJECT_OBJFLAG_UPDATE_DISABLED)
#define FH_SKIP_TEXT_OP_FIRST         1
#define FH_SKIP_TEXT_OP_CALLBACK      9
#define FH_SKIP_TEXT_OP_LAST_DRAW     14

typedef struct FhSkipActor {
  GameObject* obj;
  s16 curFrame;
  u8 runState;
} FhSkipActor;

extern u8 gWarpRequested;
extern ObjLinkedList gObjUpdateList;
extern int gSubtitleActive;

unsigned char gFhCutsceneSkipWorldEffect;
unsigned char gFhCutsceneSkipMuteSfx;
unsigned char gFhCutsceneSkipHideSubtitles;
unsigned char gFhCutsceneSkipEndSubtitles;

static int sSkipKeyPressed;
static int sAutoKeyPressed;
static int sAutoSkip;
static int sAutoHoldSlot = -1;
static s16 sAutoHoldSeqValue;
static int sAutoHoldSawWait;
static u32 sFrameCounter;
static int sRequestPending;
static u32 sRequestFrame;
static int sInBatch;

static int sActiveSlot = -1;
static s16 sActiveSeqValue;
static int sTotalSteps;
static int sStallFrames;

static int sSubtitleSlot = -1;
static s16 sSubtitleSeqValue;

static ObjSeqState* seq_state(GameObject* obj) {
  return (ObjSeqState*)obj->extra;
}

static int is_live_seq_object(GameObject* obj) {
  return obj->anim.classId == FH_SKIP_SEQ_CLASS_ID && obj->extra != NULL && obj->anim.placementData != NULL &&
         (obj->objectFlags & FH_SKIP_INACTIVE_FLAGS) == 0;
}

static int is_runtime_slot(int slot) {
  return slot >= FH_SKIP_FIRST_RUNTIME_SLOT && slot < FH_SKIP_SLOT_LIMIT && gObjSeqSlotSeqIdTable[slot] != 0;
}

static int collect_slot_actors(int slot, FhSkipActor* out, int max) {
  uintptr_t cur = gObjUpdateList.head;
  int linkOffset = gObjUpdateList.nextOffset;
  int count = 0;

  while (cur != 0 && count < max) {
    GameObject* obj = (GameObject*)cur;

    if (is_live_seq_object(obj) && seq_state(obj)->slot == slot) {
      out[count].obj = obj;
      out[count].curFrame = seq_state(obj)->curFrame;
      out[count].runState = seq_state(obj)->runState;
      count++;
    }
    cur = *(uintptr_t*)((u8*)cur + linkOffset);
  }
  return count;
}

static int find_cutscene_slot(GameObject* player) {
  u8 scores[FH_SKIP_SLOT_LIMIT];
  uintptr_t cur = gObjUpdateList.head;
  int linkOffset = gObjUpdateList.nextOffset;
  int best = -1;
  int bestScore = 0;
  int slot;

  memset(scores, 0, sizeof(scores));
  while (cur != 0) {
    GameObject* obj = (GameObject*)cur;

    if (is_live_seq_object(obj)) {
      ObjSeqState* state = seq_state(obj);

      slot = state->slot;
      if (is_runtime_slot(slot) && state->runState != 0) {
        if (state->isCameraSeq != 0) {
          scores[slot] |= 2;
        }
        if (player != NULL && state->targetObj == player) {
          scores[slot] |= 1;
        }
      }
    }
    cur = *(uintptr_t*)((u8*)cur + linkOffset);
  }

  for (slot = FH_SKIP_FIRST_RUNTIME_SLOT; slot < FH_SKIP_SLOT_LIMIT; slot++) {
    int score = scores[slot];

    if (score == 0) {
      continue;
    }
    if (slot == curSeqNo) {
      score |= 4;
    }
    if (score > bestScore) {
      best = slot;
      bestScore = score;
    }
  }
  return best;
}

static int gameplay_blocked(void) {
  return getGameState() != FH_SKIP_GAME_STATE_RUNNING || getCurUiDll() != FH_SKIP_UI_GAMEPLAY || timeStop != 0 ||
         gWarpRequested != 0 || gGameLoopReloadRequested != 0 || gGameLoopMapLoadPending != 0 ||
         Obj_GetPlayerObject() == NULL;
}

static int actors_waiting(const FhSkipActor* actors, int count) {
  int i;
  int k;

  if (isTalkingToNpc() != 0) {
    return 1;
  }
  for (i = 0; i < count; i++) {
    ObjSeqState* state;

    if ((actors[i].obj->objectFlags & FH_SKIP_INACTIVE_FLAGS) != 0) {
      continue;
    }
    state = seq_state(actors[i].obj);
    for (k = 0; k < FH_SKIP_CONDITION_SLOTS; k++) {
      u8 op = state->conditionOpcodes[k];

      if (op == FH_SKIP_COND_BUTTON_A || op == FH_SKIP_COND_BUTTON_B || op == FH_SKIP_COND_DIALOGUE_CLOSED) {
        return 1;
      }
    }
  }
  return 0;
}

static void subtitle_cleanup_update(void) {
  if (sSubtitleSlot < 0) {
    return;
  }
  if (gFhCutsceneSkipHideSubtitles == 0) {
    sSubtitleSlot = -1;
    return;
  }
  if (gObjSeqSlotSeqIdTable[sSubtitleSlot] != sSubtitleSeqValue) {
    gFhCutsceneSkipEndSubtitles = 1;
    sSubtitleSlot = -1;
  }
}

static void end_skip(void) {
  gObjSeqStreamSuppressed = 0;
  gFhCutsceneSkipMuteSfx = 0;
  sActiveSlot = -1;
}

static void finish_completed(void) {
  end_skip();
  subtitle_cleanup_update();
}

static void stop_early(void) {
  sAutoHoldSlot = sActiveSlot;
  sAutoHoldSeqValue = sActiveSeqValue;
  sAutoHoldSawWait = 0;
  end_skip();
}

static int text_commands_are_draw_only(int first, int end) {
  int i;

  for (i = first; i < end; i++) {
    int op = gGameTextCommandSlots[i].opcode;

    if (op < FH_SKIP_TEXT_OP_FIRST || op > FH_SKIP_TEXT_OP_LAST_DRAW || op == FH_SKIP_TEXT_OP_CALLBACK) {
      return 0;
    }
  }
  return 1;
}

static int step_slot(int slot, const FhSkipActor* actors, int count) {
  u8 savedFrames = framesThisStep;
  u8 savedUnclamped = framesThisStepUnclamped;
  f32 savedDelta = timeDelta;
  f32 savedInverse = oneOverTimeDelta;
  int savedTextCount = gGameTextCommandCount;
  char* savedTextCursor = gGameTextCommandStringCursor;
  int keptText = 0;
  int i;

  framesThisStep = 1;
  framesThisStepUnclamped = 1;
  timeDelta = 1.0f;
  oneOverTimeDelta = 1.0f;
  for (i = 0; i < count; i++) {
    if ((actors[i].obj->objectFlags & FH_SKIP_INACTIVE_FLAGS) == 0) {
      Obj_UpdateObject(actors[i].obj);
    }
  }
  if (gObjSeqSlotSeqIdTable[slot] == sActiveSeqValue) {
    ObjSeq_advanceSlotFrame(slot);
  }
  framesThisStep = savedFrames;
  framesThisStepUnclamped = savedUnclamped;
  timeDelta = savedDelta;
  oneOverTimeDelta = savedInverse;
  if (gGameTextCommandCount > savedTextCount) {
    if (text_commands_are_draw_only(savedTextCount, gGameTextCommandCount)) {
      gGameTextCommandCount = savedTextCount;
      gGameTextCommandStringCursor = savedTextCursor;
    } else {
      keptText = 1;
    }
  }
  return keptText;
}

static void run_batch(void) {
  FhSkipActor actors[FH_SKIP_MAX_ACTORS];
  int count;
  int steps = 0;
  int stalledSteps = 0;
  int i;

  while (steps < FH_SKIP_STEPS_PER_FRAME) {
    int progressed = 0;
    int looped = 0;

    if (gObjSeqSlotSeqIdTable[sActiveSlot] != sActiveSeqValue) {
      finish_completed();
      return;
    }
    if (gameplay_blocked()) {
      stop_early();
      return;
    }
    count = collect_slot_actors(sActiveSlot, actors, FH_SKIP_MAX_ACTORS);
    if (count == 0) {
      finish_completed();
      return;
    }
    if (actors_waiting(actors, count) || sTotalSteps >= FH_SKIP_MAX_TOTAL_STEPS) {
      stop_early();
      return;
    }

    gFhCutsceneSkipWorldEffect = 0;
    if (step_slot(sActiveSlot, actors, count) != 0) {
      gFhCutsceneSkipWorldEffect = 1;
    }
    steps++;
    sTotalSteps++;

    if (gObjSeqSlotSeqIdTable[sActiveSlot] != sActiveSeqValue) {
      finish_completed();
      return;
    }
    for (i = 0; i < count; i++) {
      ObjSeqState* state;

      if ((actors[i].obj->objectFlags & FH_SKIP_INACTIVE_FLAGS) != 0) {
        progressed = 1;
        continue;
      }
      state = seq_state(actors[i].obj);
      if (state->runState != actors[i].runState || state->curFrame != actors[i].curFrame) {
        progressed = 1;
      }
      if (actors[i].runState == FH_SKIP_RUNSTATE_RUNNING && state->runState == FH_SKIP_RUNSTATE_RUNNING &&
          actors[i].curFrame - state->curFrame > 1) {
        looped = 1;
      }
    }
    if (looped != 0) {
      stop_early();
      return;
    }
    if (progressed != 0) {
      sStallFrames = 0;
      stalledSteps = 0;
    } else if (++stalledSteps >= FH_SKIP_STALLED_STEPS_PER_FRAME) {
      if (++sStallFrames >= FH_SKIP_STALL_FRAMES) {
        stop_early();
      }
      return;
    }
    if (gFhCutsceneSkipWorldEffect != 0) {
      return;
    }
  }
}

static void try_accept(void) {
  FhSkipActor actors[FH_SKIP_MAX_ACTORS];
  int slot;
  int count;

  if (gameplay_blocked()) {
    return;
  }
  slot = find_cutscene_slot(Obj_GetPlayerObject());
  if (slot < 0) {
    return;
  }
  count = collect_slot_actors(slot, actors, FH_SKIP_MAX_ACTORS);
  if (actors_waiting(actors, count)) {
    return;
  }

  sActiveSlot = slot;
  sActiveSeqValue = gObjSeqSlotSeqIdTable[slot];
  sTotalSteps = 0;
  sStallFrames = 0;
  fhCutsceneSkipNotify("Cutscene Skipped", 1);

  ObjSeq_releaseSlotStream(slot);
  gObjSeqStreamSuppressed = 1;
  gFhCutsceneSkipMuteSfx = 1;
  if (gSubtitleActive != 0) {
    gFhCutsceneSkipHideSubtitles = 1;
    gFhCutsceneSkipEndSubtitles = 0;
    sSubtitleSlot = slot;
    sSubtitleSeqValue = sActiveSeqValue;
  }
}

void fhCutsceneSkipKeyDown(int scancode) {
  if (scancode == SDL_SCANCODE_F9) {
    sSkipKeyPressed = 1;
  } else if (scancode == SDL_SCANCODE_F10) {
    sAutoKeyPressed = 1;
  }
}

void fhCutsceneSkipUpdate(void) {
  sFrameCounter++;
  if (sSkipKeyPressed != 0) {
    sSkipKeyPressed = 0;
    if (sActiveSlot < 0 && !gameplay_blocked()) {
      sRequestPending = 1;
      sRequestFrame = sFrameCounter;
    }
  }

  if (sAutoKeyPressed != 0) {
    sAutoKeyPressed = 0;
    sAutoSkip = !sAutoSkip;
    sAutoHoldSlot = -1;
    fhCutsceneSkipNotify(sAutoSkip ? "Cutscene Skip Enabled" : "Cutscene Skip Disabled", 0);
  }

  subtitle_cleanup_update();
}

int fhCutsceneSkipIsActive(void) {
  return sActiveSlot >= 0;
}

static int auto_skip_ready(void) {
  FhSkipActor actors[FH_SKIP_MAX_ACTORS];
  int slot;
  int count;
  int waiting;

  if (gameplay_blocked()) {
    sAutoHoldSawWait = 1;
    return 0;
  }
  slot = find_cutscene_slot(Obj_GetPlayerObject());
  if (slot < 0) {
    return 0;
  }
  count = collect_slot_actors(slot, actors, FH_SKIP_MAX_ACTORS);
  waiting = actors_waiting(actors, count);
  if (slot == sAutoHoldSlot && gObjSeqSlotSeqIdTable[slot] == sAutoHoldSeqValue) {
    if (waiting) {
      sAutoHoldSawWait = 1;
      return 0;
    }
    if (sAutoHoldSawWait == 0) {
      return 0;
    }
    sAutoHoldSlot = -1;
  }
  return !waiting;
}

void fhCutsceneSkipRunFrame(void) {
  if (sInBatch != 0) {
    return;
  }
  sInBatch = 1;
  if (sAutoSkip != 0 && sActiveSlot < 0 && sRequestPending == 0 && auto_skip_ready() != 0) {
    sRequestPending = 1;
    sRequestFrame = sFrameCounter;
  }
  if (sRequestPending != 0) {
    sRequestPending = 0;
    if (sFrameCounter - sRequestFrame <= FH_SKIP_REQUEST_MAX_AGE) {
      try_accept();
    }
  }
  if (sActiveSlot >= 0) {
    run_batch();
  }
  sInBatch = 0;
}
