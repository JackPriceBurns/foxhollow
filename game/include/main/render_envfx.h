#ifndef MAIN_RENDER_ENVFX_H_
#define MAIN_RENDER_ENVFX_H_

#include "types.h"

int getEnvfxActImmediately(void* source, void* target, u16 index, int flags);
int getEnvfxAct(void* source, void* target, u16 index, int flags);

extern int gSky2EnvfxActIndex;

#endif /* MAIN_RENDER_ENVFX_H_ */
