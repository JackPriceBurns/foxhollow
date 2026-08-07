#ifndef DLLS_OBJECTS_481_DIM_BOSSGUT_H_
#define DLLS_OBJECTS_481_DIM_BOSSGUT_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "main/objseq.h"

int DIM_BossGut_SeqFn(GameObject* obj, int unused, ObjSeqState* animUpdate);
int DIM_BossGut_getExtraSize(void);
int DIM_BossGut_getObjectTypeId(void);
void DIM_BossGut_free(void);
void DIM_BossGut_render(GameObject* obj, int renderArg2, int renderArg3, int renderArg4, int renderArg5,
                        s8 shouldRender);
void DIM_BossGut_hitDetect(void);
void DIM_BossGut_update(void);
void DIM_BossGut_init(GameObject* obj);
void DIM_BossGut_release(void);
void DIM_BossGut_initialise(void);

extern ObjectDescriptor gDIM_BossGutObjDescriptor;

#endif /* DLLS_OBJECTS_481_DIM_BOSSGUT_H_ */
