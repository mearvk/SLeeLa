# SLeeLa Subject Libraries

This directory unifies the five SLeeLa subject libraries — **Math**, **Physics**,
**Economics**, **Chemistry**, and **Financial** — under a single, predictable
layout. Each subject has its own folder. Everything a subject owns (its library
implementation, its frontend lowering, and its data/model files) lives together
in that folder.

```
subjects/
├── native/       Shared native dispatcher + AST builders for the arithmetic subjects
│   ├── native_api.{h,cpp}   validate imports, enforce the math dependency,
│   │                        dispatch to the per-subject builders, and lower
│   │                        qualified calls (e.g. math.sqrt) into native methods
│   ├── native_builders.h    shared inline AST-builder helpers (V, D, Bn, C0..C2,
│   │                        Neg, Ret, Decl, Set, Loop, M0..M4)
│   └── native_add.h         declarations of addMath / addPhysics / addEconomics
├── math/         math_native.cpp        → addMath        (__NativeMath)
├── physics/      physics_native.cpp     → addPhysics     (__NativePhysics)
├── economics/    economics_native.cpp   → addEconomics   (__NativeEconomics)
├── chemistry/    chemistry.{h,cpp}, chemistry_api.{h,cpp}, *.model
└── finance/      financial.{h,cpp}, financial_api.{h,cpp}, FINANCIAL.model, README.md
```

## Two shapes of subject library

The subjects come in two shapes, and the layout makes both explicit:

1. **Arithmetic subjects (math, physics, economics)** — these synthesize
   ordinary Sleela Core methods from a shared set of AST-builder primitives.
   Because they share those builders and a single dispatcher, the common code
   lives in **`native/`** while each subject's own class builder lives in its
   own single file (`math/`, `physics/`, `economics/`). This gives one file per
   subject *and* one unified place for the shared machinery.

   - **Math** is the foundational numeric domain.
   - **Physics** and **Economics** depend on Math; `native_api.cpp` enforces
     that `import physics` / `import economics` requires `import math`.

2. **Standalone compiled subjects (chemistry, finance)** — these are
   independent compiled C++ libraries (`chemistry.cpp`, `financial.cpp`) each
   paired with a frontend adapter (`chemistry_api.cpp`, `financial_api.cpp`)
   and their declarative model files. They are lowered separately from the
   native dispatcher so inferred/modeled results stay distinguishable from
   measured or specified values.

## Build

The subject sources are compiled by the top-level [`impl/Makefile`](../Makefile).
Each subject include directory is on the compiler search path, so subject
headers are included by bare name (e.g. `#include "native_api.h"`). Per-subject
build checks are available:

```sh
make test-math test-physics test-economics test-chemistry test-financial
```
