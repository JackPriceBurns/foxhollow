#pragma once

#include <functional>
#include <string>
#include <string_view>
#include <vector>

struct SDL_Window;

namespace borealis::file_select {

enum class Status {
    Selected,
    Canceled,
    Unsupported,
    Failed,
};

struct Filter {
    /** User-visible filter label, such as "Disc images". */
    std::string name;
    /** Semicolon-separated extensions using SDL's dialog-filter syntax. */
    std::string pattern;
};

struct FileOptions {
    SDL_Window* parentWindow = nullptr;
    std::vector<Filter> filters;
    std::string defaultLocation;
    /** Allow multiple selections when supported. */
    bool multiSelect = false;
};

struct FolderOptions {
    SDL_Window* parentWindow = nullptr;
    std::string defaultLocation;
};

struct Result {
    Status status = Status::Failed;
    /** Filesystem paths, or content:// URIs for files selected on Android. */
    std::vector<std::string> locations;
    /** Backend diagnostic for logging; ports provide user-facing text. */
    std::string message;
};

struct Capabilities {
    bool canOpenFile = false;
    bool canOpenFolder = false;
};

using Callback = std::function<void(Result)>;

/** Returns backend support. Runtime failures do not change these values. */
Capabilities capabilities() noexcept;

/**
 * Opens a non-blocking file dialog. Call from SDL's main thread. The callback runs
 * exactly once on that thread when non-empty and may run before this function returns.
 */
void open_file(FileOptions options, Callback callback);

/** Opens a non-blocking single-folder dialog with the same callback contract. */
void open_folder(FolderOptions options, Callback callback);

/** Returns a display name for a filesystem path or platform URI. */
std::string display_name(std::string_view location);

}  // namespace borealis::file_select
