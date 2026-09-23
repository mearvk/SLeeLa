# SLeeLa Subject Libraries

Subject libraries are source-backed, executable vocabularies for scientific and analytical domains.

Current families: Mathematics, Physics, Chemistry, Economics, Finance, Inference, Data Analytics, Astrophysics, and Sociology.

The shared semantic chain is:
Subject -> Quantity -> Unit -> Assumption -> Relation -> Formula -> Transformation -> Result -> Evidence -> Validation

## XML contract

XML is the declarative interchange layer for all subject families. A model declares quantities, units, constants and formulas. A procedure declares ordered, allow-listed operations. An observation records measured data and its context. A result records a derived value and its model inputs.

The generic schema is subject-model.xsd. Every family uses the same model/procedure/observation vocabulary; domain-specific mathematics stays in the corresponding native C/C++ library. Astrophysics examples are in astrophysics/.

XML is declarative: it does not grant arbitrary shell, native-code, network, or filesystem execution. A dispatcher must explicitly recognize each procedure operation.

## Observed-data persistence

Observed data is persisted with subject, target, timestamp, quantity, value, unit, instrument/source and provenance. The native astrophysics store provides an append-only XML record writer. Larger tabular astronomical datasets can use XML/VOTable-compatible interchange; IVOA VOTable 1.5 is an XML data-exchange recommendation and the IVOA Provenance Data Model models provenance around entities, activities and agents. citeturn2search5turn2search2

## Source-addition rule

A subject belongs in native source when its semantics are stable enough to test. Native additions should include documented units and domain assumptions, deterministic formulas and valid ranges, C ABI functions when useful, C++/frontend bindings when needed, XML model/procedure examples, persistence/provenance representation, focused tests, and explicit unsupported behavior for undeclared operations.

Measured, derived, assumed and simulated values must remain distinguishable.


## Sociology

Sociology adds quantitative population and group measures while preserving a strict distinction between description, inference, model, and observation. See [sociology/README.md](sociology/README.md).
