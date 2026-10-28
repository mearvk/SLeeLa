# Runtime Native Build

C++17 or newer is required. Build the runtime smoke test with:

c++ -std=c++17 -pthread -Iapi/native/runtime api/native/runtime/sleela_runtime.cpp tests/runtime_smoke.cpp -o /tmp/sleela-runtime-smoke

Then execute /tmp/sleela-runtime-smoke.

The portable core uses standard C++ synchronization primitives. OS-specific event sources should be added as adapters rather than embedded in the portable core.
