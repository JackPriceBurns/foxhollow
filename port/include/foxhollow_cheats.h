#ifndef FOXHOLLOW_CHEATS_H_
#define FOXHOLLOW_CHEATS_H_

#ifdef __cplusplus
extern "C" {
#endif

struct GameObject;

int fhCheatsGameplayActive(void);

int fhCheatsGodModeEnabled(void);
void fhCheatsSetGodMode(int enabled);
int fhCheatsInfiniteMagicEnabled(void);
void fhCheatsSetInfiniteMagic(int enabled);
int fhCheatsPlayerIsFox(void);
int fhCheatsTrickyPresent(void);

int fhCheatsPlayerDamage(int damage);
int fhCheatsMagicCost(int cost);

int fhCheatsFastRunEnabled(void);
void fhCheatsSetFastRun(int enabled);
float fhCheatsMoveScale(struct GameObject* obj);

int fhCheatsArwingActive(void);
int fhCheatsArwingGodModeEnabled(void);
void fhCheatsSetArwingGodMode(int enabled);
int fhCheatsArwingInfiniteBombsEnabled(void);
void fhCheatsSetArwingInfiniteBombs(int enabled);
int fhCheatsArwingRapidFireEnabled(void);
void fhCheatsSetArwingRapidFire(int enabled);
int fhCheatsArwingMaxRingsEnabled(void);
void fhCheatsSetArwingMaxRings(int enabled);
void fhCheatsArwingUpdate(struct GameObject* arwing);
void fhCheatsArwingRefillHealth(struct GameObject* arwing);
unsigned short fhCheatsArwingFireInput(unsigned short pressed, unsigned short held);

void fhCheatsArwingBombLaunched(void);
int fhCheatsArwingBombIgnoreButton(void);
int fhCheatsArwingBombIgnoreHit(struct GameObject* bomb, struct GameObject* hitObj);

void fhCheatsSetCloudRunner(struct GameObject* cloudRunner, int alive);
int fhCheatsCloudRunnerActive(void);
int fhCheatsCloudRunnerRapidFireEnabled(void);
void fhCheatsSetCloudRunnerRapidFire(int enabled);
int fhCheatsCloudRunnerRapidFire(int held);

int fhCheatsInfiniteItemsEnabled(void);
void fhCheatsSetInfiniteItems(int enabled);
int fhCheatsInfiniteTrickyEnergyEnabled(void);
void fhCheatsSetInfiniteTrickyEnergy(int enabled);
void fhCheatsUpdate(void);

void fhCheatsGiveAllItems(void);
void fhCheatsGiveAllStaffAbilities(void);

void fhCheatsDrawOverlay(void);

#ifdef __cplusplus
}
#endif

#endif
