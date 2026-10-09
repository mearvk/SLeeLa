cmake_minimum_required(VERSION 3.20)
project(utf4088 LANGUAGES CXX)

include(CTest)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(utf4088
    utf4088.cpp
    character_map.cpp
    registry_generator.cpp
    digraph_engine.cpp
    voltage_field.cpp
    algebra/glyph8x12.cpp
)
target_include_directories(utf4088 PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/algebra"
)
if(MSVC)
    target_compile_options(utf4088 PRIVATE /W4 /permissive-)
else()
    target_compile_options(utf4088 PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(BUILD_TESTING)
    add_executable(utf4088_tests tests/utf4088_tests.cpp)
    target_link_libraries(utf4088_tests PRIVATE utf4088)
    add_test(NAME utf4088_tests COMMAND utf4088_tests)
endif()
