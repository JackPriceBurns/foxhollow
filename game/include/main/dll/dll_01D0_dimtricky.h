#ifndef MAIN_DLL_DLL_01D0_DIMTRICKY_H_
#define MAIN_DLL_DLL_01D0_DIMTRICKY_H_

#include "game/objects/object.h"

int dim_tricky_getExtraSize(void);
int dim_tricky_getObjectTypeId(void);
void dim_tricky_free(void);
void dim_tricky_render(GameObject* obj, int p2, int p3, int p4, int p5, s8 visible);
void dim_tricky_hitDetect(void);
void dim_tricky_update(GameObject* obj);
void dim_tricky_init(GameObject* obj);

#endif /* MAIN_DLL_DLL_01D0_DIMTRICKY_H_ */
