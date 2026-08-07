#include "dlls/object_descriptor.h"
#include "main/dll/dll_003E_dummy3e.h"

int Dummy3E_func05_ret_1(void)
{
    return 1;
}

void Dummy3E_func04_nop(void)
{
}

int Dummy3E_func03_ret_0(void)
{
    return 0;
}

void Dummy3E_release(void)
{
}

void Dummy3E_initialise(void)
{
}

ObjectDescriptor6 Dummy3E_funcs = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_6_SLOTS,
    (ObjectDescriptorCallback)Dummy3E_initialise,
    (ObjectDescriptorCallback)Dummy3E_release,
    0,
    (ObjectDescriptorCallback)Dummy3E_func03_ret_0,
    (ObjectDescriptorCallback)Dummy3E_func04_nop,
    (ObjectDescriptorCallback)Dummy3E_func05_ret_1,
};
