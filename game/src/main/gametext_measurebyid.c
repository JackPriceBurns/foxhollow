#include "main/gametext_internal.h"
#include "main/gametext_shared_internal.h"

void gameTextMeasureById(int id, int a, int b, int* outMinX, int* outMaxX, int* outMinY, int* outMaxY)
{
    GameTextDef* e;
    TextFont* fonts;
    int count;
    int i;
    int found;

    fonts = gameTextFonts;
    if (fonts->status != 2)
    {
        found = 0;
    }
    else
    {
        e = fonts->entries;
        count = fonts->entryCount;
        for (i = 0; i != count || (found = 0, 0); i++)
        {
            if (e->identifier == id)
            {
                found = 1;
                break;
            }
            e++;
        }
    }
    if (!found)
    {
        *outMinX = 0;
        *outMaxX = 0;
        *outMinY = 0;
        *outMaxY = 0;
        return;
    }
    gGameTextMeasureOnly = 1;
    gGameTextBoundsMinX = 0x7FFFFFFF;
    gGameTextBoundsMaxX = 0;
    gGameTextBoundsMinY = 0x7FFFFFFF;
    gGameTextBoundsMaxY = 0;
    gameTextRenderById(id, a, b);
    gGameTextMeasureOnly = 0;
    if (outMinY != NULL)
    {
        *outMinY = gGameTextBoundsMinY >> 2;
    }
    if (outMaxY != NULL)
    {
        *outMaxY = gGameTextBoundsMaxY >> 2;
    }
    if (outMinX != NULL)
    {
        *outMinX = gGameTextBoundsMinX >> 2;
    }
    if (outMaxX != NULL)
    {
        *outMaxX = gGameTextBoundsMaxX >> 2;
    }
}
