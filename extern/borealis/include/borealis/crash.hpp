#pragma once

namespace borealis::crash {

/**
 * Installs crash/termination handlers that write a crash report to stderr and the active
 * borealis::log file when available. Call after borealis::log initialization.
 */
void install();

}  // namespace borealis::crash
