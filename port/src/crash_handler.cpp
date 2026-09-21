#include <borealis/crash.hpp>

extern "C" void fhInstallCrashHandler(void) { borealis::crash::install(); }
