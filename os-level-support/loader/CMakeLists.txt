cmake_minimum_required(VERSION 3.16)

add_library(sleela_native_loader STATIC native_loader.c)
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
endif()
