# `@native` and Native Drive — ahead-of-time native C/C++ path

This document describes the current `@native` AOT boundary and the Native Drive
integration. `sleela aot` still finds annotated methods, verifies the supported
offloadable subset, and emits C source. The new `tools/sleela-native-drive.py`
can compile direct C/C++ inputs into executables or shared libraries and can
compile SLeeLa AOT output into a native executable. Unsupported SLeeLa constructs are still rejected; the existing
C/C++ frontend classes under `lib/cpu` are not full production parsers.

*MEARVK LLC — 2026.*

---

## The `@native` annotation

Mark a hot method with `@native` to request ahead-of-time native compilation:

```sleela
#sleela 2.5
material;
class Hot {
  @native
  int sumTo(int n) {
    int total = 0;
    int i = 1;
    while (i <= n) { total = total + i; i = i + 1; }
    return total;
  }
}
```

- `@native` is a **marker** annotation (no arguments).
- It reuses the reserved `native` modifier keyword as the annotation name; the
  parser accepts it after `@` (an annotation name lives in its own namespace).
- It is registered as a **known language annotation** (so it is not warned as
  unknown metadata) and is validated like other markers.
- It is **behaviour-neutral today**: a program with `@native` methods checks and
  runs exactly as before — `@native` only *requests* native treatment, which the
  later stages will fulfil.

## `sleela aot`

```
sleela aot --list <file.sleela>    list the @native methods and their verdict
sleela aot --emit <file.sleela>    emit C/C++ (stdout) for an in-subset program
sleela aot <file.sleela>           same as --list
```

- **`--list`** prints, per `@native` method, either `ok (within the offloadable
  subset)` or `REFUSED — <reason>`. If any method is refused, the command exits
  non-zero (fail-closed) and emits nothing.
- **`--emit`** emits C/C++ to **stdout** (the per-method report goes to stderr,
  so stdout is the code). It uses the Nordshrift C target
  (`impl/nordshrift/sleela_emit.cpp`).

### Exit codes

| Code | Meaning |
|---|---|
| 0 | all `@native` methods in-subset (and, for `--emit`, C/C++ emitted) |
| 1 | a `@native` method is **outside the subset** — refused, nothing emitted |
| 2 | methods are in-subset, but the whole-program C emitter cannot render the surrounding code yet (a Part-1 limitation, reported honestly) |

## The offloadable subset (Part 1)

A `@native` method is accepted only if its body uses **only**: integer/double/
boolean/null literals and variables; unary and binary arithmetic/comparison
operators; assignment; free (unqualified) calls; and the `if` / `while` / `for`
control-flow and `return` / `print` statements.

It is **refused** (with the exact reason) for: `String` literals, object
construction (`new`), instance/qualified calls (`receiver.method(...)`), array
indexing, the `:::` structural-offload operator, and anything else. These are
the gaps the later stages will close; refusing them now keeps Part 1 honest.

## Native Drive integration

The new Native Drive accepts direct `.c`, `.cc`, `.cpp`, and `.cxx` source and
compiles it through the configured host compiler. For `.sleela`, it invokes
`sleela aot --emit` and compiles the emitted C translation unit. It also provides
shell-free process execution, optional stdin-file input, stdout/stderr capture,
and a two-process stdout-to-stdin pipeline with timeouts. The `sleelactl`
registry exposes it as `native-drive`:

```sh
sleelactl info native-drive
sleelactl build native-drive source.cpp -o build/source
sleelactl run native-drive --capture -- build/source
```

See [`../impl/native-drive/README.md`](../impl/native-drive/README.md).

## What is NOT here yet

- A full production C/C++ parser integrated into the SLeeLa frontend (the
  `lib/cpu/SLFrontendC` and `SLFrontendCpp` classes document limited lowering
  models, not complete language implementations).
- Automatic registration of emitted SLeeLa methods with the existing native
  loader, plus VM in-process dispatch to native symbols with interpreter fallback and
  differential conformance checks.
- Expanded AOT coverage for strings, arrays, object layouts, exceptions, and
  language-specific ABI details.
- Declarative process annotations wired to a validated process capability
  policy, plus generated systemd unit lifecycle integration.

The process CLI is an explicit host-process tool, not a sandbox. Do not run
untrusted native code under elevated privileges.

## Verify

```sh
cd impl && SLEELA_SHA256_MANIFEST="$(pwd)/../security/sha256-manifest.json" make build/sleela && cd ..

# a method using 'new'/arrays is refused:
sleela aot --list some.sleela            # => REFUSED ... (reason)

# an in-subset whole program emits compilable C:
sleela aot --emit scalar-only.sleela 2>/dev/null | sed -n '/^#include/,$p' > out.c
gcc -std=c11 -O2 -c out.c -o out.o       # compiles
```
