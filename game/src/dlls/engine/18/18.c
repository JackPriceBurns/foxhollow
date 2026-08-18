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
