#ifndef MAIN_DLL_CURVE_WALKER_H_
#define MAIN_DLL_CURVE_WALKER_H_

#include "global.h"
#include "main/curve_types.h"

/* rom-curve walker record (the f32* "state" the RomCurve_* family walks) -
 * sits at the head of curve-following extra blocks (objfsa.c census, lifted
 * per the deref-cleanup wave). Kept in its own header so TUs with legacy
 * arity-0 externs (objfsa.c drift bodies) can take the typedef without
 * curves.h's prototype namespace; curves.h includes this, so its consumers
 * see RomCurveWalker as before. */
typedef struct RomCurveWalker {
    Curve curve;
    void* node9C;
    void* nodeA0;  /* current node */
    void* nodeA4;  /* next node */
    f32 hermX[4];  /* 0xA8: hermite endpoints+tangents, X */
    f32 hermX2[4]; /* 0xB8: previous-segment X set */
    f32 hermY[4];  /* 0xC8 */
    f32 hermY2[4]; /* 0xD8 */
    f32 hermZ[4];  /* 0xE8 */
    f32 hermZ2[4]; /* 0xF8 */
} RomCurveWalker;

STATIC_ASSERT(offsetof(RomCurveWalker, curve) == 0x00);
STATIC_ASSERT(offsetof(RomCurveWalker, hermX) == 0xA8);
STATIC_ASSERT(sizeof(RomCurveWalker) == 0x108);

#endif /* MAIN_DLL_CURVE_WALKER_H_ */
