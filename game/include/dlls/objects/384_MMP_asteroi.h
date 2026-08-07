#ifndef DLLS_OBJECTS_384_MMP_ASTEROI_H_
#define DLLS_OBJECTS_384_MMP_ASTEROI_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "main/dll/partfx_interface.h"
#include "main/objseq.h"

typedef struct MMPAsteroidReState {
    u8 eventFlags; /* effect requests plus the sequence-tick latch */
    u8 phase;      /* initialized from gamebit 0x87B */
    u8 intensity;  /* gamebit 0x88C, or 1 while gamebit 0xD52 is set */
    u8 unknown03;
    f32 gameBitClearTimer; /* clears gamebit 0x88B when it expires */
    f32 periodicFxTimer;
    f32 baseY; /* lower motion bound and dust-height reference */
    f32 unknown10;
    u16 bobPhase;
    u16 rollPhase;
    u16 pitchPhase;
    u8 unknown1A[0x1C - 0x1A];
} MMPAsteroidReState;

STATIC_ASSERT(sizeof(MMPAsteroidReState) == 0x1C);
STATIC_ASSERT(offsetof(MMPAsteroidReState, eventFlags) == 0x00);
STATIC_ASSERT(offsetof(MMPAsteroidReState, phase) == 0x01);
STATIC_ASSERT(offsetof(MMPAsteroidReState, intensity) == 0x02);
STATIC_ASSERT(offsetof(MMPAsteroidReState, unknown03) == 0x03);
STATIC_ASSERT(offsetof(MMPAsteroidReState, gameBitClearTimer) == 0x04);
STATIC_ASSERT(offsetof(MMPAsteroidReState, periodicFxTimer) == 0x08);
STATIC_ASSERT(offsetof(MMPAsteroidReState, baseY) == 0x0C);
STATIC_ASSERT(offsetof(MMPAsteroidReState, unknown10) == 0x10);
STATIC_ASSERT(offsetof(MMPAsteroidReState, bobPhase) == 0x14);
STATIC_ASSERT(offsetof(MMPAsteroidReState, rollPhase) == 0x16);
STATIC_ASSERT(offsetof(MMPAsteroidReState, pitchPhase) == 0x18);
STATIC_ASSERT(offsetof(MMPAsteroidReState, unknown1A) == 0x1A);

extern f32 gMMPAsteroidIntensityHeightTable[4];
extern PartFxSpawnParams gMMPAsteroidDustSpawnParams;
extern int gMMPAsteroidDustHeightParam;
extern ObjectDescriptor gMMPAsteroidReObjDescriptor;

int mmpAsteroidRe_processAnimEvents(GameObject* obj, int unusedArg2, ObjSeqState* animUpdate);
int mmpAsteroidRe_getExtraSize(void);
int mmpAsteroidRe_getObjectTypeId(void);
void mmpAsteroidRe_free(void);
void mmpAsteroidRe_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void mmpAsteroidRe_hitDetect(void);
void mmpAsteroidRe_update(GameObject* obj);
void mmpAsteroidRe_init(GameObject* obj);
void mmpAsteroidRe_release(void);
void mmpAsteroidRe_initialise(void);

#endif /* DLLS_OBJECTS_384_MMP_ASTEROI_H_ */
