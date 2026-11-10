# Automatic VM dispatch for `@native` methods

**Status: Per-method ABI-v1 C source emission is implemented on branch `feat/native-method-aot-emitter-2026-11-10`; local compile/load conformance remains pending.**

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

Authorized host binding and ordinary-call dispatch now use the explicit APIs
`slvm_bind_native_method()` and `slvm_unbind_native_method()`. A binding is
accepted only for an annotated static method and an already-open, authorized
module handle. The plugin export key is the exact canonical signature stored
in artifact metadata. Bound modules cannot be closed until all method bindings
are removed. The ordinary `OP_CALL` path invokes that export only when a
binding exists and the arguments fit the scalar/UTF-8 ABI; NOT_HANDLED and
unsupported VM-owned arguments retain the original bytecode path. Plugin,
stale-handle, and ABI failures remain errors.

The focused opt-in test verifies unbound bytecode behavior and ordinary
source-level calls reaching an authorized plugin export. The new AOT emitter
generates per-method C entry points, but a generated-plugin build/load test and
broader platform/concurrency conformance remain outstanding. No CI workflow is
required or invoked.

This document specifies the missing connection between the source-level
`@native` annotation and the existing VM native-module dispatcher. It does not
claim that annotated methods are automatically compiled, loaded, or dispatched.

## 1. Existing mechanisms (keep them distinct)

SLeeLa currently has two relevant but separate paths:

1. **Source AOT request:** `@native` is parsed on a method and inspected by
   `sleela aot --list/--emit`. The `--emit` mode now produces standalone
   per-method C plugin source with exact canonical-signature dispatch. It does
   not compile, load, authorize, or bind the plugin in the VM.
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
    static int sum(int a, int b) {
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
- The existing typed module ABI transports null, bool, signed 64-bit integer,
  double, and length-delimited UTF-8 strings. The generated method emitter's
  initial source subset uses int64, double, bool, String, and void returns; it
  does not yet expose null-typed method parameters.
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

1. **Artifact metadata — implemented:** version-4 metadata is append-only,
   validated, and preserved through save/load; versions 1-3 remain
   interpreted-only.
2. **Per-method code generation — source emitter implemented:** `sleela aot --emit`
   emits a standalone C plugin with one independently addressable method body
   per annotated static method. The plugin exports the typed ABI v1 loader
   symbols and dispatches exact canonical signature strings. Supported parameter
   and return types are `int`, `double`, `boolean`, and length-delimited
   `String`; void returns are represented as NULL. String values can pass
   through parameters, locals, assignments, and returns. String literals and
   string operators are excluded. Division/remainder, method-to-method calls,
   print side effects, objects, arrays, and instance methods are also refused.
   The generated source is not compiled or loaded automatically; the host must
   compile it, authorize its resulting library, and bind it explicitly.
   The opt-in target `make -C impl test-native-method-aot` generates the fixture
   source and compiles its C translation unit. It is outside CI and has not yet
   been run in this environment. Generated-plugin loading and VM dispatch
   conformance remain outstanding.
3. **Host binding API — implemented:** `slvm_bind_native_method()` accepts
   only annotated static methods and a module handle already opened through
   the host's authorization policy. `slvm_unbind_native_method()` is required
   before a bound module can be closed. Binding never loads or authorizes a
   path.
4. **VM ordinary-call fast path — implemented:** `OP_CALL` tries the bound
   native entry for scalar/UTF-8 arguments, copies results while the module
   lock is held, and preserves bytecode fallback for missing bindings,
   unsupported values, and NOT_HANDLED. Native failures do not fall back.
5. **Conformance — initial tests added:** the focused target checks native
   ordinary-call dispatch, unbound bytecode execution, module-close protection,
   metadata round trips, digest corruption, and v3 compatibility. Cross-platform
   execution, race stress, and the remaining failure matrix still need to run.

## 6. Acceptance tests

- An unannotated method never enters the native binding table.
- An annotated method with no host binding executes its bytecode body.
- An authorized, correctly bound native implementation is reached through an
  ordinary SLeeLa method call without an explicit source `nativeCall`.
- An unbound annotated method executes its bytecode body.
- NOT_HANDLED executes the bytecode body exactly once.
- A VM-owned array/struct argument is never passed to the plugin and uses the
  bytecode body.
- A native plugin error is reported and is not hidden by bytecode fallback.
- `sleela aot --emit` produces a C plugin that compiles and exposes each
  annotated static method under its exact artifact signature.
- A denied module path cannot be made executable by adding `@native`.
- Artifact save/load preserves method identities and rejects unsupported
  metadata versions.
- Existing `nativeOpen/nativeCall/nativeClose` programs remain compatible.
- Linux, Windows 10+, and macOS exercise the same semantics.

## 7. Current conclusion

The existing `OP_NATIVE_CALL` path is a foundation to reuse, not the missing
feature itself. Versioned method metadata, per-method native C source emission,
host-owned authorized binding, and ordinary `OP_CALL` dispatch are now present.
The source emitter and generated C compilation target are implemented, but the
target has not yet been run and generated-plugin load/dispatch conformance is
still outstanding. Until those checks pass, normal SLeeLa bytecode remains the
verified baseline and native code must be explicitly compiled, authorized, and
bound by the host.

*MEARVK LLC — 2026.*
