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
