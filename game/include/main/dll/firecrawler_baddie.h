#ifndef H_MAIN_DLL_FIRECRAWLER_BADDIE_H
#define H_MAIN_DLL_FIRECRAWLER_BADDIE_H

#include "global.h"

void crawler_update(struct GameObject* obj, u8* state);
void crawler_updateB(struct GameObject* obj, u8* state);
void crawler_updateC(struct GameObject* obj, u8* state);
void crawler_initModelVariant(struct GameObject* obj, u8* state);

#endif /* H_MAIN_DLL_FIRECRAWLER_BADDIE_H */
