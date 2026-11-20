# Java converter series: overall analysis, 2026-11-19

Measured on master after `1fd398d81`. Host: Linux x86-64, gcc. JDK: Corretto 25.0.5.1 (`lib/src.zip`, extracted with
Python `zipfile`). Runtime: `impl/build/sleela` 1.0121. Converter: `sleelactl/convert` (C++, model 1.5.0).
Every number below comes from a run in this session.

## 1. Where the series stands

| Measure | Before this session's fixes | After |
|---|---|---|
| Differential suite (`tests/javaconv_differential_test.sh`) | 108 / 108 | 110 / 110 |
| `java.base` subset (lang, util, math, text; 884 files) | 259 converted, 625 failed closed | unchanged |
| Subset tree check (`tests/javaconv_check_tree.sh`) | 256 of 267 clean, 22 errors | unchanged |
| All of `java.base` (3,344 files) | 842 converted (860 SLeeLa files) | 854 converted (871 SLeeLa files) |
| `java.base` output linked once, total semantic errors | 137 | 86 |

The subset is unchanged because both fixes act outside its four packages, mostly in `sun.security`, `javax.crypto`
and `sun.reflect.generics`.

"Converted" means the converter accepted the file. The linked error count is the stronger measure: it says how
much accepted output still fails the SLeeLa checker. Per-file attribution over all of `java.base` (leave-one-out
over about 870 files) does not fit the sandbox time limit. The whole-tree number here is a single link of all
emitted files with the stub from `tests/javaconv_check_tree.sh`.

## 2. Fixes made in this analysis

Both fixes are in `sleelactl/convert/java_to_sleela.cpp`.

1. **Interface methods are implicitly public (JLS 9.4).**
   - Methods declared in an interface took only their explicit modifiers, so `int size();` was recorded with package
     visibility. Interface fields were already public.
   - Every call through an interface type from another package was refused as `[member-access]`. That covered
     `List.size`, `Map.put`, `Consumer.accept`, `CharSequence.length` and others.
   - `member-access` refusal sites dropped from 3,753 to 115 (114 with both fixes). A `private` interface method (Java 9+) stays private.
   - Test: the `project-iface-public` project in the differential suite, with a cross-package call plus a default
     method reaching a private one.
2. **A method call on an array is refused.**
   - `array.clone()` (JLS 10.7) was emitted unchanged, but SLeeLa arrays have no methods. So every file using it was
     "converted" yet failed the checker. This was the most common linked error: 32 `clone` errors, plus 22 errors
     that followed from them.
   - Such calls are now refused `[object-method]` at the call site.
   - 12 files that used to count as converted are now refused, which is correct: their output never checked.
   - Test: `tests/reject/ArrayClone.java`.

I measured the two fixes separately. Interface visibility alone gave 867 converted with 148 linked errors: it
unblocked 25 files, and it exposed 13 errors that the refusal had been hiding. Adding the array refusal gives
854 converted with 86 errors.

## 3. What blocks the rest of `java.base`

Of the 2,490 refused files, 1,112 have exactly one blocking category. Fixing that category alone would convert them.
That is the most useful way to rank the work. Counting refusal sites overstates categories that appear many times
in files that have other blockers too.

| Category | Files blocked by this alone | Files where it appears |
|---|---|---|
| `type-ambiguous` | 324 | 680 |
| `library-type` | 127 | 493 |
| `library-member` | 98 | 443 |
| `library-stub` | 96 | 320 |
| `object-method` | 72 | 369 |
| `nested-ambiguous` | 55 | 408 |
| `nested` | 41 | 290 |
| `generic-inference` | 35 | 380 |
| `untyped` | 34 | 365 |
| `synchronized` | 32 | 261 |
| `generic-constructor-diamond` | 29 | 327 |
| `init-block` | 27 | 329 |

The top four categories, by what causes them:

- **`type-ambiguous`: one flat SLeeLa type namespace.**
  - Converted documents share a single namespace, so two Java types with the same simple name cannot both exist.
  - The most frequent names are `SecureRandom` (`java.security` and `sun.security.provider`), `Extension`, `Event`,
    `CertificateException`, `Era`, `Annotation`, `Certificate` and `Signature`.
  - This is the largest single lever, and it is structural. It needs package-qualified type naming in the emitted
    SLeeLa, or package namespaces in the language, rather than a converter rule.
- **`library-type` and `library-member`: missing `lib/java/jdk-28` units or members.**
  - The most frequent missing types are `ObjectIdentifier`, `StringBuf`, `LocaleProviderAdapter`, `DerValue` and
    `HandshakeContext`.
  - The most frequent missing members are `BigInteger.valueOf` (13 files), `IntegerPolynomial` constructors and
    `Unsafe.getUnsafe`.
  - This is library work in `lib/java`, not converter work.
- **`library-stub`: the `/lib` member exists only as a stub that returns a default value.**
  - The converter correctly refuses these.
  - The most frequent are `Objects.requireNonNull` (31 files blocked by stubs alone), `System.arraycopy` (11),
    `ObjectInputStream.defaultReadObject` and `Thread.currentThread`.
  - Implementing `Objects.requireNonNull` and `System.arraycopy` in `lib/java/jdk-28` has a better payoff per unit
    of work than anything else on this list.
- **`object-method`.** It now includes the array-method refusals from fix 2. Supporting `array.clone()` needs a typed
  shallow-copy helper in `lib/java` and a converter rule to call it.

## 4. Open defects (accepted output that fails the checker)

These 5 linked errors were hidden behind the `member-access` refusal before fix 1. They are converter bugs, because
the converter accepted code that does not check:

| Error | Where |
|---|---|
| `call to unknown method 'requireNonNull'` | a newly converted `java.base` file |
| `call to unknown method 'name'` | a newly converted `java.base` file |
| `class 'TerminalFieldBuilder' has no method 'constantPool'`, and `method 'constantPool' returns error` | `jdk/internal/classfile/impl/ChainedFieldBuilder` |
| `'Set.iterator()' must return an iterator object, got Any` | a newly converted `java.base` file |

The other 81 linked errors come from earlier conversions, including the 22 in the subset's 11 files
(`StringCharacterIterator` 7, `NavigableSet` 4, and others). The defect list in
`sleelactl/MEARVK.LLC.MAX.RUPPLIN.JAVA-CONVERT.SESSION.2026-11-12.md` tracks them.

## 5. Suggested order of work

1. **`lib/java/jdk-28`:** implement `Objects.requireNonNull`, `System.arraycopy`, `BigInteger.valueOf` and the
   `BigInteger`/`Collections`/`StandardCharsets` constants that are still placeholders. This is library-only work,
   and it unblocks the `library-stub` and `library-member` files directly.
2. Fix the 5 open checker defects in section 4, so that "converted" keeps meaning "checks clean".
3. Decide how to qualify type names across packages. This is a language or design decision, and it unblocks
   `type-ambiguous`, the largest category.
4. Add a typed `array.clone()` helper.
5. Then return to the converter categories: `nested-ambiguous`, `generic-inference` and `untyped`.

Not measured in this analysis: per-file attribution over all of `java.base`, the JDK 28 campaign, and Windows or
macOS hosts. The release decision in `JAVA.CONVERSION.STATUS.md` stays at **HOLD**.
