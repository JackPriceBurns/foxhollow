#pragma once

#include <fmt/base.h>
#include <fmt/format.h>

#include <cstdint>
#include <filesystem>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace borealis {

enum class LogLevel : int {
    Trace,
    Debug,
    Info,
    Warning,
    Error,
    Fatal,
};

/** Returns the uppercase log level name. */
std::string_view to_string(LogLevel level) noexcept;

/** Parses a case-insensitive log level, including "warn" and "warning". */
std::optional<LogLevel> level_from_string(std::string_view text) noexcept;

namespace log {

struct Message {
    LogLevel level;
    std::string_view module;
    std::string_view text;
};

/** Log output called under the logger lock. Implementations must not log. */
class Sink {
public:
    virtual ~Sink() = default;
    virtual void write(const Message& message) = 0;
    virtual void flush() = 0;
};

struct Options {
    /** Global minimum log level. */
    LogLevel level = LogLevel::Info;
    /** Flush sinks after messages at or above this level. */
    LogLevel flushOn = LogLevel::Warning;
    /** Write to stdout/stderr (stderr at Error and above). Android uses logcat instead. */
    bool console = true;
    /** Log directory. Empty disables file logging. */
    std::filesystem::path fileDirectory{};
    /** Prefix for <prefix>-YYYYmmdd-HHMMSS[-N].log, where -N resolves collisions. */
    std::string filePrefix = "borealis";
    /** Previous file prefixes included in retention pruning. */
    std::vector<std::string> legacyFilePrefixes{};
    /** Maximum retained files, including the active file. Pruned during init(). */
    std::uint32_t maxRetainedFiles = 10;
    /** Maximum total size of previous log files. */
    std::uint64_t maxRetainedBytes = 100ull * 1024ull * 1024ull;
    /** In-memory message count. 0 disables the ring buffer. */
    std::size_t ringCapacity = 1024;
    /**
     * Return true to consume a non-fatal message before it reaches the sinks.
     * Called without the logger lock and may run concurrently.
     */
    std::function<bool(const Message&)> divert{};
    /** Called after flushing a fatal message and before aborting. */
    std::function<void(std::string_view)> onFatal{};
};

/**
 * Initializes or reinitializes the logger.
 *
 * Pre-init messages are buffered and replayed to non-console sinks. Reinitializing
 * clears the ring buffer but preserves module levels and added sinks. Call before
 * starting threads that log.
 */
void init(const Options& options);

/** Flush and close the log file. Console logging remains available. */
void shutdown();

/** Flush all sinks. */
void flush();

LogLevel level();
void set_level(LogLevel level);

/**
 * Sets the minimum level for a module prefix. The longest prefix wins; Fatal is
 * never filtered.
 */
void set_module_level(std::string_view modulePrefix, LogLevel level);
void clear_module_levels();

/** Registers an additional sink. */
void add_sink(std::shared_ptr<Sink> sink);

/** UTF-8 path of the active log file, or nullptr. Valid until reinit/shutdown. */
const char* file_path();

/** File descriptor of the active log file, or -1. Safe to read from crash handlers. */
int file_descriptor();

/** Visits retained messages, oldest first, under the logger lock. */
void visit_ring(const std::function<void(const Message&)>& visitor);

namespace detail {
bool enabled(LogLevel level, std::string_view module) noexcept;
void write(LogLevel level, std::string_view module, std::string_view text);
[[noreturn]] void write_fatal(std::string_view module, std::string_view text);
}  // namespace detail

}  // namespace log

/** Named logging interface compatible with aurora::Module. */
struct Log {
    const char* name;

    template <typename... T>
    void report(const LogLevel level, fmt::format_string<T...> fmt, T&&... args) const noexcept {
        if (level == LogLevel::Fatal) {
            log::detail::write_fatal(name, fmt::format(fmt, std::forward<T>(args)...));
        }
        if (!log::detail::enabled(level, name)) {
            return;
        }
        const auto message = fmt::format(fmt, std::forward<T>(args)...);
        log::detail::write(level, name, message);
    }

    template <typename... T>
    void trace(fmt::format_string<T...> fmt, T&&... args) const noexcept {
        report(LogLevel::Trace, fmt, std::forward<T>(args)...);
    }

    template <typename... T>
    void debug(fmt::format_string<T...> fmt, T&&... args) const noexcept {
        report(LogLevel::Debug, fmt, std::forward<T>(args)...);
    }

    template <typename... T>
    void info(fmt::format_string<T...> fmt, T&&... args) const noexcept {
        report(LogLevel::Info, fmt, std::forward<T>(args)...);
    }

    template <typename... T>
    void warn(fmt::format_string<T...> fmt, T&&... args) const noexcept {
        report(LogLevel::Warning, fmt, std::forward<T>(args)...);
    }

    template <typename... T>
    void error(fmt::format_string<T...> fmt, T&&... args) const noexcept {
        report(LogLevel::Error, fmt, std::forward<T>(args)...);
    }

    template <typename... T>
    [[noreturn]] void fatal(fmt::format_string<T...> fmt, T&&... args) const noexcept {
        const auto message = fmt::format(fmt, std::forward<T>(args)...);
        log::detail::write_fatal(name, message);
    }
};

}  // namespace borealis

template <>
struct fmt::formatter<borealis::LogLevel> : formatter<std::string_view> {
    auto format(borealis::LogLevel level, format_context& ctx) const -> format_context::iterator;
};
