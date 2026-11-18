# javaconv: Java -> SLeeLa converter (sleelactl `java-convert`)

Converts the Java source of a presumed JDK version into **functionally
equivalent** SLeeLa source. Standing rules: `.kiro/steering/java-to-sleela-conversion.md`.

```
Java source --java_lex.c--> tokens --java_to_sleela.cpp--> SLeeLa 3.1 document(s)
                                          ^
                          conv_model.c <--+-- impl/subjects/conversion/conversion.model
```

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
The refusal rows are the `unsupported` records of the model.

## Verification

`tests/javaconv_differential_test.sh` (also `make -C sleelactl/convert test`):

- `tests/java/*.java` (47 differential fixtures currently present) run on a real JDK and, converted, on the SLeeLa VM; outputs must be
  identical: Anon, Basics, Bounds, Control, Enums, Floats, Flow, Formats, Functional, Grid, Init, Nested,
  ObjectText, Operands (191 lines of int/long/double/narrowing results), Records, Refs, Resources, Shapes, Strings,
  Switches, Sync, Varargs, IdentifierParity, OverloadWidening, ReservedIdentifiers, GenericIdentity, BoxingOverload, GenericCommonReference, NestedGenericInference, BoundedGeneric, ClassGenericReceiver, InheritedGenericReceiver, WildcardGeneric, StaticImports, BoundedClassType, LowerBoundWildcardInference, GenericWildcardLowerBound. The new generic fixtures and the updated `Refs` overload/method-reference fixture were changed after the recorded 2026-11-13 run and are not yet verified. A `// flags:` line passes converter options (`Sync` uses `--single-thread`). Options: `--single-thread`, `--unchecked-sizes` (omit the `NegativeArraySizeException` wrapper), `--bundle`/`--split`.
- `tests/reject/*.java` (24 programs) must be refused with the construct named on their `// expect:` line
  and write nothing.
- stale output: a file converted on one run and failing closed on the next leaves no `.sleela` behind.

## Status (honest)

Converted and proven against a JDK (`tests/javaconv_differential_test.sh`, 22 programs plus 9 refusals and a
stale-output check, 32 of 32 passing on 2026-11-13; in CI as `.github/workflows/java-convert.yml`). The later `IdentifierParity`, `OverloadWidening`, `ReservedIdentifiers`, `GenericIdentity`, `StaticImports`, and updated `Refs` fixtures, the `AmbiguousWideningOverload` and `BoundedGenericInference` refusal fixtures, and the overload-resolution/import changes, class-bound and deferred wildcard-lower-bound fixtures, and the `BoxingOverload`, `GenericCommonReference`, `BoundedGeneric`, `NestedGenericInference`, `ClassGenericReceiver`, and `WildcardGeneric` fixtures have not yet been run:
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

The project index retains the declared package for each collected source type. Duplicate declarations within one compilation unit and across source units are rejected deterministically. Class type parameters retain declared upper bounds; concrete arguments are checked against supported simple bounds. Parameterized/intersection class bounds and unknown/wildcard actual arguments remain partial and are not claimed as fully checked.  The current resolver supports same-package lookup, explicit single-type imports, ordinary package on-demand imports, explicit imports of indexed nested types, and package-qualified source type names. Type accessibility is checked for public, package-private, protected, and private nested types where their declaring type is indexed.

Member lookup records Java visibility for fields, methods, and constructors. Package-private and private access are restricted, protected inherited access is permitted from a subclass, and protected instance method access checks the qualifying receiver type. The converter remains fail-closed for unresolved duplicate simple names because the project symbol table still has a simple-name keyed declaration store; complete simultaneous resolution of same-named declarations in different packages requires the next symbol-table migration.

The following Java source forms are explicitly refused with construct-specific diagnostics rather than being treated as ordinary declarations:

- JPMS module declarations (`module-info.java`).
- Annotation interface declarations.

This is not a claim of complete JLS accessibility or Java language coverage. In particular, protected field receiver checks, all nested-type access combinations, static nested-type wildcard imports, module readability, and the remaining preview/newer Java constructs still require dedicated implementation and JDK differential fixtures.
