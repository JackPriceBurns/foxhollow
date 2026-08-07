#ifndef MAIN_DLL_BADDIE_FROZEN_H_
#define MAIN_DLL_BADDIE_FROZEN_H_

#include "game/objects/object.h"
#include "dolphin/mtx/vec_types.h"
#include "main/dll/dll_00C9_enemy.h"

u8 sharpClawHandleHitMessage(GameObject* obj, u8* state, GameObject* attacker, int hit, int p5, int p6, Vec* hitPos,
                             int sector, f32 hDist, f32 vDist);
void guardClawUpdateWhileFrozen(GameObject* obj, u8* state, GameObject* attacker, int hit, int p5, int p6, Vec* hitPos,
                                int sector);
void gcRobotPatrol_updateWhileFrozen(int obj, u8* state, GameObject* attacker, int hit, int p5, int p6, Vec* hitPos,
                                     int sector);
void mikaladon_updateWhileFrozen(int obj, u8* state, GameObject* attacker, int hit, int p5, int p6, Vec* hitPos,
                                 int sector);
void vambat_updateWhileFrozen(int obj, u8* state, GameObject* attacker, int hit, int p5, int p6, Vec* hitPos,
                              int sector);
void kooshy_updateWhileFrozen(GameObject* obj, u8* state, GameObject* attacker, int hit, int p5, int p6, Vec* hitPos,
                              int sector);
void weevil_updateWhileFrozen(GameObject* obj, u8* state, GameObject* attacker, int hit, int p5, int p6, Vec* hitPos,
                              int sector);
void pinPon_updateWhileFrozen(int obj, EnemyState* state, GameObject* attacker, int hit, int p5, int p6, Vec* hitPos,
                              int sector);
void wbUpdateWhileFrozen(int obj, u8* state, GameObject* attacker, int hit, int p5, int p6, Vec* hitPos, int sector);
void mutatedEbaUpdateWhileFrozen(int obj, u8* state, GameObject* attacker, int hit, int p5, int p6, Vec* hitPos,
                                 int sector);
void hoodedZyckUpdateWhileFrozen(int obj, u8* state, GameObject* attacker, int hit, int p5, int p6, Vec* hitPos,
                                 int sector);
void battleDroidUpdateWhileFrozen(int obj, u8* state, GameObject* attacker, int hit, int p5, int p6, Vec* hitPos,
                                  int sector);
void crawler_onHit(GameObject* obj, u8* state, GameObject* attacker, int hit, int p5, int p6, Vec* hitPos, int sector);
void hagabonMK2_updateWhileFrozen(int obj, u8* state, GameObject* attacker, int hit, int p5, int p6, Vec* hitPos,
                                  int sector);

void rachnopUpdateWhileFrozen(GameObject* obj, u8* state, GameObject* attacker, int hit, int p5, int p6, Vec* hitPos,
                              int sector);
void spittingEbaUpdateWhileFrozen(int obj, u8* state, GameObject* attacker, int hit, int p5, int p6, Vec* hitPos,
                                  int sector);
void whirlpool_updateWhileFrozen(int obj, u8* state, GameObject* attacker, int hit, int p5, int p6, Vec* hitPos,
                                 int sector);
void snowworm_updateWhileFrozen(int obj, u8* state, GameObject* attacker, int hit, int p5, int p6, Vec* hitPos,
                                int sector);

#endif /* MAIN_DLL_BADDIE_FROZEN_H_ */
