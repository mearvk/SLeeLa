cmake_minimum_required(VERSION 3.16)

add_library(sleela_native_loader STATIC native_loader.c native_module_table.c)
target_include_directories(sleela_native_loader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(sleela_native_loader PUBLIC c_std_99)
if(CMAKE_DL_LIBS)
  target_link_libraries(sleela_native_loader PUBLIC ${CMAKE_DL_LIBS})
endif()

add_library(sleela_native_loader_cpp STATIC native_loader.cpp)
target_include_directories(sleela_native_loader_cpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(sleela_native_loader_cpp PUBLIC cxx_std_11)
target_link_libraries(sleela_native_loader_cpp PUBLIC sleela_native_loader)

if(BUILD_TESTING)
  add_library(loader_fixture SHARED tests/fixture.c)
  target_include_directories(loader_fixture PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  target_compile_definitions(loader_fixture PRIVATE SL_NATIVE_PLUGIN_BUILD)

  add_executable(loader_test_c tests/test_c.c)
  target_link_libraries(loader_test_c PRIVATE sleela_native_loader)
  add_test(NAME loader_test_c COMMAND loader_test_c $<TARGET_FILE:loader_fixture>)

  add_executable(loader_test_cpp tests/test_cpp.cpp)
  target_link_libraries(loader_test_cpp PRIVATE sleela_native_loader_cpp)
  add_test(NAME loader_test_cpp COMMAND loader_test_cpp $<TARGET_FILE:loader_fixture>)

  add_library(loader_fixture_bad_abi SHARED tests/fixture_bad_abi.c)
  target_include_directories(loader_fixture_bad_abi PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  add_library(loader_fixture_missing_entry SHARED tests/fixture_missing_entry.c)
  target_include_directories(loader_fixture_missing_entry PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  add_executable(loader_test_reject tests/test_reject.c)
  target_link_libraries(loader_test_reject PRIVATE sleela_native_loader)
  add_test(NAME loader_test_reject COMMAND loader_test_reject
    $<TARGET_FILE:loader_fixture_bad_abi>
    $<TARGET_FILE:loader_fixture_missing_entry>)

  add_executable(loader_test_table tests/test_table.c)
  target_link_libraries(loader_test_table PRIVATE sleela_native_loader)
  add_test(NAME loader_test_table COMMAND loader_test_table
    $<TARGET_FILE:loader_fixture>
    $<TARGET_FILE:loader_fixture_bad_abi>)
endif()
