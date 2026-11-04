cmake_minimum_required(VERSION 3.16)
project(sleela_os_support LANGUAGES C CXX)

add_library(sleela_os_support STATIC
    src/sleela_os_support.c
    src/sleela_os_support.cpp)
target_include_directories(sleela_os_support PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(sleela_os_support PUBLIC c_std_99 cxx_std_11)

include(CTest)
if(BUILD_TESTING)
  add_executable(sleela_os_support_test tests/os_support_test.c)
  target_link_libraries(sleela_os_support_test PRIVATE sleela_os_support)
  add_test(NAME sleela_os_support_test COMMAND sleela_os_support_test)
endif()
