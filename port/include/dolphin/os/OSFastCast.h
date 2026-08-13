#ifndef _DOLPHIN_OSFASTCAST_H_
#define _DOLPHIN_OSFASTCAST_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline void OSInitFastCast(void) {}

static inline s16 __OSf32tos16(f32 value)
{
    if (value >= 32767.0f)
        return 32767;
    if (value <= -32768.0f)
        return -32768;
    return (s16)value;
}

static inline void OSf32tos16(f32* value, s16* out) { *out = __OSf32tos16(*value); }

static inline u8 __OSf32tou8(f32 value)
{
    if (value >= 255.0f)
        return 255;
    if (value <= 0.0f)
        return 0;
    return (u8)value;
}

static inline void OSf32tou8(f32* value, u8* out) { *out = __OSf32tou8(*value); }

static inline s8 __OSf32tos8(f32 value)
{
    if (value >= 127.0f)
        return 127;
    if (value <= -128.0f)
        return -128;
    return (s8)value;
}

static inline void OSf32tos8(f32* value, s8* out) { *out = __OSf32tos8(*value); }

static inline u16 __OSf32tou16(f32 value)
{
    if (value >= 65535.0f)
        return 65535;
    if (value <= 0.0f)
        return 0;
    return (u16)value;
}

static inline void OSf32tou16(f32* value, u16* out) { *out = __OSf32tou16(*value); }

static inline f32 __OSs8tof32(const s8* value) { return (f32)*value; }
static inline void OSs8tof32(const s8* value, f32* out) { *out = __OSs8tof32(value); }

static inline f32 __OSs16tof32(const s16* value) { return (f32)*value; }
static inline void OSs16tof32(const s16* value, f32* out) { *out = __OSs16tof32(value); }

static inline f32 __OSu8tof32(const u8* value) { return (f32)*value; }
static inline void OSu8tof32(const u8* value, f32* out) { *out = __OSu8tof32(value); }

static inline f32 __OSu16tof32(const u16* value) { return (f32)*value; }
static inline void OSu16tof32(const u16* value, f32* out) { *out = __OSu16tof32(value); }

#ifdef __cplusplus
}
#endif

#endif
