#pragma once

#include "borealis/app_info.hpp"
#include "borealis/http.hpp"
#include "borealis/version.h"

#include <chrono>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace borealis::update {

enum class Status {
    /** No HTTP backend is available. */
    Disabled,
    UpToDate,
    UpdateAvailable,
    Failed,
};

struct Asset {
    std::string name;
    std::string browserDownloadUrl;
    std::string digest;
};

struct Release {
    std::string tagName;
    std::string name;
    std::string htmlUrl;
    std::string body;
    std::vector<Asset> assets;
};

struct Result {
    Status status = Status::Failed;
    std::string message;
    /** Latest parsed release, including when version parsing fails. */
    Release latest;
};

/** Parsed semantic version. Build metadata is discarded. */
struct Version {
    int major = 0;
    int minor = 0;
    int patch = 0;
    std::vector<std::string> prerelease;
};

/**
 * Parses semver with optional leading 'v', git-describe distance, and "-dirty".
 * Returns nullopt for invalid input.
 */
std::optional<Version> parse_version(std::string_view value);

/** Compares semver precedence: -1 if lhs < rhs, 0 if equal, 1 if lhs > rhs. */
int compare_version(const Version& lhs, const Version& rhs);

/** Parses GitHub release JSON. Throws nlohmann::json::exception on malformed input. */
Release parse_github_release(std::string_view json);

struct Options {
    /** Version to compare against. */
    std::string_view currentVersion = BOREALIS_APP_DESCRIBE;
    /** Include prereleases when resolving the newest published GitHub release. */
    bool includePrereleases = false;
    /** Request transport. Defaults to http::get and bypasses availability checks when set. */
    std::function<http::Result(const http::Request&)> fetch;
    std::chrono::milliseconds timeout{10000};
};

/** Checks the latest GitHub release against the current version. Never throws. */
Result check_latest_github_release(const AppInfo& info, const Options& options = {});

}  // namespace borealis::update
