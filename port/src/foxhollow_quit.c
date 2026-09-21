#include "foxhollow_quit.h"

static int sQuitRequested;

void fhRequestQuit(void) { sQuitRequested = 1; }

int fhQuitRequested(void) { return sQuitRequested; }
