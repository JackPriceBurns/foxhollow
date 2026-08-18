#ifndef MAIN_DLL_DLL_0012_UNK_H_
#define MAIN_DLL_DLL_0012_UNK_H_

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

void dll_12_func0A_nop(void);
void dll_12_func09(void);
void dll_12_func08_nop(void);
void dll_12_func07_nop(void);
int dll_12_func06_ret_0(void);
void dll_12_func04_nop(void);
void dll_12_func03_nop(void);
void dll_12_func05_nop(void);
void Dummy12_release(void);
void Dummy12_initialise(void);

#endif /* MAIN_DLL_DLL_0012_UNK_H_ */
