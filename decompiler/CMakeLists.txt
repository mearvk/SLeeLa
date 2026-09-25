cmake_minimum_required(VERSION 3.20)
project(SLeeLaDecompiler VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(sleela_decompiler STATIC
    src/artifact.cpp
    src/decoder.cpp
    src/pe.cpp
    src/elf.cpp
    src/slir.cpp
    src/vm.cpp
    src/analyzer.cpp
    src/library.cpp
    src/archive.cpp
)

target_include_directories(sleela_decompiler PUBLIC include)
target_compile_features(sleela_decompiler PUBLIC cxx_std_20)

if (MSVC)
  target_compile_options(sleela_decompiler PRIVATE /W4)
else()
  target_compile_options(sleela_decompiler PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(sleela-decompiler cli/main.cpp)
target_link_libraries(sleela-decompiler PRIVATE sleela_decompiler)

enable_testing()
add_executable(sleela-decompiler-test tests/test_core.cpp)
target_link_libraries(sleela-decompiler-test PRIVATE sleela_decompiler)
add_test(NAME sleela-decompiler-core COMMAND sleela-decompiler-test)

option(SLEE_LA_BUILD_API_EXAMPLES "Build Slecompiler API exemplar programs" ON)

if (SLEE_LA_BUILD_API_EXAMPLES)
  add_executable(slecompiler-api-inspect examples/api_inspect.cpp)
  target_link_libraries(slecompiler-api-inspect PRIVATE sleela_decompiler)

  add_executable(slecompiler-api-cfg examples/api_decode_cfg.cpp)
  target_link_libraries(slecompiler-api-cfg PRIVATE sleela_decompiler)

  add_executable(slecompiler-api-library examples/api_library_metadata.cpp)
  target_link_libraries(slecompiler-api-library PRIVATE sleela_decompiler)

  add_executable(slecompiler-api-graph examples/api_library_graph.cpp)
  target_link_libraries(slecompiler-api-graph PRIVATE sleela_decompiler)

  add_executable(slecompiler-api-slir-vm examples/api_slir_vm.cpp)
  target_link_libraries(slecompiler-api-slir-vm PRIVATE sleela_decompiler)
endif()
