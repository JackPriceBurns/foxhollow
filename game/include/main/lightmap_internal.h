#ifndef MAIN_LIGHTMAP_INTERNAL_H_
#define MAIN_LIGHTMAP_INTERNAL_H_

#include "main/dll/ppcwgpipe_struct.h"
#include "types.h"

typedef struct EnvironmentUpdateInterface {
    void (*create)(void);
    void (*destroy)(void);
    void (*update)(void);
} EnvironmentUpdateInterface;

extern EnvironmentUpdateInterface** gEnvironmentUpdateInterface;

typedef struct {
    uintptr_t a;
    uintptr_t b;
    u32 key;
    u32 d;
} LightSortEntry;

typedef struct {
    uintptr_t a, b;
    u32 c, d;
} LightmapQEnt;

typedef struct MapLayerBuffers {
    u8 reserved[0x41cc];
    u8* cellStates[5];
    u8* blockDescriptors[5];
    u8* blockIndices[5];
} MapLayerBuffers;

#endif /* MAIN_LIGHTMAP_INTERNAL_H_ */
