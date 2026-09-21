#include "borealis/update.hpp"

#include <gtest/gtest.h>

#include <fstream>
#include <sstream>
#include <string>

using borealis::AppInfo;
using borealis::update::Options;
using borealis::update::Release;
using borealis::update::Result;
using borealis::update::Status;
using borealis::update::Version;
namespace http = borealis::http;
namespace update = borealis::update;

namespace {

constexpr AppInfo TestApp{
    .orgName = "TwilitRealm",
    .appName = "Dusklight",
    .githubOwner = "TwilitRealm",
    .githubRepo = "dusklight",
    .discordApplicationId = "1495632471994405035",
};

std::string read_fixture(const char* name) {
    const std::string path = std::string(BOREALIS_TEST_FIXTURE_DIR) + "/" + name;
    std::ifstream stream(path, std::ios::binary);
    EXPECT_TRUE(stream.is_open());
    std::ostringstream contents;
    contents << stream.rdbuf();
    return contents.str();
}

Version parsed(std::string_view value) {
    const auto version = update::parse_version(value);
    if (!version) {
        ADD_FAILURE() << "Invalid test version: " << value;
        return {};
    }
    return *version;
}

int compare(std::string_view lhs, std::string_view rhs) {
    return update::compare_version(parsed(lhs), parsed(rhs));
}

std::function<http::Result(const http::Request&)> replay(http::Result result) {
    return [result = std::move(result)](const http::Request&) { return result; };
}

http::Result ok_response(std::string body) {
    return {
        .response =
            {
                .statusCode = 200,
                .body = std::move(body),
            },
    };
}

TEST(Update, ParseBasics) {
    const Version version = parsed("1.2.3");
    EXPECT_EQ(version.major, 1);
    EXPECT_EQ(version.minor, 2);
    EXPECT_EQ(version.patch, 3);
    EXPECT_TRUE(version.prerelease.empty());

    EXPECT_EQ(parsed("v10.0.42").major, 10);
    EXPECT_EQ(parsed("v10.0.42").patch, 42);

    const Version prerelease = parsed("v1.0.0-rc.1");
    EXPECT_EQ(prerelease.prerelease.size(), 2);
    EXPECT_EQ(prerelease.prerelease[0], "rc");
    EXPECT_EQ(prerelease.prerelease[1], "1");

    // Build metadata is validated and discarded.
    EXPECT_TRUE(parsed("1.2.3+build.5").prerelease.empty());
    EXPECT_EQ(parsed("1.2.3-rc.1+build.5").prerelease.size(), 2);
}

TEST(Update, ParseRejectsMalformed) {
    EXPECT_FALSE(update::parse_version(""));
    EXPECT_FALSE(update::parse_version("v"));
    EXPECT_FALSE(update::parse_version("1"));
    EXPECT_FALSE(update::parse_version("1.2"));
    EXPECT_FALSE(update::parse_version("1.2.x"));
    EXPECT_FALSE(update::parse_version("1.2.3.4"));
    EXPECT_FALSE(update::parse_version("nightly"));
    EXPECT_FALSE(update::parse_version("UNKNOWN-VERSION"));
    EXPECT_FALSE(update::parse_version("1.2.3-"));
    EXPECT_FALSE(update::parse_version("1.2.3-rc..1"));
    EXPECT_FALSE(update::parse_version("1.2.3-rc_1"));
    EXPECT_FALSE(update::parse_version("1.2.3+"));
    EXPECT_FALSE(update::parse_version("1.2.3+build..5"));
}

TEST(Update, GitDescribeSuffixes) {
    // Git-describe distance does not make a build older than its tag.
    EXPECT_TRUE(parsed("v1.2.3-4").prerelease.empty());
    EXPECT_EQ(compare("v1.2.3-4", "v1.2.3"), 0);
    EXPECT_EQ(compare("v1.2.3-4-dirty", "v1.2.3"), 0);

    // Prerelease tags retain their prerelease identifiers.
    const Version afterPrerelease = parsed("v1.0.0-rc.1-4");
    EXPECT_EQ(afterPrerelease.prerelease.size(), 2);
    EXPECT_EQ(afterPrerelease.prerelease[1], "1");
    EXPECT_EQ(compare("v1.0.0-rc.1-4", "v1.0.0-rc.1"), 0);
    EXPECT_EQ(compare("v1.0.0-rc.1-4-dirty", "v1.0.0-rc.1"), 0);

    EXPECT_EQ(parsed("v1.0.0-beta").prerelease.size(), 1);
    EXPECT_EQ(parsed("v1.0.0-beta").prerelease[0], "beta");

    // Numeric-only prereleases are indistinguishable from git-describe distance.
    // Borealis tags do not use them.
    EXPECT_TRUE(parsed("1.0.0-1").prerelease.empty());
    EXPECT_EQ(compare("1.0.0-1", "1.0.0"), 0);
}

TEST(Update, CompareOrdering) {
    EXPECT_EQ(compare("1.2.3", "1.2.3"), 0);
    EXPECT_EQ(compare("2.0.0", "1.9.9"), 1);
    EXPECT_EQ(compare("1.10.0", "1.9.0"), 1);
    EXPECT_EQ(compare("1.2.4", "1.2.3"), 1);
    EXPECT_EQ(compare("1.2.3", "2.0.0"), -1);

    EXPECT_EQ(compare("1.0.0-rc.1", "1.0.0"), -1);
    EXPECT_EQ(compare("1.0.0", "1.0.0-rc.1"), 1);

    // Semver 11.4: alpha < alpha.1 < alpha.beta < beta < beta.2 < beta.11 < rc.1.
    EXPECT_EQ(compare("1.0.0-alpha", "1.0.0-alpha.1"), -1);
    EXPECT_EQ(compare("1.0.0-alpha.1", "1.0.0-alpha.beta"), -1);
    EXPECT_EQ(compare("1.0.0-alpha.beta", "1.0.0-beta"), -1);
    EXPECT_EQ(compare("1.0.0-beta", "1.0.0-beta.2"), -1);
    // Numeric identifiers compare numerically.
    EXPECT_EQ(compare("1.0.0-beta.2", "1.0.0-beta.11"), -1);
    EXPECT_EQ(compare("1.0.0-beta.11", "1.0.0-rc.1"), -1);
    // Numeric identifiers precede alphanumeric identifiers.
    EXPECT_EQ(compare("1.0.0-1.x", "1.0.0-alpha.x"), -1);
    // Leading zeroes do not affect numeric identifiers.
    EXPECT_EQ(compare("1.0.0-rc.007", "1.0.0-rc.7"), 0);

    // Build metadata is ignored.
    EXPECT_EQ(compare("1.2.3+build.1", "1.2.3+build.2"), 0);
}

TEST(Update, ParseGithubReleaseFixture) {
    // The full GitHub response fixture includes ignored fields.
    const Release release =
        update::parse_github_release(read_fixture("github_release_latest.json"));
    EXPECT_EQ(release.tagName, "v1.4.1");
    EXPECT_EQ(release.name, "v1.4.1");
    EXPECT_EQ(release.htmlUrl, "https://github.com/TwilitRealm/dusklight/releases/tag/v1.4.1");
    EXPECT_TRUE(release.body.starts_with("**What's new:**"));

    ASSERT_EQ(release.assets.size(), 8);
    EXPECT_EQ(release.assets[0].name, "Dusklight-v1.4.1-android-arm64.apk");
    EXPECT_TRUE(release.assets[0].browserDownloadUrl ==
                "https://github.com/TwilitRealm/dusklight/releases/download/v1.4.1/"
                "Dusklight-v1.4.1-android-arm64.apk");
    for (const update::Asset& asset : release.assets) {
        EXPECT_FALSE(asset.name.empty());
        EXPECT_TRUE(asset.browserDownloadUrl.starts_with("https://github.com/"));
        EXPECT_TRUE(asset.digest.starts_with("sha256:"));
    }
}

TEST(Update, ParseGithubReleaseToleratesMissingFields) {
    // Digests and asset lists are optional.
    const Release release = update::parse_github_release(
        R"({"tag_name": "v1.0.0", "assets": [{"name": "old.zip"}, "not-an-object"]})");
    EXPECT_EQ(release.tagName, "v1.0.0");
    EXPECT_TRUE(release.name.empty());
    EXPECT_TRUE(release.body.empty());
    ASSERT_EQ(release.assets.size(), 1);
    EXPECT_EQ(release.assets[0].name, "old.zip");
    EXPECT_TRUE(release.assets[0].digest.empty());
    EXPECT_TRUE(release.assets[0].browserDownloadUrl.empty());

    EXPECT_TRUE(update::parse_github_release(R"({"tag_name": "v1.0.0"})").assets.empty());
}

TEST(Update, CheckReportsUpdate) {
    const std::string body = read_fixture("github_release_latest.json");
    for (const std::string_view current : {"v1.4.0", "v1.4.1-rc.1", "v0.9.0-3-dirty"}) {
        Options options{
            .currentVersion = current,
            .fetch = replay(ok_response(body)),
        };
        const Result result = update::check_latest_github_release(TestApp, options);
        EXPECT_EQ(result.status, Status::UpdateAvailable);
        EXPECT_EQ(result.latest.tagName, "v1.4.1");
        EXPECT_EQ(result.latest.assets.size(), 8);
    }
}

TEST(Update, CheckReportsUpToDate) {
    const std::string body = read_fixture("github_release_latest.json");
    // Current release, post-release build, and newer build.
    for (const std::string_view current : {"v1.4.1", "v1.4.1-6-dirty", "v1.5.0"}) {
        Options options{
            .currentVersion = current,
            .fetch = replay(ok_response(body)),
        };
        const Result result = update::check_latest_github_release(TestApp, options);
        EXPECT_EQ(result.status, Status::UpToDate);
        // Return the release used for comparison.
        EXPECT_EQ(result.latest.tagName, "v1.4.1");
    }
}

TEST(Update, CheckSendsExpectedRequest) {
    http::Request seen;
    Options options{
        .currentVersion = "v1.4.0",
        .fetch =
            [&](const http::Request& request) {
                seen = request;
                return ok_response(read_fixture("github_release_latest.json"));
            },
        .timeout = std::chrono::milliseconds(2500),
    };
    EXPECT_EQ(
        update::check_latest_github_release(TestApp, options).status, Status::UpdateAvailable);

    EXPECT_EQ(seen.url, "https://api.github.com/repos/TwilitRealm/dusklight/releases/latest");
    EXPECT_EQ(seen.timeout, std::chrono::milliseconds(2500));

    bool sawUserAgent = false;
    bool sawAccept = false;
    for (const http::Header& header : seen.headers) {
        if (header.name == "User-Agent") {
            sawUserAgent = true;
            // GitHub requires a versioned User-Agent.
            EXPECT_TRUE(header.value.starts_with("Dusklight/"));
            EXPECT_EQ(header.value, std::string("Dusklight/") + BOREALIS_APP_DESCRIBE);
        } else if (header.name == "Accept") {
            sawAccept = true;
            EXPECT_EQ(header.value, "application/vnd.github+json");
        }
    }
    EXPECT_TRUE(sawUserAgent);
    EXPECT_TRUE(sawAccept);
}

TEST(Update, CheckCanIncludePrereleases) {
    http::Request seen;
    Options options{
        .currentVersion = "v1.4.0",
        .includePrereleases = true,
        .fetch =
            [&](const http::Request& request) {
                seen = request;
                return ok_response(
                    R"([{"tag_name":"v1.5.0-rc.1","prerelease":true,"draft":false}])");
            },
    };

    const Result result = update::check_latest_github_release(TestApp, options);
    EXPECT_EQ(result.status, Status::UpdateAvailable);
    EXPECT_EQ(result.latest.tagName, "v1.5.0-rc.1");
    EXPECT_EQ(
        seen.url,
        "https://api.github.com/repos/TwilitRealm/dusklight/releases?per_page=10");
}

TEST(Update, CheckFailurePaths) {
    const auto check = [](Options options) {
        return update::check_latest_github_release(TestApp, options);
    };

    // Transport errors pass through.
    Result result = check({
        .fetch = replay({.error = http::Error::Timeout, .message = "Request timed out"}),
    });
    EXPECT_EQ(result.status, Status::Failed);
    EXPECT_EQ(result.message, "Request timed out");

    // HTTP errors include the status code.
    result = check({.fetch = replay({.response = {.statusCode = 403}})});
    EXPECT_EQ(result.status, Status::Failed);
    EXPECT_NE(result.message.find("403"), std::string::npos);

    // JSON errors are returned.
    result = check({.fetch = replay(ok_response("{ not json"))});
    EXPECT_EQ(result.status, Status::Failed);
    EXPECT_TRUE(result.message.starts_with("Failed to parse GitHub release JSON"));

    // Invalid tags still return release metadata.
    result = check({.fetch = replay(ok_response(R"({"tag_name": "nightly"})"))});
    EXPECT_EQ(result.status, Status::Failed);
    EXPECT_NE(result.message.find("nightly"), std::string::npos);
    EXPECT_EQ(result.latest.tagName, "nightly");

    // Unversioned builds cannot be compared.
    result = check({
        .currentVersion = "UNKNOWN-VERSION",
        .fetch = replay(ok_response(read_fixture("github_release_latest.json"))),
    });
    EXPECT_EQ(result.status, Status::Failed);
    EXPECT_NE(result.message.find("UNKNOWN-VERSION"), std::string::npos);
}

TEST(Update, CheckRequiresAppInfo) {
    bool fetched = false;
    const auto fetch = [&](const http::Request&) {
        fetched = true;
        return ok_response("{}");
    };

    constexpr AppInfo empty{};
    const Result result = update::check_latest_github_release(empty, {.fetch = fetch});
    EXPECT_EQ(result.status, Status::Failed);
    EXPECT_FALSE(fetched);
}

TEST(Update, CheckWithoutBackend) {
    // Custom transports bypass backend availability and keep tests offline.
    if (http::available()) {
        return;
    }
    const Result result = update::check_latest_github_release(TestApp);
    EXPECT_EQ(result.status, Status::Disabled);
    EXPECT_EQ(result.message, "No HTTP backend is available");
}

}  // namespace
