# Language Reference

The Sleela language is a Java-like language compiled by **Sleelvac™** and executed on the C/C++ core. This page is the practical reference; the normative documents are `COMPILER.md`, `SOURCE.md`, `SLEELA.md`, `STRUCTS.md`, and `VERSION.md`.

> Supported `.sleela` syntax versions: **1.3 – 1.6** (default **1.6**). A file selects its version with the `#sleela` pragma; the compiler rejects a version outside this range with a clear diagnostic. Earlier 1.0–1.2 sources describe the historical surface and are not accepted by the current compiler.

---

## Program structure

A program is one or more `class` declarations. There are **no top-level statements** — code lives in methods. Execution starts at a `main()` method.

```sleela
#sleela 1.6

class Greeter {
    static String who() { return "Sleela"; }

    void main() {
        print("Hello, " + who() + "!");
    }
}
```

- `import <module>;` lines (native modules / `/lib` packages) come before class declarations.
- A class holds **fields** and **methods**; `static` marks class-level members.
- **Multiple source files** can be compiled together as one program (see *Compiler commands*): a class in one file may use a class or method defined in another.

---

## Keywords & types

**Control / declaration keywords:** `class`, `interface`, `enum`, `record`, `struct`, `static`, `final`, `abstract`, `sealed`, `non-sealed`, `public`, `private`, `protected`, `import`, `native`, `extends`, `implements`, `throws`, `transient`, `volatile`, `strictfp`.

**Statement keywords:** `if`, `else`, `while`, `do`, `for`, `switch`, `case`, `default`, `break`, `continue`, `return`, `throw`, `try`, `catch`, `finally`, `assert`, `yield`, `synchronized`, `print`, `new`, `instanceof`, `this`, `super`, `true`, `false`, `null`.

**Types:**

| Type | Meaning |
|---|---|
| `int` | 64-bit integer |
| `double` | floating point |
| `boolean` | `true` / `false` |
| `String` | text; `+` concatenates |
| `void` | no return value |
| `struct` types | named aggregates (syntax **1.2+**; see `STRUCTS.md`) |
| `T[]` arrays | dynamic, zero-indexed sequences (see **Arrays**) |

`print(x)` writes a line to standard output. `+` concatenates when either side is a `String` (Java-like). A built/printed String is not length-capped by the small legacy buffer — large documents (generated HTML, DXF, JSON) build and print whole.

---

## Statements & expressions

```sleela
int i = 0;                         // declaration
i = i + 1;                         // assignment (also += -= *= /= %=)
if (i < 10) { ... } else { ... }
while (i < 10) { i = i + 1; }
do { i = i - 1; } while (i > 0);
for (int k = 0; k < n; k = k + 1) { ... }
switch (code) { case 1: ...; break; default: ...; }
try { ... } catch (e) { ... } finally { ... }
return expr;                       // (or bare `return;` in a void method)
```

Operators: arithmetic `+ - * / %`, comparison `== != < <= > >=`, logical `&& || !`, ternary `?:`, `instanceof`, cast `(Type)x`, unary `-`.

**`&&` and `||` short-circuit** (Java/C semantics): the right operand — and any method call or side effect in it — is **not** evaluated when the left operand already decides the result. For example, in `p != null && p.ok()` the call `p.ok()` runs only when `p` is non-null.

---

## Structs (syntax 1.2+)

A `struct` is a named aggregate — data, as opposed to a `class` (code). `new Type()` builds an instance; `.field` reads/writes; struct values are reference types reached by a bounded VM handle. `structPack` / `structUnpack` move a struct over the wire as JSON. Full detail in `STRUCTS.md`.

```sleela
#sleela 1.6

struct Point { int x; int y; }

class Geo {
    void main() {
        Point p = new Point();
        p.x = 3; p.y = 4;
        print(p.x + p.y);        // 7
    }
}
```

---

## Arrays

A `T[]` is a dynamic, growable, zero-indexed sequence of values — a first-class type reached by a bounded VM handle (like a struct).

```sleela
#sleela 1.6

class Demo {
    void main() {
        int[] a = new int[3];       // three zero/null elements
        a[0] = 10; a[1] = 20;
        a[2] = a[0] + a[1];         // index write/read -> 30
        print(arrayLength(a));      // 3
        int n = arrayPush(a, 40);   // append; grows -> new length 4
        print(a[3]);                // 40
        print(a);                   // [10, 20, 30, 40]
    }
}
```

Array built-ins:

| Call | Meaning |
|---|---|
| `new T[n]` / `arrayNew(n)` | fresh array of `n` null/zero elements (`n` may be any runtime `int`) |
| `arrayLength(a)` | element count (`int`) |
| `arrayGet(a, i)` / `a[i]` | element at `i` (runtime bounds-checked) |
| `arraySet(a, i, v)` / `a[i] = v` | write element `i` (runtime bounds-checked) |
| `arrayPush(a, v)` | append `v`, growing the array; returns the new length |

---

## The `#sleela` version pragma

The first line may declare the syntax version:

```sleela
#sleela 1.6
```

- The compiler accepts **1.3** through **1.6**.
- Feature gates: **network / file I/O built-ins require 1.1+**, **structs & struct transport require 1.2+**, **Synchro / Munction / best-of built-ins require 1.3+** (so they are available across the whole supported range).
- A missing pragma emits a warning and assumes the default (**1.6**).
- A version newer than the compiler's max, or a malformed pragma, is rejected with a clear diagnostic.

---

## Native modules

Import a native module to call its functions with `module.function(...)` syntax. The authoritative, importable packages are discovered recursively under `/lib`; common native-subject modules include:

`math`, `physics`, `economics`, `inference`, `chemistry`, `financial`, plus the standard-library families under `/lib` (`text`, `collections`, `ui`, `codecs`, `languages`, `website`, `autocad`, …).

Physics and Economics build on `math` (declare `import math` explicitly).

```sleela
#sleela 1.6
import math;
import economics;

class Model {
    static double grow() {
        return economics.future_value(1000.0, 0.05, 10.0);
    }
    void main() { print(grow()); }
}
```

### Financial functions

The financial library lowers `financial.*` calls to the native math kernels:

`future_value`, `present_value`, `annuity_present`, `annuity_future`, `npv4`, `bond_price`, `capm`, `wacc`, `determinant2`, `solve2x2_x`, `solve2x2_y`, `quadratic_discriminant`, `quadratic_root_plus`, `quadratic_root_minus`, `ratio`.

```sleela
#sleela 1.6
import math;
import financial;

class Desk {
    void main() {
        print(financial.future_value(1000.0, 0.05, 10.0));   // 1628.89
        print(financial.capm(0.04, 1.2, 0.10));              // 0.112
    }
}
```

---

## Conducted methods (the object catalog)

Sleela exposes the `SHEET.sheet` catalog as *conducted methods* — queries over the catalog of common system objects:

| Method | Returns |
|---|---|
| `insight(Object)` | the object's one-line gloss |
| `role(Object)` | the object's conduct role |
| `congruent(A, B)` | whether two objects are directly connectable |
| `route(A, B)` | the connection route between two objects |
| `sysdepth()` | the system depth invariant (**3024**) |
| `degreemax()` | the max complexity degree (**4**) |

See `impl/examples/conduct.sleela`.

---

## Networking & file I/O (syntax 1.1+)

Network built-ins lower to the core socket ABI (`LISTEN / ACCEPT / CONNECT / SOCKREAD / SOCKWRITE / SOCKCLOSE`):

`listen`, `accept`, `connect`, `sockread`, `sockwrite`, `sockclose`.

File / pipe built-ins: `openFile`, `read`, `write`, `close`, `unlinkFile`, `pipe`, `pipePeer`, `fifoCreate`.

See `impl/examples/network_echo.sleela` and `impl/examples/fileio.sleela`.

---

## Synchro, Munction, and best-of (syntax 1.3+)

Honest packet measurement and route selection are first-class:

- **Synchro** — `synchroOpen`, `synchroDispatch`, `synchroReport`, `synchroClose`, and the stat readers (`synchroMean`, `synchroLoss`, …).
- **Munction** — the reach-composition sentence: `Munction.start(name)` then `.connect`, `.enable`, `.send`, `.consume`, `.reception`, `.close`.
- **best-of** — `bestOfNew`, `bestOfWeight`, `bestOfCandidate`, `bestOfBest`, `bestOfReport`, …

See `impl/examples/bestof_route.sleela`.

---

## Concurrency

The core is thread-friendly; see `impl/examples/threads.sleela`. `spawn(method)` runs a zero-arg method on a new thread; `join()` waits. Threading maps onto the catalog's concurrency objects (`Thread`, `Lock`, `Future`, `Join`, …), which Nordshrift renders per target.

---

## Time, audio, and codecs

- **Time** (`SLEELA_TIME_API.md`): `timeUtcMillis`, `timeMonotonicNanos`, `timeHttpDate`, `timeJson`, `timeNtp`, `timeSetLocation`, …
- **Audio** (`audio/`): `audioNew`, `audioAdd`, `audioControls`, `audioValidate`, `audioRender`, `audioClose`.
- **Codecs** (`codecs/`): programs call the **Codec Loader / Manager**, never an individual codec. The manager loads/unloads codec plugins and dispatches decode/encode; native codecs (PCM/WAV, AIFF, G.711 μ-law/A-law) work in-package, and backend codecs dispatch to a registered library adapter. The SLeeLa-facing handle is `lib/codecs/SLCodecLoader`.

---

## Localization (the `languages` family)

SLeeLa output and prompts can be presented in a language other than English. The `languages/` folder holds dependency-free `key = value` packs (English, Spanish, French, German, Portuguese, Italian, Japanese, Chinese, Hindi, Korean, Thai, and right-to-left Arabic), chosen via `languages/settings.conf`; the `lib/languages` library (`SLLanguage`, `SLLanguageCatalog`, `SLLanguageSettings`) resolves the active locale and returns localized text. String literals accept raw UTF-8, so non-Latin and RTL text round-trips through `print`.

---

## The `.sleela` file — a Wrapper™

Every `.sleela` file is a **Wrapper™**: the program unit that carries the SL-META-0001 metadocument addend. It also supports a configurable **Sigil QR code** and a deterministic **248×48 steganographic frame**, generated by the dependency-free tool in `tools/sigil/`. See `SLEELA.md`.

---

## Compiler commands (Sleelvac™)

```sh
sleela run     <file.sleela> [more.sleela ...]        # compile + run (one or more files, merged)
sleela run     <program.sleela>                       # run a prebuilt Core artifact
sleela run     <file.xclass> [more...]                # ingest SecureJDK 28 .xclass and run
sleela compile <file.sleela> [more.sleela ...] -o <out.sleela>   # persistent runnable artifact
sleela check   <file.sleela>                          # parse/validate only
sleela xclass  [--run|--emit|--info] <file.xclass> [more...]
sleela langin  [--run|--emit-sleela|--emit-xclass|--info] <file>
sleela nordshrift [--emit] [--target=sleela|java|c] <file.sleela>
sleela version
```

Both `run` and `compile` accept **multiple `.sleela` files**, parsed independently and merged into one compilation unit; duplicate definitions across files are reported.

---

## See also

- **[Nordshrift Sheet Reference](Nordshrift-Sheet-Reference)** — driving builds with `.sst`
- **[Writing a Subject](Writing-a-Subject)** — attaching explicit semantics
- `COMPILER.md`, `SOURCE.md`, `VERSION.md`, `SLEELA.md`, `STRUCTS.md`, `NATIVE_API.md`
