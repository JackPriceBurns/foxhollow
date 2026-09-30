#include "foxhollow_compat.h"
#include "foxhollow_cheats.h"

#include <dolphin/types.h>
#include <dolphin/pad.h>
#include <dolphin/vi.h>

#include "game/objects/object.h"
#include "main/gamebit_ids.h"
#include "main/gamebits.h"
#include "main/gameloop.h"
#include "main/mapEventTypes.h"
#include "main/model_engine_ui.h"
#include "main/obj_list.h"
#include "main/dll/ARW/dll_029A_arwarwing.h"
#include "main/dll/cmenu_item_table.h"
#include "main/dll/player.h"
#include "main/dll/player_status.h"
#include "main/dll/savegame_load.h"
#include "sys/objects.h"
#include "sys/objects/lifecycle.h"

#define FH_CHEATS_GAME_STATE_RUNNING 1
#define FH_CHEATS_UI_DLL_GAMEPLAY 1
#define FH_CHEATS_UI_DLL_FRONTEND_FIRST 2
#define FH_CHEATS_UI_DLL_FRONTEND_LAST 7
#define FH_CHEATS_ARWING_BOMBS 3
#define FH_CHEATS_ARWING_RINGS 10
#define FH_CHEATS_MAX_SCARABS 200
#define FH_CHEATS_GLITCHED_ITEM_TEXT 0x404
#define FH_CHEATS_FAST_MOVEMENT_SCALE 2.0f

typedef struct FhCheatsItem {
  int gameBit;
  int max;
} FhCheatsItem;

static const FhCheatsItem sItems[] = {
    {GAMEBIT_ITEM_TrickyFood_Count, 15}, {GAMEBIT_ITEM_BombSpore_Count, 7}, {GAMEBIT_ITEM_MoonSeed_Count, 7},
    {GAMEBIT_ITEM_Firefly_Count, 31},    {GAMEBIT_ITEM_FuelCell_Count, 42},
};

extern CMenuItemDef gCMenuCollectableItems[];
extern CMenuItemDef gCMenuStaffAbilities[];

static int sGodMode;
static int sInfiniteMagic;
static int sFastMovement;
static int sInfiniteItems;
static int sInfiniteTrickyEnergy;
static int sArwingGodMode;
static int sArwingInfiniteBombs;
static int sArwingRapidFire;
static int sArwingMaxRings;
static u32 sArwingBombLaunchRetrace;
static GameObject* sCloudRunner;
static int sCloudRunnerRapidFire;
static int sSessionActive;

int fhCheatsGameplayActive(void) {
  return getGameState() == FH_CHEATS_GAME_STATE_RUNNING && getCurUiDll() == FH_CHEATS_UI_DLL_GAMEPLAY &&
         getSaveGameLoadStatus() == 0 && (Obj_GetPlayerObject() != NULL || getArwing() != NULL);
}

static void reset_cheats(void) {
  sGodMode = 0;
  sInfiniteMagic = 0;
  sFastMovement = 0;
  sInfiniteItems = 0;
  sInfiniteTrickyEnergy = 0;
  sArwingGodMode = 0;
  sArwingInfiniteBombs = 0;
  sArwingRapidFire = 0;
  sArwingMaxRings = 0;
  sArwingBombLaunchRetrace = 0;
  sCloudRunner = NULL;
  sCloudRunnerRapidFire = 0;
}

void fhCheatsUpdateSession(void) {
  int uiDll;

  if (fhCheatsGameplayActive()) {
    sSessionActive = 1;
    return;
  }
  uiDll = getCurUiDll();
  if (sSessionActive && (getGameState() != FH_CHEATS_GAME_STATE_RUNNING ||
                         (uiDll >= FH_CHEATS_UI_DLL_FRONTEND_FIRST && uiDll <= FH_CHEATS_UI_DLL_FRONTEND_LAST))) {
    sSessionActive = 0;
    reset_cheats();
  }
}

int fhCheatsGodModeEnabled(void) { return sGodMode; }

void fhCheatsSetGodMode(int enabled) {
  GameObject* player;
  int current;
  int missing;

  enabled = enabled != 0;
  if (enabled && !sGodMode && fhCheatsGameplayActive() && (player = Obj_GetPlayerObject()) != NULL) {
    current = playerGetCurHealth(player);
    missing = playerGetMaxHealth(player) - current;
    if (current > 0 && missing > 0) {
      playerAddHealth(player, missing);
    }
  }
  sGodMode = enabled;
}

int fhCheatsInfiniteMagicEnabled(void) { return sInfiniteMagic; }

void fhCheatsSetInfiniteMagic(int enabled) {
  GameObject* player;
  int missing;

  enabled = enabled != 0;
  if (enabled && !sInfiniteMagic && fhCheatsGameplayActive() && (player = Obj_GetPlayerObject()) != NULL) {
    missing = playerGetMaxMagic(player) - playerGetCurMagic(player);
    if (missing > 0) {
      playerAddRemoveMagic(player, missing);
    }
  }
  sInfiniteMagic = enabled;
}

int fhCheatsPlayerIsFox(void) {
  GameObject* player = Obj_GetPlayerObject();
  return player != NULL && objIsCurModelNotZero(player) != 0;
}

int fhCheatsTrickyPresent(void) { return getTrickyObject() != NULL; }

int fhCheatsPlayerDamage(int damage) { return sGodMode && damage > 0 ? 0 : damage; }

int fhCheatsMagicCost(int cost) { return sInfiniteMagic && cost > 0 ? 0 : cost; }

int fhCheatsFastRunEnabled(void) { return sFastMovement; }

void fhCheatsSetFastRun(int enabled) { sFastMovement = enabled != 0; }

float fhCheatsMoveScale(GameObject* obj) {
  if (!sFastMovement || obj != Obj_GetPlayerObject()) {
    return 1.0f;
  }
  return FH_CHEATS_FAST_MOVEMENT_SCALE;
}

int fhCheatsArwingActive(void) { return fhCheatsGameplayActive() && getArwing() != NULL; }

int fhCheatsArwingGodModeEnabled(void) { return sArwingGodMode; }

void fhCheatsSetArwingGodMode(int enabled) { sArwingGodMode = enabled != 0; }

int fhCheatsArwingInfiniteBombsEnabled(void) { return sArwingInfiniteBombs; }

void fhCheatsSetArwingInfiniteBombs(int enabled) { sArwingInfiniteBombs = enabled != 0; }

int fhCheatsArwingRapidFireEnabled(void) { return sArwingRapidFire; }

void fhCheatsSetArwingRapidFire(int enabled) { sArwingRapidFire = enabled != 0; }

int fhCheatsArwingMaxRingsEnabled(void) { return sArwingMaxRings; }

void fhCheatsSetArwingMaxRings(int enabled) { sArwingMaxRings = enabled != 0; }

void fhCheatsArwingRefillHealth(GameObject* arwing) {
  int current;
  int max;

  if (!sArwingGodMode || arwing == NULL) {
    return;
  }
  current = arwarwing_getHealth(arwing);
  max = arwarwing_getMaxHealth(arwing);
  if (current < max) {
    arwarwing_addHealth(arwing, max - current);
  }
}

unsigned short fhCheatsArwingFireInput(unsigned short pressed, unsigned short held) {
  if (sArwingRapidFire && (held & PAD_BUTTON_A) != 0) {
    return (unsigned short)(pressed | PAD_BUTTON_A);
  }
  return pressed;
}

void fhCheatsArwingUpdate(GameObject* arwing) {
  ArwingState* state = arwing->extra;

  if (sArwingMaxRings) {
    state->collectedRings = FH_CHEATS_ARWING_RINGS;
  }
  if (sArwingInfiniteBombs && state->bombCount < FH_CHEATS_ARWING_BOMBS) {
    state->bombCount = FH_CHEATS_ARWING_BOMBS;
  }
  fhCheatsArwingRefillHealth(arwing);
}

void fhCheatsArwingBombLaunched(void) { sArwingBombLaunchRetrace = VIGetRetraceCount(); }

int fhCheatsArwingBombIgnoreButton(void) { return VIGetRetraceCount() == sArwingBombLaunchRetrace; }

int fhCheatsArwingBombIgnoreHit(GameObject* bomb, GameObject* hitObj) {
  GameObject** objects;
  int start = 0;
  int count = 0;
  int i;

  if (hitObj == NULL) {
    return 0;
  }
  objects = ObjList_GetObjects(&start, &count);
  for (i = start; i < count; i++) {
    if (objects[i] == hitObj) {
      return hitObj->anim.romDefNo == bomb->anim.romDefNo;
    }
  }
  return 0;
}

void fhCheatsSetCloudRunner(GameObject* cloudRunner, int alive) {
  if (alive) {
    sCloudRunner = cloudRunner;
  } else if (cloudRunner == sCloudRunner) {
    sCloudRunner = NULL;
  }
}

int fhCheatsCloudRunnerActive(void) { return sCloudRunner != NULL && fhCheatsGameplayActive(); }

int fhCheatsCloudRunnerRapidFireEnabled(void) { return sCloudRunnerRapidFire; }

void fhCheatsSetCloudRunnerRapidFire(int enabled) { sCloudRunnerRapidFire = enabled != 0; }

int fhCheatsCloudRunnerRapidFire(int held) { return sCloudRunnerRapidFire && held; }

int fhCheatsInfiniteItemsEnabled(void) { return sInfiniteItems; }

void fhCheatsSetInfiniteItems(int enabled) { sInfiniteItems = enabled != 0; }

int fhCheatsInfiniteTrickyEnergyEnabled(void) { return sInfiniteTrickyEnergy; }

void fhCheatsSetInfiniteTrickyEnergy(int enabled) { sInfiniteTrickyEnergy = enabled != 0; }

static void fill_scarabs(GameObject* player) {
  int money;

  if (mainGetBit(GAMEBIT_ITEM_200ScarabBag_Got) == 0) {
    mainSetBits(GAMEBIT_ITEM_200ScarabBag_Got, 1);
  }
  money = playerGetMoney(player);
  if (money < FH_CHEATS_MAX_SCARABS) {
    playerAddMoney(player, FH_CHEATS_MAX_SCARABS - money);
  }
}

static void refill_items(GameObject* player) {
  int i;

  for (i = 0; i < (int)(sizeof(sItems) / sizeof(sItems[0])); i++) {
    if ((int)mainGetBit(sItems[i].gameBit) < sItems[i].max) {
      mainSetBits(sItems[i].gameBit, sItems[i].max);
    }
  }
  fill_scarabs(player);
}

void fhCheatsUpdate(void) {
  GameObject* player;
  TrickyStats* stats;

  if (!fhCheatsGameplayActive()) {
    return;
  }
  player = Obj_GetPlayerObject();
  if (sInfiniteItems && player != NULL) {
    refill_items(player);
  }
  if (sInfiniteTrickyEnergy && getTrickyObject() != NULL) {
    stats = (*gMapEventInterface)->getTrickyStats();
    if (stats != NULL && stats->energy < stats->maxEnergy) {
      stats->energy = stats->maxEnergy;
    }
  }
}

void fhCheatsGiveAllItems(void) {
  const CMenuItemDef* item;
  GameObject* player;

  if (!fhCheatsGameplayActive()) {
    return;
  }
  for (item = gCMenuCollectableItems; item->ownedGameBit != -1; item++) {
    if (item->nameTextId == FH_CHEATS_GLITCHED_ITEM_TEXT || item->ownedGameBit == GAMEBIT_ITEM_FireGem_Got ||
        item->ownedGameBit == GAMEBIT_ITEM_GiveScarabs_Count) {
      continue;
    }
    if (item->usedGameBit != -1 && mainGetBit(item->usedGameBit) != 0) {
      continue;
    }
    if (mainGetBit(item->ownedGameBit) == 0) {
      mainSetBits(item->ownedGameBit, 1);
    }
  }
  player = Obj_GetPlayerObject();
  if (player != NULL) {
    refill_items(player);
  }
}

void fhCheatsGiveAllStaffAbilities(void) {
  const CMenuItemDef* ability;
  GameObject* player;
  int missing;

  if (!fhCheatsGameplayActive()) {
    return;
  }
  for (ability = gCMenuStaffAbilities; ability->ownedGameBit != -1; ability++) {
    if (mainGetBit(ability->ownedGameBit) == 0) {
      mainSetBits(ability->ownedGameBit, 1);
    }
    if (ability->activeGameBit != -1 && mainGetBit(ability->activeGameBit) != 0) {
      mainSetBits(ability->activeGameBit, 0);
    }
  }
  if (mainGetBit(GAMEBIT_ITEM_Magic_Got) == 0) {
    mainSetBits(GAMEBIT_ITEM_Magic_Got, 1);
  }
  player = Obj_GetPlayerObject();
  if (player != NULL) {
    missing = playerGetMaxMagic(player) - playerGetCurMagic(player);
    if (missing > 0) {
      playerAddRemoveMagic(player, missing);
    }
  }
}
