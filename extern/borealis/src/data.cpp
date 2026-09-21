#include "data_internal.hpp"

#include "borealis/io.hpp"
#include "borealis/log.hpp"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_misc.h>
#include <SDL3/SDL_stdinc.h>

#include <nlohmann/json.hpp>

#include <algorithm>
#include <chrono>
#include <fstream>
#include <ranges>
#include <string_view>

#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace borealis::data {
namespace {

constexpr Log Log{"borealis::data"};
constexpr std::string_view kLocationDescriptorName = "data_location.json";

std::filesystem::path normalized_path(const std::filesystem::path& path) {
    std::error_code ec;
    auto normalized = std::filesystem::weakly_canonical(path, ec);
    if (!ec) {
        return normalized;
    }

    normalized = std::filesystem::absolute(path, ec);
    if (!ec) {
        return normalized.lexically_normal();
    }
    return path.lexically_normal();
}

std::filesystem::path absolute_path(const std::filesystem::path& path) {
    std::error_code ec;
    const auto absolute = std::filesystem::absolute(path, ec);
    return ec ? path.lexically_normal() : absolute.lexically_normal();
}

bool same_path(const std::filesystem::path& lhs, const std::filesystem::path& rhs) {
    return normalized_path(lhs) == normalized_path(rhs);
}

bool is_same_or_inside(const std::filesystem::path& root, const std::filesystem::path& path) {
    const auto normalizedRoot = normalized_path(root);
    const auto normalizedPath = normalized_path(path);
    const auto relative = normalizedPath.lexically_relative(normalizedRoot);
    if (relative.empty() || relative == ".") {
        return normalizedPath == normalizedRoot;
    }
    if (relative.is_absolute()) {
        return false;
    }
    const auto it = relative.begin();
    return it == relative.end() || *it != "..";
}

bool path_exists_or_symlink(const std::filesystem::path& path, std::error_code& ec) {
    const auto status = std::filesystem::symlink_status(path, ec);
    return !ec && status.type() != std::filesystem::file_type::not_found;
}

bool contains(const std::vector<std::string>& values, std::string_view value) {
    return std::ranges::find(values, value) != values.end();
}

bool matches_migration_rules(const std::filesystem::path& sourcePath,
    const std::filesystem::path& sourceRoot, const MigrationRules& rules, bool isDirectory) {
    const auto relative = sourcePath.lexically_relative(sourceRoot);
    if (relative.empty() || relative.is_absolute()) {
        return false;
    }

    auto it = relative.begin();
    if (it == relative.end() || *it == "..") {
        return false;
    }

    if (contains(rules.directories, io::fs_path_to_string(*it))) {
        return true;
    }
    if (isDirectory) {
        return false;
    }

    ++it;
    if (it != relative.end()) {
        return false;
    }

    const std::string filename = io::fs_path_to_string(relative.filename());
    if (contains(rules.files, filename) ||
        contains(rules.extensions, io::fs_path_to_string(relative.extension())))
    {
        return true;
    }

    return std::ranges::any_of(rules.filenamePatterns, [&](const FilenamePattern& pattern) {
        return filename.starts_with(pattern.prefix) && filename.ends_with(pattern.suffix);
    });
}

struct MigrationStats {
    std::uintmax_t directoriesCreated = 0;
    std::uintmax_t filesMoved = 0;
    std::uintmax_t symlinksMoved = 0;
    std::uintmax_t sourcesRemoved = 0;
    std::uintmax_t emptyDirectoriesRemoved = 0;
    std::uintmax_t skippedExistingTargets = 0;
    std::uintmax_t skippedDescriptorFiles = 0;
    std::uintmax_t skippedNestedTargets = 0;
    std::uintmax_t skippedUnsupportedEntries = 0;
    std::uintmax_t failures = 0;

    bool complete() const noexcept { return failures == 0 && skippedExistingTargets == 0; }
};

bool try_rename_entry(const std::filesystem::path& source, const std::filesystem::path& target) {
    std::error_code ec;
    if (path_exists_or_symlink(target, ec)) {
        return false;
    }
    ec.clear();
    std::filesystem::create_directories(target.parent_path(), ec);
    if (ec) {
        return false;
    }
    std::filesystem::rename(source, target, ec);
    return !ec;
}

bool ensure_parent(const std::filesystem::path& target, MigrationStats& stats) {
    std::error_code ec;
    std::filesystem::create_directories(target.parent_path(), ec);
    if (!ec) {
        return true;
    }
    ++stats.failures;
    Log.warn("Failed to create migration target parent '{}': {}",
        io::fs_path_to_string(target.parent_path()), ec.message());
    return false;
}

void remove_migrated_source(const std::filesystem::path& source, MigrationStats& stats) {
    std::error_code ec;
    if (std::filesystem::remove(source, ec)) {
        ++stats.sourcesRemoved;
        return;
    }
    ++stats.failures;
    Log.warn("Migrated '{}' but failed to remove the source: {}", io::fs_path_to_string(source),
        ec ? ec.message() : "source still exists");
}

void migrate_symlink(const std::filesystem::path& source, const std::filesystem::path& target,
    MigrationStats& stats) {
    std::error_code ec;
    if (path_exists_or_symlink(target, ec)) {
        ++stats.skippedExistingTargets;
        return;
    }
    if (!ensure_parent(target, stats)) {
        return;
    }

    std::filesystem::copy_symlink(source, target, ec);
    if (ec) {
        ++stats.failures;
        Log.warn("Failed to migrate symlink '{}' to '{}': {}", io::fs_path_to_string(source),
            io::fs_path_to_string(target), ec.message());
        return;
    }
    ++stats.symlinksMoved;
    remove_migrated_source(source, stats);
}

void migrate_regular_file(const std::filesystem::path& source, const std::filesystem::path& target,
    MigrationStats& stats) {
    std::error_code ec;
    if (path_exists_or_symlink(target, ec)) {
        ++stats.skippedExistingTargets;
        return;
    }
    if (try_rename_entry(source, target)) {
        ++stats.filesMoved;
        ++stats.sourcesRemoved;
        return;
    }
    if (!ensure_parent(target, stats)) {
        return;
    }

    const bool copied = std::filesystem::copy_file(
        source, target, std::filesystem::copy_options::skip_existing, ec);
    if (ec) {
        ++stats.failures;
        Log.warn("Failed to migrate file '{}' to '{}': {}", io::fs_path_to_string(source),
            io::fs_path_to_string(target), ec.message());
        return;
    }
    if (!copied) {
        ++stats.skippedExistingTargets;
        return;
    }
    ++stats.filesMoved;
    remove_migrated_source(source, stats);
}

std::uintmax_t remove_empty_directories(
    const std::filesystem::path& root, bool includeRoot, MigrationStats& stats) {
    std::error_code ec;
    if (!std::filesystem::exists(root, ec) || ec) {
        return 0;
    }

    std::vector<std::filesystem::path> directories;
    std::filesystem::recursive_directory_iterator it(
        root, std::filesystem::directory_options::skip_permission_denied, ec);
    const std::filesystem::recursive_directory_iterator end;
    while (!ec && it != end) {
        const auto status = it->symlink_status(ec);
        if (!ec && std::filesystem::is_directory(status)) {
            directories.push_back(it->path());
        }
        it.increment(ec);
    }
    if (ec) {
        ++stats.failures;
        Log.warn("Failed to scan empty directories under '{}': {}", io::fs_path_to_string(root),
            ec.message());
    }

    std::uintmax_t removed = 0;
    for (const auto& directory : std::views::reverse(directories)) {
        ec.clear();
        if (std::filesystem::is_empty(directory, ec) && !ec &&
            std::filesystem::remove(directory, ec))
        {
            ++removed;
        }
    }
    if (includeRoot) {
        ec.clear();
        if (std::filesystem::is_empty(root, ec) && !ec && std::filesystem::remove(root, ec)) {
            ++removed;
        }
    }
    return removed;
}

MigrationStats migrate_directory(const std::filesystem::path& from, const std::filesystem::path& to,
    const std::filesystem::path& preferencePath, const MigrationRules& rules) {
    MigrationStats stats;
    if (from.empty() || to.empty() || same_path(from, to)) {
        return stats;
    }

    std::error_code ec;
    if (!std::filesystem::exists(from, ec)) {
        if (ec) {
            ++stats.failures;
            Log.warn("Failed to inspect migration source '{}': {}", io::fs_path_to_string(from),
                ec.message());
        }
        return stats;
    }
    std::filesystem::create_directories(to, ec);
    if (ec) {
        ++stats.failures;
        Log.warn(
            "Failed to create migration target '{}': {}", io::fs_path_to_string(to), ec.message());
        return stats;
    }

    std::filesystem::recursive_directory_iterator it(
        from, std::filesystem::directory_options::skip_permission_denied, ec);
    const std::filesystem::recursive_directory_iterator end;
    if (ec) {
        ++stats.failures;
        Log.warn("Failed to begin migration scan for '{}': {}", io::fs_path_to_string(from),
            ec.message());
        return stats;
    }
    while (it != end) {
        if (ec) {
            ++stats.failures;
            Log.warn(
                "Migration scan error under '{}': {}", io::fs_path_to_string(from), ec.message());
            ec.clear();
        }

        const auto source = it->path();
        const auto status = it->symlink_status(ec);
        if (ec) {
            ++stats.failures;
            Log.warn("Failed to inspect migration source '{}': {}", io::fs_path_to_string(source),
                ec.message());
            ec.clear();
            it.increment(ec);
            continue;
        }

        if (is_same_or_inside(to, source)) {
            ++stats.skippedNestedTargets;
            if (std::filesystem::is_directory(status)) {
                it.disable_recursion_pending();
            }
            it.increment(ec);
            continue;
        }

        const auto relative = source.lexically_relative(from);
        if (relative == kLocationDescriptorName) {
            ++stats.skippedDescriptorFiles;
            it.increment(ec);
            continue;
        }
        if (!matches_migration_rules(source, from, rules, std::filesystem::is_directory(status))) {
            ++stats.skippedUnsupportedEntries;
            if (std::filesystem::is_directory(status)) {
                it.disable_recursion_pending();
            }
            it.increment(ec);
            continue;
        }

        const auto target = to / relative;
        if (std::filesystem::is_symlink(status)) {
            migrate_symlink(source, target, stats);
        } else if (std::filesystem::is_directory(status)) {
            if (try_rename_entry(source, target)) {
                ++stats.directoriesCreated;
                ++stats.sourcesRemoved;
                it.disable_recursion_pending();
            } else {
                std::filesystem::create_directories(target, ec);
                if (ec) {
                    ++stats.failures;
                    Log.warn("Failed to create migration target directory '{}': {}",
                        io::fs_path_to_string(target), ec.message());
                    ec.clear();
                    it.disable_recursion_pending();
                } else {
                    ++stats.directoriesCreated;
                }
            }
        } else if (std::filesystem::is_regular_file(status)) {
            migrate_regular_file(source, target, stats);
        } else {
            ++stats.skippedUnsupportedEntries;
        }
        it.increment(ec);
    }

    stats.emptyDirectoriesRemoved =
        remove_empty_directories(from, !same_path(from, preferencePath), stats);
    if (stats.filesMoved > 0 || stats.symlinksMoved > 0 || stats.sourcesRemoved > 0 ||
        stats.skippedExistingTargets > 0 || stats.failures > 0)
    {
        Log.info("Finished data migration from '{}' to '{}': {} files, {} symlinks, {} sources "
                 "removed, {} existing targets skipped, {} failures",
            io::fs_path_to_string(from), io::fs_path_to_string(to), stats.filesMoved,
            stats.symlinksMoved, stats.sourcesRemoved, stats.skippedExistingTargets,
            stats.failures);
    }
    return stats;
}

std::string file_url_from_path(const std::filesystem::path& path) {
    const std::string generic = io::fs_path_to_generic_string(path);
    std::string url = "file://";
#if defined(_WIN32)
    url += '/';
#endif
    constexpr std::string_view kHexDigits = "0123456789ABCDEF";
    for (const char character : generic) {
        const auto byte = static_cast<unsigned char>(character);
        const bool unreserved = (byte >= 'A' && byte <= 'Z') || (byte >= 'a' && byte <= 'z') ||
                                (byte >= '0' && byte <= '9') || byte == '-' || byte == '.' ||
                                byte == '_' || byte == '~' || byte == '/' || byte == ':';
        if (unreserved) {
            url += character;
        } else {
            url += '%';
            url += kHexDigits[byte >> 4];
            url += kHexDigits[byte & 0xF];
        }
    }
    return url;
}

const char* mode_id(LocationMode mode) {
    switch (mode) {
    case LocationMode::Default:
        return "default";
    case LocationMode::Portable:
        return "portable";
    case LocationMode::Custom:
        return "custom";
    }
    return "default";
}

std::filesystem::path sdl_preference_path(
    std::string_view orgNameView, std::string_view appNameView) {
    const std::string orgName{orgNameView};
    const std::string appName{appNameView};
    const char* orgNameArgument = orgName.empty() ? nullptr : orgName.c_str();
    if (char* preferencePath = SDL_GetPrefPath(orgNameArgument, appName.c_str())) {
        std::filesystem::path result = io::fs_path_from_utf8(preferencePath);
        SDL_free(preferencePath);
        return result;
    }
    return {};
}

detail::Environment discover_environment(const AppInfo& appInfo, const Options& options) {
    detail::Environment environment;
    environment.preferencePath = sdl_preference_path(appInfo.orgName, appInfo.appName);
    if (environment.preferencePath.empty()) {
        Log.error("Unable to get the SDL preference path: {}", SDL_GetError());
    }

    for (const LegacyAppIdentity& legacyApp : options.legacyApps) {
        if (legacyApp.appName.empty()) {
            Log.warn("Ignoring a legacy SDL identity with an empty application name");
            continue;
        }
        const auto path = sdl_preference_path(legacyApp.orgName, legacyApp.appName);
        if (path.empty()) {
            Log.warn("Unable to get the legacy SDL preference path for '{}': {}", legacyApp.appName,
                SDL_GetError());
            continue;
        }
        if (!same_path(path, environment.preferencePath) &&
            std::ranges::none_of(environment.legacyPreferencePaths,
                [&](const auto& existing) { return same_path(existing, path); }))
        {
            environment.legacyPreferencePaths.push_back(path);
        }
    }

    if (const char* basePath = SDL_GetBasePath()) {
        environment.basePath = io::fs_path_from_utf8(basePath);
    }
    if (const char* homePath = SDL_GetUserFolder(SDL_FOLDER_HOME)) {
        environment.homePath = io::fs_path_from_utf8(homePath);
    }
    if (const char* documentsPath = SDL_GetUserFolder(SDL_FOLDER_DOCUMENTS)) {
        environment.documentsPath = io::fs_path_from_utf8(documentsPath);
    }

#if defined(_WIN32)
    environment.isWindows = true;
#endif
#if defined(__APPLE__) && TARGET_OS_IOS && !TARGET_OS_TV && !TARGET_OS_MACCATALYST
    environment.isIOS = true;
#endif
#if defined(_WIN32) ||                                                                             \
    (defined(__APPLE__) && !TARGET_OS_IOS && !TARGET_OS_TV && !TARGET_OS_MACCATALYST) ||           \
    (defined(__linux__) && !defined(__ANDROID__))
    environment.canOpenFolder = true;
#endif
    environment.canChangeLocation = !environment.isIOS;
    return environment;
}

}  // namespace

namespace detail {

ManagerBackend::ManagerBackend(Options options, Environment environment)
    : options_{std::move(options)}, environment_{std::move(environment)} {}

std::vector<std::filesystem::path> ManagerBackend::legacy_pref_paths() const {
    std::vector<std::filesystem::path> paths;
    for (const auto& path : environment_.legacyPreferencePaths) {
        if (!same_path(path, environment_.preferencePath) &&
            std::ranges::none_of(
                paths, [&](const auto& existing) { return same_path(existing, path); }))
        {
            paths.push_back(path);
        }
    }
    return paths;
}

std::vector<std::filesystem::path> ManagerBackend::descriptor_read_paths() const {
    std::vector<std::filesystem::path> paths;
    const auto add = [&](const std::filesystem::path& path) {
        if (!path.empty() &&
            std::ranges::none_of(paths, [&](const auto& value) { return same_path(value, path); }))
        {
            paths.push_back(path);
        }
    };
    if (!environment_.basePath.empty()) {
        add(environment_.basePath / kLocationDescriptorName);
    }
    add(environment_.preferencePath / kLocationDescriptorName);
    for (const auto& legacyPath : legacy_pref_paths()) {
        add(legacyPath / kLocationDescriptorName);
    }
    return paths;
}

std::vector<std::filesystem::path> ManagerBackend::descriptor_write_paths(LocationMode mode) const {
    if (!activeDescriptorPath_.empty()) {
        return {activeDescriptorPath_};
    }
    if (mode == LocationMode::Portable && !environment_.basePath.empty()) {
        return {environment_.basePath / kLocationDescriptorName,
            environment_.preferencePath / kLocationDescriptorName};
    }
    return {environment_.preferencePath / kLocationDescriptorName};
}

std::optional<LocatedDescriptor> ManagerBackend::read_location_descriptor() const {
    for (const auto& path : descriptor_read_paths()) {
        std::error_code ec;
        if (!std::filesystem::exists(path, ec) || ec) {
            continue;
        }
        try {
            std::ifstream input{path, std::ios::binary};
            if (!input) {
                Log.warn("Ignoring unreadable data location descriptor '{}'",
                    io::fs_path_to_string(path));
                continue;
            }
            nlohmann::json json;
            input >> json;
            if (!json.is_object() || json.value("version", 0) != 1) {
                Log.warn("Ignoring data location descriptor '{}' with unsupported format",
                    io::fs_path_to_string(path));
                continue;
            }

            LocationDescriptor descriptor;
            const std::string mode = json.value("mode", "");
            if (mode == "default") {
                descriptor.mode = LocationMode::Default;
            } else if (mode == "portable") {
                descriptor.mode = LocationMode::Portable;
            } else if (mode == "custom") {
                descriptor.mode = LocationMode::Custom;
            } else {
                Log.warn("Ignoring data location descriptor '{}' with unknown mode '{}'",
                    io::fs_path_to_string(path), mode);
                continue;
            }

            if (const auto it = json.find("customPath"); it != json.end() && it->is_string()) {
                descriptor.customPath = io::fs_path_from_utf8(it->get<std::string>());
            }
            if (const auto it = json.find("previousPath"); it != json.end() && it->is_string()) {
                descriptor.previousPath = io::fs_path_from_utf8(it->get<std::string>());
            }
            if (descriptor.mode == LocationMode::Custom && descriptor.customPath.empty()) {
                Log.warn("Ignoring custom data location descriptor '{}' without a path",
                    io::fs_path_to_string(path));
                continue;
            }
            return LocatedDescriptor{.descriptor = std::move(descriptor), .path = path};
        } catch (const std::exception& exception) {
            Log.warn("Ignoring data location descriptor '{}': {}", io::fs_path_to_string(path),
                exception.what());
        }
    }
    return std::nullopt;
}

Status ManagerBackend::ensure_directory(const std::filesystem::path& path) const {
    if (path.empty()) {
        return {.code = ErrorCode::EmptyPath, .path = path};
    }
    std::error_code ec;
    std::filesystem::create_directories(path, ec);
    if (ec) {
        return {.code = ErrorCode::CreateDirectoryFailed, .path = path, .systemError = ec};
    }
    if (!std::filesystem::is_directory(path, ec) || ec) {
        return {.code = ErrorCode::NotDirectory, .path = path, .systemError = ec};
    }
    return {};
}

Status ManagerBackend::validate_writable_data_path(const std::filesystem::path& path) const {
    if (const Status status = ensure_directory(path); !status) {
        return status;
    }

    const auto probe =
        path /
        (".borealis-write-probe-" +
            std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()) + ".tmp");
    {
        std::ofstream output{probe, std::ios::binary | std::ios::trunc};
        output << "borealis";
        if (!output) {
            std::error_code removeError;
            std::filesystem::remove(probe, removeError);
            return {.code = ErrorCode::WriteProbeFailed,
                .path = path,
                .systemError = std::make_error_code(std::io_errc::stream)};
        }
    }
    std::error_code ec;
    std::filesystem::remove(probe, ec);
    if (ec) {
        return {.code = ErrorCode::WriteProbeCleanupFailed, .path = probe, .systemError = ec};
    }
    return {};
}

std::filesystem::path ManagerBackend::default_data_path() const {
    if (environment_.isIOS && options_.defaultPath.useDocumentsOnIOS &&
        !environment_.documentsPath.empty())
    {
        return environment_.documentsPath;
    }
    if (environment_.isWindows && options_.defaultPath.useExecutableDirectoryOnWindows &&
        !environment_.basePath.empty())
    {
        if (validate_writable_data_path(environment_.basePath)) {
            return environment_.basePath;
        }
        Log.warn("Executable directory '{}' is not writable; using '{}'",
            io::fs_path_to_string(environment_.basePath),
            io::fs_path_to_string(environment_.preferencePath));
    }
    return environment_.preferencePath;
}

std::filesystem::path ManagerBackend::resolve_data_path(
    const LocationDescriptor* descriptor) const {
    if (descriptor == nullptr || descriptor->mode == LocationMode::Default) {
        return default_data_path();
    }
    if (descriptor->mode == LocationMode::Portable) {
        if (options_.portableRelativePath.empty() || environment_.basePath.empty()) {
            Log.warn("Ignoring portable data mode because this application disables it");
            return default_data_path();
        }
        return environment_.basePath / options_.portableRelativePath;
    }
    return descriptor->customPath;
}

Status ManagerBackend::write_descriptor(
    const std::filesystem::path& path, const LocationDescriptor& descriptor) const {
    nlohmann::json json{
        {"version", 1},
        {"mode", mode_id(descriptor.mode)},
    };
    if (descriptor.mode == LocationMode::Custom) {
        json["customPath"] = io::fs_path_to_string(descriptor.customPath);
    }
    if (!descriptor.previousPath.empty()) {
        json["previousPath"] = io::fs_path_to_string(descriptor.previousPath);
    }

    std::error_code ec;
    std::filesystem::create_directories(path.parent_path(), ec);
    if (ec) {
        return {.code = ErrorCode::DescriptorWriteFailed, .path = path, .systemError = ec};
    }
    const auto temporary =
        path.parent_path() /
        (io::fs_path_to_string(path.filename()) + ".tmp-" +
            std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
    {
        std::ofstream output{temporary, std::ios::binary | std::ios::trunc};
        output << json.dump(4) << '\n';
        output.flush();
        if (!output) {
            std::filesystem::remove(temporary, ec);
            return {.code = ErrorCode::DescriptorWriteFailed,
                .path = path,
                .systemError = std::make_error_code(std::io_errc::stream)};
        }
    }

#if defined(_WIN32)
    if (!MoveFileExW(
            temporary.c_str(), path.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
    {
        ec = std::error_code{static_cast<int>(GetLastError()), std::system_category()};
    }
#else
    std::filesystem::rename(temporary, path, ec);
#endif
    if (ec) {
        std::error_code removeError;
        std::filesystem::remove(temporary, removeError);
        return {.code = ErrorCode::DescriptorWriteFailed, .path = path, .systemError = ec};
    }
    return {};
}

Status ManagerBackend::initialize(const std::filesystem::path& userDirectoryOverride) {
    if (environment_.preferencePath.empty()) {
        return {.code = ErrorCode::PreferencePathUnavailable};
    }
    if (const Status status = ensure_directory(environment_.preferencePath); !status) {
        return status;
    }

    paths_.cachePath = environment_.preferencePath;
    if (!userDirectoryOverride.empty()) {
        paths_.userPath = absolute_path(userDirectoryOverride);
        if (const Status status = ensure_directory(paths_.userPath); !status) {
            return status;
        }
        configuredDataPath_ = paths_.userPath;
        configuredMode_ = LocationMode::Custom;
        activeDescriptorPath_.clear();
        userDirectoryOverride_ = true;
        initialized_ = true;
        return {};
    }

    userDirectoryOverride_ = false;
    const auto located = read_location_descriptor();
    if (located) {
        activeDescriptorPath_ = located->path;
        configuredMode_ = located->descriptor.mode;
        if (configuredMode_ == LocationMode::Portable &&
            (options_.portableRelativePath.empty() || environment_.basePath.empty()))
        {
            configuredMode_ = LocationMode::Default;
        }
    } else {
        activeDescriptorPath_.clear();
        configuredMode_ = LocationMode::Default;
    }
    paths_.userPath = resolve_data_path(located ? &located->descriptor : nullptr);
    configuredDataPath_ = paths_.userPath;
    if (const Status status = ensure_directory(paths_.userPath); !status) {
        return status;
    }

    bool migrationComplete = true;
    std::filesystem::path incompleteSource;
    if (located && !located->descriptor.previousPath.empty()) {
        const auto stats = migrate_directory(located->descriptor.previousPath, paths_.userPath,
            environment_.preferencePath, options_.migration);
        migrationComplete = stats.complete();
        if (!migrationComplete) {
            incompleteSource = located->descriptor.previousPath;
        }
    }
    for (const auto& legacyPath : legacy_pref_paths()) {
        const auto stats = migrate_directory(
            legacyPath, paths_.userPath, environment_.preferencePath, options_.migration);
        if (!stats.complete()) {
            migrationComplete = false;
            if (incompleteSource.empty()) {
                incompleteSource = legacyPath;
            }
        }
    }

    initialized_ = true;
    if (located && !located->descriptor.previousPath.empty() && migrationComplete) {
        auto completedDescriptor = located->descriptor;
        completedDescriptor.previousPath.clear();
        if (const Status status = write_descriptor(located->path, completedDescriptor); !status) {
            return status;
        }
    }
    if (!migrationComplete) {
        return {.code = ErrorCode::MigrationIncomplete, .path = incompleteSource};
    }
    return {};
}

Status ManagerBackend::write_location_descriptor(
    LocationMode mode, const std::filesystem::path& targetPath) {
    if (!initialized_) {
        return {.code = ErrorCode::NotInitialized};
    }
    if (userDirectoryOverride_) {
        return {.code = ErrorCode::OverrideActive};
    }

    const auto resolvedTarget =
        mode == LocationMode::Custom ? absolute_path(targetPath) : targetPath;
    LocationDescriptor descriptor{.mode = mode};
    if (mode == LocationMode::Custom) {
        descriptor.customPath = resolvedTarget;
    }
    if (!same_path(paths_.userPath, resolvedTarget)) {
        descriptor.previousPath = paths_.userPath;
    }

    Status lastStatus{.code = ErrorCode::DescriptorWriteFailed};
    for (const auto& path : descriptor_write_paths(mode)) {
        lastStatus = write_descriptor(path, descriptor);
        if (lastStatus) {
            activeDescriptorPath_ = path;
            configuredDataPath_ = resolvedTarget;
            configuredMode_ = mode;
            return {};
        }
    }
    return lastStatus;
}

Status ManagerBackend::set_custom_data_path(const std::filesystem::path& path) {
    if (!initialized_) {
        return {.code = ErrorCode::NotInitialized};
    }
    if (userDirectoryOverride_) {
        return {.code = ErrorCode::OverrideActive};
    }
    if (!environment_.canChangeLocation) {
        return {.code = ErrorCode::Unsupported, .path = path};
    }
    if (const Status status = validate_writable_data_path(path); !status) {
        return status;
    }
    return write_location_descriptor(LocationMode::Custom, path);
}

Status ManagerBackend::set_portable_data_path() {
    if (options_.portableRelativePath.empty() || environment_.basePath.empty() ||
        !environment_.canChangeLocation)
    {
        return {.code = ErrorCode::Unsupported};
    }
    return write_location_descriptor(
        LocationMode::Portable, environment_.basePath / options_.portableRelativePath);
}

Status ManagerBackend::reset_data_path() {
    if (!environment_.canChangeLocation) {
        return {.code = ErrorCode::Unsupported};
    }
    return write_location_descriptor(LocationMode::Default, default_data_path());
}

bool ManagerBackend::is_default_data_path() const {
    return initialized_ && same_path(configuredDataPath_, default_data_path());
}

bool ManagerBackend::is_data_path_restart_pending() const {
    return initialized_ && !same_path(paths_.userPath, configuredDataPath_);
}

Capabilities ManagerBackend::capabilities() const noexcept {
    const bool canChange =
        initialized_ && !userDirectoryOverride_ && environment_.canChangeLocation;
    return {
        .canOpenFolder = initialized_ && environment_.canOpenFolder,
        .canChangeLocation = canChange,
        .canUsePortableLocation =
            canChange && !environment_.basePath.empty() && !options_.portableRelativePath.empty(),
    };
}

std::filesystem::path ManagerBackend::normalized_display_path(
    const std::filesystem::path& path) const {
    return normalized_path(path);
}

std::string ManagerBackend::abbreviated_path_string(const std::filesystem::path& path) const {
    if (path.empty() || environment_.homePath.empty()) {
        return io::fs_path_to_string(path);
    }
    const auto normalizedPath = normalized_path(path);
    const auto normalizedHome = normalized_path(environment_.homePath);
    if (normalizedPath == normalizedHome) {
        return "~";
    }
    const auto relative = normalizedPath.lexically_relative(normalizedHome);
    if (!relative.empty() && !relative.is_absolute()) {
        const auto it = relative.begin();
        if (it == relative.end() || *it != "..") {
            return io::fs_path_to_string(std::filesystem::path{"~"} / relative);
        }
    }
    return io::fs_path_to_string(path);
}

}  // namespace detail

Manager::Manager(AppInfo appInfo, Options options)
    : backend_{std::make_unique<detail::ManagerBackend>(
          options, discover_environment(appInfo, options))} {}

Manager::~Manager() = default;
Manager::Manager(Manager&&) noexcept = default;
Manager& Manager::operator=(Manager&&) noexcept = default;

Status Manager::initialize(const std::filesystem::path& userDirectoryOverride) {
    return backend_->initialize(userDirectoryOverride);
}

const Paths& Manager::paths() const noexcept {
    return backend_->paths();
}

const std::filesystem::path& Manager::active_data_path() const noexcept {
    return backend_->active_data_path();
}

const std::filesystem::path& Manager::configured_data_path() const noexcept {
    return backend_->configured_data_path();
}

LocationMode Manager::configured_mode() const noexcept {
    return backend_->configured_mode();
}

bool Manager::has_user_directory_override() const noexcept {
    return backend_->has_user_directory_override();
}

bool Manager::is_default_data_path() const {
    return backend_->is_default_data_path();
}

bool Manager::is_data_path_restart_pending() const {
    return backend_->is_data_path_restart_pending();
}

Capabilities Manager::capabilities() const noexcept {
    return backend_->capabilities();
}

Status Manager::set_custom_data_path(const std::filesystem::path& path) {
    return backend_->set_custom_data_path(path);
}

Status Manager::set_portable_data_path() {
    return backend_->set_portable_data_path();
}

Status Manager::reset_data_path() {
    return backend_->reset_data_path();
}

Status Manager::open_active_data_path() const {
    if (!capabilities().canOpenFolder) {
        return {.code = ErrorCode::Unsupported, .path = active_data_path()};
    }
    const auto path = normalized_display_path(active_data_path());
    const std::string url = file_url_from_path(path);
    if (!SDL_OpenURL(url.c_str())) {
        Log.warn(
            "Failed to open data folder '{}': {}", io::fs_path_to_string(path), SDL_GetError());
        return {.code = ErrorCode::OpenFolderFailed, .path = path};
    }
    return {};
}

std::filesystem::path Manager::base_path_relative(const std::filesystem::path& path) const {
    return backend_->base_path().empty() ? path : backend_->base_path() / path;
}

std::filesystem::path Manager::user_home_path() const {
    return backend_->home_path();
}

std::filesystem::path Manager::normalized_display_path(const std::filesystem::path& path) const {
    return backend_->normalized_display_path(path);
}

std::string Manager::abbreviated_path_string(const std::filesystem::path& path) const {
    return backend_->abbreviated_path_string(path);
}

}  // namespace borealis::data
