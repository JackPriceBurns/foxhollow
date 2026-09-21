#include "borealis/update.hpp"

#include <fmt/format.h>
#include <nlohmann/json.hpp>

#include <exception>
#include <stdexcept>
#include <utility>

namespace borealis::update {
namespace {

using json = nlohmann::json;

constexpr std::string_view GitHubApiVersion = "2026-03-10";

std::string json_string(const json& value, const char* key) {
    const auto iter = value.find(key);
    if (iter == value.end() || !iter->is_string()) {
        return {};
    }
    return iter->get<std::string>();
}

Release release_from_json(const json& value) {
    Release release{
        .tagName = json_string(value, "tag_name"),
        .name = json_string(value, "name"),
        .htmlUrl = json_string(value, "html_url"),
        .body = json_string(value, "body"),
    };

    const auto assets = value.find("assets");
    if (assets != value.end() && assets->is_array()) {
        for (const auto& asset : *assets) {
            if (!asset.is_object()) {
                continue;
            }
            release.assets.push_back({
                .name = json_string(asset, "name"),
                .browserDownloadUrl = json_string(asset, "browser_download_url"),
                .digest = json_string(asset, "digest"),
            });
        }
    }

    return release;
}

std::string release_url(std::string_view owner, std::string_view repo, bool includePrereleases) {
    if (includePrereleases) {
        return fmt::format("https://api.github.com/repos/{}/{}/releases?per_page=10", owner, repo);
    }
    return fmt::format("https://api.github.com/repos/{}/{}/releases/latest", owner, repo);
}

Release parse_release_response(std::string_view value, bool includePrereleases) {
    const json parsed = json::parse(value);
    if (!includePrereleases) {
        return release_from_json(parsed);
    }
    if (!parsed.is_array()) {
        throw std::runtime_error("expected a GitHub releases array");
    }
    for (const auto& release : parsed) {
        if (!release.is_object() || release.value("draft", false)) {
            continue;
        }
        return release_from_json(release);
    }
    throw std::runtime_error("GitHub returned no published releases");
}

}  // namespace

Release parse_github_release(std::string_view value) {
    return release_from_json(json::parse(value));
}

Result check_latest_github_release(const AppInfo& info, const Options& options) {
    auto fetch = options.fetch;
    if (!fetch) {
        // Custom transports do not require a compiled-in backend.
        if (!http::available()) {
            return {
                .status = Status::Disabled,
                .message = "No HTTP backend is available",
            };
        }
        fetch = http::get;
    }
    if (info.appName.empty() || info.githubOwner.empty() || info.githubRepo.empty()) {
        return {
            .status = Status::Failed,
            .message = "AppInfo is missing appName, githubOwner, or githubRepo",
        };
    }

    const http::Request request{
        .url = release_url(info.githubOwner, info.githubRepo, options.includePrereleases),
        .headers =
            {
                {.name = "User-Agent", .value = user_agent(info)},
                {.name = "Accept", .value = "application/vnd.github+json"},
                {.name = "X-GitHub-Api-Version", .value = std::string(GitHubApiVersion)},
            },
        .timeout = options.timeout,
    };

    const http::Result result = fetch(request);
    if (result.error != http::Error::None) {
        return {
            .status = Status::Failed,
            .message = result.message,
        };
    }
    if (result.response.statusCode != 200) {
        return {
            .status = Status::Failed,
            .message = fmt::format("GitHub returned HTTP {}", result.response.statusCode),
        };
    }

    Release latest;
    try {
        latest = parse_release_response(result.response.body, options.includePrereleases);
    } catch (const std::exception& e) {
        return {
            .status = Status::Failed,
            .message = fmt::format("Failed to parse GitHub release JSON: {}", e.what()),
        };
    }

    const std::optional<Version> latestVersion = parse_version(latest.tagName);
    if (!latestVersion) {
        return {
            .status = Status::Failed,
            .message = fmt::format("Failed to parse release tag '{}'", latest.tagName),
            .latest = std::move(latest),
        };
    }
    const std::optional<Version> currentVersion = parse_version(options.currentVersion);
    if (!currentVersion) {
        return {
            .status = Status::Failed,
            .message = fmt::format("Failed to parse current version '{}'", options.currentVersion),
            .latest = std::move(latest),
        };
    }

    const bool updateAvailable = compare_version(*latestVersion, *currentVersion) > 0;
    return {
        .status = updateAvailable ? Status::UpdateAvailable : Status::UpToDate,
        .message = updateAvailable ? "Update available" : "Up to date",
        .latest = std::move(latest),
    };
}

}  // namespace borealis::update
