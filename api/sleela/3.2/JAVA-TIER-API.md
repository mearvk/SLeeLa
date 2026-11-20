# SLeeLa 3.2 Java tier API

This page stands on its own: it describes the Java tier as it is used with syntax 3.2, the current edition.

Everything specific to Java source is specified apart from the language, in the **Java Specific Section**,
[`lib/java/JAVA.SPEC.md`](../../../lib/java/JAVA.SPEC.md). The ledger of what is converted and what is left is
[`lib/java/CONVERSION.PRINCIPLES.md`](../../../lib/java/CONVERSION.PRINCIPLES.md). The SLeeLa compiler, VM and runtime
stay language-neutral. Java support is library code under `lib/java` and the converter `sleelactl java-convert`.

## Language edition used

The C++ converter writes `#sleela 3.2` at the top of every converted document. The language features the tier relies on
are specified in `SLEELA.syntax`:

| Feature | Section | Available from |
|---|---|---|
| character literal `'a'`, array initializer `{..}`, labeled `break`/`continue`, `finally` on `return`/`break`/`continue` | 44 | 3.1 |
| typed `char` shown as text, `a.length`, `new T[a][b]` | 45, 46 | 3.2 |

`#sleela 3.01` is the numeric spelling of 3.1 and does not mean 3.2. Under `#sleela 3.1` or `3.01`, `a.length` is rejected
("requires #sleela 3.2"; use `arrayLength(a)`). The compiler accepts 1.0 to 3.2 and a source with no `#sleela` line is
read as 3.2. The converter never edits the compiler, VM or runtime; Java semantics live in the model and in `lib/java`.

## Library classes of the tier (generated pages under the package `java`)

| Class | Role |
|---|---|
| `JavaOperands` | 32-bit `int` arithmetic, narrowing, shifts, division corners |
| `JavaFloat` | binary32 `float`: `f32(d)` rounds a double to the nearest float, `text(f)` is `Float.toString` |
| `JavaFormat` | `String.format` conversions (`%d %x %X %o %f %s %c %b`, flags, width, precision), `String.join`, and `objectText` for library objects shown as text |
| `JavaChecks` | `size(n)` throws `NegativeArraySizeException` for a negative array size |
| `support/SLJavaFaults` | builds the Java exception for a VM fault (divide by zero, index out of range, failed cast) |
| `jdk-25/`, `jdk-28/` | one SLeeLa unit per public JDK type |

## Converter

```sh
bin/sleelactl build java-convert
bin/sleelactl run java-convert --jdk-version 28            # sleelactl/jdk/28/java -> sleelactl/jdk/28/sleela
bin/sleelactl run java-convert --file Foo.java --out DIR   # one runnable bundled document
```

Options: `--single-thread` (`synchronized` and `volatile` become plain), `--unchecked-sizes` (omit the
negative-size wrapper), `--bundle` / `--split`, `--self-check`. A construct that cannot be converted equivalently is
refused with `file:line:col [construct] reason`, and a file with a refused member writes nothing. The conversion laws
are the model `impl/subjects/conversion/conversion.model`.

## Verification

- `tests/javaconv_differential_test.sh` runs each fixture on a real JDK and, converted, on the SLeeLa VM, and requires
  identical output. Last run on `master`: pass=100, fail=0.
- `tests/javaconv_check_tree.sh <out-dir>` links every emitted file and runs the semantic checker over the set.
  "Converted" means the converter accepted the file; this script measures whether the output also checks. On the
  `java.base` subset (`java/lang`, `java/util`, `java/math`, `java/text`, 884 source files), measured 2026-11-20 with
  model 1.5.0: 341 files converted (381 SLeeLa files), 271 of those 381 have no attributed semantic error and 110 do
  (874 errors with all files linked; attribution is by leave-one-out and is approximate).
- After the resolution fixes of 2026-11-20 (`lib/java/JAVA.SPEC.md` 5.1), the same subset gives 290 converted files
  (325 SLeeLa files); 295 have no attributed semantic error and 30 do (108 errors with all files linked). The
  remaining errors are calls into `/lib` units that lack the member or declare it differently.
