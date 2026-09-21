#include "borealis/cli.hpp"

#include "borealis/io.hpp"

#include <string>

namespace borealis::cli {
namespace {

constexpr const char* kLogLevelHelp = "Log level: trace, debug, info, warning, error or fatal";

// operator[] throws for unregistered options.
bool given(const cxxopts::ParseResult& parsed, const char* name) {
    return parsed.count(name) > 0;
}

std::filesystem::path read_path(const cxxopts::ParseResult& parsed, const char* name) {
    if (!given(parsed, name)) {
        return {};
    }
    const auto value = parsed[name].as<std::string>();
    if (value.empty()) {
        throw cxxopts::exceptions::parsing(std::string("--") + name + " expects a directory path");
    }
    return io::fs_path_from_utf8(value);
}

}  // namespace

void StandardOptions::apply_to(log::Options& options) const {
    if (level) {
        options.level = *level;
    }
    if (!logDir.empty()) {
        options.fileDirectory = logDir;
    }
}

void add_standard_options(cxxopts::Options& options, const StandardOptionSet& set) {
    auto adder = options.add_options("Standard");
    if (set.logLevel) {
        adder("L,log-level", kLogLevelHelp, cxxopts::value<std::string>());
    }
    if (set.logDir) {
        adder("log-dir", "Directory to write log files to", cxxopts::value<std::string>());
    }
    if (set.userDir) {
        adder("user-dir", "Directory to read and write user data from",
            cxxopts::value<std::string>());
    }
    if (set.console) {
        adder("console", "Show the Windows console window for logs",
            cxxopts::value<bool>()->default_value("false")->implicit_value("true"));
    }
}

StandardOptions parse(const cxxopts::ParseResult& parsed) {
    StandardOptions standard{};

    if (given(parsed, "log-level")) {
        const auto text = parsed["log-level"].as<std::string>();
        standard.level = level_from_string(text);
        if (!standard.level) {
            throw cxxopts::exceptions::parsing(
                "--log-level: unknown level '" + text + "' (" + kLogLevelHelp + ")");
        }
    }
    standard.logDir = read_path(parsed, "log-dir");
    standard.userDir = read_path(parsed, "user-dir");
    if (given(parsed, "console")) {
        standard.console = parsed["console"].as<bool>();
    }

    return standard;
}

}  // namespace borealis::cli
