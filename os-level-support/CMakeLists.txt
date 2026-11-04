cmake_minimum_required(VERSION 3.16)
project(os_level_support LANGUAGES C CXX)

add_library(os_level_support osls.c osls.cpp)
target_include_directories(os_level_support PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(os_level_support PUBLIC c_std_99 cxx_std_11)

if(WIN32)
  target_compile_definitions(os_level_support PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

include(CTest)
if(BUILD_TESTING)
  add_executable(osls_test test_osls.c)
  target_link_libraries(osls_test PRIVATE os_level_support)
  add_test(NAME osls_test COMMAND osls_test)
endif()

# The C/C++ native plugin loader is part of this OS support module.
add_subdirectory(loader)
