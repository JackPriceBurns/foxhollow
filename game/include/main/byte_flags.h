#ifndef MAIN_BYTE_FLAGS_H_
#define MAIN_BYTE_FLAGS_H_

#include "global.h"

typedef struct ByteFlags
{
    /* Native little-endian compilers allocate these fields from the least
     * significant bit upward. Keep each name aligned with its mask value. */
    u8 b01 : 1;
    u8 b02 : 1;
    u8 b04 : 1;
    u8 b08 : 1;
    u8 b10 : 1;
    u8 b20 : 1;
    u8 b40 : 1;
    u8 b80 : 1;
} ByteFlags;

static inline u8 ByteFlags_GetRaw(const ByteFlags* flags)
{
    return *(const u8*)flags;
}

static inline void ByteFlags_SetRaw(ByteFlags* flags, u8 value)
{
    *(u8*)flags = value;
}

#endif /* MAIN_BYTE_FLAGS_H_ */
