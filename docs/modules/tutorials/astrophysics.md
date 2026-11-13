<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# astrophysics: Tutorial

| | |
|---|---|
| Nature | subject (`bin/sleelactl help astrophysics`): native C/C++ library compiled into the `sleela` runtime |
| Registry summary | Native astrophysics subject (astronomical formula models); import astrophysics; |
| Lives in | `impl/subjects/astrophysics/` (`astrophysics_native.cpp` = SLeeLa side; `astrophysics_math.c`, `astrophysics.cpp`, `observation_store.c` = C ABI and C++ facade); XML models in `api/sleela/2.1/subjects/astrophysics/` |
| Writeup type | Tutorial, because you write a SLeeLa program that calls `astrophysics.*` and every step below ran successfully in this checkout. |
| Verified | 2026-11-13 against `master` e048707fd: `sleelactl run astrophysics` exits 0 and matches `astrophysics.sleela.expected.txt`; demo and a new program checked with python3; the C smoke test was compiled in a scratch dir and passes. |

## What it is

From SLeeLa, `astrophysics` offers six functions, built in [`astrophysics_native.cpp`](../../../impl/subjects/astrophysics/astrophysics_native.cpp) from SLeeLa Core methods:

| Call | Formula in the code |
|---|---|
| `flux_from_luminosity(L, r)` | `L / (4*pi*r^2)` (W, m) |
| `redshift(observed, rest)` | `observed/rest - 1` |
| `observed_wavelength(rest, z)` | `rest*(1+z)` |
| `wien_peak(T)` | `2.897771955e-3 / T` (m, K) |
| `kepler_period(a, mass)` | `2*pi*sqrt(a^3/(G*mass))` with `G = 6.67430e-11` |
| `escape_velocity(mass, radius)` | `sqrt(1.33486e-10 * mass / radius)` (that constant is 2G) |

`import astrophysics;` requires `import math;` (enforced in `native_api.cpp`); it does not require `import physics;`. A separate C ABI (`astrophysics_math.c`, 11 functions) and a C++ class `sleela::astrophysics::Model` exist for native callers; they are not what `.sleela` programs call.

## Before you start

```sh
cd SLeeLa            # the repository root
export SLEELA_HOME=$PWD SLEELA_LIB=$PWD/lib SLEELA_SHEET=$PWD/SHEET.sheet \
       SLEELA_SHA256_MANIFEST=$PWD/security/sha256-manifest.json
```

Use `sleela run`, not `check`. Keep your own programs outside the repo, for example in `~/sleela-scratch/`. SLeeLa comment rules: only `<`/`<<` blocks at line start, no apostrophes in comments; `close`, `record`, `native` are reserved.

## Step 1 - Run the reference demo

```sh
timeout 120 bin/sleelactl run astrophysics
```

```
  ✓ astrophysics — Native astrophysics subject (astronomical formula models); import astrophysics;
  ...
[radiation]
  flux at 1 AU from the Sun            = 1361.17 W/m^2
    (the solar constant, ~1361 W/m^2)
  wien_peak(Sun, 5772 K)               = 5.02039e-07 m
    (~502 nm, green-peaked visible light)
[spectroscopy]  H-alpha rest = 656.3 nm
  redshift(observed 660 nm, rest)      = 0.00563767
  observed_wavelength(rest, z)         = 6.6e-07 m
[orbits & gravity]
  kepler_period(1 AU, Sun)             = 3.15535e+07 s
    (~3.156e7 s, i.e. one year)
  escape_velocity(Earth)               = 11177.9 m/s
...
  ✓ astrophysics: ran the native subject demo (its __Native* methods executed)
```

A direct run of `lib/ui/example/science/astrophysics.sleela` matches `astrophysics.sleela.expected.txt` (diff empty). python3 gives `2.897771955e-3/5772` = 5.020394932e-07 and `660/656.3 - 1` = 0.0056376657, the same as the demo.

## Step 2 - Write your own program

Create `astrophysics_tour.sleela`:

```
#sleela 2.5
import math;
import astrophysics;

class AstrophysicsTour {
    void main() {
        double au = 1.495978707e11;
        double mSun = 1.989e30;
        double z = astrophysics.redshift(700.0e-9, 656.3e-9);
        print("redshift             = " + z);
        print("observed_wavelength  = " + astrophysics.observed_wavelength(656.3e-9, z));
        print("wien_peak(5772 K)    = " + astrophysics.wien_peak(5772.0));
        print("kepler_period(1 AU)  = " + astrophysics.kepler_period(au, mSun));
        print("flux_from_luminosity = " + astrophysics.flux_from_luminosity(3.828e26, au));
        print("escape_velocity      = " + astrophysics.escape_velocity(5.97e24, 6.3781e6));
    }
}
```

```sh
timeout 120 impl/build/sleela run ~/sleela-scratch/astrophysics_tour.sleela 2>/dev/null
```

```
[security] SHA-256 verification required before build/execution/diagnostics
verification: PASS (18667 item(s))
redshift             = 0.0665854
observed_wavelength  = 7e-07
wien_peak(5772 K)    = 5.02039e-07
kepler_period(1 AU)  = 3.15535e+07
flux_from_luminosity = 1361.17
escape_velocity      = 11177.9
```

python3 reference values: redshift `700/656.3 - 1` = 0.06658540301691307; `656.3e-9*(1+z)` = 7e-07; Kepler `2*pi*sqrt(au^3/(G*M))` = 31553514.07; flux `3.828e26/(4*pi*au^2)` = 1361.1664654; escape `sqrt(2*G*5.97e24/6.3781e6)` = 11177.877946830502. All agree to the printed digits.

## Step 3 - Run the native C kernels (optional)

The C ABI has its own smoke test, [`impl/tests/subjects/astrophysics_smoke.c`](../../../impl/tests/subjects/astrophysics_smoke.c). `make test-astrophysics` would build into `impl/build/`, which was already prepared and must stay untouched, so this step compiles the same sources with gcc into a scratch directory instead:

```sh
mkdir -p ~/sleela-scratch/build && cd ~/sleela-scratch
S=$SLEELA_HOME/impl
gcc -O1 -I$S/subjects/astrophysics $S/tests/subjects/astrophysics_smoke.c \
    $S/subjects/astrophysics/astrophysics_math.c $S/subjects/astrophysics/observation_store.c \
    -o build/astro_smoke -lm
./build/astro_smoke
```

```
astrophysics: PASS
```

## What you did

You ran the astrophysics demo, confirmed it against python3, called all six SLeeLa functions with your own inputs, and compiled and ran the C ABI smoke test.

## Limits found

- Only 6 of the 11 C functions are reachable from `.sleela`. `astrophysics.planck(5.0e-7, 5772.0)` fails: `sleelvac: Semantic error: call to unknown method '__native_astrophysics_planck'`. `luminosity_from_flux`, `gravitational_parameter`, `angular_size`, `distance_modulus` are likewise absent from `astrophysics_native.cpp`.
- The SLeeLa functions carry their own inline formulas and do not use `astrophysics_math.c`. The C kernels guard against bad input (`flux` returns 0 for `r <= 0`, `redshift` returns 0 for `rest == 0`); the SLeeLa versions divide without guards.
- `observation_store.c` is broken as written. The smoke test's call writes (see `build/astrophysics-observations.xml`):

  ```
  <observation id="obs-1" ... timestamp="2026-09-22T00:00:00Z">\n  <measurement quantity="wavelength" value="0" unit="m"/>\n  <instrument>demo</instrument>\n ...</observation>\n
  ```

  The newlines are the two characters `\n` (the source writes `"\\n"` inside a C string), and `value` is `0` although the record holds `500e-9`: the `%.17g` conversion is in a `fprintf` with no argument, while `r->value` is passed to a later `fprintf` whose format has no conversion. The smoke test only checks that the function returns 1.
- `api/sleela/2.1/subjects/astrophysics/README.md` says the subject is "built on Mathematics and Physics". The code requires only `import math;`.
- The two `escape_velocity` implementations (physics uses `G*mass`, astrophysics uses the literal `1.33486e-10`) give the same 11177.877946830502 for Earth in my python3 check.

## Next steps

- Docs: [`api/sleela/2.1/subjects/astrophysics/README.md`](../../../api/sleela/2.1/subjects/astrophysics/README.md) and `model.xml`, `observation.xml`, `procedure.xml` in the same folder (not parsed by any step here).
- Demo source: [`lib/ui/example/science/astrophysics.sleela`](../../../lib/ui/example/science/astrophysics.sleela).
- Related: [physics](physics.md), [math](math.md).

## Sources

Files read: `bin/sleelactl.d/registry.conf` (line 82), everything under `impl/subjects/astrophysics/`, `impl/subjects/native/native_api.cpp`, `impl/Makefile` (astrophysics targets), `impl/tests/subjects/astrophysics_smoke.c`, `api/sleela/2.1/subjects/astrophysics/README.md`, `lib/ui/example/science/astrophysics.sleela` and `.sleela.expected.txt`.

Commands run: `sleelactl info astrophysics`, `sleelactl run astrophysics`, direct `sleela run` plus fixture diff (identical), `sleela run` on `astrophysics_tour.sleela` and `astrophysics_planck.sleela` (error above), the gcc build and run shown in Step 3 (output in the scratch `build/` dir), python3 for reference values.
