# Author: Max Rupplin — MEARVK LLC — 2026
cmake_minimum_required(VERSION 3.16)
project(SLeeLaSignalProcessing VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(sleela_signal_processing STATIC
    src/slep_core.c src/slep_filter.c src/slep_fft.c src/slep_cpp.cpp)
target_include_directories(sleela_signal_processing PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
if(MSVC)
    target_compile_options(sleela_signal_processing PRIVATE /W4)
else()
    target_compile_options(sleela_signal_processing PRIVATE -Wall -Wextra -Wpedantic)
    target_link_libraries(sleela_signal_processing PUBLIC m)
endif()

add_executable(sleela_sp_c_example examples/c/basic.c)
target_link_libraries(sleela_sp_c_example PRIVATE sleela_signal_processing)
set_target_properties(sleela_sp_c_example PROPERTIES LINKER_LANGUAGE CXX)
add_executable(sleela_sp_cpp_example examples/cpp/basic.cpp)
target_link_libraries(sleela_sp_cpp_example PRIVATE sleela_signal_processing)

include(CTest)
if(BUILD_TESTING)
    add_executable(sleela_sp_test_c tests/test_signal_processing.c)
    target_link_libraries(sleela_sp_test_c PRIVATE sleela_signal_processing)
    set_target_properties(sleela_sp_test_c PROPERTIES LINKER_LANGUAGE CXX)
    add_test(NAME signal_processing_c COMMAND sleela_sp_test_c)
    add_executable(sleela_sp_test_cpp tests/test_signal_processing.cpp)
    target_link_libraries(sleela_sp_test_cpp PRIVATE sleela_signal_processing)
    add_test(NAME signal_processing_cpp COMMAND sleela_sp_test_cpp)
endif()

include(GNUInstallDirs)
install(TARGETS sleela_signal_processing
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES include/sleela_signal_processing.h include/sleela_signal_processing.hpp
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
