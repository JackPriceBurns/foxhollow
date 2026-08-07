#ifndef MAIN_DLL_PLAYER_MOTION_API_H_
#define MAIN_DLL_PLAYER_MOTION_API_H_

#include "global.h"
#include "game/objects/object.h"
#include "main/dll/baddie_state.h"

void playerUpdateMotionState(GameObject* obj, void* motionState, BaddieState* baddieState);
void playerClampVelocityAndMove(GameObject* obj, f32 timeDelta);
void playerUpdateVelocityFromMotion(GameObject* obj, void* motionState, BaddieState* baddieState, f32 unusedTimeDelta);

#endif /* MAIN_DLL_PLAYER_MOTION_API_H_ */
