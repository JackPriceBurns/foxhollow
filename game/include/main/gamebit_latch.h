#ifndef MAIN_GAMEBIT_LATCH_H_
#define MAIN_GAMEBIT_LATCH_H_

#include "global.h"

void GameBitLatch_Update(int* activeMask, int mask, s16 clearIfSetBit, s16 clearIfClearBit, s16 latchBit, int musicId);
void GameBitLatch_UpdateInverted(int* activeMask, int mask, s16 clearIfSetBit, s16 clearIfClearBit, s16 latchBit,
                                 int musicId);
void GameBitLatch_UpdateByte(u8* activeMask, u8 mask, s16 clearIfSetBit, s16 clearIfClearBit, s16 latchBit,
                             int musicId);
void GameBitLatch_UpdateByteInverted(u8* activeMask, u8 mask, s16 clearIfSetBit, s16 clearIfClearBit, s16 latchBit,
                                     int musicId);

#endif
