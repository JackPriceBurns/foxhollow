#include "dlls/object_descriptor.h"
#include "dolphin/types.h"
#include "main/dll/dll_0004_dummy04.h"

void Dummy04_func14_nop(void)
{
}
void Dummy04_func26_nop(void)
{
}
void Dummy04_func25_nop(void)
{
}
int Dummy04_func24_ret_0(void)
{
    return 0;
}
void Dummy04_func23_nop(void)
{
}
int Dummy04_func22_ret_127(void)
{
    return 0x7f;
}
int Dummy04_func21_ret_0(void)
{
    return 0;
}
void Dummy04_func20_nop(void)
{
}
void Dummy04_func1F_nop(void)
{
}
void Dummy04_func1E_nop(void)
{
}
int Dummy04_func1D_ret_0(void)
{
    return 0;
}
void Dummy04_func1C_nop(void)
{
}
void Dummy04_func1B_nop(void)
{
}
void Dummy04_func1A_nop(void)
{
}
void Dummy04_func19_nop(void)
{
}
void Dummy04_func18_nop(void)
{
}
void Dummy04_func17_nop(void)
{
}
void Dummy04_func16_nop(void)
{
}
void Dummy04_onSetupPlayer(void)
{
}
void Dummy04_func15_nop(void* obj)
{
}
void Dummy04_func13_nop(void)
{
}
void Dummy04_func12_nop(void)
{
}
int Dummy04_func11_ret_0(int arg0, int arg1)
{
    return 0;
}
void Dummy04_func10_nop(void)
{
}
int Dummy04_func0F_ret_0(void)
{
    return 0;
}
void Dummy04_func0E_nop(void)
{
}
int Dummy04_func0D_ret_0(int arg0)
{
    return 0;
}
void Dummy04_func0C_nop(void)
{
}
int Dummy04_func0B_ret_0(void)
{
    return 0;
}
int Dummy04_func0A_ret_0(int arg0)
{
    return 0;
}
void Dummy04_onSelectSave(int arg0, int arg1, int arg2, int arg3, int arg4)
{
}
void Dummy04_func08_nop(void)
{
}
void Dummy04_func07_nop(void* obj)
{
}
int Dummy04_func05_ret_0(void* obj, u16 arg1, int arg2, int arg3, int arg4)
{
    return 0;
}
void Dummy04_func04_nop(void* obj, int arg1, int arg2, int arg3, int arg4)
{
}
int Dummy04_func03_ret_m1(void)
{
    return -1;
}
void Dummy04_release(void)
{
}
void Dummy04_initialise(void)
{
}
typedef struct Dummy04DllInterface {
    u32 reserved0;
    u32 reserved1;
    u32 reserved2;
    u32 slotCountAndFlags;
    ObjectDescriptorCallback initialise;
    ObjectDescriptorCallback release;
    ObjectDescriptorCallback slot02;
    ObjectDescriptorCallback slot03;
    ObjectDescriptorCallback slot04;
    ObjectDescriptorCallback slot05;
    ObjectDescriptorCallback onSetupPlayer;
    ObjectDescriptorCallback slot07;
    ObjectDescriptorCallback slot08;
    ObjectDescriptorCallback onSelectSave;
    ObjectDescriptorCallback slot0A;
    ObjectDescriptorCallback slot0B;
    ObjectDescriptorCallback slot0C;
    ObjectDescriptorCallback slot0D;
    ObjectDescriptorCallback slot0E;
    ObjectDescriptorCallback slot0F;
    ObjectDescriptorCallback slot10;
    ObjectDescriptorCallback slot11;
    ObjectDescriptorCallback slot12;
    ObjectDescriptorCallback slot13;
    ObjectDescriptorCallback slot14;
    ObjectDescriptorCallback slot15;
    ObjectDescriptorCallback slot16;
    ObjectDescriptorCallback slot17;
    ObjectDescriptorCallback slot18;
    ObjectDescriptorCallback slot19;
    ObjectDescriptorCallback slot1A;
    ObjectDescriptorCallback slot1B;
    ObjectDescriptorCallback slot1C;
    ObjectDescriptorCallback slot1D;
    ObjectDescriptorCallback slot1E;
    ObjectDescriptorCallback slot1F;
    ObjectDescriptorCallback slot20;
    ObjectDescriptorCallback slot21;
    ObjectDescriptorCallback slot22;
    ObjectDescriptorCallback slot23;
    ObjectDescriptorCallback slot24;
    ObjectDescriptorCallback slot25;
    ObjectDescriptorCallback slot26;
    ObjectDescriptorCallback slot27;
    ObjectDescriptorCallback slot28;
    ObjectDescriptorCallback slot29;
    ObjectDescriptorCallback slot2A;
    ObjectDescriptorCallback slot2B;
} Dummy04DllInterface;

Dummy04DllInterface Dummy04_funcs = {
    0,
    0,
    0,
    0x002b0000,
    (ObjectDescriptorCallback)Dummy04_initialise,
    (ObjectDescriptorCallback)Dummy04_release,
    0,
    (ObjectDescriptorCallback)Dummy04_func03_ret_m1,
    (ObjectDescriptorCallback)Dummy04_func04_nop,
    (ObjectDescriptorCallback)Dummy04_func05_ret_0,
    (ObjectDescriptorCallback)Dummy04_onSetupPlayer,
    (ObjectDescriptorCallback)Dummy04_func07_nop,
    (ObjectDescriptorCallback)Dummy04_func08_nop,
    (ObjectDescriptorCallback)Dummy04_onSelectSave,
    (ObjectDescriptorCallback)Dummy04_func0A_ret_0,
    (ObjectDescriptorCallback)Dummy04_func0B_ret_0,
    (ObjectDescriptorCallback)Dummy04_func0C_nop,
    (ObjectDescriptorCallback)Dummy04_func0D_ret_0,
    (ObjectDescriptorCallback)Dummy04_func0E_nop,
    (ObjectDescriptorCallback)Dummy04_func0F_ret_0,
    (ObjectDescriptorCallback)Dummy04_func10_nop,
    (ObjectDescriptorCallback)Dummy04_func11_ret_0,
    (ObjectDescriptorCallback)Dummy04_func12_nop,
    (ObjectDescriptorCallback)Dummy04_func13_nop,
    (ObjectDescriptorCallback)Dummy04_func14_nop,
    (ObjectDescriptorCallback)Dummy04_func15_nop,
    (ObjectDescriptorCallback)Dummy04_func16_nop,
    (ObjectDescriptorCallback)Dummy04_func17_nop,
    (ObjectDescriptorCallback)Dummy04_func18_nop,
    (ObjectDescriptorCallback)Dummy04_func19_nop,
    (ObjectDescriptorCallback)Dummy04_func1A_nop,
    (ObjectDescriptorCallback)Dummy04_func1B_nop,
    (ObjectDescriptorCallback)Dummy04_func1C_nop,
    (ObjectDescriptorCallback)Dummy04_func1D_ret_0,
    (ObjectDescriptorCallback)Dummy04_func1E_nop,
    (ObjectDescriptorCallback)Dummy04_func1F_nop,
    (ObjectDescriptorCallback)Dummy04_func20_nop,
    (ObjectDescriptorCallback)Dummy04_func21_ret_0,
    (ObjectDescriptorCallback)Dummy04_func22_ret_127,
    (ObjectDescriptorCallback)Dummy04_func23_nop,
    (ObjectDescriptorCallback)Dummy04_func24_ret_0,
    (ObjectDescriptorCallback)Dummy04_func25_nop,
    (ObjectDescriptorCallback)Dummy04_func26_nop,
    0,
    0,
    0,
    0,
    0,
};
