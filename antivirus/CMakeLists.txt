# Sleelavirin(TM) native antivirus engine - portable build.
# Max Rupplin - MEARVK LLC - 2026.
#
# One build definition for every supported target:
#   * Windows 10+/7+  : MSVC (Visual Studio) or MinGW-w64 (GCC/Clang)
#   * Windows XP      : an XP-targeting toolchain (see PLATFORMS.md); set
#                       -DSLEELAVIRIN_WINXP=ON to drop the version floor to 0x0501
#   * macOS           : Apple Clang / Xcode (set CMAKE_OSX_DEPLOYMENT_TARGET)
#   * Linux / Unix    : GCC or Clang
#
# Produces: libsleelavirin (C static lib), libsleelavirin_cpp (C++ static lib),
# and the sleelavirin-scan command-line scanner. Enable tests with
# -DSLEELAVIRIN_BUILD_TESTS=ON (default ON) and run `ctest`.
#
# Examples:
#   cmake -S . -B build && cmake --build build            # native host
#   cmake -S . -B build -DSLEELAVIRIN_WINXP=ON             # Windows XP floor
#   cmake -S . -B build -DCMAKE_OSX_DEPLOYMENT_TARGET=10.9 # older macOS

cmake_minimum_required(VERSION 3.10)
project(sleelavirin C CXX)

option(SLEELAVIRIN_BUILD_TESTS "Build and register the test suite" ON)
option(SLEELAVIRIN_WINXP "Target the Windows XP API floor (0x0501)" OFF)

# Language levels: portable C11 + C++17 (the whole engine targets these).
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Warnings (match the GNU Makefile's strictness where practical) --------
if(MSVC)
    add_compile_options(/W4 /permissive-)
    # We use the standard CRT deliberately; silence the "use *_s" deprecations.
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(-O2 -Wall -Wextra -Wpedantic)
endif()

# ---- Windows API version floor --------------------------------------------
if(WIN32)
    if(SLEELAVIRIN_WINXP)
        add_compile_definitions(_WIN32_WINNT=0x0501 WINVER=0x0501)
    endif()
    # (If neither is set, sleelavirin_compat.h defaults to the Windows 7 floor.)
endif()

# ---- Sources ---------------------------------------------------------------
set(SLEELAVIRIN_C_SOURCES
    src/sleelavirin_scan.c
    src/sleelavirin_heuristic.c
    src/sleelavirin_native.c
    src/sleelavirin_ac.c
    src/sleelavirin_sig.c
    src/sleelavirin_bridge.c)

add_library(sleelavirin STATIC ${SLEELAVIRIN_C_SOURCES})
target_include_directories(sleelavirin PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

add_library(sleelavirin_cpp STATIC src/sleelavirin_cpp.cpp)
target_include_directories(sleelavirin_cpp PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)
target_link_libraries(sleelavirin_cpp PUBLIC sleelavirin)

add_executable(sleelavirin-scan src/sleelavirin_native_main.c)
target_link_libraries(sleelavirin-scan PRIVATE sleelavirin)

# ---- Install rules ---------------------------------------------------------
install(TARGETS sleelavirin sleelavirin_cpp
        ARCHIVE DESTINATION lib)
install(TARGETS sleelavirin-scan
        RUNTIME DESTINATION bin)
install(FILES
        src/sleelavirin_scan.h
        src/sleelavirin_heuristic.h
        src/sleelavirin_native.h
        src/sleelavirin_ac.h
        src/sleelavirin_sig.h
        src/sleelavirin_bridge.h
        src/sleelavirin_compat.h
        src/sleelavirin_cpp.hpp
        DESTINATION include/sleelavirin)

# ---- Tests -----------------------------------------------------------------
if(SLEELAVIRIN_BUILD_TESTS)
    enable_testing()

    add_executable(test_scan tests/test_scan.c)
    target_link_libraries(test_scan PRIVATE sleelavirin)

    add_executable(test_ac tests/test_ac.c)
    target_link_libraries(test_ac PRIVATE sleelavirin)

    add_executable(test_bridge tests/test_bridge.c)
    target_link_libraries(test_bridge PRIVATE sleelavirin)

    add_executable(test_cpp_scan tests/test_cpp_scan.cpp)
    target_link_libraries(test_cpp_scan PRIVATE sleelavirin_cpp sleelavirin)

    # The hash/cpp tests read fixtures relative to the antivirus/ directory.
    add_test(NAME scan   COMMAND test_scan   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    add_test(NAME ac     COMMAND test_ac     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    add_test(NAME bridge COMMAND test_bridge WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    add_test(NAME cpp    COMMAND test_cpp_scan WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()
