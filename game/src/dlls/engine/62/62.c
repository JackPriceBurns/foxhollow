#include "dlls/object_descriptor.h"
#include "main/dll/dll_003E_dummy3e.h"

int Dummy3E_func05_ret_1(void) {
    return 1;
}

void Dummy3E_func04_nop(void) {
}

int Dummy3E_func03_ret_0(void) {
    return 0;
}

void Dummy3E_release(void) {
}

void Dummy3E_initialise(void) {
}

typedef struct Dummy3ECallbacks {
    void* reserved02;
    __typeof__(Dummy3E_func03_ret_0)* func03;
    __typeof__(Dummy3E_func04_nop)* func04;
    __typeof__(Dummy3E_func05_ret_1)* func05;
} Dummy3ECallbacks;

RESOURCE_DESCRIPTOR_TYPE(Dummy3EResourceDescriptor, Dummy3ECallbacks);
RESOURCE_ACQUIRE_ADAPTER(Dummy3E_acquire, Dummy3E_initialise)

Dummy3EResourceDescriptor Dummy3E_funcs = {
    { { 0, 0, 0, OBJECT_DESCRIPTOR_FLAGS_6_SLOTS }, Dummy3E_acquire, Dummy3E_release },
    { NULL, Dummy3E_func03_ret_0, Dummy3E_func04_nop, Dummy3E_func05_ret_1 },
};
