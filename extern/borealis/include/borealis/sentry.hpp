#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace borealis::sentry {

enum class Consent {
    Unavailable,
    Unknown,
    Given,
    Revoked,
};

struct Options {
    /** Sentry release name, conventionally "<appName>@<describe>". */
    std::string release;
    /** Directory used by sentry-native/crashpad for its persistent database. */
    std::filesystem::path databaseDirectory;
    /** Files to include with reports, such as the active log file. */
    std::vector<std::filesystem::path> attachments;
};

/** Returns whether sentry-native is available. */
bool available();

/** Initializes sentry-native. Returns false if unavailable, disabled, or invalid. */
bool initialize(const Options& options);
void shutdown();

Consent get_consent();
void set_consent(bool enabled);

}  // namespace borealis::sentry
