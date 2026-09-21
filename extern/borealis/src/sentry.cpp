#include "borealis/sentry.hpp"

#include "borealis/log.hpp"
#include "borealis/version.h"
#include "sentry_config.hpp"

#include <cstdlib>
#include <optional>
#include <string_view>
#include <system_error>

#ifndef BOREALIS_HAS_SENTRY
#define BOREALIS_HAS_SENTRY 0
#endif
#ifndef BOREALIS_SENTRY_DSN
#define BOREALIS_SENTRY_DSN ""
#endif
#ifndef BOREALIS_SENTRY_ENVIRONMENT
#define BOREALIS_SENTRY_ENVIRONMENT "development"
#endif

#if BOREALIS_HAS_SENTRY
#include <sentry.h>
#endif

namespace borealis::sentry {
namespace {

constexpr Log SentryLog{"sentry"};

#if BOREALIS_HAS_SENTRY
bool g_initialized = false;

std::optional<std::string_view> environment_value(const char* name) {
    if (const char* value = std::getenv(name)) {
        return value;
    }
    return std::nullopt;
}

detail::EffectiveConfig effective_config() {
    return detail::resolve_config(BOREALIS_SENTRY_DSN, environment_value("BOREALIS_SENTRY_ENABLED"),
        environment_value("BOREALIS_SENTRY_DSN"), environment_value("BOREALIS_SENTRY_DEBUG"));
}

void configure_paths(sentry_options_t* sentryOptions, const Options& options) {
    std::error_code ec;
    std::filesystem::create_directories(options.databaseDirectory, ec);
    if (ec) {
        SentryLog.warn("Unable to create database directory '{}': {}",
            options.databaseDirectory.string(), ec.message());
    }

#ifdef _WIN32
    const std::wstring databaseDirectory = options.databaseDirectory.wstring();
    sentry_options_set_database_pathw(sentryOptions, databaseDirectory.c_str());
    for (const auto& attachment : options.attachments) {
        const std::wstring path = attachment.wstring();
        sentry_options_add_attachmentw(sentryOptions, path.c_str());
    }
#else
    const std::string databaseDirectory = options.databaseDirectory.string();
    sentry_options_set_database_path(sentryOptions, databaseDirectory.c_str());
    for (const auto& attachment : options.attachments) {
        const std::string path = attachment.string();
        sentry_options_add_attachment(sentryOptions, path.c_str());
    }
#endif
}
#endif

}  // namespace

bool available() {
    return BOREALIS_HAS_SENTRY != 0;
}

bool initialize(const Options& options) {
#if BOREALIS_HAS_SENTRY
    if (g_initialized) {
        return true;
    }

    const detail::EffectiveConfig config = effective_config();
    if (!config.enabled) {
        SentryLog.info("Crash reporting disabled by BOREALIS_SENTRY_ENABLED");
        return false;
    }
    if (config.dsn.empty()) {
        SentryLog.warn("Crash reporting is enabled but no Sentry DSN is configured");
        return false;
    }
    if (options.release.empty()) {
        SentryLog.warn("Crash reporting requires a non-empty release name");
        return false;
    }
    if (options.databaseDirectory.empty()) {
        SentryLog.warn("Crash reporting requires a database directory");
        return false;
    }

    sentry_options_t* sentryOptions = sentry_options_new();
    if (!sentryOptions) {
        SentryLog.warn("Failed to allocate Sentry options");
        return false;
    }
    sentry_options_set_dsn(sentryOptions, config.dsn.c_str());
    sentry_options_set_release(sentryOptions, options.release.c_str());
    sentry_options_set_environment(sentryOptions, BOREALIS_SENTRY_ENVIRONMENT);
    sentry_options_set_debug(sentryOptions, config.debug ? 1 : 0);
    sentry_options_set_require_user_consent(sentryOptions, 1);
    sentry_options_set_cache_keep(sentryOptions, 1);
    sentry_options_set_max_breadcrumbs(sentryOptions, 100);
    configure_paths(sentryOptions, options);

    if (sentry_init(sentryOptions) != 0) {
        SentryLog.warn("Failed to initialize Sentry crash reporting");
        return false;
    }

    sentry_set_tag("git_branch", BOREALIS_APP_BRANCH);
    sentry_set_tag("build_type", BOREALIS_BUILD_TYPE);
    g_initialized = true;
    SentryLog.info("Initialized Sentry crash reporting");
    return true;
#else
    (void)options;
    return false;
#endif
}

void shutdown() {
#if BOREALIS_HAS_SENTRY
    if (!g_initialized) {
        return;
    }
    sentry_close();
    g_initialized = false;
#endif
}

Consent get_consent() {
#if BOREALIS_HAS_SENTRY
    if (!g_initialized) {
        return Consent::Unavailable;
    }
    switch (sentry_user_consent_get()) {
    case SENTRY_USER_CONSENT_GIVEN:
        return Consent::Given;
    case SENTRY_USER_CONSENT_REVOKED:
        return Consent::Revoked;
    case SENTRY_USER_CONSENT_UNKNOWN:
    default:
        return Consent::Unknown;
    }
#else
    return Consent::Unavailable;
#endif
}

void set_consent(bool enabled) {
#if BOREALIS_HAS_SENTRY
    if (!g_initialized) {
        return;
    }
    if (enabled) {
        sentry_user_consent_give();
    } else {
        sentry_user_consent_revoke();
    }
#else
    (void)enabled;
#endif
}

}  // namespace borealis::sentry
