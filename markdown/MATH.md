# SLeeLa Mathematics

## Purpose

This document defines the mathematical scope of the native Sleela mathematics facility. The educational phrases below are project design markers, not claims about a literal educational ranking of any person or population.

The native mathematics layer operates beyond elementary arithmetic and is intended to support classical analysis, transforms, systems mathematics, numerical methods, and physics-aware mathematical models.

## Mathematical Education Levels

- **7th Order Educated on Planet** — baseline design marker for treating mathematics as a serious systems discipline rather than merely arithmetic.
- **17th Grade of Education** — advanced design marker for transforms, groups, systems, higher-order analysis, and mathematical abstraction.

The higher-order layer is intended to make sophisticated mathematics available without requiring users to reconstruct fundamental numerical machinery in application source.

## Core Mathematical Domains

The native `math` facility includes an executable subset covering absolute value, sign, minimum, maximum, clamping, roots, powers, exponentials, logarithms, trigonometric functions, hypot, and remainder operations. The implementation is lowered into ordinary Sleela Core methods, so the resulting bytecode remains directly runnable from a `.sleela` artifact.

Additional numerical classifications, inverse trigonometric functions, hyperbolic functions, and transform objects remain part of the expansion path.

## M-Fourier and Laplace L Transforms

**M-Fouriers** and **Laplace L's** remain first-class transform families in the mathematical model.

The design target is not merely scalar helper functions. A transform should operate on a mathematical object such as a sequence, sampled signal, function representation, or system response.

Fourier analysis maps suitable functions or signals into frequency-domain representations. Laplace transformation maps suitable time-domain functions into a complex-domain representation useful for differential equations, systems, stability analysis, and response functions. Discrete and continuous forms must remain distinguishable.

The eventual object API includes forms such as `math.fourier(...)`, `math.inverse_fourier(...)`, `math.laplace(...)`, and `math.inverse_laplace(...)`.

## Groups and Transform Structure

A collection is called a **Group** only when the required closure, associativity, identity, and inverse properties hold. Related structures—monoids, rings, fields, vector spaces, modules, and algebras—should likewise be named according to their actual axioms.

This gives the mathematical layer a principled route from:

**value → function → transform → structure → system**

## Seventeenth-Grade Mathematics Target

The advanced target includes linear algebra, vector and matrix operations, complex numbers, differential and integral calculus, ordinary differential equations, numerical approximation, Fourier analysis, Laplace transforms, probability, statistics, optimization, algebraic structures, systems analysis, stability, and response analysis.

## Neutrino Awareness

Sleela's mathematical vocabulary may be **neutrino-aware**: it can represent quantities and models relevant to neutrino phenomena without making the language itself a physics claim. Mathematical models remain distinct from experimentally established physical conclusions.

## Permanent Enlightenment

**Enlightenment is permanent** is a software/documentation principle: established definitions, invariants, transformations, and documented semantics should remain inspectable and should not silently degrade into opaque implementation details.

Compiled artifacts should preserve the semantics required for direct runtime execution.

## Reviews Are Objects

A **Review** is an Object in the Sleela conceptual model, with identity, subject, observations, evidence, conclusions, provenance, limitations, and status. A review is therefore not merely an unstructured comment.

## Native API Direction

The source model is:

```sleela
import math;

class Analysis {
    static double magnitude(double x) {
        return math.abs(x);
    }
}
```

Imports keep dependencies explicit and prevent hundreds of mathematical names from entering the global namespace.

The executable architecture is:

**import → module validation → native lowering → Sleela Core bytecode → runnable `.sleela` artifact**

## Mathematical Object Principle

Where mathematics naturally has an object representation, Sleela should preserve it: vectors, matrices, complex values, sampled signals, transforms, probability distributions, groups, differential-equation systems, reviews, and other system objects.

## Status

The current runtime contains an executable native scalar mathematics subset. Physics and economics build on that executable math layer. Fourier/Laplace object transforms, richer algebraic objects, dimensional typing, and additional numerical kernels remain planned expansion areas.


## Native Structures — Initial Kernel

The first native linear-algebra C ABI is defined in `impl/subjects/math/linear_algebra.h` and implemented in `linear_algebra.cpp`. It currently provides:

- N-dimensional vector addition, subtraction, scaling, dot product, stable Euclidean norm, and normalization;
- 3D vector cross product;
- row-major dense matrix multiplication, transpose, and identity construction;
- explicit status codes for invalid arguments, dimension mismatch, size overflow, allocation failure, and singular-operation reservations.

The ABI supports exact in-place vector operations and stages matrix results so matrix multiplication and transpose can safely write over their inputs. Partially overlapping vector ranges are outside the contract. The opt-in regression commands are `make -C impl test-linear-algebra`, `make -C impl test-linear-algebra-vm`, and `make -C impl test-linear-algebra-bytecode`.

The VM/compiler integration provides append-only `OP_MATH_CALL` (opcode 179) and source built-ins under `#sleela 3.0` for vector add/subtract/dot/norm/scale/normalize/cross3, matrix multiply/transpose/inverse/QR, complex add/subtract/multiply/divide, symmetric eigendecomposition, and FFT. The built-ins require preallocated output arrays and nonnegative integer-literal dimensions; the VM checks array tags and delegates to the typed Core bridge.


## Expanded native mathematical structures

The native kernel now also exposes:

- **LU:** partial-pivoting factorization, triangular solve, and inverse.
- **QR:** thin modified Gram-Schmidt factorization for tall/square matrices.
- **Complex:** add, subtract, multiply, stable divide, magnitude, principal phase, conjugate.
- **Arrays:** copy, strided slice, and elementwise arithmetic over caller-owned double buffers.
- **Spectral:** symmetric real eigendecomposition using Jacobi rotations; eigenvectors are returned as columns and eigenvalues are unsorted.
- **Fourier:** in-place radix-2 complex FFT with inverse normalization. Length must be a nonzero power of two.

The public header also defines slmath_buffer, operation identifiers, and slmath_vm_dispatch. This is the typed, ownership-neutral VM bridge contract: the VM must validate and pin array storage before dispatch and keep it pinned until the call returns. Buffer descriptors do not own or retain storage. The Core now exposes slvm_math_dispatch_arrays to marshal VM-local SL_ARRAY handles to temporary contiguous doubles under the array mutex, call the kernel, and write numeric results back.

**Integration boundary:** the native object is linked into the sleela and sleelvac executables and libslcore.a. The current branch now includes the heap-array marshalling helper and an append-only `OP_MATH_CALL` interpreter instruction (opcode 179). Its operand is the `slmath_operation`; the stack carries four optional/required array handles, eight nonnegative shape dimensions, a numeric scalar, and nonnegative flags. Vector dot/norm return a double; array-producing operations return the output handle. The instruction delegates to the Core adapter, which validates and marshals the VM-local arrays under the array mutex. The compiler lowers the vector, matrix, complex, QR, symmetric-eigen, and FFT built-ins listed above to `OP_MATH_CALL`. These source built-ins require `#sleela 3.0`; dimensions are currently compile-time nonnegative integer literals so the fixed-shape VM frame is unambiguous. Array arguments are VM `SL_ARRAY` values, not exposed integer handles. The runtime validates those tags and converts them to VM-local handles before calling the bridge. An end-to-end source fixture and opt-in `test-linear-algebra-vm` target exercise the compiler/interpreter/array path. A local build and execution are still required before treating the tests as passing.

Build and run all opt-in native math regressions with:

```sh
make -C impl test-linear-algebra test-linear-algebra-vm test-linear-algebra-bytecode
```

The test suite covers vector/matrix primitives, LU solve and inverse, QR orthogonality, complex division, strided arrays, symmetric eigenvalue invariants, FFT round-trip, and typed dispatch. It is deliberately not part of the default make test aggregate yet.
