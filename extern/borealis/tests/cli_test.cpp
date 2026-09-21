#include "borealis/cli.hpp"

#include <gtest/gtest.h>

#include <string>
#include <vector>

using borealis::LogLevel;

namespace {

// Parses standard options with one test game option.
cxxopts::ParseResult parse_args(
    std::vector<std::string> args, const borealis::cli::StandardOptionSet& set = {}) {
    static cxxopts::Options options{"test", "borealis::cli test"};
    options = cxxopts::Options{"test", "borealis::cli test"};
    borealis::cli::add_standard_options(options, set);
    options.add_options()("stage", "A game-owned option", cxxopts::value<std::string>());
    options.allow_unrecognised_options();

    std::vector<char*> argv;
    args.insert(args.begin(), "test");
    argv.reserve(args.size());
    for (std::string& arg : args) {
        argv.push_back(arg.data());
    }
    int argc = static_cast<int>(argv.size());
    char** argvData = argv.data();
    return options.parse(argc, argvData);
}

TEST(Cli, NothingGiven) {
    const auto standard = borealis::cli::parse(parse_args({}));
    EXPECT_FALSE(standard.level.has_value());
    EXPECT_TRUE(standard.logDir.empty());
    EXPECT_TRUE(standard.userDir.empty());
    EXPECT_FALSE(standard.console);

    borealis::log::Options options{};
    options.level = LogLevel::Warning;
    options.fileDirectory = "/ports/own/logs";
    standard.apply_to(options);
    EXPECT_EQ(options.level, LogLevel::Warning);
    EXPECT_EQ(options.fileDirectory, "/ports/own/logs");
}

TEST(Cli, Overlay) {
    const auto standard =
        borealis::cli::parse(parse_args({"--log-level", "trace", "--log-dir", "/tmp/l"}));
    EXPECT_EQ(standard.level, LogLevel::Trace);
    EXPECT_EQ(standard.logDir, "/tmp/l");

    borealis::log::Options options{};
    options.level = LogLevel::Warning;
    options.filePrefix = "game";
    standard.apply_to(options);
    EXPECT_EQ(options.level, LogLevel::Trace);
    EXPECT_EQ(options.fileDirectory, "/tmp/l");
    EXPECT_EQ(options.filePrefix, "game");
}

TEST(Cli, ShortFlagAndEqualsForm) {
    EXPECT_EQ(borealis::cli::parse(parse_args({"-L", "error"})).level, LogLevel::Error);
    EXPECT_TRUE(borealis::cli::parse(parse_args({"--log-level=warn"})).level == LogLevel::Warning);
}

TEST(Cli, ConsoleImplicitValue) {
    EXPECT_TRUE(borealis::cli::parse(parse_args({"--console"})).console);
    EXPECT_FALSE(borealis::cli::parse(parse_args({"--console=false"})).console);
    EXPECT_FALSE(borealis::cli::parse(parse_args({})).console);
}

TEST(Cli, InvalidLevelThrows) {
    try {
        borealis::cli::parse(parse_args({"--log-level", "verbose"}));
        FAIL() << "Expected cxxopts parsing error";
    } catch (const cxxopts::exceptions::exception& e) {
        EXPECT_NE(std::string(e.what()).find("verbose"), std::string::npos);
    }
}

TEST(Cli, EmptyPathRejected) {
    EXPECT_THROW(
        borealis::cli::parse(parse_args({"--user-dir", ""})), cxxopts::exceptions::exception);
}

TEST(Cli, UnregisteredOptionsAreAbsent) {
    // Disabled options remain absent from the parse result.
    borealis::cli::StandardOptionSet set{};
    set.userDir = false;
    set.console = false;
    const auto standard = borealis::cli::parse(parse_args({"--log-level", "info"}, set));
    EXPECT_EQ(standard.level, LogLevel::Info);
    EXPECT_TRUE(standard.userDir.empty());
    EXPECT_FALSE(standard.console);
}

TEST(Cli, GameOptionsCoexist) {
    const auto parsed = parse_args({"--stage", "F_SP103", "-L", "debug", "--unknown-thing"});
    EXPECT_EQ(borealis::cli::parse(parsed).level, LogLevel::Debug);
    EXPECT_EQ(parsed["stage"].as<std::string>(), "F_SP103");
    // Preserve unknown options for SDL and Aurora.
    EXPECT_EQ(parsed.unmatched().size(), 1);
}

}  // namespace
