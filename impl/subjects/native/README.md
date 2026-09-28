# Native subject dispatcher (math / physics / economics)

The arithmetic subject libraries — **Math**, **Physics**, and **Economics** —
share one dispatcher and one set of AST-builder primitives, which live here.
Each subject's own class builder lives in its own single file one level up
(`../math/math_native.cpp`, `../physics/physics_native.cpp`,
`../economics/economics_native.cpp`).

| File | Role |
|---|---|
| `native_api.{h,cpp}` | Public entry point: `validateImports`, `lowerProgram`. Validates imports, enforces that physics/economics require `import math`, invokes the per-subject builders, and lowers qualified calls (e.g. `math.sqrt`) into the synthesized native methods. |
| `native_builders.h` | Shared inline AST-builder helpers (`V`, `I`, `D`, `Bn`, `C0`, `C1`, `C2`, `Neg`, `Ret`, `Decl`, `Set`, `Loop`, `M0`–`M4`) in `sleela::native::builders`. |
| `native_add.h` | Internal declarations for `addMath` / `addPhysics` / `addEconomics`, defined in the per-subject files. |

Chemistry and Financial are **not** part of this dispatcher — they are
standalone compiled libraries with their own frontend lowering under
`../chemistry/` and `../finance/`.
