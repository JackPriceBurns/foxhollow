#ifndef MAIN_DLL_SP_DLL_0285_SPSHOP_H_
#define MAIN_DLL_SP_DLL_0285_SPSHOP_H_

#include "game/objects/object.h"
#include "types.h"

typedef struct ShopBuyItemState
{
    s8 unk0;      /* 0x0 */
    s8 itemIndex; /* 0x1 shop item type: purchase-effect switch + items-table index */
    u8 pad2[0x4 - 0x2];
    u8 unk4;
    u8 pad5[0x56 - 0x5];
    u8 unk56;
    u8 pad57[0x6E - 0x57];
    s16 unk6E;
    u8 pad70[0x90 - 0x70];
    u8 unk90;
    u8 pad91[0x9B0 - 0x91];
    s32 unk9B0;
    u8 pad9B4[0x9D6 - 0x9B4];
    u8 unk9D6;
    u8 pad9D7[0x9D8 - 0x9D7];
} ShopBuyItemState;

typedef struct ShopItemRow
{
    u8 price;      /* 0x0 "P$" */
    u8 discount[3]; /* 0x1 "D1".."D3"; one is picked at random into minPrice */
    u8 specialPrice; /* 0x4 */
    u8 minPrice;   /* 0x5 */
    s16 availBit;  /* 0x6 "available" GameBit slot (-1 = always available) */
    s16 boughtBit; /* 0x8 "bought" GameBit slot (-1 = none) */
    s16 textId;    /* 0xa */
} ShopItemRow;

/* gShopObjDescriptor from slot02 onwards: the export table other objects reach
   through obj->anim.dll. */
typedef struct ShopInterface
{
    void* pad00[8];
    s32 (*getStateField0)(GameObject* shop);
    void (*playSequence)(GameObject* shop, int playSequence, int sequenceIndex);
    int (*isItemAvailable)(GameObject* shop, int slot);
    int (*isItemBought)(GameObject* shop, int slot);
    int (*getItemMinPrice)(GameObject* shop, int slot);
    int (*getItemSpecialPrice)(GameObject* shop, int slot);
    int (*getItemPrice)(GameObject* shop, int slot);
    int (*getItemTextId)(GameObject* shop, int slot);
    void (*setItemIndex)(GameObject* shop, int slot);
    s32 (*getItemIndex)(GameObject* shop);
    void (*buyItem)(GameObject* shop, int price);
    void (*func15)(GameObject* shop, int v);
    void (*func16)(GameObject* shop, int delta3, int delta2);
    void (*func17)(GameObject* shop, int* out_b3, int* out_b2, int* out_b4);
} ShopInterface;

#define SHOP_INTERFACE(shop) ((ShopInterface*)*((GameObject*)(shop))->anim.dll)

STATIC_ASSERT(offsetof(ShopInterface, getStateField0) == 0x20);
STATIC_ASSERT(offsetof(ShopInterface, playSequence) == 0x24);
STATIC_ASSERT(offsetof(ShopInterface, isItemAvailable) == 0x28);
STATIC_ASSERT(offsetof(ShopInterface, isItemBought) == 0x2C);
STATIC_ASSERT(offsetof(ShopInterface, getItemMinPrice) == 0x30);
STATIC_ASSERT(offsetof(ShopInterface, getItemSpecialPrice) == 0x34);
STATIC_ASSERT(offsetof(ShopInterface, getItemPrice) == 0x38);
STATIC_ASSERT(offsetof(ShopInterface, getItemTextId) == 0x3C);
STATIC_ASSERT(offsetof(ShopInterface, setItemIndex) == 0x40);
STATIC_ASSERT(offsetof(ShopInterface, getItemIndex) == 0x44);
STATIC_ASSERT(offsetof(ShopInterface, buyItem) == 0x48);
STATIC_ASSERT(offsetof(ShopInterface, func15) == 0x4C);
STATIC_ASSERT(offsetof(ShopInterface, func16) == 0x50);
STATIC_ASSERT(offsetof(ShopInterface, func17) == 0x54);
STATIC_ASSERT(sizeof(ShopInterface) == 0x58);

void shop_func17(GameObject* obj, int* out_b3, int* out_b2, int* out_b4);
void shop_func16(GameObject* obj, int p2, int p3);
void shop_func15(GameObject* obj, int v);
void shop_buyItem(GameObject* obj, int price);
s32 shop_getItemIndex(GameObject* obj);
void shop_setItemIndex(GameObject* obj, int v);
s16 shop_getItemTextId(GameObject* obj, int idx);
int shop_getItemPrice(GameObject* obj, int idx);
u8 shop_getItemSpecialPrice(GameObject* obj, int idx);
u8 shop_getItemMinPrice(GameObject* obj, int idx);
int shop_isItemBought(GameObject* obj, int idx);
int shop_isItemAvailable(GameObject* obj, int idx);
void shop_playSequence(GameObject* obj, int playSequence, int sequenceIndex);
s32 shop_getStateField0(GameObject* obj);
int shop_getExtraSize(void);
int shop_getObjectTypeId(void);
void shop_free(GameObject* obj);
void shop_render(GameObject* obj, int p2, int p3, int p4, int p5, s8 visible);
void shop_hitDetect(void);
void shop_update(GameObject* obj);
void shop_init(GameObject* obj, int objDef);
void shop_release(void);
void shop_initialise(void);

#endif /* MAIN_DLL_SP_DLL_0285_SPSHOP_H_ */
