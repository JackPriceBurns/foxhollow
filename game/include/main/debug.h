#ifndef MAIN_DEBUG_H_
#define MAIN_DEBUG_H_

#include "types.h"

void debugPrintf(char* fmt, ...);
void logPrintf(char* fmt, ...);
void debugPrintSetColor(u8 r, u8 g, u8 b, u8 a);

extern u8 enableDebugText;

#endif /* MAIN_DEBUG_H_ */
