#ifndef MAIN_FRUSTUM_H_
#define MAIN_FRUSTUM_H_

#include "types.h"
#include "main/vec_types.h"

typedef struct FrustumPlane {
    f32 normalX;
    f32 normalY;
    f32 normalZ;
    f32 distance;
    u8 aabbCornerIndex;
    u8 pad[3];
} FrustumPlane;

extern FrustumPlane gViewFrustumPlanes[];
extern FrustumPlane gPlayerRelativeFrustumPlanes[];

void frustumPlanes_updateAabbCornerIndices(FrustumPlane *planes, int count);
int ViewFrustum_IsSphereVisible(float *center, float radius);

#endif /* MAIN_FRUSTUM_H_ */
