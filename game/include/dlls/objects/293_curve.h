#ifndef DLLS_OBJECTS_293_CURVE_H_
#define DLLS_OBJECTS_293_CURVE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "main/dll/dll_0015_curves.h"

int curve_func0B(void);
void curve_func0A(void);
int curve_getExtraSize(void);
int curve_getObjectTypeId(void);
void curve_free(void);
void curve_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void curve_init(GameObject* obj, RomCurveDef* placement);

extern ObjectDescriptor12 gCurveObjDescriptor;

#endif /* DLLS_OBJECTS_293_CURVE_H_ */
