#ifndef DLLS_OBJECTS_292_H_
#define DLLS_OBJECTS_292_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"

int DeathGas_getExtraSize(void);
void DeathGas_free(GameObject* obj);
void DeathGas_update(GameObject* obj);
void DeathGas_init(GameObject* obj);

extern ObjectDescriptor gDeathGasObjDescriptor;

#endif /* DLLS_OBJECTS_292_H_ */
