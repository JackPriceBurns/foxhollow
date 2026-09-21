#ifndef H_MAIN_DLL_DUSTER_WB_H
#define H_MAIN_DLL_DUSTER_WB_H

#include "global.h"

struct GameObject;

void wbUpdateEngaged(struct GameObject* obj, void* state);
void wbUpdateIdle(struct GameObject* obj, void* state);
void mutatedEbaUpdateEngaged(GameObject* obj, void* state);
void mutatedEbaUpdateIdle(GameObject* obj, void* state);
void mutatedEbaInit(GameObject* unused, void* state);

#endif /* H_MAIN_DLL_DUSTER_WB_H */
