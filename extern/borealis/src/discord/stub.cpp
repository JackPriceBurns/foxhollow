#include "borealis/discord.hpp"

namespace borealis::discord {

bool available() {
    return false;
}

bool initialize(const AppInfo& info, EventHandlers handlers) {
    (void)info;
    (void)handlers;
    return false;
}

void run_callbacks() {}

bool update_presence(Presence presence) {
    (void)presence;
    return false;
}

bool clear_presence() {
    return false;
}

void shutdown() {}

}  // namespace borealis::discord
