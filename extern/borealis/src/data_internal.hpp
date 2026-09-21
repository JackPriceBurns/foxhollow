#pragma once

#include "borealis/data.hpp"

#include <optional>

namespace borealis::data::detail {

struct Environment {
    std::filesystem::path preferencePath;
    std::filesystem::path basePath;
    std::filesystem::path homePath;
    std::filesystem::path documentsPath;
    std::vector<std::filesystem::path> legacyPreferencePaths;
    bool isWindows = false;
    bool isIOS = false;
    bool canOpenFolder = false;
    bool canChangeLocation = true;
};

struct LocationDescriptor {
    LocationMode mode = LocationMode::Default;
    std::filesystem::path customPath;
    std::filesystem::path previousPath;
};

struct LocatedDescriptor {
    LocationDescriptor descriptor;
    std::filesystem::path path;
};

class ManagerBackend {
public:
    ManagerBackend(Options options, Environment environment);

    Status initialize(const std::filesystem::path& userDirectoryOverride);

    const Paths& paths() const noexcept { return paths_; }
    const std::filesystem::path& active_data_path() const noexcept { return paths_.userPath; }
    const std::filesystem::path& configured_data_path() const noexcept {
        return configuredDataPath_;
    }
    LocationMode configured_mode() const noexcept { return configuredMode_; }
    bool has_user_directory_override() const noexcept { return userDirectoryOverride_; }
    bool is_default_data_path() const;
    bool is_data_path_restart_pending() const;
    Capabilities capabilities() const noexcept;

    Status set_custom_data_path(const std::filesystem::path& path);
    Status set_portable_data_path();
    Status reset_data_path();

    const std::filesystem::path& base_path() const noexcept { return environment_.basePath; }
    const std::filesystem::path& home_path() const noexcept { return environment_.homePath; }
    std::filesystem::path default_data_path() const;
    std::filesystem::path normalized_display_path(const std::filesystem::path& path) const;
    std::string abbreviated_path_string(const std::filesystem::path& path) const;

private:
    std::vector<std::filesystem::path> legacy_pref_paths() const;
    std::vector<std::filesystem::path> descriptor_read_paths() const;
    std::vector<std::filesystem::path> descriptor_write_paths(LocationMode mode) const;
    std::optional<LocatedDescriptor> read_location_descriptor() const;
    std::filesystem::path resolve_data_path(const LocationDescriptor* descriptor) const;
    Status ensure_directory(const std::filesystem::path& path) const;
    Status validate_writable_data_path(const std::filesystem::path& path) const;
    Status write_location_descriptor(LocationMode mode, const std::filesystem::path& targetPath);
    Status write_descriptor(
        const std::filesystem::path& path, const LocationDescriptor& descriptor) const;

    Options options_;
    Environment environment_;
    Paths paths_;
    std::filesystem::path configuredDataPath_;
    std::filesystem::path activeDescriptorPath_;
    LocationMode configuredMode_ = LocationMode::Default;
    bool initialized_ = false;
    bool userDirectoryOverride_ = false;
};

}  // namespace borealis::data::detail
