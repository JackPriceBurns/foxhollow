#include "musyx/mcmd_loop.h"

#include "musyx/hw_init.h"
#include "musyx/snd_service.h"

void mcmdLoop(McmdVoiceState* state, McmdCommandArgs* params)
{
    do
    {
        if (state->loop == 0)
        {
            if (((params->flags >> 16) & 1) != 0)
            {
                state->loop = (u16)sndRand() % (u16)(params->value >> 16);
            }
            else
            {
                state->loop = (u16)(params->value >> 16);
            }

            if (state->loop == MCMD_LOOP_COUNTER_FOREVER)
            {
                break;
            }
            state->loop = state->loop + 1;
        }
        else if (state->loop == MCMD_LOOP_COUNTER_FOREVER)
        {
            break;
        }

        if (--state->loop == 0)
        {
            return;
        }
    } while (0);

    if (((u8)(params->flags >> 8) & 1) != 0 && (*(u64*)&state->cFlagsHi & 0x10000000008ULL) == 0x00000000008ULL)
    {
        state->loop = 0;
    }
    else if (((u8)(params->flags >> 24) & 1) != 0 && (*(u64*)&state->cFlagsHi & 0x20ULL) == 0 &&
             !hwIsActive(state->id & 0xff))
    {
        state->loop = 0;
    }
    else
    {
        state->curAddr = state->addr + ((params->value & 0xffff) << 3);
    }
}
