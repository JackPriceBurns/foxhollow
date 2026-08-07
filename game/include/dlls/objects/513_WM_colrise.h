#ifndef DLLS_OBJECTS_513_WM_COLRISE_H_
#define DLLS_OBJECTS_513_WM_COLRISE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "game/objects/object_setup.h"
#include "main/objseq.h"

/* Only the accessed placement prefix is recovered; the complete retail width is not established. */
typedef struct WMColrisePlacementView {
    ObjPlacement base; /* 0x00: base.posY is the column's resting height */
    s8 rotationXByte;  /* 0x18: X rotation in 1/256 turns */
    u8 unknown19[5];   /* 0x19 */
    s16 riseGameBit;   /* 0x1E: rise-enable bit, or -1 */
} WMColrisePlacementView;

/* WM_colrise_getExtraSize allocates this complete four-byte state. */
typedef struct WMColriseState {
    s16 riseGameBit; /* 0x00 */
    s8 raiseTimer;   /* 0x02 */
    u8 unknown03;    /* 0x03 */
} WMColriseState;

STATIC_ASSERT(offsetof(WMColrisePlacementView, base) == 0x00);
STATIC_ASSERT(offsetof(WMColrisePlacementView, rotationXByte) == 0x18);
STATIC_ASSERT(offsetof(WMColrisePlacementView, unknown19) == 0x19);
STATIC_ASSERT(offsetof(WMColrisePlacementView, riseGameBit) == 0x1E);

STATIC_ASSERT(offsetof(WMColriseState, riseGameBit) == 0x00);
STATIC_ASSERT(offsetof(WMColriseState, raiseTimer) == 0x02);
STATIC_ASSERT(offsetof(WMColriseState, unknown03) == 0x03);
STATIC_ASSERT(sizeof(WMColriseState) == 0x04);

int WM_colrise_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate);
int WM_colrise_getExtraSize(void);
int WM_colrise_getObjectTypeId(void);
void WM_colrise_free(void);
void WM_colrise_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void WM_colrise_hitDetect(void);
void WM_colrise_update(GameObject* obj);
void WM_colrise_init(GameObject* obj, const WMColrisePlacementView* placement);
void WM_colrise_release(void);
void WM_colrise_initialise(void);

extern ObjectDescriptor gWM_colriseObjDescriptor;

#endif /* DLLS_OBJECTS_513_WM_COLRISE_H_ */
