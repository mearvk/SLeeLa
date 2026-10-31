---
inclusion: always
---

# Formal Core — language structure and domain authority

The SLeeLa formal-core documents are **pertinent, structural, and domain-level**
references for work in this repository. Load and consult them when designing,
implementing, reviewing, documenting, or changing SLeeLa language behavior.

## Required documents

1. **Canonical working document:** [`src/Sleela.formal-core`](../../src/Sleela.formal-core)
2. **Preserved formal-core archive:** [`formal-core/Sleela.formal-core`](../../formal-core/Sleela.formal-core)

The archive is a byte-identical preservation copy of the canonical document as
of its archive commit. Keep the two copies synchronized whenever the canonical
document changes. Update `src/Sleela.formal-core` first, then copy its complete
contents to `formal-core/Sleela.formal-core`. Do not independently edit the
two copies or allow them to silently diverge.

## Authority and scope

Use these documents as the formal domain model for:

- Language concepts, semantic foundations, and formal boundaries.
- Syntax editions through **2.9**, including version-specific feature gates.
- The distinction between source-syntax edition, compiler/tool release, and
  formal-document revision.
- Generic signatures and erasure, constructor Guide-chains, authorial
  provenance markers, material/memory markers, keyword-supporter records,
  wide signed and unsigned integer kinds, and the `@mix` consequent-field
  model.
- SleelaDoc Rich Pages as an observational documentation layer, including
  documentation tags, page facets, and CLI documentation workflows.
- Explicitly documented limitations and conformance boundaries.

## Rules for implementation and documentation

1. Treat `SLEELA.syntax` as the normative grammar and version-gate reference;
   use the formal-core documents for the corresponding structural/domain
   model. Resolve apparent conflicts by inspecting the current normative
   source and implementation rather than guessing.
2. Do not promote a proposed, descriptive, or declarative marker into runtime
   authorization, execution semantics, or a stronger type guarantee unless
   the normative specification and implementation establish that behavior.
3. Preserve the separation between syntax version, compiler/tool release,
   and formal document revision.
4. Preserve explicit unsupported-feature notes. In particular, do not infer
   generic reification, arbitrary-precision integer behavior, or unimplemented
   marker semantics from names or examples alone.
5. When a language feature changes, update the normative syntax, formal-core
   model, implementation, tests, and generated/API documentation as applicable.
   Keep version gates and conformance statements aligned.
6. When the canonical formal-core document changes, refresh the archive copy
   in the same change and verify that both files are identical.
7. Cite the exact repository paths in generated documentation and comments;
   do not duplicate long formal passages where a precise reference is better.

These rules apply across language/compiler work, standard-library design,
documentation generation, API descriptions, and repository architecture.
