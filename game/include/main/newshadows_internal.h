#ifndef MAIN_NEWSHADOWS_INTERNAL_H_
#define MAIN_NEWSHADOWS_INTERNAL_H_

#include "global.h"
#include "game/objects/object.h"
#include "main/projected_shadow.h"
#include "main/texture.h"

typedef struct NewShadowEntry {
    u8 pad00[0x10];
    u8 isActive;
    u8 state;
    u8 pad12[0x2];
} NewShadowEntry;

typedef struct {
    GameObject* obj;
    f32 scale;
    u8 flags;
} NewShadowCaster;

typedef struct {
    /* GameCube IA8 texels are stored as bytes, alpha first. */
    u8 alpha;
    u8 intensity;
} NewShadowVectorTexel;

typedef struct {
    f32 x;
    f32 y;
} NewShadowVector2;

#define NEW_SHADOW_MAX_QUEUED_CASTERS 300
#define NEW_SHADOW_MAX_CASTERS        100
#define NEW_SHADOW_MAX_CAST_TEXTURES  8
#define NEW_SHADOW_FRAME_COUNT        3
#define NEW_SHADOW_ENTRY_COUNT        0x21

typedef ProjectedShadowTexture NewShadowCastSlot;

#define NEW_SHADOW_ENTRY_CAPACITY 0x25

#endif /* MAIN_NEWSHADOWS_INTERNAL_H_ */
