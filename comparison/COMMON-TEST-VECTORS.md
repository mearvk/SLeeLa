# Common Computational Test Vectors
Standard SCS-TEST-001 | Edition 1.0  
Status: baseline and SCS-TEST-002 passed; SCS-TEST-003 array/string suite pending CI.

## 1. Reporting
Each implementation records test ID, source file, compiler/runtime versions, command, expected and actual output, exit status, and PASS/FAIL/SKIP. A skip names the missing prerequisite.

### 1.1 Executable baseline mapping
The initial shared vector is intentionally narrow and deterministic:

| Vector | Operation | Expected output | Implementations |
|---|---|---|---|
| ARITH-BASE-001 | Evaluate 40 + 2 | `42` | SLeeLa VM, Java, C11, C++17 |
| FLOW-BASE-001 | Sum integers 1 through 5 with a loop | `15` | SLeeLa VM, Java, C11, C++17 |
| DATA-BASE-001 | Emit the ASCII string | `hello` | SLeeLa VM, Java, C11, C++17 |

C, C++, and Java are compiled and compared by `tests/run_native_java.py` on Ubuntu, macOS, and Windows. The SLeeLa Syntax 3.0 source is executed in a separate Ubuntu VM job. The CI workflow retains result artifacts. A configured job remains pending evidence until a completed run is visible.

## 2. Executed numeric/Unicode batch 002

The executable suite in [NUMERIC-UNICODE-EDGE-VECTORS.md](NUMERIC-UNICODE-EDGE-VECTORS.md) adds fixed-width signed limits, defined unsigned wrap, signed division/remainder, UTF-8 byte count, and Unicode code-point count. It currently targets C11, C++17, and Java 21+ on Ubuntu, macOS, and Windows. Run [42332366905](https://github.com/mearvk/SLeeLa/actions/runs/42332366905) completed successfully: C11, C++17, and Java 21+ each passed all seven assertions on Ubuntu, macOS, and Windows. SLeeLa VM parity for these added vectors is not yet claimed.

## 3. Integer boundaries
| ID | Operation | Observation |
|---|---|---|
| INT-001 | Represent zero | Exact zero |
| INT-002 | Represent positive one | Exact one |
| INT-003 | Represent negative one | Exact negative one where signed |
| INT-004 | Minimum representable value | Correct value or documented checked failure |
| INT-005 | Maximum representable value | Correct value |
| INT-006 | Add one to maximum | Language-defined overflow rule |
| INT-007 | Subtract one from minimum | Language-defined overflow rule |
| INT-008 | Divide signed operands | Quotient/remainder semantics |
| INT-009 | Signed-to-unsigned conversion | Exact conversion/range rule |
| INT-010 | Narrowing conversion | Documented result or failure |

Do not require the same overflow result unless types and contracts explicitly match.

## 3. Arithmetic and floating point
Arithmetic: addition, subtraction, multiplication, division, divide-by-zero classification, precedence, remainder, bitwise operations, shifts at 0/width-1/width/out-of-range, and numeric conversions.

Floating point: positive/negative zero, finite arithmetic, division by zero, NaN, infinities, subnormals, rounding boundaries, and float/double conversion. Specify bit patterns and tolerances separately. NaN payload identity and signed-zero behavior need explicit tests.

Do not execute undefined C/C++ operations as portable expected results. Record them as language differences; sanitizer output is supplementary evidence.

## 4. Control flow
FLOW-001 if/else; FLOW-002 short-circuit AND; FLOW-003 short-circuit OR; FLOW-004 zero-iteration loop; FLOW-005 one-iteration loop; FLOW-006 break; FLOW-007 continue; FLOW-008 nested loops; FLOW-009 bounded recursion; FLOW-010 evaluation order with side effects only where defined.

## 5. Arrays and strings
DATA-001 empty array; DATA-002 one-element array; DATA-003 first/last valid index; DATA-004 negative/one-past-end index; DATA-005 empty string; DATA-006 ASCII round trip; DATA-007 UTF-8 multibyte round trip; DATA-008 supplementary Unicode code point; DATA-009 invalid encoded input; DATA-010 length and indexing units.

Use identical Unicode input data, but record each language's indexing contract independently.

## 6. Memory and ownership
MEM-001 allocate/release; MEM-002 zero-length allocation; MEM-003 allocation failure; MEM-004 ownership transfer; MEM-005 borrowed value lifetime; MEM-006 double-release prevention; MEM-007 invalid handle; MEM-008 cleanup on error; MEM-009 bounded recursive allocation; MEM-010 shutdown.

Test prevention/diagnostics rather than forcing undefined behavior.

## 7. Interoperability
ABI-001 C calls SLeeLa-exported function; ABI-002 SLeeLa calls C; ABI-003 C++ implementation through C header; ABI-004 C++ exception contained; ABI-005 structure size/version negotiation; ABI-006 string conversion/ownership; ABI-007 null/invalid handle.

JAVA-001 Java executable discovery; JAVA-002 separate process arguments; JAVA-003 stdout/stderr/exit status; JAVA-004 runnable JAR launch; JAVA-005 class-path/main-class launch; JAVA-006 Java unavailable diagnostic; JAVA-007 JNI tests if implemented; JAVA-008 embedded JVM lifecycle tests if implemented.

## 8. Concurrency
CON-001 synchronized counter; CON-002 atomic increment; CON-003 producer/consumer handoff; CON-004 thread start/join; CON-005 shutdown with pending work; CON-006 data-race detection using relevant tooling. Define synchronization and do not demand identical scheduling.

## 9. Cross-platform
Run applicable tests on Linux, Windows, and macOS for each claimed architecture/toolchain. Missing compilers, JDKs, display servers, or services are SKIP/BLOCKED, not PASS. A platform claim requires retained results.

## 10. Benchmark separation
These are correctness tests. Performance workloads must be separately versioned and must not weaken correctness requirements.


## 4. Array, bounds, and string iteration 003

The executable suite in [ARRAY-STRING-BOUNDS-VECTORS.md](ARRAY-STRING-BOUNDS-VECTORS.md) defines ten observations for empty/single arrays, first/last access, rejected negative and one-past-end indexes, UTF-8 round-trip, strict rejection of an overlong UTF-8 sequence, byte length, and code-point count. It targets C11, C++17, and Java 21+ on Ubuntu, macOS, and Windows. Its status remains pending until the updated workflow completes successfully. SLeeLa VM array/string parity is not claimed.
