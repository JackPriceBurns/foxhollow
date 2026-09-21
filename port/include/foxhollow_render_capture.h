#ifndef FOXHOLLOW_RENDER_CAPTURE_H
#define FOXHOLLOW_RENDER_CAPTURE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

const uint8_t* fhGXGetFifoData(void);
uint32_t fhGXGetFifoSize(void);
uint32_t* fhGXGetFifoSizeAddress(void);
void* fhGXGetShadowAddress(void);

#ifdef __cplusplus
}
#endif

#endif
