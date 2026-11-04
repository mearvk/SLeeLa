cmake_minimum_required(VERSION 3.16)
project(grt_claim_model LANGUAGES C CXX)

add_library(grt_claim_model grt_claim.c grt_claim.cpp)
target_include_directories(grt_claim_model PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(grt_claim_model PUBLIC c_std_99 cxx_std_11)

include(CTest)
if(BUILD_TESTING)
  add_executable(grt_claim_test test_grt_claim.c)
  target_link_libraries(grt_claim_test PRIVATE grt_claim_model)
  add_test(NAME grt_claim_test COMMAND grt_claim_test)
endif()
