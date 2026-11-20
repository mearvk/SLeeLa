# javaconv: Java -> SLeeLa converter (sleelactl `java-convert`)

Converts the Java source of a presumed JDK version into **functionally
equivalent** SLeeLa source. Standing rules: `.kiro/steering/java-to-sleela-conversion.md`.

```
Java source --java_lex.c--> tokens --java_to_sleela.cpp--> SLeeLa 3.1 document(s)
                                          ^
                          conv_model.c <--+-- impl/subjects/conversion/conversion.model
```

> **Caveat (2026-11-20):** the edition statement above is the original wording and is kept. Syntax **3.2** is the current edition (`SLEELA.syntax` 44.1; `impl/frontend/version.h` ceiling 3.2; `api/sleela/3.2/LANGUAGE-API.md`). `#sleela 3.01` means 3.1 and does not mean 3.2. Sections 44, 45 and 46 are not one edition: 44 is 3.1, 45 and 46 are 3.2. The converter writes `#sleela 3.2` (`java_to_sleela.cpp`), not 3.1 as the diagram says. See `sleelactl/MEARVK.LLC.MAX.RUPPLIN.JAVA-CONVERT.SESSION.2026-11-12.md`, last addendum.

| File | Language | Role |
|---|---|---|
| `java_lex.{h,c}` | C99 | JLS ch. 3 tokenizer: literals decoded (hex/octal/binary, `_`, `L`, exponents, escapes, text blocks) |
| `conv_model.{h,c}` | C99 | loads and indexes the conversion subject model |
| `java_to_sleela.{h,cpp}` | C++17 | typed recursive-descent translator; two passes (signatures, then bodies) |
| `javaconv_main.cpp` | C++17 | CLI: directory walk, symbol collection over the whole tree, report |

Converted constructs and the rules for extending the converter are specified in the Java Specific
Section, [`lib/java/JAVA.SPEC.md`](../../lib/java/JAVA.SPEC.md). Java improvements go in the converter and in
`lib/java`, never in the compiler, VM or runtime.

## The conversion subject

Like `math` and `physics`, `conversion` is a subject with a model:
`impl/subjects/conversion/conversion.model`. It holds the Java types, boxing,
promotion and primitive widening (JLS 5.1.2), target type spellings, model-ranked operand classes,
operator templates for `int`/`long`/`float`/`double`/`boolean`/`String`,
the primitive cast matrix, compound-assignment and `++`/`--` rules, string
conversion, context-aware identifier replacements, built-in qualification,
and library result types. C++ overload selection consults the model for primitive
invocation widening and considers
strict fixed-arity calls before loose boxing/unboxing applicability and varargs, recursive reference-array covariance, known
reference subtyping, inherited overloads, bounded and nested method-variable
inference, wildcard compatibility, direct class-generic receiver substitution,
and most-specific parameter types. Explicit and wildcard static
method/field imports are resolved when their owner is known. Incomparable or
unsupported candidates fail closed. The SLeeLa `JConvModel` loads the same type, promotion, operator, cast, widening,
rename and qualification rows (with matching built-in defaults where required).
The native expression pass uses the shared rows for integral and binary32 promotion,
float rounding, primitive casts and text conversion. Method type variables (`T`, `T[]`, `T...`, and nested forms such as `Holder<T>`) are inferred from argument types; simple method bounds are validated, supported simple class upper bounds are retained and checked, and wildcard arguments use conservative extends compatibility plus deferred lower-bound checks after inference. Class type variables are substituted through parameterized receiver, superclass, and interface paths for method calls and inherited field reads. Common known superclass inference is supported; interface-intersection LUBs, intersection bounds, wildcard capture, and target-type inference remain fail-closed. Explicit and wildcard
static imports resolve when their owner/method is present in the shared model or
collected source signatures; competing owners fail closed. The int laws are
executed by `lib/java/JavaOperands.sleela` (verified against a JDK by
`examples/java/differential/OperandSemantics`).

## Use

```sh
bin/sleelactl build java-convert
bin/sleelactl run   java-convert --jdk-version 28            # sleelactl/jdk/28/java -> sleelactl/jdk/28/sleela
bin/sleelactl run   java-convert --jdk DIR --out DIR [--jdk-version N] [--split|--bundle]
bin/sleelactl run   java-convert --file Foo.java --out DIR   # one runnable bundled document
bin/sleelactl run   java-convert --self-check
```

Exit 0: everything converted. Exit 1: some files failed closed (see
`CONVERSION-REPORT.md/.json` in the output). Exit 2: usage/IO (including a file that cannot be written).

Each run is a directory-level transaction. The converter creates a sibling staging directory, copies the
previous output tree without following symlinks, and writes converted files, the ownership manifest,
provenance and reports only into that stage. It publishes the complete generation by renaming the old
directory aside and renaming the stage into place; if staging or publication fails, the previous output
generation is restored or left untouched. Unowned files are preserved and are never overwritten. Symlinks
and special files in the previous output tree are rejected rather than followed. Re-runs remove only stale
files listed in `<out>/.javaconv-emitted`. Inputs and emitted paths are sorted; unchanged converted files
and reports are written only when their bytes differ, preserving mtimes. Each run writes
`CONVERSION-PROVENANCE.json` with SHA-256 digests for immutable inputs, exact model bytes, converter source
inputs and emitted outputs; the manifest omits timestamps and absolute paths.

## Fail closed

Every construct the model does not cover is refused with
`file:line:col [construct] reason`. A file with any failed member writes no output.
The refusal rows are the `unsupported` records of the model. Since model 1.6.0 this is enforced: a refusal kind without a row is reported `[model]`, and the
report lists each kind with its model reason.

## Verification

`tests/javaconv_differential_test.sh` (also `make -C sleelactl/convert test`):

- `tests/java/*.java` (49 differential fixtures currently present) run on a real JDK and, converted, on the SLeeLa VM; outputs must be
  identical: Anon, Basics, Bounds, Control, Enums, Floats, Flow, Formats, Functional, Grid, Init, Nested,
  ObjectText, Operands (191 lines of int/long/double/narrowing results), Records, Refs, Resources, Shapes, Strings,
  Switches, Sync, Varargs, IdentifierParity, OverloadWidening, ReservedIdentifiers, GenericIdentity, BoxingOverload, GenericCommonReference, NestedGenericInference, BoundedGeneric, ClassGenericReceiver, InheritedGenericReceiver, WildcardGeneric, StaticImports, BoundedClassType, LowerBoundWildcardInference, GenericWildcardLowerBound, GenericConstructorExplicitArguments, GenericConstructorClassSubstitution. The new generic fixtures and the updated `Refs` overload/method-reference fixture were changed after the recorded 2026-11-13 run and are not yet verified. A `// flags:` line passes converter options (`Sync` uses `--single-thread`). Options: `--single-thread`, `--unchecked-sizes` (omit the `NegativeArraySizeException` wrapper), `--bundle`/`--split`.
- `tests/reject/*.java` (30 programs) must be refused with the construct named on their `// expect:` line
  and write nothing.
- stale output: a file converted on one run and failing closed on the next leaves no `.sleela` behind.

## Status (honest)

**2026-11-20 (final): access and indirect /lib calls; model 1.6.0.** Differential suite 116 of 116 (adds `model-registry`). Implicitly public interface,
record and enum members are visible across packages. Calls reaching `/lib` through a project type, an unqualified
inherited call, or a static import are checked for stubs. A method call on an array is refused. `java.base` subset:
262 of 884 converted. All of `java.base`: 862 of 3344 (`member-access` refusals 3,753 to 114). Record:
`sleelactl/MEARVK.LLC.MAX.RUPPLIN.JAVA-CONVERT-RESOLUTION.SESSION.2026-11-20.md`.

**2026-11-20 (latest): field resolution.** Differential suite 108 of 108. Enclosing-class type variables are in scope
in inner classes. A single static import of a field reads `Owner.NAME`, and so does a field inherited from a `/lib`
superclass. Static fields of `lib/java/jdk-28` units are checked, and a non-final placeholder declaration
(`BigInteger.ONE = null`) is refused `[library-stub]`. Enum constants are public across packages. `java.base` subset:
259 of 884 converted, 22 errors in 11 emitted files (unchanged). All of `java.base`: 842 of 3344, with four earlier
conversions now correctly refused for reading `null` placeholders (whole-tree checker not re-run on all of `java.base`).
Details: `sleelactl/MEARVK.LLC.MAX.RUPPLIN.JAVA-CONVERT.SESSION.2026-11-12.md`, last addendum.

**2026-11-19 analysis: interface visibility and array methods.** Interface methods are now implicitly public
(JLS 9.4): calls through an interface type from another package were refused `[member-access]` (3,753 sites on
all of `java.base`, now 114). A method call on an array (`array.clone()`, JLS 10.7) is now refused
`[object-method]`: it was emitted unchanged and failed the checker. Differential suite 110 of 110. All of
`java.base`: 854 of 3344 converted (871 SLeeLa files), 86 linked semantic errors (was 842 and 137). Ranked
blockers and open defects: `docs/reference/JAVA.CONVERTER.ANALYSIS.2026-11-19.md`.

**2026-11-20 (later): re-measurement.** Differential suite 103 of 103 on Corretto 25.0.5.1. `tests/javaconv_check_tree.sh`
now links its stub at `#sleela 3.2` (it was 3.1 and rejected `array.length` in 3.2 output). `java.base` subset:
254 of 884 converted, 11 emitted files with errors (22 errors). All of `java.base`: 841 of 3344 converted (859 SLeeLa files),
53 files with errors (146 errors). Most frequent first refusals on all of `java.base`: `type-ambiguous` 468,
`member-access` 322, `library-type` 268, `library-member` 202. Details and open defects:
`sleelactl/MEARVK.LLC.MAX.RUPPLIN.JAVA-CONVERT.SESSION.2026-11-12.md`, last addendum.

**2026-11-20: resolution fixes and whole-tree check.** Three converter defects let output through that does not
link: (1) a unit converted against the signatures of a unit that failed closed, so it named members nothing
defines (for example 63 calls to `Objects.requireNonNull__T`); (2) a nested type of a library type was written
`Map__Entry`, which the SLeeLa loader does not resolve (it resolves `Map.Entry`); (3) a type outside both the run
and `/lib` was written as-is. Also: two declarations with one simple name in different packages were both
emitted (`Tripwire`), and `float`/`double` constructor overloads lowered to one signature (`AssertionError`).
The converter now converts to a fixpoint, spells library nested types dotted, refuses `[library-type]`,
`[type-ambiguous]` and `[overload-erasure]`, and records the `/lib` name set in provenance (JAVA.SPEC.md 5.1).
Measured on the JDK 25.0.5 `java.base` subset (884 files) with `tests/javaconv_check_tree.sh`:

| | before | after |
|---|---|---|
| files converted | 341 (381 SLeeLa files) | 290 (325 SLeeLa files) |
| semantic errors with every emitted file linked | 874 | 108 |
| emitted files with an attributed error | 110 | 30 |
| emitted files with none | 271 | 295 |

A second pass the same day checks calls into `lib/java/jdk-28` units by name and arity (`[library-member]`),
refuses calls that would bind to signature stubs (`[library-stub]`), calls Java statics that the tier implements
as instance methods as `new T().m(...)`, and casts `Any` results back (JAVA.SPEC.md 5.1):

| | after resolution fixes | after member checks |
|---|---|---|
| files converted | 290 (325 SLeeLa files) | 254 (262 SLeeLa files) |
| semantic errors with every emitted file linked | 108 | 22 |
| emitted files with an attributed error | 30 | 11 |
| emitted files with none | 295 | 251 |

The 11 files left involve converted interfaces that extend `lib/java` classes (inherited members such as
`NavigableSet.isEmpty`, the inherited constant `CharacterIterator.DONE`), covariant overrides of a `lib/java`
return type (`constantValue`), and constructor selection by argument type (`Exception`). The most common first
refusals are now `library-type` 113, `library-member` 84, `nested-ambiguous` 63, `member-access` 53,
`generic-type` 39 and `library-stub` 36. Differential suite: 103 of 103 (adds `LibraryStatics`,
`reject/LibraryMember`, `reject/LibraryStub`, `reject/ConstructorErasure`, `cascade`, `flat-namespace`).
The text below is the earlier status, kept as the record.

Converted and proven against a JDK (`tests/javaconv_differential_test.sh`, 22 programs plus 9 refusals and a
stale-output check, 32 of 32 passing on 2026-11-13; in CI as `.github/workflows/java-convert.yml`). The later `IdentifierParity`, `OverloadWidening`, `ReservedIdentifiers`, `GenericIdentity`, `StaticImports`, and updated `Refs` fixtures, the `AmbiguousWideningOverload` and `BoundedGenericInference` refusal fixtures, and the overload-resolution/import changes, class-bound and deferred wildcard-lower-bound fixtures, the generic-constructor-diamond and generic-constructor-bound refusal fixtures, the constructor class-substitution fixtures, and the `BoxingOverload`, `GenericCommonReference`, `BoundedGeneric`, `NestedGenericInference`, `ClassGenericReceiver`, and `WildcardGeneric` fixtures have not yet been run:
integer, float, char and String semantics; `String.valueOf/join/format` (literal format); native `switch`
(fallthrough, arrow cases, switch expressions); assignment and `++`/`--` as values; enums; varargs;
try-with-resources; `synchronized`/`volatile` under `--single-thread`; arrays of any rank; anonymous
classes, lambdas and method references (captured variables as fields); nested and inner classes;
`instanceof` patterns; records; field default values; Java initialization order (field initializers and
instance initializer blocks after `super(...)`); members inherited from library superclasses; qualified
nested type names (`Format.Field` is `Format__Field`); library objects shown as text through `toString()`;
`char` arguments of `StringBuilder.append`/`insert` as text.

Not converted (refused with a located diagnostic): operands of unknown type (interface-intersection inference, wildcard capture, intersection bounds and target-type inference remain unsupported),
lambdas whose target interface is unknown, nested capture chains,
`outer.new Inner()`, local classes, switch type patterns, `static { }` blocks, `Object` methods on a type
that does not declare them, `native`. The list with reasons is section 5
of [`lib/java/JAVA.SPEC.md`](../../lib/java/JAVA.SPEC.md).

**Whole-tree measurement, 2026-11-13** (JDK 25.0.2 `src.zip`, `java.base` packages `java/lang`, `java/util`,
`java/math`, `java/text` with subpackages, 884 files, the same set as the 2026-11-12 baseline of 344):
**420 of 884 files (496 SLeeLa files); 426 with `--single-thread`**. Runtime about 8 s. The files that fail closed, by their first refusal:
`nested-ambiguous` 131 (one simple name used by several nested types in a file, for example `Entry`),
`nested` 67, `untyped` 57, `volatile` 32, `object-method` 31, `syntax` 28, `init-block` 25, `model` 18,
`synchronized` 16, `boxed` 13, `native` 12. The whole status, with the history of this number: the root
`JAVA.CONVERSION.STATUS.md`. "Converted" means the converter accepted the file. It does not mean the file
passes `sleela check`: a split-layout file names sibling nested types (`Format__Field`) that a standalone check
does not link, and calls on library types are only as good as their `lib/java` units. That check has not been
measured over the tree.

Earlier note, kept for the record: the earlier whole-tree measurement (JDK 25 `java.base`, 345 of 884
files) predates these lowerings and has not been repeated. Nothing is claimed beyond what the report of
a run and the differential tests show.

## Package identity and accessibility (incremental)

The project index retains the declared package for each collected source type. Duplicate declarations within one compilation unit and across source units are rejected deterministically. Class type parameters retain declared upper bounds; concrete arguments are checked against supported simple bounds. Parameterized/intersection class bounds and unknown/wildcard actual arguments remain partial and are not claimed as fully checked.  The current resolver supports same-package lookup, explicit single-type imports, ordinary package on-demand imports, explicit imports of indexed nested types, and package-qualified source type names. Type accessibility is checked for public, package-private, protected, and private nested types where their declaring type is indexed; nested-type access now also requires every enclosing declaration to be accessible.

Member lookup records Java visibility for fields, methods, and constructors. Package-private and private access are restricted, protected inherited access is permitted from a subclass, and protected instance method and instance-field access check the qualifying receiver type. The converter remains fail-closed for unresolved duplicate simple names because the project symbol table still has a simple-name keyed declaration store; complete simultaneous resolution of same-named declarations in different packages requires the next symbol-table migration.

The following Java source forms are explicitly refused with construct-specific diagnostics rather than being treated as ordinary declarations:

- JPMS module declarations (`module-info.java`).
- Annotation interface declarations.

This is not a claim of complete JLS accessibility or Java language coverage. Protected field receiver checks, all nested-type access combinations, module readability, and the remaining preview/newer Java constructs still require dedicated implementation and JDK differential fixtures. Static nested-type single-member imports and on-demand imports now have implementation paths that preserve the fully qualified enclosing owner; dedicated differential fixtures were added but have not been executed.


### Generic constructor class substitution (implementation only; 2026-11-18)

Explicit allocation type arguments are now carried into the constructed expression's `Ty` and passed as the receiver type to constructor overload selection. This lets the existing class-type substitution machinery instantiate constructor parameter types before applicability and argument adaptation. If a class declares constructors but no constructor remains applicable after substitution, conversion fails closed with `[constructor-resolution]`. Explicit generic arguments on anonymous-class allocation remain refused with `[generic-constructor]` until that path has equivalent semantics. Added `GenericConstructorClassSubstitution.java` (positive) and `GenericConstructorClassSubstitutionConflict.java` (negative); fixture inventory is 49 positive / 27 refusal. No build, javac, differential test, or CI was run; this is NOT VERIFIED and production remains HOLD.


### Qualified constructor allocation type names (implementation only; 2026-11-18)

`new` parsing now retains the complete dotted type name rather than discarding package/enclosing-type segments. Indexed qualified names are resolved through the package-aware resolver, including nested types such as `fixture.alpha.AlphaBase.Nested`; unindexed library names continue to fall back to their terminal type name for model resolution. The package-import differential harness now also includes fully qualified top-level and nested allocations. This does not complete canonical symbol identity: the underlying project class store is still simple/hoisted-name keyed and same-simple-name declarations across packages cannot coexist reliably. No build or test was run; NOT VERIFIED / HOLD.


### Overload-resolution status (implementation only; 2026-11-18)

The resolver separates strict fixed-arity, loose fixed-arity (boxing/unboxing), and variable-arity applicability phases. The latest selection correction compares applicable candidates by instantiated parameter specificity rather than using aggregate conversion distance as a tie-breaker; variable-arity specificity uses effective component parameters. This remains partial: maximal-candidate selection across three or more candidates, complete generic/poly-expression inference, method-reference adaptation, and comprehensive inherited/boxing/unboxing/varargs differential coverage remain open. No build, test, javac run, or CI was performed for this change. **NOT VERIFIED; production HOLD.**
