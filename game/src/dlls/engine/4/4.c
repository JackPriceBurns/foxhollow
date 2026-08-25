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
typedef struct Dummy04DllInterfaceCallbacks {
    void* slot02;
    __typeof__(Dummy04_func03_ret_m1)* slot03;
    __typeof__(Dummy04_func04_nop)* slot04;
    __typeof__(Dummy04_func05_ret_0)* slot05;
    __typeof__(Dummy04_onSetupPlayer)* onSetupPlayer;
    __typeof__(Dummy04_func07_nop)* slot07;
    __typeof__(Dummy04_func08_nop)* slot08;
    __typeof__(Dummy04_onSelectSave)* onSelectSave;
    __typeof__(Dummy04_func0A_ret_0)* slot0A;
    __typeof__(Dummy04_func0B_ret_0)* slot0B;
    __typeof__(Dummy04_func0C_nop)* slot0C;
    __typeof__(Dummy04_func0D_ret_0)* slot0D;
    __typeof__(Dummy04_func0E_nop)* slot0E;
    __typeof__(Dummy04_func0F_ret_0)* slot0F;
    __typeof__(Dummy04_func10_nop)* slot10;
    __typeof__(Dummy04_func11_ret_0)* slot11;
    __typeof__(Dummy04_func12_nop)* slot12;
    __typeof__(Dummy04_func13_nop)* slot13;
    __typeof__(Dummy04_func14_nop)* slot14;
    __typeof__(Dummy04_func15_nop)* slot15;
    __typeof__(Dummy04_func16_nop)* slot16;
    __typeof__(Dummy04_func17_nop)* slot17;
    __typeof__(Dummy04_func18_nop)* slot18;
    __typeof__(Dummy04_func19_nop)* slot19;
    __typeof__(Dummy04_func1A_nop)* slot1A;
    __typeof__(Dummy04_func1B_nop)* slot1B;
    __typeof__(Dummy04_func1C_nop)* slot1C;
    __typeof__(Dummy04_func1D_ret_0)* slot1D;
    __typeof__(Dummy04_func1E_nop)* slot1E;
    __typeof__(Dummy04_func1F_nop)* slot1F;
    __typeof__(Dummy04_func20_nop)* slot20;
    __typeof__(Dummy04_func21_ret_0)* slot21;
    __typeof__(Dummy04_func22_ret_127)* slot22;
    __typeof__(Dummy04_func23_nop)* slot23;
    __typeof__(Dummy04_func24_ret_0)* slot24;
    __typeof__(Dummy04_func25_nop)* slot25;
    __typeof__(Dummy04_func26_nop)* slot26;
    void* slot27;
    void* slot28;
    void* slot29;
    void* slot2A;
    void* slot2B;
} Dummy04DllInterfaceCallbacks;

typedef struct Dummy04DllInterface {
    ResourceDescriptorHeader header;
    Dummy04DllInterfaceCallbacks interface;
} Dummy04DllInterface;

RESOURCE_ACQUIRE_ADAPTER(gDummy04ResourceAcquire, Dummy04_initialise)

Dummy04DllInterface Dummy04_funcs = {
    {
        {0, 0, 0, 0x002b0000},
        gDummy04ResourceAcquire,
        Dummy04_release,
    },
    {
        NULL,
        Dummy04_func03_ret_m1,
        Dummy04_func04_nop,
        Dummy04_func05_ret_0,
        Dummy04_onSetupPlayer,
        Dummy04_func07_nop,
        Dummy04_func08_nop,
        Dummy04_onSelectSave,
        Dummy04_func0A_ret_0,
        Dummy04_func0B_ret_0,
        Dummy04_func0C_nop,
        Dummy04_func0D_ret_0,
        Dummy04_func0E_nop,
        Dummy04_func0F_ret_0,
        Dummy04_func10_nop,
        Dummy04_func11_ret_0,
        Dummy04_func12_nop,
        Dummy04_func13_nop,
        Dummy04_func14_nop,
        Dummy04_func15_nop,
        Dummy04_func16_nop,
        Dummy04_func17_nop,
        Dummy04_func18_nop,
        Dummy04_func19_nop,
        Dummy04_func1A_nop,
        Dummy04_func1B_nop,
        Dummy04_func1C_nop,
        Dummy04_func1D_ret_0,
        Dummy04_func1E_nop,
        Dummy04_func1F_nop,
        Dummy04_func20_nop,
        Dummy04_func21_ret_0,
        Dummy04_func22_ret_127,
        Dummy04_func23_nop,
        Dummy04_func24_ret_0,
        Dummy04_func25_nop,
        Dummy04_func26_nop,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
    },
};
