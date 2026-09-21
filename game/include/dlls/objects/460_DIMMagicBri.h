#ifndef DLLS_OBJECTS_460_DIMMAGICBRI_H_
#define DLLS_OBJECTS_460_DIMMAGICBRI_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"

#define DIM_MAGIC_BRIDGE_SEGMENT_COUNT 10

typedef struct ObjSeqState ObjSeqState;

/* The sole active-target retail placement is a fixed 9-word (0x24-byte) record. */
typedef struct DimMagicBridgePlacement {
    ObjPlacement base;
    s8 rotationXByte;
    u8 unknown19[0xB];
} DimMagicBridgePlacement;

/* getExtraSize() allocates 0x68 bytes; segmentCount is initialized to 10. */
typedef struct DimMagicBridgeState {
    f32 minVertexY;
    f32 segmentSortValues[DIM_MAGIC_BRIDGE_SEGMENT_COUNT];
    u8 unknown2C[0x14];
    u8 segmentLit[DIM_MAGIC_BRIDGE_SEGMENT_COUNT];
    u8 unknown4A[5];
    u8 segmentCount;
    u8 segmentGlow[DIM_MAGIC_BRIDGE_SEGMENT_COUNT];
    u8 unknown5A[5];
    u8 ignited;
    u16 wavePhase;
    u16 wavePhaseB;
    s16 igniteTimer;
    u8 unknown66[2];
} DimMagicBridgeState;

STATIC_ASSERT(offsetof(DimMagicBridgePlacement, base) == 0x00);
STATIC_ASSERT(offsetof(DimMagicBridgePlacement, rotationXByte) == 0x18);
STATIC_ASSERT(offsetof(DimMagicBridgePlacement, unknown19) == 0x19);
STATIC_ASSERT(sizeof(DimMagicBridgePlacement) == 0x24);

STATIC_ASSERT(offsetof(DimMagicBridgeState, minVertexY) == 0x00);
STATIC_ASSERT(offsetof(DimMagicBridgeState, segmentSortValues) == 0x04);
STATIC_ASSERT(offsetof(DimMagicBridgeState, unknown2C) == 0x2C);
STATIC_ASSERT(offsetof(DimMagicBridgeState, segmentLit) == 0x40);
STATIC_ASSERT(offsetof(DimMagicBridgeState, unknown4A) == 0x4A);
STATIC_ASSERT(offsetof(DimMagicBridgeState, segmentCount) == 0x4F);
STATIC_ASSERT(offsetof(DimMagicBridgeState, segmentGlow) == 0x50);
STATIC_ASSERT(offsetof(DimMagicBridgeState, unknown5A) == 0x5A);
STATIC_ASSERT(offsetof(DimMagicBridgeState, ignited) == 0x5F);
STATIC_ASSERT(offsetof(DimMagicBridgeState, wavePhase) == 0x60);
STATIC_ASSERT(offsetof(DimMagicBridgeState, wavePhaseB) == 0x62);
STATIC_ASSERT(offsetof(DimMagicBridgeState, igniteTimer) == 0x64);
STATIC_ASSERT(offsetof(DimMagicBridgeState, unknown66) == 0x66);
STATIC_ASSERT(sizeof(DimMagicBridgeState) == 0x68);

void dimmagicbridge_updateVertexWave(GameObject* obj, u8* stateBytes);
void dimmagicbridge_scrollTextureChannels(GameObject* obj, u8* stateBytes);
int dimmagicbridge_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate);
int dimmagicbridge_getExtraSize(void);
int dimmagicbridge_getObjectTypeId(void);
void dimmagicbridge_free(void);
void dimmagicbridge_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void dimmagicbridge_hitDetect(void);
void dimmagicbridge_update(GameObject* obj);
void dimmagicbridge_init(GameObject* obj, const DimMagicBridgePlacement* placement);
void dimmagicbridge_release(void);
void dimmagicbridge_initialise(void);

extern ObjectDescriptor gDIMMagicBridgeObjDescriptor;

#endif /* DLLS_OBJECTS_460_DIMMAGICBRI_H_ */
