cmake_minimum_required(VERSION 3.25)
project(foxhollow_example_code_mod C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(FOXHOLLOW_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../.." CACHE PATH "Path to the Foxhollow source root")
set(FOXHOLLOW_BINARY "${FOXHOLLOW_ROOT}/build/foxhollow" CACHE FILEPATH "Game binary or link stub to link against")

if (APPLE)
    if (CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm64|aarch64)$")
        set(FH_MOD_PLATFORM "macos-arm64")
    else ()
        set(FH_MOD_PLATFORM "macos-x86_64")
    endif ()
elseif (WIN32)
    set(FH_MOD_PLATFORM "windows-amd64")
else ()
    set(FH_MOD_PLATFORM "linux-amd64")
endif ()

add_library(mod MODULE src/mod.c)
set_target_properties(mod PROPERTIES
        PREFIX ""
        OUTPUT_NAME "mod"
        C_VISIBILITY_PRESET hidden
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib/${FH_MOD_PLATFORM}")
target_include_directories(mod PRIVATE "${FOXHOLLOW_ROOT}/port/include")

if (APPLE)
    target_link_options(mod PRIVATE -Xlinker -bundle_loader -Xlinker "${FOXHOLLOW_BINARY}")
    set_property(TARGET mod APPEND PROPERTY LINK_DEPENDS "${FOXHOLLOW_BINARY}")
elseif (WIN32)
    target_link_libraries(mod PRIVATE "${FOXHOLLOW_BINARY}")
else ()
    target_link_options(mod PRIVATE -Wl,--allow-shlib-undefined)
endif ()
