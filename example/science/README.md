# `example/science` — runnable science examples for every discipline

This directory has **two** kinds of hands-on, deterministic science example:

1. **SLeeLa programs** (`*.sleela`) — runnable programs on the **science subject
   libraries** (`import math;`, `import chemistry;`, …). See
   [SLeeLa science programs](#sleela-science-programs).
2. **BODI XML projects** (`*.xml`) — declarative projects for the **BODI** XML
   science runner. See [BODI XML examples](#bodi-xml-examples).

Both are reproducible with no network access, and each example ships a recorded
expected-output evidence file (`*.sleela.expected.txt` / `*.expected.txt`).

---

## SLeeLa science programs

These are `.sleela` programs built on SLeeLa's **science subject libraries**
(`impl/subjects/`, surfaced to a program as `import <subject>;`). The subject
families are **Mathematics, Physics, Chemistry, Astrophysics, and Sociology**;
every non-math subject depends on math, so each program begins with
`import math;`.

| File | Imports | What it shows |
|---|---|---|
| [`math.sleela`](math.sleela) | `math` | The full math facade: constants (`pi`/`e`/`tau`), unary (`abs`/`sqrt`/`exp`/`log`/`log10`/`sin`/`cos`/`tan`/`floor`/`ceil`/`trunc`/`sign`), binary (`pow`/`hypot`/`min`/`max`/`fmod`), and `clamp`. |
| [`physics.sleela`](physics.sleela) | `math`, `physics` | The native `physics` facade — mechanics, gravitation, special relativity, electricity, waves (`physics.velocity(…)`, `physics.escape_velocity(…)`, …), with CODATA constants and Earth data. Earth escape velocity ≈ 11,178 m/s. |
| [`chemistry.sleela`](chemistry.sleela) | `math`, `chemistry` | The native `chemistry` facade — `chemistry.ratio(a,b)` and `chemistry.similarity(a,b)` — over IUPAC standard atomic weights. |
| [`astrophysics.sleela`](astrophysics.sleela) | `math`, `astrophysics` | The native `astrophysics` facade — flux, redshift, Wien peak, Kepler period, escape velocity (`astrophysics.redshift(…)`, …). Solar constant ≈ 1361 W/m²; 1-AU Kepler period ≈ 1 year. |
| [`sociology.sleela`](sociology.sleela) | `math`, `sociology` | The native `sociology` facade — proportion, rate, growth rate, transition probability (`sociology.rate(…)`, …) — over real US labor/population figures. |

> **Every program calls its subject library's native facade directly** —
> `math.sqrt(…)`, `physics.velocity(…)`, `chemistry.ratio(…)`,
> `astrophysics.redshift(…)`, `sociology.rate(…)`. Each non-math subject
> depends on math, which is why every program also `import math;`. The facades
> are the executable native subject libraries under
> [`impl/subjects/`](../../impl/subjects); these programs are exercised by the
> same engine as the repository's subject tests
> ([`impl/tests/subjects`](../../impl/tests/subjects)).

### Running the SLeeLa programs

A built `sleela` binary is required — `./scripts/build-linux.sh` (or
`make -C impl`) produces one at `impl/build/sleela`.

```sh
./example/science/run-science-sleela.sh     # run all five, with the env set for you

# or via make (from this directory):
make sleela          # run every .sleela program
make sleela-check    # run every .sleela program and diff vs *.sleela.expected.txt
```

Manual invocation (the fail-closed SHA-256 gate needs both variables):

```sh
export SLEELA_SHEET="$PWD/SHEET.sheet"
export SLEELA_SHA256_MANIFEST="$PWD/security/sha256-manifest.json"
./impl/build/sleela run example/science/physics.sleela
```

---

## BODI XML examples

These are declarative projects for the SLeeLa **BODI** XML science runner. Each
project exercises one (or, for the grand tour, all) of the runner's executable
science disciplines and emits one `BODI witness … status=executed` line per
operation.

The disciplines and their operations come straight from the native dispatcher
in [`api/bodi/bodi_xml_runner.cpp`](../../api/bodi/bodi_xml_runner.cpp)
(`mathop`, `physicsop`, `chemistryop`, `analyticsop`). The inputs are grounded,
wherever possible, in the real, sourced **Churn** datasets under
[`churn/`](../../churn).

## The examples

| File | Discipline(s) | What it shows | Grounded in |
|---|---|---|---|
| [`math.xml`](math.xml) | math | Every math op at its correct arity: `abs`, `sqrt`, `exp`, `log`, `log10`, `sin`, `cos`, `tan`, `pow`, `hypot`, `min`, `max`, `clamp`. | — |
| [`physics.xml`](physics.xml) | physics | Every physics op: kinematics, dynamics, Newtonian gravitation, special relativity (`lorentz_gamma`, `relativistic_energy`, `momentum`), electricity, waves, and a neutrino-oscillation probability. | `c`, `G` (CODATA 2022) and Earth mass/radius from `churn/2/data/physics-constants.json` and `churn/2/data/solar-system.json`. |
| [`chemistry.xml`](chemistry.xml) | chemistry | Both chemistry ops — `ratio(a,b)` and `similarity(a,b)` — over standard atomic weights (H, C, N, O, He). | IUPAC 2021 atomic weights in `churn/2/data/chemical-elements.txt`. |
| [`data-analytics.xml`](data-analytics.xml) | data-analytics | Every analytics op: `sum`, `mean`, `min`, `max`, `variance`, `stddev`, and `correlation` of the eight planets' mean distance vs. orbital period (≈ 0.988, Kepler's third law). | `churn/2/data/solar-system.json`. |
| [`social.xml`](social.xml) | data-analytics + chemistry | The social-science face: descriptive statistics over recent US labor-market rates, and population density as a `ratio`. | `churn/2/data/social-labor-us.json`, `churn/4/data/social-density-us.json` (BLS / Census). |
| [`all-disciplines.xml`](all-disciplines.xml) | all four | A single-document grand tour mixing math, physics, chemistry, and data-analytics in one project. | all of the above |

> The native dispatcher's executable disciplines are **math, physics,
> chemistry, and data-analytics**. The **social** domain is modeled with
> data-analytics (and a density `ratio`) over real published social statistics;
> every such figure is an **OBSERVED** value and any relation drawn between them
> is a **MODEL, not a causal claim** (ASSOCIATION ≠ CAUSATION — see the
> [Churn README](../../churn/README.md)).

### Running the XML examples

Build the runner once, then run an example (or all of them):

```sh
cd ../../api/bodi && make          # builds bodi-xml-runner
cd ../../example/science

make                               # run every XML example, print witnesses
make physics                       # run a single XML example
make check                         # run every XML example and diff vs *.expected.txt
```

Or invoke the runner directly:

```sh
../../api/bodi/bodi-xml-runner physics.xml
```

`make check-all` runs **both** the XML `check` and the `.sleela` `sleela-check`.

## XML vocabulary

Every project is a `<bodi version="1">` document with a
`<project … kind="science"/>` and a `<science>` block containing one or more
`<discipline name="…">` elements. Each `<discipline>` holds `<operation
name="…" sequence="…">` elements, and each operation takes its arguments as
`<arg value="…"/>` children (max 16). The vocabulary is declarative and
allow-listed: it cannot invoke shell commands or arbitrary native functions, it
only opens no sockets, and an unsupported operation or wrong arity is reported
as `status=rejected` rather than crashing the run.

## Evidence files

Each `*.expected.txt` (BODI witness stream) and `*.sleela.expected.txt` (SLeeLa
program output) records the exact, deterministic output for its example. They
are evidence artifacts; regenerate them with `make regen` (XML) or
`make sleela-regen` (SLeeLa) if the output format changes intentionally.
