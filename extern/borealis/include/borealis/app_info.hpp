#pragma once

#include "borealis/version.h"

#include <string>
#include <string_view>

namespace borealis {

/** Application identity for borealis modules. */
struct AppInfo {
    /** Org/vendor name, e.g. "Twilit Realm". */
    std::string_view orgName;
    /** Application name, e.g. "Dusklight". */
    std::string_view appName;
    /** GitHub owner, e.g. "TwilitRealm". */
    std::string_view githubOwner;
    /** GitHub repository, e.g. "dusklight". */
    std::string_view githubRepo;
    /** Discord application/client ID. */
    std::string_view discordApplicationId;
};

/** User-Agent header value for outgoing requests: <appName>/<version> */
inline std::string user_agent(const AppInfo& info) {
    std::string agent(info.appName);
    agent += '/';
    agent += BOREALIS_APP_DESCRIBE;
    return agent;
}

}  // namespace borealis
