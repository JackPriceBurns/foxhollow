#ifndef MAIN_DLL_TRICKYCURVE_H_
#define MAIN_DLL_TRICKYCURVE_H_

#include "game/objects/object.h"
#include "main/objseq.h"

int TrickyCurve_getExtraSize(void);
int TrickyCurve_getObjectTypeId(void);
void TrickyCurve_free(GameObject* obj);
void TrickyCurve_render(void);
void TrickyCurve_hitDetect(void);
void TrickyCurve_update(GameObject* obj);
void TrickyCurve_init(GameObject* obj, u8* def);
void TrickyCurve_release(void);
void TrickyCurve_initialise(void);
void TrickyCurve_updateBurstTrigger(GameObject* obj);

#endif /* MAIN_DLL_TRICKYCURVE_H_ */
