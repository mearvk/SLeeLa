# SLeeLa 3.1 Java tier API

Everything specific to Java source is specified apart from the language, in the **Java Specific Section**:
[`lib/java/JAVA.SPEC.md`](../../../lib/java/JAVA.SPEC.md). The ledger of what is converted and what is left is
[`lib/java/CONVERSION.PRINCIPLES.md`](../../../lib/java/CONVERSION.PRINCIPLES.md). The SLeeLa compiler, VM and runtime
stay language-neutral; Java support is library code under `lib/java` and the converter `sleelactl java-convert`.

## Library classes of the tier (generated pages under the package `java`)

| Class | Role |
|---|---|
| `JavaOperands` | 32-bit `int` arithmetic, narrowing, shifts, division corners |
| `JavaFloat` | binary32 `float`: `f32(d)` rounds a double to the nearest float, `text(f)` is `Float.toString` |
| `JavaFormat` | `String.format` conversions (`%d %x %X %o %f %s %c %b`, flags, width, precision) and `String.join` |
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
negative-size wrapper), `--bundle` / `--split`, `--self-check`. Converted output declares `#sleela 3.1`.
A construct that cannot be converted equivalently is refused with `file:line:col [construct] reason`, and a file with
a refused member writes nothing. The conversion laws are the model `impl/subjects/conversion/conversion.model`.
