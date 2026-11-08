# `@native` and `sleela aot` — ahead-of-time native (C/C++) emission (Part 1)

This is **Part 1** of the SLeeLa → native C/C++ path: the `@native` annotation
and the `sleela aot` command that finds `@native` methods, verifies each is
within the currently supported *offloadable subset*, and emits C/C++ for them.
It **fails closed** — a method that uses anything the emitter cannot yet
translate is refused with a precise reason, so a half-translated method is never
produced. The compile+link step and the VM fast-path dispatch (calling the
native code with an interpreter fallback) are **later stages**; nothing is
compiled to a binary yet.

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

## What is NOT here yet (later stages)

- Compiling the emitted C/C++ to `libslnat_*.so` (`g++ -O2 -shared -fPIC`).
- VM dispatch that calls the native symbol with a transparent interpreter
  fallback, plus output-diff verification against the interpreted result.
- Widening the subset (String, arrays, objects) and wiring `@native` through
  `sleelactl`/systemctl for the daemon case.

## Verify

```sh
cd impl && SLEELA_SHA256_MANIFEST="$(pwd)/../security/sha256-manifest.json" make build/sleela && cd ..

# a method using 'new'/arrays is refused:
sleela aot --list some.sleela            # => REFUSED ... (reason)

# an in-subset whole program emits compilable C:
sleela aot --emit scalar-only.sleela 2>/dev/null | sed -n '/^#include/,$p' > out.c
gcc -std=c11 -O2 -c out.c -o out.o       # compiles
```
