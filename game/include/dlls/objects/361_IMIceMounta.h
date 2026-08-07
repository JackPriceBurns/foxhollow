#ifndef DLLS_OBJECTS_361_IMICEMOUNTA_H_
#define DLLS_OBJECTS_361_IMICEMOUNTA_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "dlls/objects/430_SH_LevelCon.h"

struct ObjSeqState;

typedef struct IMIceMountainState {
    u8 eventState;
    u8 pad01[0x03];
    GameBitLatchState gameBitLatch;
    s8 warpCountdown;
    u8 pad09;
    s16 musicTrack;
    u8 mapAct;
    u8 pad0D[0x03];
    f32 warningTextTimer;
} IMIceMountainState;

STATIC_ASSERT(offsetof(IMIceMountainState, eventState) == 0x00);
STATIC_ASSERT(offsetof(IMIceMountainState, pad01) == 0x01);
STATIC_ASSERT(offsetof(IMIceMountainState, gameBitLatch) == 0x04);
STATIC_ASSERT(offsetof(IMIceMountainState, warpCountdown) == 0x08);
STATIC_ASSERT(offsetof(IMIceMountainState, pad09) == 0x09);
STATIC_ASSERT(offsetof(IMIceMountainState, musicTrack) == 0x0A);
STATIC_ASSERT(offsetof(IMIceMountainState, mapAct) == 0x0C);
STATIC_ASSERT(offsetof(IMIceMountainState, pad0D) == 0x0D);
STATIC_ASSERT(offsetof(IMIceMountainState, warningTextTimer) == 0x10);
STATIC_ASSERT(sizeof(IMIceMountainState) == 0x14);

void IMIceMountain_enterWorldMap(GameObject* obj);
void IMIceMountain_exitWorldMap(GameObject* obj, IMIceMountainState* state);
void IMIceMountain_updateEventState(GameObject* obj);
int IMIceMountain_sequenceCallback(GameObject* obj, int unused, const struct ObjSeqState* animUpdate);
int IMIceMountain_getExtraSize(void);
int IMIceMountain_getObjectTypeId(void);
void IMIceMountain_free(void);
void IMIceMountain_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void IMIceMountain_hitDetect(void);
void IMIceMountain_update(GameObject* obj);
void IMIceMountain_init(GameObject* obj);

extern ObjectDescriptor gIMIceMountainObjDescriptor;

#endif /* DLLS_OBJECTS_361_IMICEMOUNTA_H_ */
