#ifndef MAIN_DLL_MODGFX_H_
#define MAIN_DLL_MODGFX_H_

#include "main/dll/projgfx_interface.h"
#include "dlls/object_descriptor.h"

RESOURCE_DESCRIPTOR_TYPE(ProjgfxDllInterface, ProjgfxInterface);

extern ProjgfxDllInterface projgfx_funcs;

#define PROJGFX_SPAWN_FLAG_USE_ATTACHED_SOURCE 0x200000

void projgfx_release_doUnsupported(void);
int projgfx_rayhit_doUnsupported(void);
int projgfx_setzscale_doUnsupported(void);
int projgfx_func04_ret_m1(void);
void projgfx_func05_nop(void);
void projgfx_func06_nop(void);
void projgfx_func07_nop(void);
int projgfx_getObjectTypeId(void);
void projgfx_onMapSetup(void);
void projgfx_initialise(void);

#endif /* MAIN_DLL_MODGFX_H_ */
