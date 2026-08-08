#ifndef MAIN_DLL_PLAYER_H_
#define MAIN_DLL_PLAYER_H_

#include "game/objects/object_fwd.h"
#include "global.h"

struct PlayerState;

int playerUpdateAirborneMotion(GameObject* obj, int inner, int state);
void playerUpdate(GameObject* obj);
void playerProcessMessages(GameObject* obj, int inner, int state);
void playerProcessHitResponse(GameObject* obj, int inner, int state);
struct GameObject;
void playerDoHitDetection(struct GameObject* obj);
int playerCheckCommonTransitions(GameObject* obj, int state, int inner, f32 fv);

int playerCheckIfClimbingOntoWall(GameObject* obj, int state, int state2, void* out, f32 fv, u32 mask);
int playerStateMoving(GameObject* obj, int state, f32 fv);
int playerStateOnLadder(GameObject* obj, int state);
int playerStateClimbWall(GameObject* obj, int state);
int playerStateAimStaff(GameObject* obj, int state, f32 fv);
int playerStateAttack(GameObject* obj, int state, f32 fv);
int playerState1D(GameObject* obj, struct PlayerState* state, f32 fv);
int playerStateIdle(GameObject* obj, int state, f32 fv);
int playerState08(GameObject* obj, int state, f32 fv);

#endif
