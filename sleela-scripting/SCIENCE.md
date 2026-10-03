# Sleela Script Science and Engineering

Turing 5 gives scientific and engineering work first-class language facilities.

## Numerical domains

The standard profile reserves APIs for integers, arbitrary-precision integers, floating-point values, rational values, complex values, vectors, matrices, intervals, uncertainty bounds, quantities with units, symbolic expressions and observed/derived values.

## Calculus

The language provides a calculus vocabulary including derivative, partial derivative, gradient, divergence, curl, integral, numerical quadrature, differential-equation solving, finite differences, limits, Taylor/series expansion, root finding and optimization.

Example:

    let f = fn(x) { x^3 + 2*x }
    let slope = calculus.derivative(f, 2)
    let area = calculus.integrate(f, 0, 2)

Symbolic and numerical results are distinct. A numerical approximation is never silently represented as an exact identity.

## Engineering quantities

Where the unit system is available:

    let force = 120 N
    let distance = 4 m
    let work = force * distance
    assert units(work) == J

Domain libraries can provide mechanics, electricity, signals, thermodynamics, materials, controls, structures and related engineering operations.

## Definitions and lookups

Scientific lookup is explicit:

    let definition = lookup.definition("entropy")
    let constant = lookup.constant("speed_of_light")
    let material = lookup.material("aluminum")

Lookup results can carry provenance and evidence status. External information is not silently promoted to an observed fact.

## Performance

Pure mathematical expressions should be eligible for native optimization, vectorization, constant caching and specialized numerical kernels. Observable host operations remain explicit barriers so optimization cannot reorder them incorrectly.
