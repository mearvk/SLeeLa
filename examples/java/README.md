# Java SE/JDK 28 conformance programs

These programs test the SLeeLa implementation of the JDK 28 API under
[`lib/java`](../../lib/java/JDK28-PROCEDURAL-IMPLEMENTATION.md). Each one
calls the implementation, compares every result with the value Java SE
returns for the same call, prints a `FAIL` line for each mismatch, and ends
with a summary line.

| Program | Covers |
|---|---|
| `JavaLangThrowableConformance` | Throwable hierarchy: `extends` chain and `instanceof`, messages, cause, suppressed, real `throw`/`catch` |
| `JavaLangMathConformance` | `java.lang.Math` against IEEE reference doubles; `addExact` overflow throws |
| `JavaLangIntegralConformance` | `Integer`, `Long`, `Short`, `Byte` parse/format/bit operations, `StrictMath` |
| `JavaLangFloatingConformance` | `Double`/`Float` `toString`, parse, bit views, hex strings, compare |
| `JavaLangDoubleRoundTrip` | `Double.toString` then `parseDouble` round trip over 1,348 values from 1e-300 to 1e300 |
| `JavaLangCharacterConformance` | `Character` classification, case mapping, surrogates; `Boolean` |
| `JavaUtilCollectionsConformance` | Lists, deques, `Stack`, `Vector`, hash and tree maps and sets, `PriorityQueue` |
| `JavaUtilUtilitiesConformance` | `Arrays`, `Collections`, `Random` (Java LCG sequences), `Optional*`, `Objects` |

Run all of them:

```sh
examples/java/run-conformance.sh            # uses impl/build/sleela when built
```

To run one:

```sh
cd examples/java && SLEELA_HOME=../.. SLEELA_LIB=../../lib ../../impl/build/sleela run JavaUtilCollectionsConformance.sleela
```
