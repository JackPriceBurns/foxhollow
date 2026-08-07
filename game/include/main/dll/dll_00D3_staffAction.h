#ifndef MAIN_DLL_STAFFACTION_H_
#define MAIN_DLL_STAFFACTION_H_

#include "game/objects/object.h"
#include "types.h"

typedef struct DllD3Placement DllD3Placement;

void dll_D3_initialise(void);
void dll_D3_release_nop(void);
void dll_D3_init(GameObject* obj, DllD3Placement* def, int flag);
void dll_D3_update(GameObject* obj);
void dll_D3_hitDetect_nop(void);
void dll_D3_render(GameObject* obj, int p2, int p3, int p4, int p5, s8 visible);
void dll_D3_free(GameObject* obj);
int dll_D3_getObjectTypeId(void);
int dll_D3_getExtraSize_ret_1188(void);

/* extern-cleanup: defining-file public prototypes */
void LandedArwing_OnPlayerContact(GameObject* obj, GameObject* otherObj);

extern int gStaffActionHitReactionMoves[30];
extern u8 gStaffActionHitReactionDamage[32];

#endif /* MAIN_DLL_STAFFACTION_H_ */
