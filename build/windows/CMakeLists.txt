cmake_minimum_required(VERSION 3.21)
project(SLeeLaNativeWindows LANGUAGES C CXX)

# This project deliberately builds the existing source files in place. It is a
# first MSVC validation slice for the OS abstraction layer, not a claim that
# the complete GNU Make-based runtime has been ported to MSVC yet.
if(NOT WIN32 OR NOT MSVC)
  message(FATAL_ERROR "Use this project with the Visual Studio MSVC toolchain on Windows.")
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SLEELA_IMPL "${CMAKE_CURRENT_LIST_DIR}/../../impl")
set(SLEELA_CORE "${SLEELA_IMPL}/core")

include(CTest)
find_package(Threads REQUIRED)

function(sleela_platform_smoke target test_source implementation_source)
  add_executable(${target}
    "${SLEELA_CORE}/${test_source}"
    "${SLEELA_CORE}/${implementation_source}"
  )
  target_include_directories(${target} PRIVATE "${SLEELA_CORE}")
  target_compile_definitions(${target} PRIVATE
    WIN32_LEAN_AND_MEAN
    NOMINMAX
    _CRT_SECURE_NO_WARNINGS
    _WINSOCK_DEPRECATED_NO_WARNINGS
  )
  add_test(NAME ${target} COMMAND ${target})
endfunction()

sleela_platform_smoke(thread_platform_smoke thread_platform_smoke.c sleela_thread.c)
target_link_libraries(thread_platform_smoke PRIVATE Threads::Threads)

sleela_platform_smoke(io_platform_smoke io_platform_smoke.c sleela_io.c)

sleela_platform_smoke(net_platform_smoke net_platform_smoke.c sleela_net.c)
target_link_libraries(net_platform_smoke PRIVATE ws2_32)

sleela_platform_smoke(path_platform_smoke path_platform_smoke.c sleela_path.c)

sleela_platform_smoke(terminal_platform_smoke terminal_platform_smoke.c sleela_terminal.c)

sleela_platform_smoke(library_platform_smoke library_platform_smoke.c sleela_library.c)

sleela_platform_smoke(time_platform_smoke time_platform_smoke.c sleela_time.c)

message(STATUS "SLeeLa MSVC slice: OS abstraction smoke tests only; full runtime is not yet an MSVC target.")
