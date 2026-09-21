#include "borealis/file_select.hpp"

#include "file_select_internal.hpp"

#include <SDL3/SDL_dialog.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>

#include <memory>
#include <utility>

#if defined(__APPLE__)
#include <TargetConditionals.h>
#endif

#if defined(__APPLE__) && TARGET_OS_IOS && !TARGET_OS_TV && !TARGET_OS_MACCATALYST
#define BOREALIS_USE_IOS_FILE_DIALOG 1
#else
#define BOREALIS_USE_IOS_FILE_DIALOG 0
#endif

#if defined(__APPLE__) && !TARGET_OS_IOS && !TARGET_OS_TV && !TARGET_OS_MACCATALYST
#define BOREALIS_USE_MACOS_FOLDER_DIALOG 1
#else
#define BOREALIS_USE_MACOS_FOLDER_DIALOG 0
#endif

namespace borealis::file_select {
namespace detail {
namespace {

struct CompletionState {
    Callback callback;
    Result result;
};

void invoke_completion(void* userdata) {
    std::unique_ptr<CompletionState> state{static_cast<CompletionState*>(userdata)};
    state->callback(std::move(state->result));
}

}  // namespace

void complete(Callback callback, Result result) {
    if (!callback) {
        return;
    }

    auto state = std::make_unique<CompletionState>(
        CompletionState{.callback = std::move(callback), .result = std::move(result)});
    if (SDL_RunOnMainThread(&invoke_completion, state.get(), false)) {
        state.release();
        return;
    }

    // Run inline if SDL cannot queue the callback.
    invoke_completion(state.release());
}

Result result_from_file_list(const char* const* fileList, const char* error) {
    if (fileList == nullptr) {
        return {
            .status = Status::Failed,
            .message = error == nullptr || error[0] == '\0' ? "File dialog failed" : error,
        };
    }
    if (fileList[0] == nullptr) {
        return {.status = Status::Canceled};
    }

    Result result{.status = Status::Selected};
    for (const char* const* location = fileList; *location != nullptr; ++location) {
        result.locations.emplace_back(*location);
    }
    return result;
}

std::string fallback_display_name(std::string_view location) {
    if (location.empty()) {
        return {};
    }

    while (location.size() > 1 && (location.back() == '/' || location.back() == '\\')) {
        location.remove_suffix(1);
    }

    const auto separator = location.find_last_of("/\\");
    if (separator == std::string_view::npos || separator + 1 >= location.size()) {
        return std::string{location};
    }
    return std::string{location.substr(separator + 1)};
}

namespace {

struct SDLDialogState {
    Callback callback;
    std::vector<Filter> filters;
    std::vector<SDL_DialogFileFilter> sdlFilters;
    std::string defaultLocation;
};

void sdl_dialog_finished(void* userdata, const char* const* fileList, int) {
    std::unique_ptr<SDLDialogState> state{static_cast<SDLDialogState*>(userdata)};
    const char* error = fileList == nullptr ? SDL_GetError() : nullptr;
    complete(std::move(state->callback), result_from_file_list(fileList, error));
}

std::unique_ptr<SDLDialogState> make_sdl_state(
    Callback callback, std::vector<Filter> filters, std::string defaultLocation) {
    auto state = std::make_unique<SDLDialogState>();
    state->callback = std::move(callback);
    state->filters = std::move(filters);
    state->defaultLocation = std::move(defaultLocation);
    state->sdlFilters.reserve(state->filters.size());
    for (const auto& filter : state->filters) {
        state->sdlFilters.push_back({filter.name.c_str(), filter.pattern.c_str()});
    }
    return state;
}

void fail_wrong_thread(Callback callback) {
    complete(
        std::move(callback), {
                                 .status = Status::Failed,
                                 .message = "File selection must be started on SDL's main thread",
                             });
}

}  // namespace
}  // namespace detail

Capabilities capabilities() noexcept {
#if defined(__APPLE__) && TARGET_OS_TV
    return {};
#elif BOREALIS_USE_IOS_FILE_DIALOG
    return {.canOpenFile = true, .canOpenFolder = false};
#else
    return {.canOpenFile = true, .canOpenFolder = true};
#endif
}

void open_file(FileOptions options, Callback callback) {
    if (!callback) {
        return;
    }
    if (!SDL_IsMainThread()) {
        detail::fail_wrong_thread(std::move(callback));
        return;
    }
    if (!capabilities().canOpenFile) {
        detail::complete(
            std::move(callback), {
                                     .status = Status::Unsupported,
                                     .message = "File selection is not supported on this platform",
                                 });
        return;
    }

#if BOREALIS_USE_IOS_FILE_DIALOG
    detail::open_ios_file(std::move(options), std::move(callback));
#else
    const int filterCount = static_cast<int>(options.filters.size());
    auto state = detail::make_sdl_state(
        std::move(callback), std::move(options.filters), std::move(options.defaultLocation));
    const auto* filters = state->sdlFilters.empty() ? nullptr : state->sdlFilters.data();
    const auto* defaultLocation =
        state->defaultLocation.empty() ? nullptr : state->defaultLocation.c_str();
    SDL_ShowOpenFileDialog(&detail::sdl_dialog_finished, state.release(), options.parentWindow,
        filters, filterCount, defaultLocation, options.multiSelect);
#endif
}

void open_folder(FolderOptions options, Callback callback) {
    if (!callback) {
        return;
    }
    if (!SDL_IsMainThread()) {
        detail::fail_wrong_thread(std::move(callback));
        return;
    }
    if (!capabilities().canOpenFolder) {
        detail::complete(std::move(callback),
            {
                .status = Status::Unsupported,
                .message = "Folder selection is not supported on this platform",
            });
        return;
    }

#if BOREALIS_USE_MACOS_FOLDER_DIALOG
    detail::open_macos_folder(std::move(options), std::move(callback));
#elif defined(__ANDROID__) || defined(ANDROID)
    detail::open_android_folder(std::move(options), std::move(callback));
#else
    auto state =
        detail::make_sdl_state(std::move(callback), {}, std::move(options.defaultLocation));
    const auto* defaultLocation =
        state->defaultLocation.empty() ? nullptr : state->defaultLocation.c_str();
    SDL_ShowOpenFolderDialog(&detail::sdl_dialog_finished, state.release(), options.parentWindow,
        defaultLocation, false);
#endif
}

std::string display_name(std::string_view location) {
#if defined(__ANDROID__) || defined(ANDROID)
    if (location.starts_with("content:") || location.starts_with("file:")) {
        std::string name = detail::android_display_name(location);
        if (!name.empty()) {
            return name;
        }
    }
#endif
    return detail::fallback_display_name(location);
}

}  // namespace borealis::file_select
