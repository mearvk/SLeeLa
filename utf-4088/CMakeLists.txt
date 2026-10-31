cmake_minimum_required(VERSION 3.20)
project(utf4088 LANGUAGES CXX)

include(CTest)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(utf4088
    utf4088.cpp
    encoder_decoder.cpp
    character_map.cpp
    sleela_alphabet_adapter.cpp
    registry_generator.cpp
    digraph_engine.cpp
    voltage_field.cpp
    algebra/glyph8x12.cpp
    semantics/concept_symbols.cpp
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

add_executable(utf4088-inspect tools/utf4088-inspect.cpp)
target_link_libraries(utf4088-inspect PRIVATE utf4088)

if(BUILD_TESTING)
    add_executable(utf4088_tests tests/utf4088_tests.cpp)
    target_link_libraries(utf4088_tests PRIVATE utf4088)
    add_test(NAME utf4088_tests COMMAND utf4088_tests)

    add_executable(utf4088_encoder_decoder_tests tests/encoder_decoder_tests.cpp)
    target_link_libraries(utf4088_encoder_decoder_tests PRIVATE utf4088)
    add_test(NAME utf4088_encoder_decoder_tests COMMAND utf4088_encoder_decoder_tests)

    find_package(Python3 COMPONENTS Interpreter QUIET)
    if(Python3_Interpreter_FOUND)
        add_test(NAME utf4088_multilingual_schema
            COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/tests/multilingual-schema-validation.py")
    endif()
endif()
