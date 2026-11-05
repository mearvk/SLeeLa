See upstream component documentation in SLeeLa's native module loader. This directory provides a C ABI and move-only C++ RAII wrapper for loading a trusted shared library (.dll, .dylib, or .so). Loading executes native code in-process; it is not a sandbox. Only load trusted modules after any integrity checks required by application policy. The loader does not invoke a shell or compile caller-provided source at runtime.

Export `sl_native_abi_version` and `sl_native_run` with C linkage. The ABI version is checked before the entry point is exposed. Compile and link C/C++ translation units into a shared library before calling `sl_native_open` or `sleela::NativeModule::open`. Native plugins are limited to nine linked source/object units by the documented build contract.

Build and test from the repository root:

```sh
cmake -S os-level-support -B build/os-level-support
cmake --build build/os-level-support
ctest --test-dir build/os-level-support --output-on-failure
```
