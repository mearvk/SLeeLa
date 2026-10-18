# `example/science` — runnable science examples for every BODI discipline

These are hands-on, deterministic examples for the SLeeLa **BODI** XML science
runner. Each project exercises one (or, for the grand tour, all) of the
runner's executable science disciplines and emits one `BODI witness …
status=executed` line per operation. No network access is required, so every
run is reproducible, and each project ships a recorded expected-output evidence
file (`*.expected.txt`).

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

## Running

Build the runner once, then run an example (or all of them):

```sh
cd ../../api/bodi && make          # builds bodi-xml-runner
cd ../../example/science

make                               # run every example, print witnesses
make physics                       # run a single example
make check                         # run every example and diff vs *.expected.txt
```

Or invoke the runner directly:

```sh
../../api/bodi/bodi-xml-runner physics.xml
```

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

Each `*.expected.txt` records the exact witness stream for its project. They are
evidence artifacts; regenerate them with `make regen` if the runner's output
format changes intentionally.
