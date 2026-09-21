#ifndef H_MAIN_DLL_SNOWWORM_H
#define H_MAIN_DLL_SNOWWORM_H

#include "global.h"

void snowworm_update(struct GameObject* obj, u8* state);
void snowworm_applyReactionState(struct GameObject* obj, int* st);
void snowworm_init(struct GameObject* obj, int* st);

#endif /* H_MAIN_DLL_SNOWWORM_H */
