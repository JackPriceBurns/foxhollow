# Detects application versions from Git tags.
#
# Safe to call before project():
#   include(extern/borealis/cmake/DetectVersion.cmake)
#   borealis_detect_version()
#   project(dusklight LANGUAGES C CXX VERSION ${BOREALIS_APP_VERSION})
#
# Sets: BOREALIS_APP_DESCRIBE, BOREALIS_APP_VERSION (4-part), BOREALIS_APP_SHORT_VERSION
#       (3-part), BOREALIS_APP_VERSION_CODE (Android-style integer),
#       BOREALIS_APP_REVISION, BOREALIS_APP_BRANCH, BOREALIS_APP_DATE.
#
# Writes APP_VERSION and APP_VERSION_CODE to $GITHUB_ENV in CI.
include_guard(GLOBAL)

function(borealis_git_describe _bv_root _bv_out)
    find_package(Git QUIET)
    if (NOT GIT_FOUND)
        set(${_bv_out} "" PARENT_SCOPE)
        return()
    endif ()
    execute_process(WORKING_DIRECTORY "${_bv_root}"
            COMMAND ${GIT_EXECUTABLE} describe --tags --always --dirty --match "v*"
            OUTPUT_VARIABLE _bv_describe
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ERROR_QUIET)
    set(${_bv_out} "${_bv_describe}" PARENT_SCOPE)
endfunction()

macro(borealis_detect_version)
    if (${ARGC} GREATER 0)
        set(_bv_root "${ARGV0}")
    elseif (BOREALIS_APP_SOURCE_DIR)
        set(_bv_root "${BOREALIS_APP_SOURCE_DIR}")
    else ()
        set(_bv_root "${CMAKE_CURRENT_SOURCE_DIR}")
    endif ()

    set(BOREALIS_APP_VERSION_OVERRIDE "" CACHE STRING "Override version string (skips git detection and format validation)")

    if (BOREALIS_APP_VERSION_OVERRIDE)
        set(BOREALIS_APP_DESCRIBE "${BOREALIS_APP_VERSION_OVERRIDE}")
        set(BOREALIS_APP_VERSION "0.0.0.0")
        set(BOREALIS_APP_SHORT_VERSION "0.0.0")
        set(BOREALIS_APP_VERSION_CODE "1")
        set(BOREALIS_APP_REVISION "")
        set(BOREALIS_APP_BRANCH "")
        set(BOREALIS_APP_DATE "")
        message(STATUS "borealis: application version overridden to ${BOREALIS_APP_DESCRIBE}")
    else ()
        find_package(Git)
        if (GIT_FOUND)
            # Reconfigure when Git HEAD changes.
            execute_process(WORKING_DIRECTORY ${_bv_root} COMMAND ${GIT_EXECUTABLE} rev-parse --git-path HEAD
                    OUTPUT_VARIABLE _bv_git_head_filename
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
            get_filename_component(_bv_git_head_filename "${_bv_git_head_filename}" ABSOLUTE BASE_DIR "${_bv_root}")
            set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${_bv_git_head_filename}")

            execute_process(WORKING_DIRECTORY ${_bv_root} COMMAND ${GIT_EXECUTABLE} rev-parse --symbolic-full-name HEAD
                    OUTPUT_VARIABLE _bv_git_head_symbolic
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
            execute_process(WORKING_DIRECTORY ${_bv_root}
                    COMMAND ${GIT_EXECUTABLE} rev-parse --git-path ${_bv_git_head_symbolic}
                    OUTPUT_VARIABLE _bv_git_head_symbolic_filename
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
            get_filename_component(_bv_git_head_symbolic_filename "${_bv_git_head_symbolic_filename}" ABSOLUTE BASE_DIR "${_bv_root}")
            set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${_bv_git_head_symbolic_filename}")

            execute_process(WORKING_DIRECTORY ${_bv_root} COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
                    OUTPUT_VARIABLE BOREALIS_APP_REVISION
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
            execute_process(WORKING_DIRECTORY ${_bv_root} COMMAND ${GIT_EXECUTABLE} describe --tags --long --dirty --match "v*"
                    OUTPUT_VARIABLE BOREALIS_APP_DESCRIBE
                    OUTPUT_STRIP_TRAILING_WHITESPACE
                    ERROR_QUIET)

            # Remove the Git hash and a clean "-0" suffix.
            string(REGEX REPLACE "-[^-]+(-dirty|)$" "\\1" BOREALIS_APP_DESCRIBE "${BOREALIS_APP_DESCRIBE}")
            string(REGEX REPLACE "-0$" "" BOREALIS_APP_DESCRIBE "${BOREALIS_APP_DESCRIBE}")

            execute_process(WORKING_DIRECTORY ${_bv_root} COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
                    OUTPUT_VARIABLE BOREALIS_APP_BRANCH
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
            execute_process(WORKING_DIRECTORY ${_bv_root} COMMAND ${GIT_EXECUTABLE} log -1 --format=%ad
                    OUTPUT_VARIABLE BOREALIS_APP_DATE
                    OUTPUT_STRIP_TRAILING_WHITESPACE)
        else ()
            message(STATUS "Unable to find git, commit information will not be available")
        endif ()

        if (BOREALIS_APP_DESCRIBE MATCHES "^v([0-9]+)\\.([0-9]+)\\.([0-9]+)([-+].*)?$")
            set(BOREALIS_APP_SHORT_VERSION "${CMAKE_MATCH_1}.${CMAKE_MATCH_2}.${CMAKE_MATCH_3}")
            set(_bv_ver_major ${CMAKE_MATCH_1})
            set(_bv_ver_minor ${CMAKE_MATCH_2})
            set(_bv_ver_patch ${CMAKE_MATCH_3})
            set(BOREALIS_APP_VERSION_TWEAK "0")
            if (BOREALIS_APP_DESCRIBE MATCHES "^v[0-9]+\\.[0-9]+\\.[0-9]+-([0-9]+)(-dirty)?$")
                set(BOREALIS_APP_VERSION_TWEAK "${CMAKE_MATCH_1}")
            elseif (BOREALIS_APP_DESCRIBE MATCHES "^v[0-9]+\\.[0-9]+\\.[0-9]+-[0-9A-Za-z.-]+-([0-9]+)(-dirty)?$")
                set(BOREALIS_APP_VERSION_TWEAK "${CMAKE_MATCH_1}")
            endif ()
            set(BOREALIS_APP_VERSION "${BOREALIS_APP_SHORT_VERSION}.${BOREALIS_APP_VERSION_TWEAK}")
            if (BOREALIS_APP_VERSION_TWEAK GREATER 999)
                set(_bv_tweak 999)
            else ()
                set(_bv_tweak ${BOREALIS_APP_VERSION_TWEAK})
            endif ()
            # major*1e7 + minor*1e5 + patch*1e3 + tweak
            math(EXPR BOREALIS_APP_VERSION_CODE
                    "${_bv_ver_major} * 10000000 + ${_bv_ver_minor} * 100000 + ${_bv_ver_patch} * 1000 + ${_bv_tweak}")
        else ()
            set(BOREALIS_APP_DESCRIBE "UNKNOWN-VERSION")
            set(BOREALIS_APP_VERSION "0.0.0.0")
            set(BOREALIS_APP_SHORT_VERSION "0.0.0")
            set(BOREALIS_APP_VERSION_CODE "1")
        endif ()
    endif ()

    if (DEFINED ENV{GITHUB_ENV})
        file(APPEND "$ENV{GITHUB_ENV}" "APP_VERSION=${BOREALIS_APP_DESCRIBE}\n")
        file(APPEND "$ENV{GITHUB_ENV}" "APP_VERSION_CODE=${BOREALIS_APP_VERSION_CODE}\n")
    endif ()
    message(STATUS "borealis: application version set to ${BOREALIS_APP_DESCRIBE}")
endmacro()

# Sets BOREALIS_PLATFORM_NAME.
macro(borealis_detect_platform_name)
    if (CMAKE_SYSTEM_NAME STREQUAL Windows)
        set(BOREALIS_PLATFORM_NAME win32)
    elseif (CMAKE_SYSTEM_NAME STREQUAL Darwin)
        if (IOS)
            set(BOREALIS_PLATFORM_NAME ios)
        elseif (TVOS)
            set(BOREALIS_PLATFORM_NAME tvos)
        else ()
            set(BOREALIS_PLATFORM_NAME macos)
        endif ()
    else ()
        string(TOLOWER "${CMAKE_SYSTEM_NAME}" BOREALIS_PLATFORM_NAME)
    endif ()
endmacro()

macro(borealis_configure_version_header _bv_template _bv_output)
    borealis_detect_platform_name()
    configure_file("${_bv_template}" "${_bv_output}")
endmacro()
