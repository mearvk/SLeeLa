# IDE Build, Run, and Test

The IDE exposes repository operations through a thin adapter.

Required operations:
```
check
build
run
test
clean
diagnostics
```

The IDE invokes the same compiler/runtime entrypoints used by CI and must not silently substitute another compiler.

SLeeLa uses the SLeeLa toolchain; C/C++ use the configured native toolchain; Java uses the configured JVM/Java toolchain. Cross-language dependencies are declared by the project model.
