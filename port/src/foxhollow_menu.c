#include "foxhollow_menu.h"

#include <stddef.h>

static char sExitToDesktop[] = "Exit To Desktop";

const char* fhMenuTextForId(int textId)
{
    if (textId == FH_MENU_TEXT_ID_EXIT_TO_DESKTOP)
    {
        return sExitToDesktop;
    }
    return NULL;
}
