cmake_minimum_required(VERSION 3.25)
project(foxhollow_example_mirror_mode C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(FOXHOLLOW_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../.." CACHE PATH "Path to the Foxhollow source root")

string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" FH_MOD_ARCH)
if (FH_MOD_ARCH MATCHES "^(arm64|aarch64)$")
    set(FH_MOD_ARCH "arm64")
elseif (FH_MOD_ARCH MATCHES "^(x86_64|amd64)$")
    set(FH_MOD_ARCH "amd64")
endif ()

if (APPLE)
    set(FH_MOD_PLATFORM "macos-${FH_MOD_ARCH}")
    set(FH_MOD_DEFAULT_BINARY "${FOXHOLLOW_ROOT}/build/foxhollow")
elseif (WIN32)
    set(FH_MOD_PLATFORM "windows-${FH_MOD_ARCH}")
    set(FH_MOD_DEFAULT_BINARY "${FOXHOLLOW_ROOT}/build/foxhollow.lib")
else ()
    set(FH_MOD_PLATFORM "linux-${FH_MOD_ARCH}")
    set(FH_MOD_DEFAULT_BINARY "")
endif ()

set(FOXHOLLOW_BINARY "${FH_MOD_DEFAULT_BINARY}" CACHE FILEPATH
        "Game binary, import library, or link stub to resolve game symbols against")

add_library(mod MODULE src/mod.c)
set_target_properties(mod PROPERTIES
        PREFIX ""
        SUFFIX "${CMAKE_SHARED_MODULE_SUFFIX}"
        OUTPUT_NAME "mod"
        C_VISIBILITY_PRESET hidden
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib/${FH_MOD_PLATFORM}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib/${FH_MOD_PLATFORM}")
target_include_directories(mod PRIVATE "${FOXHOLLOW_ROOT}/port/include")

if (APPLE)
    target_link_options(mod PRIVATE -Xlinker -bundle_loader -Xlinker "${FOXHOLLOW_BINARY}")
    set_property(TARGET mod APPEND PROPERTY LINK_DEPENDS "${FOXHOLLOW_BINARY}")
elseif (WIN32)
    if (NOT FOXHOLLOW_BINARY MATCHES "\\.lib$")
        message(FATAL_ERROR "FOXHOLLOW_BINARY must be the game's import library (.lib) on Windows")
    endif ()
    target_link_libraries(mod PRIVATE "${FOXHOLLOW_BINARY}")
else ()
    target_link_options(mod PRIVATE -Wl,--allow-shlib-undefined)
endif ()
