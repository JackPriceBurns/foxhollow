#ifndef MAIN_LIGHTMAP_API_H_
#define MAIN_LIGHTMAP_API_H_

#include "types.h"
#include "global.h"
#include "main/map_block.h"
#include "main/lightmap_text_color_api.h"

typedef struct LightmapVertex {
    s16 x;
    s16 y;
    s16 z;
    s16 pad;
    s16 s;
    s16 t;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} LightmapVertex;

typedef struct LightmapTriangle {
    u8 flags;
    u8 vertexIndices[3];
    s16 textureCoordinates[3][2];
} LightmapTriangle;

STATIC_ASSERT(sizeof(LightmapVertex) == 0x10);
STATIC_ASSERT(offsetof(LightmapVertex, s) == 0x08);
STATIC_ASSERT(offsetof(LightmapVertex, r) == 0x0c);

STATIC_ASSERT(offsetof(LightmapTriangle, flags) == 0x00);
STATIC_ASSERT(offsetof(LightmapTriangle, vertexIndices) == 0x01);
STATIC_ASSERT(offsetof(LightmapTriangle, textureCoordinates) == 0x04);
STATIC_ASSERT(sizeof(LightmapTriangle) == 0x10);

MapBlockData* mapGetBlockAtPos(int x, int y, int layer);
void* RomList_GetLoadedPages(void);
MapBlockData* mapGetBlock(int index);
int coordsToMapCell(f32 x, f32 z);
int isInBounds(f32 x, f32 z);
int isWidescreen(void);
int setWidescreen(u8 enabled);
u8 isOvercast(void);
u32 shouldDrawShadows(void);
int shouldDrawClouds(void);
int isDrawDistanceEnabled(void);
int objPosToMapBlockIdx(f32 x, f32 y, f32 z);
void mapGetBlockOriginForPos(f32 x, f32 y, f32 z, f32* outX, f32* outZ);
void setPendingMapLoad(int pending);
void lightmapDrawTriangleList(const void* vertexBase, u8* triList, int triCount);
void lightmapObjectRenderBegin(int arg0, int arg1);
void lightmapObjectRenderEnd(int arg0, int arg1);
void setTitleScreenActive(int active);
void setIsOvercast(int value);

void setStarsHidden(int v);

s8* mapGetBlockIdx(int layer);

extern u8 colorScale;

#endif /* MAIN_LIGHTMAP_API_H_ */
