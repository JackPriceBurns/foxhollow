#include "musyx/hw_input.h"
#include "musyx/hw_dspctrl.h"



u32 hwAddInput(u8 studio, SND_STUDIO_INPUT* input)
{
    return salAddStudioInput(&dspStudio[studio], input);
}
