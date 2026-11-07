# SLeeLa Computational Standards

Edition 1.0 | Baseline: SLeeLa Syntax 3.0 | Status: initial standard

## Purpose
This directory establishes a common computational vocabulary and repeatable comparison method for SLeeLa, Java/JVM, native C, and native C++. It supports language design, compiler qualification, runtime engineering, interoperability, and performance research.

## Documents
1. [Computational Standards](COMPUTATIONAL-STANDARDS.md)
2. [Type and Value Mapping](TYPE-AND-VALUE-MAPPING.md)
3. [Memory and Execution Models](MEMORY-AND-EXECUTION-MODELS.md)
4. [ABI and Interoperability](ABI-AND-INTEROPERABILITY.md)
5. [Conformance Matrix](CONFORMANCE-MATRIX.md)
6. [Common Test Vectors](COMMON-TEST-VECTORS.md)

## Architectural principle
One computational specification, multiple implementation environments.

SLeeLa supplies language semantics, compiler, VM, and artifact model. Java supplies Java language and JVM behavior. C and C++ supply native language implementations and platform ABI integration. SLeeLa's own native implementation may use C/C++, but that does not make SLeeLa source semantics identical to either language.

## Execution model
SLeeLa source -> Sleelvac -> SLeeLa IR/artifact -> SLeeLa VM  
Java source -> javac -> class files -> JVM  
C/C++ source -> native compiler -> object/executable -> processor/OS

Java process delegation, JNI, and an embedded JVM are distinct arrangements. The repository's documented default Java path is external process delegation, not an embedded JVM.

## Normative hierarchy
1. Applicable SLeeLa syntax specification and version gates.
2. SLeeLa VM, artifact, and ABI specifications.
3. Java Language Specification and Java Virtual Machine Specification.
4. Applicable ISO C and ISO C++ standards.
5. Target platform ABI and operating-system contracts.
6. This directory's cross-environment comparison and test guidance.

This directory does not override upstream standards.

## Evidence vocabulary
Normative, Implemented, Tested, Qualified, Planned, Not applicable, and Unknown have distinct meanings. A source file existing is not proof that a runtime capability works. Every qualification claim must identify its feature, toolchain, platform, test command, and retained result.

## Version policy
Record SLeeLa syntax, compiler version, VM artifact ABI, Java/JDK release, C/C++ language modes, compiler versions, target architecture, OS, and optimization settings. Syntax version, VM generation, artifact format, and ABI version are separate numbers.

## Qualification levels
SCS-0 Described; SCS-1 Compiles; SCS-2 Executes; SCS-3 Semantic tests pass; SCS-4 ABI/interoperability tests pass; SCS-5 Platform-qualified. A level applies to a feature and toolchain, not to a whole language by implication.

## Executable baseline

The initial executable baseline is in `tests/`:

- `tests/native/common_vectors.c`: C11 arithmetic, bounded loop, and ASCII output.
- `tests/native/common_vectors.cpp`: C++17 equivalents.
- `tests/java/CommonVectors.java`: Java equivalents.
- `tests/sleela/common_vectors.sleela`: SLeeLa Syntax 3.0 VM equivalents.
- `tests/run_native_java.py`: compiles and runs C, C++, and Java, then compares their output to the shared expected vector.

Run the native/Java parity checks locally with Python 3, a C11 compiler, a C++17 compiler, and JDK 21 (or a compatible JDK):

```sh
python3 comparison/tests/run_native_java.py
```

GitHub Actions workflow: [Comparison Standards Conformance](../.github/workflows/comparison-conformance.yml). It runs native/Java parity on Ubuntu, macOS, and Windows, and builds/runs the SLeeLa VM vector on Ubuntu. Each job retains its result/output artifact.

This is a deliberately small first slice: it exercises arithmetic, loop accumulation, and ASCII output. It does not yet qualify integer overflow, floating-point edge cases, Unicode indexing, memory ownership, C ABI interoperability, Java process delegation, JNI, concurrency, or performance. Those remain unassessed until executable tests and retained CI results exist. Workflow configuration is not evidence of a successful run.
