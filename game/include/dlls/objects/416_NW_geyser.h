#ifndef DLLS_OBJECTS_416_NW_GEYSER_H_
#define DLLS_OBJECTS_416_NW_GEYSER_H_

#include "dlls/object_descriptor.h"
#include "game/objects/object_fwd.h"
#include "main/objseq.h"

extern ObjectDescriptor gNWGeyserObjDescriptor;

int nwGeyser_processAnimEvents(GameObject* obj, int unusedArg, ObjSeqState* animUpdate);
void nwGeyser_free(GameObject* obj);
void nwGeyser_update(GameObject* obj);
void nwGeyser_init(GameObject* obj);

#endif /* DLLS_OBJECTS_416_NW_GEYSER_H_ */
