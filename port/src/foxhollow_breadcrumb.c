#include "foxhollow_crash.h"

#include <stdio.h>

void fhNoteMapLoaded(int mapId)
{
    fprintf(stderr, "[foxhollow] map-loaded id=%d\n", mapId);
    fflush(stderr);
}
