# Demesresmes™ Constants Schema

The constants registry is intentionally simple enough to load quickly at startup and strict enough for scientific work.

## Required registry fields

- `name`: canonical dotted identifier.
- `aliases`: optional explicit aliases.
- `category`: mathematics, physics, chemistry, engineering, or another registered scientific domain.
- `symbol`: human/scientific symbol where applicable.
- `value`: decimal or scientific-notation string so precision is not lost by a JSON parser.
- `numeric_type`: runtime numeric domain.
- `exact`: whether the represented quantity is exact in the declared source convention.
- `unit`: dimensional unit expression.
- `source`: provenance identifier.

Optional fields include `uncertainty`, `relative_standard_uncertainty`, `description`, `note`, `revision`, and future evidence/provenance fields.

## Runtime behavior

The interpreter should parse `value` according to `numeric_type`, not through an intermediate binary floating-point conversion when arbitrary precision is requested.

For an inexact value, the runtime should preserve uncertainty metadata. For an exact derived value whose decimal display is rounded, the record must say so rather than pretending the displayed decimal is the full exact representation.

## Namespacing

Canonical names prevent collisions such as mathematical Euler's number `math.euler` and the elementary charge `physics.elementary_charge`, both commonly represented by `e`.

The same principle applies to `R`, `G`, and other symbols that have multiple scientific meanings.

## Revisions

The registry is versioned. A future CODATA update should add a new dataset revision and should not silently rewrite a pinned scientific run.

Suggested API:

```sleela-script
let c = constant.get("physics.speed_of_light")
let old = constant.get("physics.speed_of_light", { revision: "CODATA-2022" })
let registry = constant.registry()
```
