#include <aurora/aurora.h>
#include <dolphin/types.h>

extern "C" u32 AIGetStreamPlayState(void) { return 0; }

namespace aurora {
AuroraConfig g_config{};
char g_gameName[4]{};
} // namespace aurora
