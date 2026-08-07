#include "dlls/object_descriptor.h"
#include "main/dll/dll_003A_dummy3a.h"

void Dummy3A_render(void)
{
}

void Dummy3A_frameEnd(void)
{
}

int Dummy3A_frameStart(void)
{
    return 0;
}

void Dummy3A_release(void)
{
}

void Dummy3A_initialise(void)
{
}

ObjectDescriptor6 Dummy3A_funcs = {
    0,
    0,
    0,
    OBJECT_DESCRIPTOR_FLAGS_6_SLOTS,
    (ObjectDescriptorCallback)Dummy3A_initialise,
    (ObjectDescriptorCallback)Dummy3A_release,
    0,
    (ObjectDescriptorCallback)Dummy3A_frameStart,
    (ObjectDescriptorCallback)Dummy3A_frameEnd,
    (ObjectDescriptorCallback)Dummy3A_render,
};
