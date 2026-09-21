#ifndef MAIN_DLL_ARW_DLL_02A4_H
#define MAIN_DLL_ARW_DLL_02A4_H

#include "global.h"
#include "game/objects/object.h"
#include "main/dll/ARW/dll_02A3.h"

int dll_2A4_getExtraSize_ret_12(void);
int dll_2A4_getObjectTypeId(void);
void dll_2A4_free_nop(void);
void dll_2A4_hitDetect_nop(void);
void dll_2A4_render(GameObject* obj, int p2, int p3, int p4, int p5);
void dll_2A4_update(GameObject* obj);
void dll_2A4_init(GameObject* obj);
void dll_2A4_release_nop(void);
void dll_2A4_initialise_nop(void);

#endif
