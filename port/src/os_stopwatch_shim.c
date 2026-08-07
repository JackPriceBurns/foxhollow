#include <dolphin/os.h>

void OSStartStopwatch(OSStopwatch* sw) {
  sw->running = 1;
  sw->last = OSGetTime();
}

void OSStopStopwatch(OSStopwatch* sw) {
  if (sw->running) {
    sw->total += OSGetTime() - sw->last;
    sw->running = 0;
    sw->hits++;
  }
}

void OSResetStopwatch(OSStopwatch* sw) {
  sw->total = 0;
  sw->hits = 0;
  sw->min = 0;
  sw->max = 0;
  sw->running = 0;
}

OSTime OSCheckStopwatch(OSStopwatch* sw) {
  return sw->running ? sw->total + (OSGetTime() - sw->last) : sw->total;
}
