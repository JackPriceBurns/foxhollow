#include "dlls/object_descriptor.h"
#include "dolphin/types.h"
#include "main/dll/dll_0012_unk.h"

unsigned int lbl_803DD458;

void dll_12_func0A_nop(void)
{
}

void dll_12_func09(void)
{
    lbl_803DD458 = 0x3;
}

void dll_12_func08_nop(void)
{
}

void dll_12_func07_nop(void)
{
}

int dll_12_func06_ret_0(void)
{
    return 0x0;
}

void dll_12_func04_nop(void)
{
}

void dll_12_func03_nop(void)
{
}

void dll_12_func05_nop(void)
{
}

void Dummy12_release(void)
{
}

void Dummy12_initialise(void)
{
}
typedef struct Dummy12DllInterface {
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
    ObjectDescriptorCallback slot06;
    ObjectDescriptorCallback slot07;
    ObjectDescriptorCallback slot08;
    ObjectDescriptorCallback slot09;
    ObjectDescriptorCallback slot0A;
    ObjectDescriptorCallback slot0B;
} Dummy12DllInterface;

Dummy12DllInterface Dummy12_funcs = {
    0,
    0,
    0,
    0x000A0000,
    (ObjectDescriptorCallback)Dummy12_initialise,
    (ObjectDescriptorCallback)Dummy12_release,
    0,
    (ObjectDescriptorCallback)dll_12_func03_nop,
    (ObjectDescriptorCallback)dll_12_func04_nop,
    (ObjectDescriptorCallback)dll_12_func05_nop,
    (ObjectDescriptorCallback)dll_12_func06_ret_0,
    (ObjectDescriptorCallback)dll_12_func07_nop,
    (ObjectDescriptorCallback)dll_12_func08_nop,
    (ObjectDescriptorCallback)dll_12_func09,
    (ObjectDescriptorCallback)dll_12_func0A_nop,
    0,
};
