#include "musyx/sal_dsp.h"
#include "dolphin/os.h"
#include "dolphin/os/OSInterrupt.h"

void hwInitIrq(void)
{
    oldState = OSDisableInterrupts();
    hwIrqLevel = 1;
}
