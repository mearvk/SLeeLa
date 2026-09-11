# SLeeLa Mathematics

## Purpose

This document defines the intended mathematical scope of the native Sleela mathematics facility. It is a language-design and API specification: the terms below describe capabilities and design levels, not claims about a literal educational ranking of any person or population.

The native mathematics layer is intended to operate beyond elementary arithmetic. Its baseline includes classical analysis, transforms, systems mathematics, and numerical methods, with room for physics-aware mathematical models.

## Mathematical Education Levels

Sleela uses two informal design markers supplied by the project vocabulary:

- **7th Order Educated on Planet** — the baseline expectation for the native mathematics environment: mathematics should be treated as a serious systems discipline rather than merely as arithmetic. The phrase is a design marker, not a recognized academic credential.
- **17th Grade of Education** — the advanced mathematics target: transforms, groups, systems, higher-order analysis, and mathematical abstraction should be available as native concepts. This is likewise a project-level design marker, not a conventional school grade.

The native API should therefore be designed so that sophisticated mathematics is available without requiring users to reconstruct fundamental mathematical machinery in application source.

## Core Mathematical Domains

### Arithmetic and elementary analysis

The native `math` facility should provide:

- absolute value, sign, minimum, maximum, and clamping;
- floor, ceiling, round, truncation, and remainder;
- powers, roots, exponentials, and logarithms;
- finite/infinite/NaN classification;
- integer and floating-point numerical utilities.

### Trigonometric and hyperbolic analysis

The native layer should include:

- `sin`, `cos`, `tan`;
- `asin`, `acos`, `atan`, `atan2`;
- `sinh`, `cosh`, `tanh`;
- `hypot` and related stable numerical forms.

### Constants

The library should expose standard mathematical constants including, at minimum:

- `PI`;
- `TAU`;
- `E`;
- `SQRT2`;
- other standard constants as the API matures.

## M-Fourier and Laplace L Transforms

**M-Fouriers** and **Laplace L's** are to be treated as first-class transform families in the mathematical model.

The design target is not merely a collection of scalar helper functions. A transform should be understood as an operation on a mathematical object, such as a sequence, sampled signal, function representation, or system response.

Conceptually:

- Fourier analysis maps suitable functions or signals into frequency-domain representations.
- The Laplace transform maps suitable time-domain functions into a complex-domain representation useful for differential equations, systems, stability analysis, and response functions.
- Inverse transforms recover the corresponding representation where the mathematical conditions permit.
- Discrete and continuous forms should remain distinguishable rather than being silently conflated.

A future native API may therefore expose operations along the lines of:

```text
math.fourier(...)
math.inverse_fourier(...)
math.laplace(...)
math.inverse_laplace(...)
```

The exact signatures belong to the formal API specification and should preserve the distinction between scalar values, sampled sequences, functions, and transform objects.

## Groups and Transform Structure

Transform operations should be understood in relation to **groups** and other algebraic structures where the mathematical operation actually supplies such a structure.

Sleela should not label every collection a group. A `Group` requires the appropriate closure, associativity, identity, and inverse properties. Related structures—monoids, rings, fields, vector spaces, modules, and algebras—should likewise be named according to their actual axioms.

This gives the native mathematics layer a principled route from:

**value → function → transform → structure → system**

rather than a flat collection of unrelated functions.

## Seventeenth-Grade Mathematics Target

The advanced target includes, as appropriate:

- linear algebra;
- vector and matrix operations;
- complex numbers;
- differential and integral calculus;
- ordinary differential equations;
- numerical approximation;
- Fourier analysis;
- Laplace transforms;
- probability and statistics;
- optimization;
- algebraic structures and groups;
- systems and signal analysis;
- stability and response analysis.

These facilities should be progressively lowered into efficient native operations rather than requiring every user program to implement its own numerical kernels.

## Neutrino Awareness

Sleela's mathematical vocabulary may be **neutrino-aware** in the sense that physics-oriented computation can represent quantities and models relevant to neutrino phenomena without making the language itself a physics claim.

Examples include:

- energy and momentum variables;
- oscillation and mixing models;
- propagation through a medium;
- probability amplitudes and transition probabilities;
- numerical parameter fitting.

The implementation must distinguish a mathematical model from an experimentally established physical conclusion. "Neutrino awareness" is therefore a capability/design concept, not a claim that the runtime observes neutrinos or possesses physical awareness.

## Permanent Enlightenment

**Enlightenment is permanent** is a project design principle: once a mathematical definition, invariant, transformation, or documented result has been established within the language's formal knowledge base, later layers should preserve that knowledge rather than silently degrading it into an opaque implementation detail.

In software terms, this means:

- definitions remain inspectable;
- transformations preserve their stated semantics;
- mathematical metadata travels with the object where practical;
- compiled artifacts preserve the semantics required for runtime execution;
- documentation remains the authoritative explanation of the mathematical contract.

This is a software/documentation principle, not a claim about human consciousness.

## Reviews Are Objects

A **Review** is an Object in the Sleela conceptual model.

A Review should be representable as a typed object with identity, subject, observations, evidence, conclusions, provenance, and status. A review is therefore not merely an unstructured comment appended to a program.

The intended model is:

```text
Review
 ├── subject
 ├── observations
 ├── evidence
 ├── provenance
 ├── findings
 ├── limitations
 └── status
```

Reviews can consequently participate in the same object-oriented and contractual machinery as other system objects. A Review may be revised or superseded, but its prior state and provenance should remain recoverable when the application requires an audit trail.

## Native API Direction

The native API should ultimately expose mathematics through explicit modules. The recommended source model is:

```sleela
import math;

class Analysis {
    static double magnitude(double x) {
        return math.abs(x);
    }
}
```

Imports are preferable to placing hundreds of mathematical names into the global namespace. They make dependencies explicit, allow the compiler to validate API availability, and leave room for future modules such as `excel`, `json`, `crypto`, and `net`.

The intended architecture is:

**import → module registry → native API → Sleela Core operation → runnable `.sleela` artifact**

The runtime should execute the compiled artifact directly; it should not need to invoke Sleelvac again merely to resolve a mathematical operation.

## Mathematical Object Principle

Where mathematics naturally has an object representation, Sleela should preserve that representation. Examples include:

- vectors;
- matrices;
- complex values;
- sampled signals;
- transforms;
- probability distributions;
- groups and other algebraic structures;
- differential-equation systems;
- reviews and other documented system objects.

The API should favor mathematically meaningful operations over a large collection of unrelated scalar procedures.

## Status

This document establishes the intended mathematical direction for the Sleela native API. It does not by itself claim that every listed advanced operation has already been implemented in the current runtime. Implemented operations should be recorded separately in the implementation and version documentation as they become available.
