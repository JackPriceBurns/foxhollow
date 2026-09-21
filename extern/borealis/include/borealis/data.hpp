#pragma once

#include "borealis/app_info.hpp"

#include <filesystem>
#include <memory>
#include <string>
#include <system_error>
#include <vector>

namespace borealis::data {

enum class LocationMode {
    Default,
    Portable,
    Custom,
};

struct FilenamePattern {
    std::string prefix;
    std::string suffix;
};

/** User data eligible for migration. */
struct MigrationRules {
    std::vector<std::string> directories;
    std::vector<std::string> files;
    std::vector<std::string> extensions;
    std::vector<FilenamePattern> filenamePatterns;
};

struct DefaultPathPolicy {
    /** Use SDL_FOLDER_DOCUMENTS instead of the preference path on iOS. */
    bool useDocumentsOnIOS = false;
    /** Use a writable executable directory, falling back to preferences, on Windows. */
    bool useExecutableDirectoryOnWindows = false;
};

struct LegacyAppIdentity {
    /** Empty uses SDL's legacy no-organization path. */
    std::string orgName;
    std::string appName;
};

struct Options {
    DefaultPathPolicy defaultPath;
    /** Executable-relative portable directory. Empty disables portable mode. */
    std::filesystem::path portableRelativePath;
    /** Previous SDL identities whose preference directories may contain data. */
    std::vector<LegacyAppIdentity> legacyApps;
    MigrationRules migration;
};

struct Paths {
    std::filesystem::path userPath;
    std::filesystem::path cachePath;
};

enum class ErrorCode {
    None,
    NotInitialized,
    PreferencePathUnavailable,
    EmptyPath,
    CreateDirectoryFailed,
    NotDirectory,
    WriteProbeFailed,
    WriteProbeCleanupFailed,
    DescriptorWriteFailed,
    /** Paths are usable, but migration needs to be retried. */
    MigrationIncomplete,
    OverrideActive,
    Unsupported,
    OpenFolderFailed,
};

struct Status {
    ErrorCode code = ErrorCode::None;
    std::filesystem::path path;
    std::error_code systemError;

    explicit operator bool() const noexcept { return code == ErrorCode::None; }
};

struct Capabilities {
    bool canOpenFolder = false;
    bool canChangeLocation = false;
    bool canUsePortableLocation = false;
};

namespace detail {
class ManagerBackend;
}

/** Resolves and migrates application data directories. */
class Manager {
public:
    Manager(AppInfo appInfo, Options options = {});
    ~Manager();

    Manager(const Manager&) = delete;
    Manager& operator=(const Manager&) = delete;
    Manager(Manager&&) noexcept;
    Manager& operator=(Manager&&) noexcept;

    /** Resolves paths and runs pending migration. MigrationIncomplete is non-fatal. */
    Status initialize(const std::filesystem::path& userDirectoryOverride = {});

    const Paths& paths() const noexcept;
    const std::filesystem::path& active_data_path() const noexcept;
    const std::filesystem::path& configured_data_path() const noexcept;
    LocationMode configured_mode() const noexcept;
    bool has_user_directory_override() const noexcept;
    bool is_default_data_path() const;
    bool is_data_path_restart_pending() const;
    Capabilities capabilities() const noexcept;

    Status set_custom_data_path(const std::filesystem::path& path);
    Status set_portable_data_path();
    Status reset_data_path();
    Status open_active_data_path() const;

    /** Resolves a path relative to the executable directory. */
    std::filesystem::path base_path_relative(const std::filesystem::path& path = {}) const;
    std::filesystem::path user_home_path() const;
    std::filesystem::path normalized_display_path(const std::filesystem::path& path) const;
    std::string abbreviated_path_string(const std::filesystem::path& path) const;

private:
    std::unique_ptr<detail::ManagerBackend> backend_;
};

}  // namespace borealis::data
