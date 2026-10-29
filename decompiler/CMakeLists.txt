cmake_minimum_required(VERSION 3.20)
project(SLeeLaDecompiler VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# The shared C++ API lives in the versioned Sleela API tree. Keep these
# paths anchored to this CMakeLists.txt so builds work from any build dir.
set(SLEE_LA_API_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../api/sleela/2.1")
set(SLEE_LA_API_INCLUDE "${SLEE_LA_API_ROOT}/include")
set(SLEE_LA_API_SOURCE "${SLEE_LA_API_ROOT}/src/api.cpp")

if (NOT EXISTS "${SLEE_LA_API_INCLUDE}/sleela/api.hpp")
  message(FATAL_ERROR "SLeeLa API header not found: ${SLEE_LA_API_INCLUDE}/sleela/api.hpp")
endif()
if (NOT EXISTS "${SLEE_LA_API_SOURCE}")
  message(FATAL_ERROR "SLeeLa API implementation not found: ${SLEE_LA_API_SOURCE}")
endif()

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
    src/output.cpp
    "${SLEE_LA_API_SOURCE}"
)

target_include_directories(sleela_decompiler PUBLIC
    include
    "${SLEE_LA_API_INCLUDE}"
)
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

include(GNUInstallDirs)
install(TARGETS sleela_decompiler sleela-decompiler
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sleela-decompiler)
install(DIRECTORY "${SLEE_LA_API_INCLUDE}/" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sleela-api)
