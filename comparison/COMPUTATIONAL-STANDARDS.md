# Computational Standards
Standard SCS-BASE-001 | Edition 1.0

## 1. Objective
Define a common method for comparing SLeeLa, Java, C, and C++. Require equivalent results where intended, explicit differences where they are not equivalent, and reproducible evidence for every claim.

## 2. Environments
| Environment | Compilation/execution | Principal contract |
|---|---|---|
| SLeeLa | Sleelvac, SLeeLa IR/artifact, SLeeLa VM | SLeeLa syntax and VM semantics |
| Java | Java compiler, class files, JVM | Java language and JVM specifications |
| C | C compiler, native object/executable | ISO C and target ABI |
| C++ | C++ compiler, native object/executable | ISO C++, library, and target ABI |

C and C++ must remain separate comparison columns. Their language rules, object models, exceptions, and library contracts differ.

## 3. Comparison dimensions
Compare syntax/static semantics; types and conversions; integer/floating arithmetic; evaluation order; arrays, strings, records, and classes; allocation and lifetime; calls and dispatch; errors and exceptions; concurrency; I/O and networking; interoperability; debugging; packaging; security; performance.

## 4. Semantic equivalence
A test is equivalent only when inputs, observable outputs, side effects, and relevant failure behavior are comparable under stated conditions. Record exact input representation, expected output, exit/error state, permitted variation, and toolchain.

Do not compare a defined SLeeLa operation with undefined C/C++ behavior as a passing equivalence test. Record it as a language difference.

## 5. Numeric rules
Record width, signedness, range, promotion, overflow, division, shifts, and conversion. Never assume SLeeLa int, Java int, C int, and C++ int have the same width. Use fixed-width native types for binary contracts. Do not silently substitute wrapping, trapping, saturation, or arbitrary precision.

For floating point, record precision, rounding, NaN/infinity behavior, signed zero, subnormals, and tolerance. Require bitwise identity only when representation and operation order are explicitly fixed.

## 6. Evaluation and control flow
Test precedence, short-circuit behavior, conditional expressions, loops, break/continue, recursion, and evaluation order independently. Where a language leaves order unspecified, test only behavior permitted by its standard.

## 7. Memory and concurrency
State allocation, lifetime, ownership, reclamation, and resource limits. A garbage collector does not imply deterministic cleanup of files, sockets, or locks. Concurrency tests must define synchronization and permitted observations; data races with undefined behavior are not valid portable expected results.

## 8. Interoperability
Use a stable C-compatible ABI for native cross-compiler boundaries. C++ internals must not leak compiler-specific class layouts, exceptions, RTTI, templates, or STL object layouts through that ABI. Identify Java external-process, JNI, and embedded-JVM modes separately.

## 9. Performance
Record workload, data, compiler flags, runtime version, warm-up, repetitions, machine, OS, and measurement method. Separate startup, compilation, warm-up, steady-state execution, allocation, and I/O. Report distributions and uncertainty, not only the best run.

## 10. Evidence
Each result includes test ID, source revision, compiler/runtime versions, language modes, target triple, OS, command, expected/actual result, status, and diagnostics. Use PASS, FAIL, SKIP, BLOCKED, and NOT APPLICABLE. Missing prerequisites are never PASS.

## 11. Security
A reference, symbol, artifact, or metadata record identifies a target; it does not authorize execution. Cross-boundary calls remain subject to resolver, capability, validation, memory, and runtime controls.
