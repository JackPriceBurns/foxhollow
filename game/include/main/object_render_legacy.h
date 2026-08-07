#ifndef MAIN_OBJECT_RENDER_LEGACY_H_
#define MAIN_OBJECT_RENDER_LEGACY_H_

#include "main/object_render.h"

#define objRenderModelAndHitVolumes(obj, p2, p3, p4, p5, scale)                                                         \
    ((void (*)(int, int, int, int, int, f32))objRenderModelAndHitVolumes)(                                              \
        (obj), (p2), (p3), (p4), (p5), (scale))

#endif /* MAIN_OBJECT_RENDER_LEGACY_H_ */
