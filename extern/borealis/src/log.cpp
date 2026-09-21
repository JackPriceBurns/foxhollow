#include "borealis/log.hpp"
#include "borealis/aurora_log.h"
#include "borealis/io.hpp"
#include "borealis/log_c.h"

#include <algorithm>
#include <array>
#include <atomic>
#include <cctype>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <mutex>
#include <system_error>
#include <vector>

#if defined(__ANDROID__)
#include <android/log.h>
#endif

#if defined(_WIN32)
#include <io.h>
#define BOREALIS_FILENO _fileno
#define BOREALIS_ISATTY _isatty
#else
#include <unistd.h>
#define BOREALIS_FILENO fileno
#define BOREALIS_ISATTY isatty
#endif

using namespace std::literals::string_view_literals;

namespace borealis {

std::string_view to_string(const LogLevel level) noexcept {
    switch (level) {
    case LogLevel::Trace:
        return "TRACE"sv;
    case LogLevel::Debug:
        return "DEBUG"sv;
    case LogLevel::Info:
        return "INFO"sv;
    case LogLevel::Warning:
        return "WARNING"sv;
    case LogLevel::Error:
        return "ERROR"sv;
    case LogLevel::Fatal:
        return "FATAL"sv;
    }
    return "??"sv;
}

std::optional<LogLevel> level_from_string(const std::string_view text) noexcept {
    std::string upper;
    upper.reserve(text.size());
    for (const char c : text) {
        upper.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(c))));
    }
    if (upper == "WARN"sv) {
        return LogLevel::Warning;
    }
    for (int i = static_cast<int>(LogLevel::Trace); i <= static_cast<int>(LogLevel::Fatal); ++i) {
        const auto level = static_cast<LogLevel>(i);
        if (upper == to_string(level)) {
            return level;
        }
    }
    return std::nullopt;
}

}  // namespace borealis

namespace borealis::log {
namespace {

std::atomic<int> g_fileFd(-1);
// Fast-path reject threshold: min(global level, all module overrides).
std::atomic<int> g_minLevel(static_cast<int>(LogLevel::Trace));
std::atomic<bool> g_initialized(false);

constexpr size_t kBootstrapCapacity = 1024;

struct StoredMessage {
    LogLevel level;
    std::string module;
    std::string text;
};

struct State {
    std::mutex mutex;
    Options options{};
    std::shared_ptr<const std::function<bool(const Message&)>> divert;
    LogLevel globalLevel = LogLevel::Info;
    std::vector<std::pair<std::string, LogLevel>> moduleLevels;
    FILE* file = nullptr;
    std::string filePath;
    std::deque<StoredMessage> ring;
    std::deque<StoredMessage> bootstrap;
    std::vector<std::shared_ptr<Sink>> extraSinks;
    bool consoleIsTty = false;
    bool initialized = false;

    void close_file_locked() {
        if (file != nullptr) {
            g_fileFd.store(-1, std::memory_order_release);
            std::fflush(file);
            std::fclose(file);
            file = nullptr;
        }
        filePath.clear();
    }
};

// Leaked to support late logging and avoid macOS mutex destruction.
State& get_state() {
    static State* state = new State;
    return *state;
}

void recompute_min_level_locked(State& state) {
    int minLevel = static_cast<int>(state.globalLevel);
    for (const auto& [prefix, level] : state.moduleLevels) {
        minLevel = std::min(minLevel, static_cast<int>(level));
    }
    g_minLevel.store(minLevel, std::memory_order_release);
}

LogLevel effective_level_locked(const State& state, std::string_view module) {
    size_t bestLength = 0;
    LogLevel best = state.globalLevel;
    for (const auto& [prefix, level] : state.moduleLevels) {
        if (module.starts_with(prefix) && prefix.size() >= bestLength) {
            bestLength = prefix.size();
            best = level;
        }
    }
    return best;
}

struct LogFileCandidate {
    std::filesystem::path path;
    std::string filename;
    uintmax_t size;
};

void warn_log_cleanup_failure(
    const char* action, const std::filesystem::path& path, const std::error_code& ec) {
    std::fprintf(stderr, "[WARNING | borealis::log] Failed to %s '%s': %s\n", action,
        io::fs_path_to_string(path).c_str(), ec.message().c_str());
}

bool is_digit_at(std::string_view value, size_t index) {
    return std::isdigit(static_cast<unsigned char>(value[index])) != 0;
}

bool matches_generated_log_name(std::string_view filename, std::string_view prefix) {
    // "<prefix>-" + "YYYYmmdd" + "-" + "HHMMSS" + optional "-N" + ".log"
    constexpr size_t kTimestampLength = 15;
    if (!filename.starts_with(prefix) || filename.size() <= prefix.size() ||
        filename[prefix.size()] != '-')
    {
        return false;
    }
    std::string_view rest = filename.substr(prefix.size() + 1);
    if (rest.size() < kTimestampLength || rest[8] != '-') {
        return false;
    }
    for (size_t i = 0; i < kTimestampLength; ++i) {
        if (i != 8 && !is_digit_at(rest, i)) {
            return false;
        }
    }
    rest.remove_prefix(kTimestampLength);
    if (rest.starts_with('-')) {
        rest.remove_prefix(1);
        size_t digits = 0;
        while (digits < rest.size() && is_digit_at(rest, digits)) {
            ++digits;
        }
        if (digits == 0) {
            return false;
        }
        rest.remove_prefix(digits);
    }
    return rest == ".log"sv;
}

bool is_generated_log_file_name(const State& state, const std::filesystem::path& path) {
    const std::string filename = io::fs_path_to_string(path.filename());
    if (matches_generated_log_name(filename, state.options.filePrefix)) {
        return true;
    }
    return std::any_of(state.options.legacyFilePrefixes.begin(),
        state.options.legacyFilePrefixes.end(), [&](const std::string& legacyPrefix) {
            return matches_generated_log_name(filename, legacyPrefix);
        });
}

void delete_log_file(const std::filesystem::path& path) {
    std::error_code ec;
    std::filesystem::remove(path, ec);
    if (ec) {
        warn_log_cleanup_failure("remove old log file", path, ec);
    }
}

void prune_old_log_files(State& state, const std::filesystem::path& logsDir) {
    std::error_code ec;
    std::filesystem::directory_iterator entries{logsDir, ec};
    if (ec) {
        warn_log_cleanup_failure("inspect log directory", logsDir, ec);
        return;
    }

    std::vector<LogFileCandidate> candidates;
    for (const auto& entry : entries) {
        const std::filesystem::path path = entry.path();
        if (!is_generated_log_file_name(state, path)) {
            continue;
        }

        ec.clear();
        const auto status = entry.symlink_status(ec);
        if (ec) {
            warn_log_cleanup_failure("inspect log file", path, ec);
            continue;
        }
        if (!std::filesystem::is_regular_file(status)) {
            continue;
        }

        ec.clear();
        const uintmax_t size = entry.file_size(ec);
        if (ec) {
            warn_log_cleanup_failure("inspect size of log file", path, ec);
            continue;
        }

        candidates.push_back({path, io::fs_path_to_string(path.filename()), size});
    }

    std::sort(candidates.begin(), candidates.end(),
        [](const LogFileCandidate& a, const LogFileCandidate& b) {
            return a.filename > b.filename;
        });

    const size_t maxRetainedOldCount =
        state.options.maxRetainedFiles > 0 ? state.options.maxRetainedFiles - 1 : 0;
    const size_t retainedCount = std::min(candidates.size(), maxRetainedOldCount);
    uintmax_t retainedBytes = 0;
    for (size_t i = 0; i < retainedCount; ++i) {
        retainedBytes += candidates[i].size;
    }

    size_t retainedAfterSizeLimit = retainedCount;
    while (retainedAfterSizeLimit > 0 && retainedBytes > state.options.maxRetainedBytes) {
        --retainedAfterSizeLimit;
        retainedBytes -= candidates[retainedAfterSizeLimit].size;
    }

    for (size_t i = retainedAfterSizeLimit; i < candidates.size(); ++i) {
        delete_log_file(candidates[i].path);
    }
}

std::string make_timestamped_log_name(const std::string& prefix, unsigned attempt) {
    const auto now = std::chrono::system_clock::now();
    const std::time_t nowTime = std::chrono::system_clock::to_time_t(now);

    std::tm localTime{};
#if defined(_WIN32)
    localtime_s(&localTime, &nowTime);
#else
    localtime_r(&nowTime, &localTime);
#endif

    std::array<char, 32> buffer{};
    std::strftime(buffer.data(), buffer.size(), "%Y%m%d-%H%M%S", &localTime);
    std::string name = prefix + "-" + buffer.data();
    if (attempt != 0) {
        name += "-" + std::to_string(attempt);
    }
    return name + ".log";
}

FILE* open_log_file(const std::filesystem::path& path) {
#if defined(_WIN32)
    return _wfopen(path.c_str(), L"wbx");
#else
    return std::fopen(path.c_str(), "wbx");
#endif
}

#if defined(__ANDROID__)
int android_priority(LogLevel level) {
    switch (level) {
    case LogLevel::Trace:
        return ANDROID_LOG_VERBOSE;
    case LogLevel::Debug:
        return ANDROID_LOG_DEBUG;
    case LogLevel::Info:
        return ANDROID_LOG_INFO;
    case LogLevel::Warning:
        return ANDROID_LOG_WARN;
    case LogLevel::Error:
        return ANDROID_LOG_ERROR;
    case LogLevel::Fatal:
        return ANDROID_LOG_FATAL;
    }
    return ANDROID_LOG_UNKNOWN;
}

void write_logcat(const Message& message) {
    const int priority = android_priority(message.level);
    const std::string module(message.module);
    // logcat is line-oriented; split multi-line messages.
    std::string_view remaining = message.text;
    do {
        const size_t newline = remaining.find('\n');
        const std::string line(remaining.substr(0, newline));
        __android_log_write(priority, module.c_str(), line.c_str());
        remaining = newline == std::string_view::npos ? ""sv : remaining.substr(newline + 1);
    } while (!remaining.empty());
}
#else
constexpr std::string_view level_color(LogLevel level) {
    switch (level) {
    case LogLevel::Trace:
        return "\033[90m"sv;  // bright black
    case LogLevel::Debug:
        return "\033[36m"sv;  // cyan
    case LogLevel::Info:
        return "\033[0m"sv;
    case LogLevel::Warning:
        return "\033[33m"sv;  // yellow
    case LogLevel::Error:
        return "\033[31m"sv;  // red
    case LogLevel::Fatal:
        return "\033[1;31m"sv;  // bold red
    }
    return "\033[0m"sv;
}

void write_console(const State& state, const Message& message, bool flushLine) {
    FILE* out = message.level >= LogLevel::Error ? stderr : stdout;
    const std::string_view levelStr = to_string(message.level);
    if (state.consoleIsTty) {
        const std::string_view color = level_color(message.level);
        std::fprintf(out, "%.*s[%.*s | %.*s]\033[0m ", static_cast<int>(color.size()), color.data(),
            static_cast<int>(levelStr.size()), levelStr.data(),
            static_cast<int>(message.module.size()), message.module.data());
    } else {
        std::fprintf(out, "[%.*s | %.*s] ", static_cast<int>(levelStr.size()), levelStr.data(),
            static_cast<int>(message.module.size()), message.module.data());
    }
    std::fwrite(message.text.data(), 1, message.text.size(), out);
    std::fputc('\n', out);
    if (flushLine) {
        std::fflush(out);
    }
}
#endif

void write_file(State& state, const Message& message, bool flushLine) {
    if (state.file == nullptr) {
        return;
    }
    const std::string_view levelStr = to_string(message.level);
    std::fprintf(state.file, "[%.*s | %.*s] ", static_cast<int>(levelStr.size()), levelStr.data(),
        static_cast<int>(message.module.size()), message.module.data());
    std::fwrite(message.text.data(), 1, message.text.size(), state.file);
    std::fputc('\n', state.file);
    if (flushLine) {
        std::fflush(state.file);
    }
}

void write_ring(State& state, const Message& message) {
    if (state.options.ringCapacity == 0) {
        return;
    }
    state.ring.push_back({message.level, std::string(message.module), std::string(message.text)});
    while (state.ring.size() > state.options.ringCapacity) {
        state.ring.pop_front();
    }
}

void write_locked(State& state, const Message& message) {
    const bool flushLine = message.level >= state.options.flushOn;
#if defined(__ANDROID__)
    write_logcat(message);
#else
    if (state.options.console) {
        write_console(state, message, flushLine);
    }
#endif
    write_file(state, message, flushLine);
    write_ring(state, message);
    for (const auto& sink : state.extraSinks) {
        sink->write(message);
        if (flushLine) {
            sink->flush();
        }
    }
}

void write_bootstrap_locked(State& state, const Message& message) {
    const bool flushLine = message.level >= state.options.flushOn;
#if defined(__ANDROID__)
    write_logcat(message);
#else
    if (state.options.console) {
        write_console(state, message, flushLine);
    }
#endif
    state.bootstrap.push_back(
        {message.level, std::string(message.module), std::string(message.text)});
    while (state.bootstrap.size() > kBootstrapCapacity) {
        state.bootstrap.pop_front();
    }
}

void write_replayed_locked(State& state, const Message& message) {
    const bool flushLine = message.level >= state.options.flushOn;
    write_file(state, message, flushLine);
    write_ring(state, message);
    for (const auto& sink : state.extraSinks) {
        sink->write(message);
        if (flushLine) {
            sink->flush();
        }
    }
}

void flush_locked(State& state) {
#if !defined(__ANDROID__)
    if (state.options.console) {
        std::fflush(stdout);
        std::fflush(stderr);
    }
#endif
    if (state.file != nullptr) {
        std::fflush(state.file);
    }
    for (const auto& sink : state.extraSinks) {
        sink->flush();
    }
}

}  // namespace

void init(const Options& options) {
    State& state = get_state();
    std::deque<StoredMessage> bootstrap;
    {
        std::lock_guard lock(state.mutex);
        state.close_file_locked();
        state.options = options;
        state.divert = options.divert
            ? std::make_shared<const std::function<bool(const Message&)>>(options.divert)
            : nullptr;
        state.globalLevel = options.level;
        state.ring.clear();
        recompute_min_level_locked(state);
#if !defined(__ANDROID__)
        state.consoleIsTty = BOREALIS_ISATTY(BOREALIS_FILENO(stdout)) != 0;
#endif
        if (state.options.filePrefix.empty()) {
            state.options.filePrefix = "borealis";
        }

        if (!state.initialized) {
            bootstrap.swap(state.bootstrap);
            state.initialized = true;
        }

        if (!state.options.fileDirectory.empty()) {
            std::error_code ec;
            const std::filesystem::path& logsDir = state.options.fileDirectory;
            std::filesystem::create_directories(logsDir, ec);
            if (ec) {
                std::fprintf(stderr,
                    "[WARNING | borealis::log] Failed to create log directory '%s': %s\n",
                    io::fs_path_to_string(logsDir).c_str(), ec.message().c_str());
            } else {
                prune_old_log_files(state, logsDir);

                constexpr unsigned kMaxOpenAttempts = 10;
                std::filesystem::path logPath;
                for (unsigned attempt = 0;
                    attempt < kMaxOpenAttempts && state.file == nullptr; ++attempt)
                {
                    logPath = logsDir / make_timestamped_log_name(state.options.filePrefix, attempt);
                    state.file = open_log_file(logPath);
                }
                if (state.file == nullptr) {
                    std::fprintf(stderr, "[WARNING | borealis::log] Failed to open log file '%s'\n",
                        io::fs_path_to_string(logPath).c_str());
                } else {
                    state.filePath = io::fs_path_to_string(logPath);
                    g_fileFd.store(BOREALIS_FILENO(state.file), std::memory_order_release);
                }
            }
        }
    }

    g_initialized.store(true, std::memory_order_release);

    for (const StoredMessage& stored : bootstrap) {
        const Message message{stored.level, stored.module, stored.text};
        bool accepted = false;
        {
            std::lock_guard lock(state.mutex);
            accepted = stored.level >= effective_level_locked(state, stored.module);
        }
        if (!accepted || (options.divert && options.divert(message))) {
            continue;
        }
        std::lock_guard lock(state.mutex);
        write_replayed_locked(state, message);
    }

    std::lock_guard lock(state.mutex);
    write_file(state, {LogLevel::Info, "borealis::log"sv, "File logging initialized"sv}, true);
}

void shutdown() {
    State& state = get_state();
    std::lock_guard lock(state.mutex);
    flush_locked(state);
    state.close_file_locked();
}

void flush() {
    State& state = get_state();
    std::lock_guard lock(state.mutex);
    flush_locked(state);
}

LogLevel level() {
    State& state = get_state();
    std::lock_guard lock(state.mutex);
    return state.globalLevel;
}

void set_level(LogLevel level) {
    State& state = get_state();
    std::lock_guard lock(state.mutex);
    state.globalLevel = level;
    recompute_min_level_locked(state);
}

void set_module_level(std::string_view modulePrefix, LogLevel level) {
    State& state = get_state();
    std::lock_guard lock(state.mutex);
    const auto it = std::find_if(state.moduleLevels.begin(), state.moduleLevels.end(),
        [&](const auto& entry) { return entry.first == modulePrefix; });
    if (it != state.moduleLevels.end()) {
        it->second = level;
    } else {
        state.moduleLevels.emplace_back(std::string(modulePrefix), level);
    }
    recompute_min_level_locked(state);
}

void clear_module_levels() {
    State& state = get_state();
    std::lock_guard lock(state.mutex);
    state.moduleLevels.clear();
    recompute_min_level_locked(state);
}

void add_sink(std::shared_ptr<Sink> sink) {
    State& state = get_state();
    std::lock_guard lock(state.mutex);
    state.extraSinks.push_back(std::move(sink));
}

const char* file_path() {
    State& state = get_state();
    std::lock_guard lock(state.mutex);
    return state.filePath.empty() ? nullptr : state.filePath.c_str();
}

int file_descriptor() {
    return g_fileFd.load(std::memory_order_acquire);
}

void visit_ring(const std::function<void(const Message&)>& visitor) {
    State& state = get_state();
    std::lock_guard lock(state.mutex);
    for (const StoredMessage& stored : state.ring) {
        visitor({stored.level, stored.module, stored.text});
    }
}

namespace detail {

bool enabled(LogLevel level, std::string_view module) noexcept {
    if (level == LogLevel::Fatal) {
        return true;
    }
    if (!g_initialized.load(std::memory_order_acquire)) {
        return true;
    }
    if (static_cast<int>(level) < g_minLevel.load(std::memory_order_acquire)) {
        return false;
    }
    State& state = get_state();
    std::lock_guard lock(state.mutex);
    return level >= effective_level_locked(state, module);
}

void write(LogLevel level, std::string_view module, std::string_view text) {
    State& state = get_state();
    const Message message{level, module, text};
    std::shared_ptr<const std::function<bool(const Message&)>> divert;
    {
        std::lock_guard lock(state.mutex);
        if (!state.initialized) {
            write_bootstrap_locked(state, message);
            return;
        }
        divert = state.divert;
    }
    // divert runs outside the lock and may log or inspect the ring.
    if (level != LogLevel::Fatal && divert && (*divert)(message)) {
        return;
    }
    std::lock_guard lock(state.mutex);
    write_locked(state, message);
}

[[noreturn]] void write_fatal(std::string_view module, std::string_view text) {
    State& state = get_state();
    std::function<void(std::string_view)> onFatal;
    {
        std::lock_guard lock(state.mutex);
        const Message message{LogLevel::Fatal, module, text};
        if (state.initialized) {
            write_locked(state, message);
        } else {
            write_bootstrap_locked(state, message);
        }
        flush_locked(state);
        onFatal = state.options.onFatal;
    }
    if (onFatal) {
        onFatal(text);
    }
    std::abort();
}

}  // namespace detail
}  // namespace borealis::log

namespace borealis::log {
namespace {

void aurora_log_callback_impl(
    AuroraLogLevel level, const char* module, const char* message, unsigned int len) {
    const std::string_view moduleView = module != nullptr ? module : "aurora";
    const std::string_view text{message != nullptr ? message : "", message != nullptr ? len : 0};
    const LogLevel mapped = from_aurora_level(level);
    if (mapped == LogLevel::Fatal) {
        detail::write_fatal(moduleView, text);
    }
    if (detail::enabled(mapped, moduleView)) {
        detail::write(mapped, moduleView, text);
    }
}

}  // namespace

AuroraLogCallback aurora_callback() noexcept {
    return &aurora_log_callback_impl;
}

LogLevel from_aurora_level(const AuroraLogLevel level) noexcept {
    switch (level) {
    case LOG_DEBUG:
        return LogLevel::Debug;
    case LOG_INFO:
        return LogLevel::Info;
    case LOG_WARNING:
        return LogLevel::Warning;
    case LOG_ERROR:
        return LogLevel::Error;
    case LOG_FATAL:
        return LogLevel::Fatal;
    }
    return LogLevel::Info;
}

AuroraLogLevel to_aurora_level(const LogLevel level) noexcept {
    switch (level) {
    case LogLevel::Trace:
    case LogLevel::Debug:
        return LOG_DEBUG;
    case LogLevel::Info:
        return LOG_INFO;
    case LogLevel::Warning:
        return LOG_WARNING;
    case LogLevel::Error:
        return LOG_ERROR;
    case LogLevel::Fatal:
        return LOG_FATAL;
    }
    return LOG_INFO;
}

}  // namespace borealis::log

auto fmt::formatter<borealis::LogLevel>::format(borealis::LogLevel level, format_context& ctx) const
    -> format_context::iterator {
    return formatter<std::string_view>::format(borealis::to_string(level), ctx);
}

using borealis::LogLevel;

namespace {

LogLevel from_c_level(BorealisLogLevel level) {
    switch (level) {
    case BOREALIS_LOG_TRACE:
        return LogLevel::Trace;
    case BOREALIS_LOG_DEBUG:
        return LogLevel::Debug;
    case BOREALIS_LOG_INFO:
        return LogLevel::Info;
    case BOREALIS_LOG_WARNING:
        return LogLevel::Warning;
    case BOREALIS_LOG_ERROR:
        return LogLevel::Error;
    case BOREALIS_LOG_FATAL:
        return LogLevel::Fatal;
    }
    return LogLevel::Info;
}

void c_write(LogLevel level, const char* module, std::string_view text) {
    const std::string_view moduleView = module != nullptr ? module : "app";
    if (level == LogLevel::Fatal) {
        borealis::log::detail::write_fatal(moduleView, text);
    }
    if (borealis::log::detail::enabled(level, moduleView)) {
        borealis::log::detail::write(level, moduleView, text);
    }
}

}  // namespace

extern "C" void borealis_log_write(
    BorealisLogLevel level, const char* module, const char* message) {
    c_write(from_c_level(level), module, message != nullptr ? message : "");
}

extern "C" void borealis_log_vprintf(
    BorealisLogLevel level, const char* module, const char* format, va_list ap) {
    const LogLevel mapped = from_c_level(level);
    if (format == nullptr) {
        c_write(mapped, module, "");
        return;
    }
    if (mapped != LogLevel::Fatal &&
        !borealis::log::detail::enabled(mapped, module != nullptr ? module : "app"))
    {
        return;
    }

    va_list sizingArgs;
    va_copy(sizingArgs, ap);
    const int length = std::vsnprintf(nullptr, 0, format, sizingArgs);
    va_end(sizingArgs);
    if (length < 0) {
        c_write(LogLevel::Error, "borealis::log", "Log message formatting failed");
        return;
    }

    std::vector<char> buffer(static_cast<size_t>(length) + 1);
    va_list writeArgs;
    va_copy(writeArgs, ap);
    std::vsnprintf(buffer.data(), buffer.size(), format, writeArgs);
    va_end(writeArgs);
    c_write(mapped, module, std::string_view(buffer.data(), static_cast<size_t>(length)));
}

extern "C" void borealis_log_printf(
    BorealisLogLevel level, const char* module, const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    borealis_log_vprintf(level, module, format, ap);
    va_end(ap);
}
