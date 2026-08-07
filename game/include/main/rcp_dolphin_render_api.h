#ifndef MAIN_RCP_DOLPHIN_RENDER_API_H_
#define MAIN_RCP_DOLPHIN_RENDER_API_H_

#include "types.h"
#include "dolphin/gx/GXStruct.h"
#include "main/texture.h"

typedef struct GameObject GameObject;
typedef struct MapRomListPage MapRomListPage;

int objShouldUnload(GameObject* obj);
void textureInitSecondaryGXTexObj(Texture* texture, GXTexObj* obj);
void Rcp_SetColorFilterEnabled(u32 x);
void ObjHits_ConvertHitPositionToWorld(GameObject* object, f32* position);
void mapInstantiateObjects(MapRomListPage* page, int mapId, int groupIndex, GameObject* parent);

#endif /* MAIN_RCP_DOLPHIN_RENDER_API_H_ */
