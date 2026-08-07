#ifndef MAIN_CAMERA_SHAKE_API_H_
#define MAIN_CAMERA_SHAKE_API_H_

#include "types.h"

void CameraShake_SetOffset(f32 offsetY);
void CameraShake_StartDampened(f32 amplitude, f32 frequency, f32 damping);
void CameraShake_ApplyRadial(f32 x, f32 y, f32 z, f32 radius, f32 intensity);
int CameraShake_IsEnabled(void);
void CameraShake_Disable(void);
void CameraShake_Enable(void);

#endif /* MAIN_CAMERA_SHAKE_API_H_ */
