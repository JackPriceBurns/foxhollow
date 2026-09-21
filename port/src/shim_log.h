#ifndef FOXHOLLOW_SHIM_LOG_H
#define FOXHOLLOW_SHIM_LOG_H

#include <stdio.h>

#define FH_STUB_ONCE(name) \
  do { \
    static int warned; \
    if (!warned) { \
      warned = 1; \
      fprintf(stderr, "[foxhollow] stub called: %s\n", name); \
    } \
  } while (0)

#endif
