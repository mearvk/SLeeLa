<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# chemistry: Tutorial

| | |
|---|---|
| Nature | subject (`bin/sleelactl help chemistry`): native C/C++ library compiled into the `sleela` runtime |
| Registry summary | Native chemistry subject (evidence-weighted chemistry support); import chemistry; |
| Lives in | `impl/subjects/chemistry/` (`chemistry_api.cpp` = SLeeLa lowering, `chemistry.cpp` = C++ model, `*.model` = declarative data) |
| Writeup type | Tutorial, because you write a SLeeLa program that calls `chemistry.*` and every step below ran successfully in this checkout. |
| Verified | 2026-11-13 against `master` e048707fd: `sleelactl run chemistry` exits 0 and matches `chemistry.sleela.expected.txt`; `chemistry_values.sleela` gives 10 PASS, 0 FAIL; a new program was checked with python3. |

## What it is

`chemistry` is an evidence-weighted scoring subject, not a table of chemistry facts. From SLeeLa you get six functions ([`chemistry_api.cpp`](../../../impl/subjects/chemistry/chemistry_api.cpp), the `allowed` set):

- `ratio(numerator, denominator)`: `a/b`, 0 when `b == 0`.
- `similarity(a, b)`: `1 - |a-b| / max(|a|, |b|, 1)`.
- `stochastic(seed, weight)`: `0.98 + 0.04 * |fmod(seed*0.6180339887498949, 1)| * clamp01(weight)`.
- `inference_level(observation, bond, valence, similarity, signal, symmetry)`: `24 * S`, clamped to 0..24.
- `confidence(...)`: `S` clamped to 0..1.
- `uncertainty(...)`: `1 - S` clamped.

where `S = 0.24*obs + 0.20*bond + 0.22*valence + 0.13*similarity + 0.12*signal + 0.09*symmetry` and each input is clamped to 0..1 first. [`markdown/CHEMISTRY.md`](../../../markdown/CHEMISTRY.md) states that the 0..24 value is a model-support score, "never experimental proof".

`import chemistry;` requires `import math;`. The lowering lives in `chemistry_api.cpp`, not in the shared `native_api.cpp` dispatcher.

## Before you start

```sh
cd SLeeLa            # the repository root
export SLEELA_HOME=$PWD SLEELA_LIB=$PWD/lib SLEELA_SHEET=$PWD/SHEET.sheet \
       SLEELA_SHA256_MANIFEST=$PWD/security/sha256-manifest.json
```

Use `sleela run`, not `check`. Keep your own programs outside the repo, for example in `~/sleela-scratch/`. SLeeLa comment rules: only `<`/`<<` blocks at line start, no apostrophes in comments; `close`, `record`, `native` are reserved.

## Step 1 - Run the reference demo

```sh
timeout 120 bin/sleelactl run chemistry
```

```
  ✓ chemistry — Native chemistry subject (evidence-weighted chemistry support); import chemistry;
  ...
[mass ratios]  ratio(a, b) = a / b
  O / C   = 1.33203
  C / H   = 11.9157
  He / H  = 3.97083
[similarity]  similarity(a, b) = 1 - |a-b| / max(|a|,|b|)
  N vs O  = 0.875492
  C vs N  = 0.8575
  H vs He = 0.251836
...
  ✓ chemistry: ran the native subject demo (its __Native* methods executed)
```

The direct run `impl/build/sleela run lib/ui/example/science/chemistry.sleela` matches `chemistry.sleela.expected.txt` (diff empty). python3 gives `15.999/12.011` = 1.3320289734410125, `12.011/1.008` = 11.915674603174603, `1 - |12.011-14.007|/14.007` = 0.8574998, `1 - |1.008-4.0026|/4.0026` = 0.2518363. They agree.

## Step 2 - Write your own program

Create `chemistry_tour.sleela`:

```
#sleela 2.5
import math;
import chemistry;

class ChemistryTour {
    void main() {
        print("ratio(15.999, 12.011)         = " + chemistry.ratio(15.999, 12.011));
        print("ratio(1, 0)                   = " + chemistry.ratio(1.0, 0.0));
        print("similarity(14.007, 15.999)    = " + chemistry.similarity(14.007, 15.999));
        print("level(0.9,0.8,0.7,0.6,0.5,0.4)       = " + chemistry.inference_level(0.9, 0.8, 0.7, 0.6, 0.5, 0.4));
        print("confidence(same evidence)     = " + chemistry.confidence(0.9, 0.8, 0.7, 0.6, 0.5, 0.4));
        print("uncertainty(same evidence)    = " + chemistry.uncertainty(0.9, 0.8, 0.7, 0.6, 0.5, 0.4));
        print("stochastic(seed 7, weight 1)  = " + chemistry.stochastic(7.0, 1.0));
    }
}
```

```sh
timeout 120 impl/build/sleela run ~/sleela-scratch/chemistry_tour.sleela 2>/dev/null
```

```
[security] SHA-256 verification required before build/execution/diagnostics
verification: PASS (18667 item(s))
ratio(15.999, 12.011)         = 1.33203
ratio(1, 0)                   = 0
similarity(14.007, 15.999)    = 0.875492
level(0.9,0.8,0.7,0.6,0.5,0.4)       = 16.896
confidence(same evidence)     = 0.704
uncertainty(same evidence)    = 0.296
stochastic(seed 7, weight 1)  = 0.99305
```

python3 check of the formulas above: `S = 0.24*.9 + 0.2*.8 + 0.22*.7 + 0.13*.6 + 0.12*.5 + 0.09*.4` = 0.704; `24*S` = 16.896; `1-S` = 0.296; `0.98 + 0.04*|fmod(7*0.6180339887498949, 1)|` = 0.99305 (python3 prints 0.9930495...). All agree.

## What you did

You ran the demo, confirmed its ratios against python3, and used the evidence-scoring functions with your own inputs, including the zero-denominator case.

## Limits found

- The SLeeLa functions are not the C++ library. `chemistry.cpp` (`evaluate`, `presume`, `stochastic_factor`, `causal_order`) is linked into `sleela`, but no SLeeLa call reaches it (`chemistry_api.cpp` builds its own methods). I compiled `chemistry.cpp` with a small C++ driver in the scratch dir and fed it the same evidence 0.9/0.8/0.7/0.6/0.5/0.4:

  | | SLeeLa `chemistry.*` | C++ `chemistry::evaluate` |
  |---|---|---|
  | level | 16.896 (continuous) | 17 (`floor(support*25)`, integer, capped 24) |
  | uncertainty | 0.296 (`1 - S`) | 0.388095 (`0.55*(1-support) + 0.45*spread`) |
  | stochastic factor, seed 7 | 0.99305 (golden-ratio fmod) | 0.993937 (SplitMix64 mixing) |

  Both define the same weights, but the two implementations give different numbers. Treat the SLeeLa versions as the executable contract (they are what `chemistry_values.sleela` tests).
- `markdown/CHEMISTRY.md` names a future `chemistry.describe(subject)`; it does not exist (`allowed` has only the six names above).
- Calls inside a ternary expression are not lowered: `double t = flag ? chemistry.ratio(1.0, 4.0) : 0.0;` fails with `Semantic error: use of undeclared variable 'chemistry'` (`math` in the same position works).
- `periodic_table.model` (118 elements, `Z | symbol | name`) and `presumed_chemistry.model` are data files. A search for their file names found only documentation, SHA-256 manifests and `impl/README.md`, no source file that opens them.
- The demo comment says `max(|a|,|b|)`; the code uses `max(|a|, |b|, 1)`. The two agree for values of at least 1, as in the demo.

## Next steps

- [`markdown/CHEMISTRY.md`](../../../markdown/CHEMISTRY.md) for the evidence model and the 0..24 norms; [`impl/examples/chemistry_presumed.sleela`](../../../impl/examples/chemistry_presumed.sleela) (not run for this writeup).
- [`impl/tests/subjects/chemistry_values.sleela`](../../../impl/tests/subjects/chemistry_values.sleela) (10 checks).
- Data files: [`impl/subjects/chemistry/periodic_table.model`](../../../impl/subjects/chemistry/periodic_table.model), [`presumed_chemistry.model`](../../../impl/subjects/chemistry/presumed_chemistry.model).

## Sources

Files read: `bin/sleelactl.d/registry.conf` (line 81), `impl/subjects/chemistry/{chemistry.h,chemistry.cpp,chemistry_api.h,chemistry_api.cpp,periodic_table.model,presumed_chemistry.model}`, `markdown/CHEMISTRY.md`, `lib/ui/example/science/chemistry.sleela` and `.sleela.expected.txt`, `impl/tests/subjects/chemistry_values.sleela`, `impl/Makefile` (object lists), a repository-wide search for readers of the `.model` files.

Commands run: `sleelactl info chemistry`, `sleelactl run chemistry`, direct `sleela run` plus fixture diff (identical), `run_subject_tests.sh` (chemistry: 10 passed), `sleela run` on `chemistry_tour.sleela` and `chem_ternary.sleela`, `g++ -std=c++17` build of `cpp_chem.cpp` + `chemistry.cpp` in `~/sleela-scratch/build/`, python3 for reference values.
