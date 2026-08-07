#ifndef MAIN_GAMETEXT_API_H_
#define MAIN_GAMETEXT_API_H_

#include "types.h"

typedef struct TextDisplayState TextDisplayState;

void gameTextAppendStr(char* str, int box);
void gameTextQueueReveal(int textId, int arg);
void gameTextFreePhrase(int* phrase);
void gameTextMeasureStringBoundsAt(char* str, int boxIdx, int x, int y, int* outMinX, int* outMaxX, int* outMinY,
                                   int* outMaxY);
void gameTextMeasureById(int id, int x, int y, int* outMinX, int* outMaxX, int* outMinY, int* outMaxY);

char** gameTextWrapLines(char* str, f32 width, f32 height, int* outCount, f32* outLineH);

extern char* sMapDirectoryNameTable[74];

#endif /* MAIN_GAMETEXT_API_H_ */
