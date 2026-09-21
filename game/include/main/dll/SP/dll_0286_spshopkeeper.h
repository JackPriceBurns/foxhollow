#ifndef MAIN_DLL_SP_DLL_0286_SPSHOPKEEPER_H_
#define MAIN_DLL_SP_DLL_0286_SPSHOPKEEPER_H_

#include "game/objects/object.h"
#include "main/dll/baddie_state.h"
#include "main/objseq.h"

int ShopKeeper_SeqFn(GameObject* obj, int p2, ObjSeqState* seq, s8 advance);
f32 ShopKeeper_turnTowardPlayer(GameObject* obj, GameObject* player, int snap);
int ShopKeeper_moveToCurvePoint(GameObject* obj, BaddieState* baddie);
int ShopKeeper_waitForShopOpen(void);
int ShopKeeper_updateScarabGame(GameObject* obj);
int ShopKeeper_updateIdle(GameObject* obj, BaddieState* baddie);
int ShopKeeper_updateTracking(GameObject* obj, BaddieState* baddie);
int ShopKeeper_startVendorSequence(GameObject* obj);
int ShopKeeper_handlePromptChoice(GameObject* obj, void* param2, int dispatch);
void ShopKeeper_startScarabGame(GameObject* obj);

#endif
