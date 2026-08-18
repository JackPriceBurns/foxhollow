#ifndef MAIN_DLL_DLL_0010_UICONTROLLER_H_
#define MAIN_DLL_DLL_0010_UICONTROLLER_H_

typedef struct UIControllerDllInterface {
    u32 reserved0;
    u32 reserved1;
    u32 reserved2;
    u32 slotCountAndFlags;
    ObjectDescriptorCallback initialise;
    ObjectDescriptorCallback release;
    ObjectDescriptorCallback slot02;
    ObjectDescriptorCallback frameStart;
    ObjectDescriptorCallback frameEnd;
    ObjectDescriptorCallback render;
} UIControllerDllInterface;

void UIController_render(void* context, int arg1, int arg2);
void UIController_frameEnd(void);
void UIController_frameStart(void);
void UIController_release(void);
void UIController_initialise(void);

#endif /* MAIN_DLL_DLL_0010_UICONTROLLER_H_ */
