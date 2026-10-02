<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa VM Source Classes

The `/lib/vm` model defines SLeeLa source-level classes for constructing SLVM and SLJVM pieces.

## Option model

VM construction is controlled by two complementary mechanisms:

- **Integer option codes** select one value from a mutually exclusive condition set: target, architecture, OS, ABI, execution mode, garbage collector, threading model, I/O model, security model, link model, and package model.
- **Integer bit masks** enable independent capabilities/features such as files, network, DNS, IPC, threads, async I/O, GUI/media, crypto/TLS, JIT/AOT, SIMD/atomics, GC, checkpointing, migration, attestation, observability, deterministic execution, resolver, broker, certificates, and sandboxing.
- **Specific option classes** hold detailed resource and policy parameters for execution, memory, CPU, concurrency, I/O, security, runtime, JVM, and build/package construction.

This keeps the compiler able to express a large VM design space without creating a separate language. The authoritative SLeeLa compiler resolves the selected codes and masks against physical limits, architecture, operating system, ABI, capabilities, module dependencies, and security policy.

## Construction

`SleelaVMSource` describes the source-level VM request. `SleelaVMCompiler` resolves it into an architecture/resource/build plan. C provides the stable VM construction ABI; C++ provides higher-level orchestration. The result is assembled piecewise from the modules that fit the requested and physically available conditions.

## Fitment rules

1. A required option that cannot fit the target architecture or physical limits is rejected.
2. Optional features may be disabled only when the source marks them optional.
3. Capability selection never grants OS authority; capabilities remain explicit.
4. SLVM uses the native C/C++ path; SLJVM adds the JVM/object-broker boundary.
5. The compiler records the selected option codes, feature masks, resource plan, ABI, and module set in the output metadata.