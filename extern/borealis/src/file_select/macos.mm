#include "file_select_internal.hpp"

#include <SDL3/SDL_properties.h>
#include <SDL3/SDL_video.h>

#import <AppKit/AppKit.h>

#include <memory>
#include <utility>

namespace borealis::file_select::detail {
namespace {

struct MacOSFolderState {
    Callback callback;
};

void finish_folder_dialog(MacOSFolderState* rawState, NSURL* url) {
    std::unique_ptr<MacOSFolderState> state{rawState};
    if (url == nil) {
        complete(std::move(state->callback), {.status = Status::Canceled});
        return;
    }

    const char* path = url.path.UTF8String;
    if (path == nullptr) {
        complete(std::move(state->callback), {
            .status = Status::Failed,
            .message = "The selected folder could not be represented as UTF-8",
        });
        return;
    }

    complete(std::move(state->callback), {
        .status = Status::Selected,
        .locations = {path},
    });
}

void configure_default_location(NSOpenPanel* panel, const std::string& defaultLocation) {
    if (panel == nil || defaultLocation.empty()) {
        return;
    }

    NSString* path = [NSString stringWithUTF8String:defaultLocation.c_str()];
    if (path == nil) {
        return;
    }

    BOOL isDirectory = NO;
    NSFileManager* fileManager = NSFileManager.defaultManager;
    NSURL* url = [NSURL fileURLWithPath:path];
    panel.directoryURL = [fileManager fileExistsAtPath:path isDirectory:&isDirectory] && isDirectory
        ? url
        : url.URLByDeletingLastPathComponent;
}

NSWindow* window_for_sdl_window(SDL_Window* window) {
    if (window == nullptr) {
        return nil;
    }

    const SDL_PropertiesID properties = SDL_GetWindowProperties(window);
    return (__bridge NSWindow*)SDL_GetPointerProperty(
        properties, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);
}

}  // namespace

void open_macos_folder(FolderOptions options, Callback callback) {
    auto state = std::make_unique<MacOSFolderState>(
        MacOSFolderState{.callback = std::move(callback)});

    NSOpenPanel* panel = NSOpenPanel.openPanel;
    if (panel == nil) {
        complete(std::move(state->callback), {
            .status = Status::Failed,
            .message = "Unable to create the macOS folder dialog",
        });
        return;
    }

    panel.canChooseFiles = NO;
    panel.canChooseDirectories = YES;
    panel.allowsMultipleSelection = NO;
    panel.canCreateDirectories = YES;
    configure_default_location(panel, options.defaultLocation);

    MacOSFolderState* rawState = state.release();
    void (^completion)(NSModalResponse) = ^(NSModalResponse response) {
        finish_folder_dialog(rawState, response == NSModalResponseOK ? panel.URL : nil);
    };

    NSWindow* parentWindow = window_for_sdl_window(options.parentWindow);
    if (parentWindow != nil) {
        [panel beginSheetModalForWindow:parentWindow completionHandler:completion];
    } else {
        [panel beginWithCompletionHandler:completion];
    }
}

}  // namespace borealis::file_select::detail
