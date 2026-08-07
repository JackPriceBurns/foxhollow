#ifndef MAIN_DLL_PLAYER_H_
#define MAIN_DLL_PLAYER_H_

#include "game/objects/object_fwd.h"
#include "global.h"

struct PlayerState;

int playerUpdateAirborneMotion(GameObject* obj, int inner, int state);
void playerUpdate(GameObject* obj);
void playerProcessMessages(int obj, int inner, int state);
void playerProcessHitResponse(int obj, int inner, int state);
void playerDoHitDetection(int obj);
int playerCheckCommonTransitions(int obj, int state, int inner, f32 fv);

int playerCheckIfClimbingOntoWall(int obj, int state, int state2, void* out, f32 fv, u32 mask);
int playerStateMoving(int obj, int state, f32 fv);
int playerStateOnLadder(int obj, int state);
int playerStateClimbWall(GameObject* obj, int state);
int playerStateAimStaff(int obj, int state, f32 fv);
int playerStateAttack(GameObject* obj, int state, f32 fv);
int playerState1D(int obj, struct PlayerState* state, f32 fv);
int playerStateIdle(int obj, int state, f32 fv);
int playerState08(GameObject* obj, int state, f32 fv);

#endif
