# This CMake entry point is retained as a relocation shim.
# The loader is now part of os-level-support; configure that directory instead:
#   cmake -S os-level-support -B build/os-level-support
add_subdirectory("../../../os-level-support/loader" "${CMAKE_CURRENT_BINARY_DIR}/os-level-support-loader")
