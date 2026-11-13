<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# physics: Tutorial

| | |
|---|---|
| Nature | subject (`bin/sleelactl help physics`): native C/C++ library compiled into the `sleela` runtime |
| Registry summary | Native physics subject (mechanics and physical models); import physics; |
| Lives in | `impl/subjects/physics/physics_native.cpp` (dispatcher: `impl/subjects/native/native_api.cpp`) |
| Writeup type | Tutorial, because you write a SLeeLa program that calls `physics.*` and every step below ran successfully in this checkout. |
| Verified | 2026-11-13 against `master` e048707fd: `sleelactl run physics` exits 0 and matches `physics.sleela.expected.txt`; `physics_values.sleela` gives 15 PASS, 0 FAIL; demo numbers and a new program were checked with python3. |

## What it is

`physics` builds a class `__NativePhysics` of SLeeLa Core methods ([`physics_native.cpp`](../../../impl/subjects/physics/physics_native.cpp)). It has nine constants and 17 functions, and it calls `math` (for `sqrt` and `tau`), so `import physics;` requires `import math;` as well.

Constants (called with parentheses): `C H HBAR E_CHARGE K_B G G0 N_A R` (SI values: `C` = 299792458, `G0` = 9.80665, `R` = 8.31446261815324, ...).

Functions: `velocity(dx, dt)`, `acceleration(dv, dt)`, `kinematic_position(x0, v0, a, t)`, `force(mass, accel)`, `kinetic_energy(mass, velocity)`, `gravitational_force(m1, m2, r)`, `escape_velocity(mass, radius)`, `lorentz_gamma(v)`, `relativistic_energy(mass, v)`, `momentum(mass, v)` (relativistic), `ohms_voltage(current, resistance)`, `electric_power(voltage, current)`, `ideal_gas_pressure(n, R, T)`, `wave_frequency(velocity, wavelength)`, `angular_frequency(frequency)`, `wavenumber(wavelength)`, `neutrino_oscillation_probability(theta, dm2, L, E)`.

## Before you start

```sh
cd SLeeLa            # the repository root
export SLEELA_HOME=$PWD SLEELA_LIB=$PWD/lib SLEELA_SHEET=$PWD/SHEET.sheet \
       SLEELA_SHA256_MANIFEST=$PWD/security/sha256-manifest.json
```

Use `sleela run`, not `check` (`check` does not load subjects). Keep your own programs outside the repo, for example in `~/sleela-scratch/`. SLeeLa comment rules: only `<`/`<<` blocks at line start, no apostrophes inside comments; `close`, `record`, `native` are reserved identifiers.

## Step 1 - Run the reference demo

```sh
timeout 120 bin/sleelactl run physics
```

```
  ✓ physics — Native physics subject (mechanics and physical models); import physics;
  ...
[mechanics]
  velocity(100 m, 5 s)                 = 20 m/s
  acceleration(30 m/s, 6 s)            = 5 m/s^2
  kinematic_position(0, 20, g, 3 s)    = 104.13 m
  force(10 kg, g)                      = 98.0665 N
  kinetic_energy(2 kg, 20 m/s)         = 400 J
[gravitation]
  gravitational_force(Earth, 100 kg, R) = 979.484 N
  escape_velocity(Earth)               = 11177.9 m/s
[special relativity]  (v = 1.5e8 m/s ~ 0.5c)
  lorentz_gamma(0.5c)                  = 1.15497
  relativistic_energy(1 kg, 0.5c)      = 1.03803e+17 J
  momentum(1 kg, 0.5c)                 = 1.73245e+08 kg m/s
[electricity]
  ohms_voltage(2 A, 60 ohm)            = 120 V
  electric_power(120 V, 2 A)           = 240 W
[waves]  (microwave, lambda = 0.06 m)
  wave_frequency(c, 0.06 m)            = 4.99654e+09 Hz
  angular_frequency(60 Hz)             = 376.991 rad/s
  wavenumber(0.06 m)                   = 104.72 rad/m
...
  ✓ physics: ran the native subject demo (its __Native* methods executed)
```

Running `impl/build/sleela run lib/ui/example/science/physics.sleela` directly and diffing (after dropping the 2 security lines) with `physics.sleela.expected.txt` shows no difference.

Independent check with python3 (`G=6.67430e-11`, `c=299792458`, `v=1.5e8`): gamma = 1.1549671837951903, gamma*c^2 = 1.0380327e17, gamma*v = 1.7324508e8, `0 + 20*3 + 0.5*9.80665*9` = 104.129925, `c/0.06` = 4.99654e9, `2*pi*60` = 376.991. All match the printed values.

## Step 2 - Write your own program

Create `physics_tour.sleela`:

```
#sleela 2.5
import math;
import physics;

class PhysicsTour {
    void main() {
        double g = physics.G0();
        print("g0                   = " + g);
        print("speed of light       = " + physics.C());
        print("force(10 kg, g0)     = " + physics.force(10.0, g));
        print("kinetic_energy(2,20) = " + physics.kinetic_energy(2.0, 20.0));
        double v = 0.5 * physics.C();
        double gam = physics.lorentz_gamma(v);
        print("gamma(0.5c) - ref    = " + (gam - 1.1547005383792517));
        print("gravitational_force  = " + physics.gravitational_force(5.972e24, 100.0, 6.371e6));
        print("ideal_gas_pressure(1, R, 273.15) = " + physics.ideal_gas_pressure(1.0, physics.R(), 273.15));
        print("wavenumber(0.06)     = " + physics.wavenumber(0.06));
    }
}
```

```sh
timeout 120 impl/build/sleela run ~/sleela-scratch/physics_tour.sleela 2>/dev/null
```

```
[security] SHA-256 verification required before build/execution/diagnostics
verification: PASS (18667 item(s))
g0                   = 9.80665
speed of light       = 2.99792e+08
force(10 kg, g0)     = 98.0665
kinetic_energy(2,20) = 400
gamma(0.5c) - ref    = 0
gravitational_force  = 981.997
ideal_gas_pressure(1, R, 273.15) = 2271.1
wavenumber(0.06)     = 104.72
```

python3 gives `1/sqrt(0.75)` = 1.1547005383792517 (residual printed 0), `G*5.972e24*100/6.371e6**2` = 981.9973426224686, `8.31446261815324*273.15` = 2271.0954641485573, `2*pi/0.06` = 104.71975511965978. All agree.

## Step 3 - See the dependency rule

Without `import math;` the program is rejected:

```
#sleela 2.5
import physics;

class PhysicsNoMath {
    void main() {
        print(physics.force(1.0, 2.0));
    }
}
```

```
sleelvac: Semantic error: physics requires import math
```

(exit status 1; message from `lowerProgram` in `native_api.cpp`).

## What you did

You ran the physics demo, confirmed its numbers independently, called constants and functions from your own program, and saw the `import math;` requirement enforced.

## Limits found

- `physics.ideal_gas_pressure(n, R, T)` has no volume argument. The code returns `n*R*T/1` (see `physics_native.cpp`), so the result above, 2271.1, is `nRT`, which is a pressure only for a volume of exactly 1 m^3. The name suggests `P = nRT/V`. No file I read documents this.
- The demo prints `relativistic_energy(1 kg, 0.5c)` for `v = 1.5e8` m/s, which is not exactly 0.5c (0.5c = 1.49896e8). The label is approximate; the value is correct for 1.5e8.
- `check` cannot validate subject programs; use `run`.
- All results inherit the accuracy limits of `math` (see [math](math.md)).

## Next steps

- Demo: [`lib/ui/example/science/physics.sleela`](../../../lib/ui/example/science/physics.sleela); assertions: [`impl/tests/subjects/physics_values.sleela`](../../../impl/tests/subjects/physics_values.sleela) (15 checks).
- The astrophysics subject repeats `escape_velocity` with its own constant: [astrophysics](astrophysics.md).
- Layout: [`impl/subjects/README.md`](../../../impl/subjects/README.md).

## Sources

Files read: `bin/sleelactl.d/registry.conf` (line 80), `impl/subjects/physics/physics_native.cpp`, `impl/subjects/native/native_api.cpp`, `impl/subjects/README.md`, `lib/ui/example/science/physics.sleela`, `.../physics.sleela.expected.txt`, `impl/tests/subjects/physics_values.sleela`.

Commands run: `sleelactl info physics`, `sleelactl run physics`, `sleela run` on the demo plus diff with the fixture (identical), `run_subject_tests.sh` (physics: 15 checks passed), `sleela run` on `physics_tour.sleela` and `physics_nomath.sleela` (exit 1, message above), python3 for the reference values. Scratch programs: `~/sleela-scratch/`.
