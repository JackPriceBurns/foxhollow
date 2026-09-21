#ifndef MUSYX_ENDIAN_H_
#define MUSYX_ENDIAN_H_

#include "types.h"

static inline u32 musyxReadBE32(const void* ptr) {
    const u8* bytes = ptr;
    return ((u32)bytes[0] << 24) | ((u32)bytes[1] << 16) | ((u32)bytes[2] << 8) | bytes[3];
}

#endif
