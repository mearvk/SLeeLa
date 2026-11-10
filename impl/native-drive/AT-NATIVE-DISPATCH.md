# Automatic VM dispatch for `@native` methods

**Status: Phase 1 implemented on branch `feat/native-method-metadata-dispatch-2026-11-09`; later phases remain pending.**

The compiler now persists method-level `@native` identity in artifact format
v4. The VM records annotation/static-or-instance flags, the method ABI version,
a canonical signature, and a deterministic FNV-1a-64 signature identity.
Version 1-3 artifacts remain loadable with all native metadata disabled.
The metadata is descriptive only: it does not load, authorize, bind, or invoke
native code.

The public metadata registration/readback API is
`slvm_set_func_native_metadata()` / `slvm_get_func_native_metadata()`.
The local regression target is `make -C impl test-native-method-metadata`;
it checks compiler-emitted static method metadata, v4 save/load,
ordinary-function zero metadata, digest corruption rejection, and v3
interpreted-only compatibility. No CI workflow is needed or
invoked for this test.

Per-method native code generation, authorized host binding, ordinary-call
dispatch, and their conformance tests are not implemented by this phase.

This document specifies the missing connection between the source-level
`@native` annotation and the existing VM native-module dispatcher. It does not
claim that annotated methods are automatically compiled, loaded, or dispatched.

## 1. Existing mechanisms (keep them distinct)

SLeeLa currently has two relevant but separate paths:

1. **Source AOT request:** `@native` is parsed on a method and inspected by
   `sleela aot --list/--emit`. The current emitter is whole-program C output,
   not a stable per-method plugin ABI. It does not install a native method
   binding in the VM.
2. **Explicit native module calls:** syntax 3.0 `nativeOpen`,
   `nativeCall`, and `nativeClose` lower to append-only VM opcodes. The host
   authorizes module paths; the typed ABI transports null, bool, int64, double,
   and UTF-8 String values. A missing/not-handled native method can use a
   same-name, same-arity SLeeLa bytecode fallback. Authorization failures,
   stale handles, ABI errors, and handled plugin failures remain errors.

The second path is real VM dispatch, but it is not automatic dispatch for
ordinary calls to an `@native`-annotated SLeeLa method.

## 2. Required end state

For an annotated method such as:

```sleela
class MathEngine {
    @native
    int sum(int a, int b) {
        return a + b;
    }
}
```

the compiler/runtime pipeline must be able to:

- preserve the annotation as method-level metadata through compilation and
  persistent artifact save/load;
- associate the SLeeLa method identity and exact arity/signature with a
  generated native entry point;
- have an ordinary call to that method select the registered native
  implementation when one has been explicitly authorized and successfully
  bound by the host;
- otherwise execute the original SLeeLa bytecode implementation;
- retain the original VM-owned arguments and values for fallback; and
- surface native execution failures instead of hiding them by retrying the
  interpreted method.

## 3. Proposed dispatch contract

### Compiler metadata

Each annotated method needs a stable identity independent of source line
numbers and declaration order. The initial identity should include:

- fully qualified class and method name;
- static/instance status;
- parameter count and canonical parameter types;
- canonical return type;
- native ABI major/minor version;
- a deterministic method signature digest.

The compiler must reject duplicate or ambiguous native identities. Metadata is
descriptive only: it does not authorize a module path or grant execution
capability.

### Host binding

The embedding host must explicitly bind a generated native module to one or
more compiler-emitted method identities after applying its own authorization
and integrity policy. Binding should be immutable for the duration of a VM
execution epoch, or changed only through a synchronized host API.

No source annotation may cause the VM to search arbitrary directories, load a
library, spawn a process, or trust a caller-supplied path.

### Ordinary call dispatch

The compiler may continue emitting the normal bytecode call as the canonical
fallback body. At runtime, a call to a method marked native should follow this
order:

1. Resolve the function identity and verify the binding belongs to this VM.
2. If no host binding exists, execute the original bytecode body.
3. Marshal only values supported by the negotiated native ABI.
4. If an argument is a VM-owned composite that the ABI does not represent,
   execute the original bytecode body without exposing VM pointers or handles.
5. Invoke the native entry point under the existing module lifetime/locking
   discipline.
6. If the entry point explicitly returns **NOT_HANDLED**, execute the original
   bytecode body.
7. If the entry point succeeds, validate and copy the result into VM-owned
   storage before returning it to the caller.
8. If authorization, handle validation, ABI negotiation, or native execution
   fails, return an error. Never silently reinterpret a handled failure as
   permission to retry in bytecode.

Fallback is not a general exception handler. It is permitted only when the
native implementation is absent, explicitly declines the method, or the
argument shape is outside the negotiated ABI before native execution begins.

## 4. ABI and ownership rules

- Version the method ABI separately from the legacy `sl_native_run` ABI.
- Initially support only null, bool, signed 64-bit integer, double, and
  length-delimited UTF-8 strings.
- Do not pass `SLValue`, VM pointers, array handles, struct handles, closures,
  or object pointers across the native boundary.
- A native result string must be validated and copied before the plugin lock is
  released and before it enters VM string storage.
- Composite parameters and results remain interpreted until a separately
  specified ownership/lifetime ABI exists.
- Instance methods require an explicit receiver representation. Until that is
  designed, automatic native dispatch must be restricted to static methods;
  instance methods must remain interpreted rather than passing `this` as a
  raw handle.

## 5. Implementation sequence

1. **Artifact metadata:** add a versioned, append-only representation of
   annotated function identities; validate it on load and preserve it on
   save/load round trips.
2. **Per-method code generation:** emit independently addressable native
   entry points with the versioned typed ABI. Do not use the current
   whole-program executable output as if it were a method plugin.
3. **Host binding API:** bind method identities to already-authorized native
   module handles. The API must not load modules or authorize paths by itself.
4. **VM ordinary-call fast path:** route only annotated functions through the
   binding table; preserve normal bytecode entry points as fallback.
5. **Conformance:** compare native and interpreted return values, errors, and
   observable behavior; test NOT_HANDLED, missing binding, unsupported
   composites, stale handles, ABI mismatch, authorization denial, plugin
   failure, close/call races, and artifact compatibility.

## 6. Acceptance tests

- An unannotated method never enters the native binding table.
- An annotated method with no host binding executes its bytecode body.
- An authorized, correctly bound native implementation is reached through an
  ordinary SLeeLa method call without an explicit source `nativeCall`.
- NOT_HANDLED executes the bytecode body exactly once.
- A VM-owned array/struct argument is never passed to the plugin and uses the
  bytecode body.
- A native plugin error is reported and is not hidden by bytecode fallback.
- A denied module path cannot be made executable by adding `@native`.
- Artifact save/load preserves method identities and rejects unsupported
  metadata versions.
- Existing `nativeOpen/nativeCall/nativeClose` programs remain compatible.
- Linux, Windows 10+, and macOS exercise the same semantics.

## 7. Current conclusion

The existing `OP_NATIVE_CALL` path is a foundation to reuse, not the missing
feature itself. Automatic `@native` dispatch requires compiler-emitted
per-method metadata, per-method native code generation, and an explicit
host-owned binding table before the ordinary `OP_CALL` path can safely select
native code. Until those pieces are implemented and tested, `@native` remains
an AOT request and normal SLeeLa bytecode remains authoritative.

*MEARVK LLC — 2026.*
