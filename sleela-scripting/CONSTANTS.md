# Demesresmes™ Constants Registry

Demesresmes™ has a built-in-facing constants registry for mathematical, physical, chemical, and engineering reference values. The registry is data, not executable code, and is intended to be resolved by the scripting runtime through explicit names.

## Name

The SLeeLa scientific scripting language is named **Demesresmes™**.

- Source extension: `.sleela-script`
- Language family: SLeeLa Scripting / Turing 5
- Registry namespace: `constant`
- Canonical data files:
  - `constants/math-science.constants.json`
  - `constants/math-science.constants.xml`

The ™ mark is part of the project-facing language name in documentation and branding; source identifiers use `Demesresmes` without punctuation.

## Lookup model

Scripts should be able to resolve a constant by canonical name:

```sleela-script
let c = constant.get("physics.speed_of_light")
let h = constant.get("physics.planck")
let pi = constant.get("math.pi")
let avogadro = constant.get("chemistry.avogadro")
```

Aliases are explicit:

```sleela-script
let c = constant.get("c")
let tau = constant.get("tau")
```

A resolved value is a typed scientific value. The runtime should retain the unit, exactness/uncertainty, source, revision, and category metadata.

## Registry rules

1. Canonical names are stable API identifiers.
2. Aliases must be explicitly declared; the runtime must not guess.
3. Exact constants are marked `exact: true`.
4. Decimal representations of mathematical constants are approximations and must not be treated as exact identities.
5. CODATA values carry their published uncertainty and revision.
6. Derived constants are marked as derived when they are computed from other constants.
7. External updates must create a registry revision rather than silently changing an existing pinned dataset.
8. A script may request a specific registry revision for reproducible science.
9. Lookup results include provenance suitable for reports and audit records.

## Initial registry

The initial registry includes:

- mathematical constants: `pi`, `tau`, `euler`, `sqrt2`, `phi`;
- SI-defining/fundamental physical constants: `c`, `h`, `hbar`, `e`, `k_B`, `N_A`;
- physics/chemistry constants: `G`, `alpha`, `R`, `F`, `sigma`, `m_u`, `eV`;
- a small set of standard engineering/reference values.

The physical values are based on the NIST/CODATA 2022 recommended values. NIST identifies those as the latest CODATA values currently available and notes that the next regularly scheduled adjustment is 2026. citeturn0search2turn0search3

## Examples

```sleela-script
script "orbital-check"

timeout 15 minutes

let c = constant.get("physics.speed_of_light")
let G = constant.get("physics.newtonian_gravitational_constant")
let h = constant.get("physics.planck")

let energy = h.value * 5.0e14
return {
    energy: energy,
    units: h.unit + " * Hz",
    provenance: h.provenance
}
```

For dimensional work, prefer quantity arithmetic over extracting bare numeric values. The scripting runtime should reject incompatible dimensions instead of silently producing a result.

## Data files

The JSON file is the machine-oriented interchange form. The XML file carries the same registry so BODI/XML-oriented tooling can consume the same named constants. They are intended to remain semantically equivalent.

See also:

- [SCIENCE.md](SCIENCE.md)
- [TYPE-SYSTEM.md](TYPE-SYSTEM.md)
- [INTEGRATION.md](INTEGRATION.md)
- [TURING-5-IMPLEMENTATION.md](TURING-5-IMPLEMENTATION.md)
