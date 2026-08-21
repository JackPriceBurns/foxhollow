#ifndef DLLS_OBJECTS_513_WM_COLRISE_H_
#define DLLS_OBJECTS_513_WM_COLRISE_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "main/objseq.h"

typedef struct WmColumnRisePlacement WmColumnRisePlacement;

int WM_colrise_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate);
int WM_colrise_getExtraSize(void);
int WM_colrise_getObjectTypeId(void);
void WM_colrise_free(void);
void WM_colrise_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5, s8 visible);
void WM_colrise_hitDetect(void);
void WM_colrise_update(GameObject* obj);
void WM_colrise_init(GameObject* obj, const WmColumnRisePlacement* placement);
void WM_colrise_release(void);
void WM_colrise_initialise(void);

extern ObjectDescriptor gWM_colriseObjDescriptor;

#endif
