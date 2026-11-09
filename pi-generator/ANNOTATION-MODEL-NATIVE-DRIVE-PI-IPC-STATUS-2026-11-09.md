# SLeeLa Pi Generator — `@native` Annotation-Model and Math-Drive IPC Status

**Date:** 2026-11-09  
**Status:** BLOCKED for VM-to-`sleelactl` native dispatch; prior VM benchmark remains PASS  
**Branch:** `pi-20000-sleelactl-native-math`  
**Baseline commit:** `7bb76e6703b165dfacc1ef9293b79e83d3b7bf80`

## Objective

Run the existing Pi generator from SLeeLa source while using the `@native` annotation model to request that hot arithmetic work be delegated from the SLeeLa VM to the registered `sleelactl` math native subject. Preserve the established Pi algorithms and independently verify the generated digits. This experiment is specifically intended to test the annotation-driven handoff, not merely to repeat the prior native-opcode benchmark.

## Existing verified baseline

The repository already contains a successful 20,000-fractional-digit Chudnovsky run:

- Source: `pi-generator/PiChudnovsky.sleela`
- Algorithm: Chudnovsky series with binary splitting
- VM arithmetic: `bigmul`, `bigadd`, `bigsub`, `bigdiv`, `bigisqrt`, and `bigdivmod`
- Result: 20,000 fractional digits; all characters matched an independent `mpmath` reference
- Output SHA-256: `977f8d4251b4f633100b4d2dbc214b874c45353b0e3debe41046753777fd6efc`
- Runtime: 10.72 s; peak resident memory: 49,724 KiB; exit status 0
- Workflow: https://github.com/mearvk/SLeeLa/actions/runs/42690168886

This is a VM/native-arithmetic baseline. It is **not** evidence that `@native` caused a VM-to-module handoff. The existing benchmark separately invokes `sh bin/sleelactl run math` as a native math subject smoke test and then runs the Pi program with `impl/build/sleela`.

## Annotation-model finding

The current contract in `pi-generator/AT-NATIVE.md` explicitly defines `@native` as a marker annotation that is behaviour-neutral today:

- The parser recognizes the annotation and the AOT command can list or emit eligible methods.
- `sleela aot --list` checks whether annotated methods fit the currently supported subset.
- `sleela aot --emit` emits whole-program C for the supported subset; it is not a per-method VM dispatch mechanism.
- The current Native Drive can compile SLeeLa AOT output and can launch native processes, capture their output, and pipe one process into another.
- The VM exposes an authorized, generation-checked native-module lifecycle API to the host, but SLeeLa bytecode does not yet dispatch methods into native symbols.

Therefore adding `@native` to Pi methods does not presently force or coerce the VM to transfer their execution to `sleelactl`. Claiming a successful offload at this stage would be inaccurate. The Pi programs also use arrays, object/class state, and other constructs outside the documented Part-1 AOT subset, so a direct whole-program AOT attempt must be expected to refuse unsupported constructs rather than silently fall back or pretend to offload.

## Interprocess communication work still required

The annotation model and IPC contract need to be connected end to end. The remaining implementation should establish:

1. **Annotation-to-lowering contract:** preserve `@native` metadata through semantic analysis and identify a stable native entry point for each eligible method.
2. **Typed call ABI:** define argument and return-value layouts, integer width/sign rules, array/buffer ownership, string encoding, error propagation, and version negotiation.
3. **Dispatch boundary:** add an explicit VM call path that resolves an authorized native subject and invokes the annotated method; define whether the subject is an in-process module or a separately supervised process.
4. **IPC transport:** specify request/response framing and implement the selected transport (for example, stdin/stdout pipes for a process boundary, or a versioned shared-memory/socket protocol where required). Include message sizes, partial reads/writes, back-pressure, and timeout behaviour.
5. **Lifecycle and safety:** bind module/process startup, authorization, capability checks, cancellation, cleanup, and failure recovery to VM lifecycle rules. Annotation metadata must request execution, never independently grant trust or authorization.
6. **Conformance tests:** compare VM fallback and native results over the same Pi arithmetic vectors, exercise malformed messages and process/module failure, and verify that the native path was actually entered using explicit dispatch telemetry.
7. **End-to-end Pi run:** only after the above is implemented, annotate a suitable arithmetic boundary, build the runtime, invoke the registered math subject through the supported dispatch route, generate 20,000 digits, compare every character with an independent reference, and record time, CPU, memory, dispatch counts, and output hash.

## Result of this experiment request

**No new annotation-driven Pi run is claimed.** The repository's current implementation does not yet provide the VM-to-`sleelactl` method-dispatch/IPC bridge required by the request. The previous 20,000-digit result remains a valid baseline, but it must not be relabelled as an `@native` offload result.

The next implementation milestone is the narrowest complete vertical slice: one annotated, ABI-safe arithmetic method; an authorized Native Drive target; a framed request/response path; VM dispatch with explicit fallback/refusal semantics; and a test proving the native subject received and returned the call. Once that slice passes, the Pi generator can be adapted incrementally without misrepresenting the current annotation semantics.

## Related records

- [`@native` and Native Drive contract](AT-NATIVE.md)
- [20,000-digit native math baseline](PI-20000-NATIVE-MATH-BENCHMARK-2026-11-09.md)
- [Machin runtime comparison](PI-MACHIN-RUNTIME-COMPARISON-2026-11-09.md)
- [Math module smoke-test workflow](../.github/workflows/pi-20000-native-math.yml)
