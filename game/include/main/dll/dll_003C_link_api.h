#ifndef MAIN_DLL_DLL_003C_LINK_API_H_
#define MAIN_DLL_DLL_003C_LINK_API_H_

#include "types.h"

u32 Link_update(void);
void setLinkNotRotated(void);
void setLinkIsRotated(void);
u16 linkGetSelectedItemId(void);
void Link_refreshOverlappingItemTimers(void);
void Link_scanItemVerticalBounds(void);

#endif /* MAIN_DLL_DLL_003C_LINK_API_H_ */
