#include "dlls/object_descriptor.h"
#include "main/dll/dll_0012_unk.h"

unsigned int lbl_803DD458;

void dll_12_func0A_nop(void) {
}

void dll_12_func09(void) {
    lbl_803DD458 = 0x3;
}

void dll_12_func08_nop(void) {
}

void dll_12_func07_nop(void) {
}

int dll_12_func06_ret_0(void) {
    return 0x0;
}

void dll_12_func04_nop(void) {
}

void dll_12_func03_nop(void) {
}

void dll_12_func05_nop(void) {
}

void Dummy12_release(void) {
}

void Dummy12_initialise(void) {
}

typedef struct Dummy12Callbacks {
    void* reserved02;
    __typeof__(dll_12_func03_nop)* func03;
    __typeof__(dll_12_func04_nop)* func04;
    __typeof__(dll_12_func05_nop)* func05;
    __typeof__(dll_12_func06_ret_0)* func06;
    __typeof__(dll_12_func07_nop)* func07;
    __typeof__(dll_12_func08_nop)* func08;
    __typeof__(dll_12_func09)* func09;
    __typeof__(dll_12_func0A_nop)* func0A;
    void* reserved0B;
} Dummy12Callbacks;

RESOURCE_DESCRIPTOR_TYPE(Dummy12DllInterface, Dummy12Callbacks);
RESOURCE_ACQUIRE_ADAPTER(Dummy12_acquire, Dummy12_initialise)

Dummy12DllInterface Dummy12_funcs = {
    { { 0, 0, 0, 0x000A0000 }, Dummy12_acquire, Dummy12_release },
    {
        NULL,
        dll_12_func03_nop,
        dll_12_func04_nop,
        dll_12_func05_nop,
        dll_12_func06_ret_0,
        dll_12_func07_nop,
        dll_12_func08_nop,
        dll_12_func09,
        dll_12_func0A_nop,
        NULL,
    },
};
