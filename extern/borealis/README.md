# Borealis

Modules for [Aurora](https://github.com/encounter/aurora)-based ports. Borealis provides cross-platform logging,
updates, crash reporting, an HTTP client, Discord rich presence, data directory handling, and more.

Supported platforms: Windows, Linux, Android, macOS, iOS and tvOS.

## Modules

| Target                   | Contents                                                                | Status  |
|--------------------------|-------------------------------------------------------------------------|---------|
| `borealis::cli`          | Standard options with cxxopts                                           | ✅       |
| `borealis::config`       | ConfigVar system with JSON storage                                      | planned |
| `borealis::core`         | Shared utilities                                                        | ✅       |
| `borealis::crash`        | In-process crash handler with backtrace unwinding & logging             | ✅       |
| `borealis::data`         | Data directory resolution, portable mode, data migration                | ✅       |
| `borealis::disc`         | Disc inspection and hash verification                                   | ✅       |
| `borealis::discord`      | Discord rich presence IPC client                                        | ✅       |
| `borealis::file_select`  | Cross-platform file/folder selection                                    | ✅       |
| `borealis::http`         | Synchronous HTTP client                                                 | ✅       |
| `borealis::log`          | fmt-based logging + sinks (console, rotating file, logcat, ring buffer) | ✅       |
| `borealis::presentation` | Android frame-rate configuration                                        | ✅       |
| `borealis::sentry`       | Optional sentry-native/crashpad integration and consent state           | ✅       |
| `borealis::update`       | Update checks via GitHub releases                                       | ✅       |

Borealis also provides an [Android platform layer](platforms/android/README.md) that integrates SDL, Aurora and provides
Java-side support for Borealis modules.

## Usage

Add borealis as a submodule alongside aurora and link the targets you use:

```cmake
add_subdirectory(extern/borealis EXCLUDE_FROM_ALL)
target_link_libraries(mygame PRIVATE borealis::log)
```

### Logging

```cpp
#include <borealis/log.hpp>

namespace {
constexpr borealis::Log Log{"mygame::data"};
}

Log.info("Loaded {}", path);
Log.fatal("Unrecoverable: {}", err);  // flushes all sinks, then Options::onFatal
```

Initialize once at startup, before `aurora_initialize`:

```cpp
borealis::log::init({
    .level = borealis::LogLevel::Info,
    .fileDirectory = cachePath / "logs",
    .filePrefix = "mygame",
});
config.logCallback = borealis::log::aurora_callback();
```

C code (e.g. `OSReport` shims) may use the printf-style bridge in `<borealis/log_c.h>`.

### Application identity

Define `borealis::AppInfo` for modules that need application identity. Version and build strings come from the generated
`<borealis/version.h>`.

```cpp
inline constexpr borealis::AppInfo AppInfo{
    .orgName = "Twilit Realm",
    .appName = "Dusklight",
    .githubOwner = "TwilitRealm",
    .githubRepo = "dusklight",
    .discordApplicationId = "1495632471994405035",
};
```

### HTTP and update checks

`borealis::http` provides a synchronous HTTP client using WinHTTP on Windows, NSURLSession on Apple, libcurl on Linux or
JNI on Android.

```cpp
const auto result = borealis::update::check_latest_github_release(AppInfo);
if (result.status == borealis::update::Status::UpdateAvailable) {
    show_update_prompt(result.latest.tagName, result.latest.htmlUrl);
}
```

`Status::Disabled` indicates the build was compiled without an available HTTP backend.

### Data directories

Create a `borealis::data::Manager` with the application identity, portable path, legacy identities, and files eligible
for migration:

```cpp
borealis::data::Manager dataManager{AppInfo, {
    .portableRelativePath = std::filesystem::path{"data"},
    .legacyApps = {
        {
            .orgName = "OldOrgName",
            .appName = "OldAppName",
        },
    },
    .migration = {
        .directories = {"saves", "texture_replacements"},
        .files = {"config.json"},
        .extensions = {".gci"},
    },
}};

const auto status = dataManager.initialize(userDirectoryOverride);
const auto& paths = dataManager.paths();
```

### Disc inspection and verification

`borealis::disc` inspects and verifies GameCube and Wii disc images.

```cpp
constexpr std::array AcceptedDiscs{
    borealis::disc::AcceptedDisc{
        .gameId = "GZ2E01",
        .expectedHash = borealis::disc::parse_xxh3_128("14e886f08e548a000afde98a3195e788"),
    },
};
constexpr std::array<std::string_view, 1> RecognizedGameIds{"RZDE01"};

borealis::disc::Progress progress;
const borealis::disc::Result result = borealis::disc::verify(path,
    {.acceptedDiscs = AcceptedDiscs, .recognizedGameIds = RecognizedGameIds}, &progress);
```

Game ID, disc number, and revision must match an accepted record. IDs in another
record or `recognizedGameIds` return `UnsupportedVersion`; other IDs return
`UnknownGame`.

Verification uses XXH3-128. `Progress` can be polled or canceled from another thread.

### Crash unwinding and logging

Install the crash handler once, after logging is initialized:

```cpp
#include <borealis/crash.hpp>

borealis::log::init(logOptions);
borealis::crash::install();
```

Reports are written to stderr and the active log file. They include build identity, fault address, module build IDs, and
relative virtual addresses for symbolication.

### Sentry crash reporting

Set `BOREALIS_ENABLE_SENTRY=ON` to include sentry-native/crashpad. The DSN and environment are build inputs:
`BOREALIS_SENTRY_DSN` and `BOREALIS_SENTRY_ENVIRONMENT`. At runtime, `BOREALIS_SENTRY_ENABLED`, `BOREALIS_SENTRY_DSN`,
and `BOREALIS_SENTRY_DEBUG` can be used as overrides.

```cpp
borealis::sentry::Options options{
    .release = std::string(AppInfo.appName) + "@" + BOREALIS_APP_DESCRIBE,
    .databaseDirectory = cachePath / "sentry",
};
if (const char* logPath = borealis::log::file_path()) {
    options.attachments.emplace_back(logPath);
}
borealis::sentry::initialize(options);
```

Reports require user consent through `get_consent()` and `set_consent()`. Call `shutdown()` before shutting down
logging.

### Discord Rich Presence

`borealis::discord` uses Discord's local IPC protocol. Set the application ID in `AppInfo`:

```cpp
borealis::discord::initialize(AppInfo, handlers);
borealis::discord::update_presence({
    .details = "Ordon Village",
    .largeImageKey = "icon",
    .largeImageText = std::string(AppInfo.appName),
});
```

Call `run_callbacks()` from the main loop and `shutdown()` during application teardown.

## License

Borealis is licensed under the [MIT License](LICENSE).
