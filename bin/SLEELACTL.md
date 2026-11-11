# sleelactl — the unified SLeeLa control plane

`sleelactl` is one command that controls every SLeeLa module family: the
daemons and servers, the HTTP and wireless protocols, the codec / compression /
video registries, the compiler and decompiler tools, and the availability of
the data systems.

```
sleelactl <verb> <module> [args]      control one module
sleelactl <verb> all                  fan a verb across every compatible module
sleelactl list [nature]               list modules (optionally by nature)
sleelactl status [module|all]         show running status
sleelactl natures                     explain the natures and their verbs
sleelactl help [verb|module]           help, or the verbs legal for a module
```

## Why the verbs differ by module

SLeeLa modules are genuinely different *kinds* of thing, so forcing one verb set
(`start/stop`) onto all of them would be dishonest — the repo's own
[`lib/core/SERVICE-STANDARD.md`](../lib/core/SERVICE-STANDARD.md) says
"a worse abstraction forced onto a subsystem is worse than a little
duplication." `sleelactl` therefore classifies each module by **nature** and
offers only the verbs that fit it. An unsupported verb is **refused** with the
correct verb set — `sleelactl` never fakes an action a module cannot perform.

<!-- natures:begin -->
| Nature | Legal verbs | What it is |
| --- | --- | --- |
| **daemon** | `start stop restart reload status enable disable` | long-lived process under the host service manager (systemd, launchd, Windows SCM) |
| **registry** | `list probe status` | closed handler set owned by a process-local Controller (SLPluginService) |
| **datasource** | `open close status available` | open/close/active data surface + pooling (SLDataSource) |
| **protocol** | `negotiate grade simulate validate info` | spec / stack / reference model |
| **tool** | `run build inspect info` | on-demand, one-shot toolchain component |
| **subject** | `run info` | native C/C++ subject library compiled into the runtime (import <name>;) |
| **library** | `info` | pure library; informational only |
<!-- natures:end -->

_Generated from the script's single `NATURE_TABLE` by `sleelactl natures --markdown`;
`tests/sleelactl_cli_test.sh` fails if this table drifts._

Mechanisms: **daemon** → the host service manager (systemd `unit=`, launchd
`launchd=`, Windows SCM `winsvc=`); **registry** → `list` reports the declared
handlers, `probe`/`status` run the reference pipeline (`demo=`) via `sleela run`;
**datasource** → reports the `dir=` surface; **protocol** → the module's `make`
targets and registered `sim=`; **tool** → `make -C dir=` + the `bin=` CLI, or a
`script=` driver; **subject** → its `demo=` via `sleela run`; **library** → info.

The verb vocabulary is drawn directly from the code:
`start/enable` from the systemd units (`server-edition/.../sleela-server.service`,
`antivirus/systemd/sleela-antivirus.service`);
`load/unload/canProcess` from `SLPluginService` and `lib/codecs`;
`open/close/active` from `SLDataSource`;
`negotiate/grade/simulate/validate` from the HTTP and CDMA protocol trees.

## Examples

```sh
sleelactl start server-edition      # start the server daemon (systemd / launchd)
sleelactl enable antivirus          # enable the antivirus coordinator at boot
sleelactl status all                # status of every controllable module (non-zero if any fails)
sleelactl --dry-run stop all        # preview stopping every daemon; changes nothing
sleelactl stop all --yes            # mutating daemon verbs with 'all' need --yes

sleelactl probe codecs              # run the codec registry reference pipeline
sleelactl list registry             # just the registry-nature modules
sleelactl list codecs               # the codec registry's declared handlers (ids 0..19)
sleelactl --json status all         # machine-readable status
sleelactl check-registry --strict   # validate the module registry
sleelactl where                     # resolved root, registry, runtime, service manager

sleelactl available database        # report the database data-source availability
sleelactl open ledger               # how to open the ledger data surface

sleelactl validate http-9.0         # validate the HTTP/9.0 protocol spec
sleelactl simulate cdma --std wcdma # run the CDMA simulator
sleelactl build decompiler          # build the Slecompiler CLI
sleelactl run decompiler <binary>   # invoke the built decompiler

sleelactl run math                  # exercise the native math subject (__NativeMath)
sleelactl info math                 # how the math subject is reached (import math;)

sleelactl info native-drive
sleelactl build native-drive main.c helper.c -o build/app
sleelactl build native-drive plugin.cpp --kind shared --require-sleela-abi -o build/plugin.so
sleelactl build native-drive hot.sleela --require-sleela-abi -o build/hot.so   # @native methods -> typed-ABI plugin
sleelactl run native-drive --capture -- build/app
sleelactl inspect native-drive xclass --mode info Example.xclass
sleelactl inspect native-drive service --name worker --executable /opt/worker --working-directory /var/lib/worker
```

The **subject** nature is for the native C/C++ subject libraries compiled into
the `sleela` runtime (`impl/subjects/<name>/`). They are not daemons, handler
registries, data connections, or specs — a program reaches them with
`import <name>;` and qualified calls (e.g. `math.sqrt(144.0)`), which the
frontend lowers onto the native `__Native<Subject>` methods. `sleelactl run
<subject>` executes that native backend by running the subject's reference demo
through `sleela run`. There is deliberately no `check` verb: `sleela check` does
not load the SHEET-registered native subjects, so `run` is the one faithful
execution verb. The first registered subject is `math`.

The `native-drive` tool builds multi-unit C/C++ executables and shared
libraries, compiles the `@native` methods of a `.sleela` file into a typed-ABI
v1 plugin (one entry point per method; `--kind shared` is the default for
`.sleela`), executes processes without an implicit shell, and routes XClass
operations through the production loader. `sleelactl` passes its resolved
`sleela` CLI to the driver, so a fresh `impl/build/sleela` works without PATH
setup. Use `sleelactl inspect native-drive service ...` to render or explicitly
install a hardened systemd unit.

The VM owns an authorized, bounded native-module lifecycle API with typed
scalar/UTF-8 dispatch and bytecode fallback, and a host may bind `@native`
methods explicitly (`slvm_bind_native_method()`). Parser-enforced process
annotations remain future compiler/runtime work. See
[Native Drive](../impl/native-drive/README.md) and
[`@native` dispatch](../impl/native-drive/AT-NATIVE-DISPATCH.md).

`sleelactl <verb> all` applies the verb to every module whose nature accepts it
(e.g. `sleelactl enable all --yes` enables the daemons; `sleelactl build all`
builds the tools). Mutating daemon verbs (`start stop restart reload enable
disable`) fanned out with `all` are refused without `--yes`; preview them with
`--dry-run`. Fan-out follows registry `after=` dependencies: `start`/`restart`/
`enable` bring dependencies up first, `stop`/`disable` tear dependents down
first.

## Options, help topics, and exit codes

```
sleelactl [--dry-run] [--yes|-y] [--json] <verb> <module|all> [args]
sleelactl list [nature|registry-module]     status [module|all]
sleelactl natures [--markdown]              help [verb|module|nature]
sleelactl check-registry [--strict]         keys [--markdown]
sleelactl where                             completion bash|zsh      version
```

- `--dry-run` prints the exact action (for daemons, the service-manager
  command, including `sudo` when it would be used) and changes nothing.
  `--yes` confirms an `all` fan-out of a mutating daemon verb. Both may also
  follow `all` (`sleelactl stop all --yes`).
- `--json` gives machine-readable output for `list`, `status`, `natures`,
  `keys`, and `where` (other commands reject it with exit 2). `status` entries carry
  `module`, `nature`, `state` (`ok`, `failed`, `unavailable`, `none`), `exit`.
- `help <verb>` lists the natures and modules that accept the verb;
  `help <nature>` lists its verbs and modules; an unknown topic exits 2.
- `where` (alias `doctor`) prints the resolved root, registry (and whether it
  came from beside the script, an installed layout, or `SLEELACTL_REGISTRY`),
  registry format, runtime and version, Python, and service manager.
- `completion bash|zsh` prints a completion script generated from the
  registry: `eval "$(sleelactl completion bash)"` (or `zsh`). `make -C install
  install` also writes both to `<prefix>/share/sleela/completions/`
  (`sleelactl.bash`, `sleelactl.zsh`) — source one from your shell profile.
- `keys [--markdown]` prints the registry key schema (see below).
- Diagnostics go to stderr, results to stdout. `SLEELACTL_VERBOSE=1` shows
  the full `sleela` stderr (verification preamble included); by default it is
  suppressed on success and the last diagnostic lines are printed on failure.

| Exit | Meaning |
| --- | --- |
| 0 | success |
| 1 | the operation ran and failed, or the module is not present in this install |
| 2 | usage error: unknown module/nature/option/topic, bad arguments, missing `--yes`, missing `SLEELACTL_REGISTRY` file, an unsafe registry entry, or an untrusted override asked to escalate with `sudo` |
| 3 | mechanism unavailable (no service manager or no mapping for it, runtime not built, simulator or CLI not built) |
| 64 | the verb is not legal for the module's nature |

`status all` returns non-zero when any controllable module fails or cannot be
checked. Protocol `validate` prefers a `validate` target, then `syntax`, then
`test`. Tool `run`/`inspect` execute only the CLI named by the registry's
`bin=` (searched in `build/`, `build/Release/`, `build/stage/bin/`, `cli/`,
`bin/`, or the tool directory) — never "the first executable found".

### Service managers

Daemon verbs go through an adapter selected by `SLEELACTL_SERVICE_MANAGER`
(`auto` by default: systemd if `systemctl` exists, launchd on macOS, the
Windows SCM if `sc.exe` exists, else `none`). Each daemon names its service per
manager in the registry: `unit=` (systemd), `launchd=` (launchd label),
`winsvc=` (Windows service). A daemon with no mapping for the active manager
exits 3 instead of guessing.

| Verb | systemd | launchd | Windows SCM |
| --- | --- | --- | --- |
| start | `systemctl --<scope> start <unit>` | `launchctl kickstart <domain>/<label>` | `sc.exe start <svc>` |
| stop | `systemctl --<scope> stop <unit>` | `launchctl kill SIGTERM <domain>/<label>` | `sc.exe stop <svc>` |
| restart | `systemctl --<scope> restart <unit>` | `launchctl kickstart -k <domain>/<label>` | stop, then start |
| reload | `systemctl --<scope> reload <unit>` | not supported (exit 3) | not supported (exit 3) |
| enable / disable | `systemctl --<scope> enable/disable <unit>` | `launchctl enable/disable <domain>/<label>` | `sc.exe config <svc> start= auto/disabled` |
| status | `systemctl --<scope> --no-pager status <unit>` | `launchctl print <domain>/<label>` | `sc.exe query <svc>` |

`SLEELACTL_SYSTEMCTL_SCOPE` (`system` default, or `user`) selects the systemd
bus and the launchd domain (`system` or `gui/<uid>`). Non-root system-scope
mutations go through `sudo`; `status` never does.

## The module registry

The registry ([`bin/sleelactl.d/registry.conf`](sleelactl.d/registry.conf)) is
the single source of truth: the dispatcher holds no module-specific knowledge.
Format 2 is versioned and keyed:

```
format=2
<name>  nature=<nature>  [key=value ...]  [summary="free text"]
```

<!-- keys:begin -->
| Nature | Key | Need | Kind | Meaning |
| --- | --- | --- | --- | --- |
| (all) | `summary=` | warn | text | short human description |
| daemon | `unit=` | one of svc | unit | systemd unit (*.service) |
| daemon | `launchd=` | one of svc | label | launchd label |
| daemon | `winsvc=` | one of svc | label | Windows service name |
| daemon | `after=` | opt | deps | modules this daemon is started after (comma list) |
| registry | `dir=` | req | path | package directory |
| registry | `demo=` | req | path | reference pipeline (.sleela) run by probe/status |
| registry | `handlers=` | warn | int | handler count; ids are 0..N-1 |
| datasource | `dir=` | req | path | package directory |
| protocol | `dir=` | req | path | module directory (Makefile, spec, docs) |
| protocol | `sim=` | opt | build | built simulator run by simulate |
| tool | `dir=` | one of src | path | tool directory built with make |
| tool | `script=` | one of src | path | Python driver that receives build/run/inspect |
| tool | `bin=` | opt | list | CLI names run by run/inspect (dir tools) |
| tool | `runsub=` | opt | list | words after run forwarded as driver subcommands |
| tool | `sub.*=` | opt | path | helper script for inspect <word> |
| tool | `doc=` | opt | path | documentation shown by info |
| subject | `demo=` | req | path | reference demo (.sleela) run by run |
| library | `dir=` | opt | path | package directory |
<!-- keys:end -->

_Generated from the script's single `KEY_TABLE` by `sleelactl keys --markdown`
(`check-registry` and the use-time path checks read the same table);
`tests/sleelactl_cli_test.sh` fails if this table drifts. "one of svc" means at
least one key of that group is required; `warn` keys are optional but reported
when missing; `path` values must exist under `--strict`, `build` values are
build outputs and are only safety-checked._

Paths are relative to the SLeeLa root and may not be absolute or contain `..`.
Legacy format-1 lines (`<name> <nature> <handle> <summary>`, no `format=`
line) are still read; `check-registry` flags them.

`sleelactl check-registry [--strict]` validates the file: parse errors,
duplicate names, unknown natures, keys not valid for a nature, missing required
keys, unsafe paths, malformed unit/label/service names, non-integer
`handlers=`, unknown or cyclic `after=` references, and (as warnings, errors
with `--strict`) source paths that do not exist. CI and `make -C install
install-check` run it with `--strict`. In an installed layout, paths of modules
that were not installed are summarised in one line (`SLEELACTL_VERBOSE=1` lists
them).

The same safety rules are enforced at use time: a module whose paths escape
the root or whose service name is malformed is refused (exit 2) before any
command runs. `SLEELACTL_REGISTRY=<path>` replaces the registry; a missing
file is an error, and a module defined by an override is never escalated with
`sudo` unless `SLEELACTL_TRUST_REGISTRY=1`.

## Ordered subject-module catalogue

This catalogue records the quantitative/science modules that a SLeeLa source
program can import, together with their implementation shape and API surface.
The order is dependency-aware: **Math is foundational**; the other quantitative
subjects that call its kernels require `import math;` as well.

Registered subjects: `math`, `physics`, `chemistry`, `astrophysics`,
`sociology`, `economics`, `inference`, and `financial` — each with a reference
demo verified to run, so `sleelactl run <subject>` and `sleelactl info
<subject>` work for all eight. The `economics` and `inference` demos live under
`impl/tests/subjects/` and `financial` under `impl/examples/`, so they exist
only in a source checkout. The `science` directory is an example grouping, not
a module name.

**Version and date policy.** The subject sources do not declare independent
module semantic versions. The module version is therefore **not separately
versioned**; use the SLeeLa repository revision when recording an exact build.
The source dates below are the latest source-change dates visible in the
repository history for the listed implementation files, not claims of original
invention or release dates. Example programs in this family declare
`#sleela 2.5`; that is a source-syntax level, not a module version. The compiler
tracks a broader source-syntax range independently.

### 1. `math` — foundational native mathematics

- **Import / qualified name:** `import math;` / `math.<function>(...)`
- **Implementation:** `impl/subjects/math/math_native.cpp`, dispatched by
  `impl/subjects/native/native_api.cpp` into `__NativeMath`.
- **Reference example:** `lib/ui/example/science/math.sleela`.
- **Latest source change:** 2026-10-09.
- **Module version:** not independently declared; repository revision applies.
- **Supported API:** `pi()`, `e()`, `tau()`, `abs(x)`, `sign(x)`,
  `min(a,b)`, `max(a,b)`, `clamp(x,lo,hi)`, `sqrt(x)`, `exp(x)`,
  `log(x)`, `log10(x)`, `trunc(x)`, `floor(x)`, `ceil(x)`,
  `pow(x,y)`, `sin(x)`, `cos(x)`, `tan(x)`, `hypot(a,b)`, and
  `fmod(a,b)`.
- **Definitions:** constants and pure numeric functions are synthesized as
  static methods on `__NativeMath`. This is the base dependency for the
  quantitative subject family.

### 2. `physics` — mechanics and physical models

- **Import / qualified name:** `import math;` then `import physics;` /
  `physics.<function>(...)`.
- **Implementation:** `impl/subjects/physics/physics_native.cpp`, dispatched
  by the shared native subject API into `__NativePhysics`.
- **Reference example:** `lib/ui/example/science/physics.sleela`.
- **Latest source change:** 2026-10-18.
- **Module version:** not independently declared; repository revision applies.
- **Supported API:** constants `C`, `H`, `HBAR`, `E_CHARGE`, `K_B`,
  `G`, `G0`, `N_A`, `R`; functions `velocity(dx,dt)`,
  `acceleration(dv,dt)`, `kinematic_position(x0,v0,a,t)`,
  `force(mass,accel)`, `kinetic_energy(mass,velocity)`,
  `gravitational_force(m1,m2,r)`, `escape_velocity(mass,radius)`,
  `lorentz_gamma(v)`, `relativistic_energy(mass,v)`, `momentum(mass,v)`,
  `ohms_voltage(current,resistance)`, `electric_power(voltage,current)`,
  `ideal_gas_pressure(n,R,T)`, `wave_frequency(velocity,wavelength)`,
  `angular_frequency(frequency)`, `wavenumber(wavelength)`, and
  `neutrino_oscillation_probability(theta,dm2,L,E)`.
- **Definitions:** pure formula methods and physical constants; depends on
  Math. Units and input conventions belong to the caller and must be supplied
  consistently.

### 3. `economics` — economic and time-value functions

- **Import / qualified name:** `import math;` then `import economics;` /
  `economics.<function>(...)`.
- **Implementation:** `impl/subjects/economics/economics_native.cpp`,
  dispatched by the shared native subject API into `__NativeEconomics`.
- **Latest source change:** 2026-10-18.
- **Module version:** not independently declared; repository revision applies.
- **Supported API:** `future_value(principal,rate,periods)`,
  `present_value(future,rate,periods)`,
  `annuity_present(payment,rate,periods)`,
  `annuity_future(payment,rate,periods)`,
  `elasticity(pct_quantity,pct_price)`,
  `real_rate(nominal,inflation)`,
  `fisher_nominal(real,inflation)`, `doubling_time(rate)`,
  `continuous_value(principal,rate,time)`, `profit(revenue,cost)`,
  `margin(profit,revenue)`, and `gdp_identity(C,I,G)`.
- **Definitions:** deterministic financial/economic formula helpers. Outputs
  are model calculations under supplied assumptions, not empirical forecasts.
  Depends on Math (`pow`, `log`, and `exp`).

### 4. `inference` — descriptive statistics and trend analysis

- **Import / qualified name:** `import math;` then `import inference;` /
  `inference.<function>(...)`.
- **Implementation:** `impl/subjects/inference/inference_native.cpp`,
  dispatched into `__NativeInference`.
- **Latest source change:** 2026-10-18.
- **Module version:** not independently declared; repository revision applies.
- **Supported API:** `mean`, `variance`, `stdev`, `slope`, `intercept`,
  `correlation`, `r_squared`, `zscore`, and the associated forecast,
  compound-growth, and classification helpers implemented in that source.
- **Definitions:** array-free statistical kernels. A SLeeLa caller accumulates
  count and running sums from its own data series, then passes those values to
  the functions. Results are descriptive/model calculations, not causal
  conclusions or official forecasts. Depends on Math.

### 5. `astrophysics` — astronomical formula models

- **Import / qualified name:** `import math;` then `import astrophysics;` /
  `astrophysics.<function>(...)`.
- **Implementation:** `impl/subjects/astrophysics/astrophysics_native.cpp`,
  registered with the shared native subject lowering.
- **Reference example:** `lib/ui/example/science/astrophysics.sleela`.
- **Latest source change:** 2026-10-18.
- **Module version:** not independently declared; repository revision applies.
- **Supported API:** `flux_from_luminosity(L,r)`,
  `redshift(observed,rest)`, `observed_wavelength(rest,z)`,
  `wien_peak(temperature)`, `kepler_period(a,mass)`, and
  `escape_velocity(mass,radius)`.
- **Definitions:** pure astronomical formula helpers for radiation,
  spectroscopy, orbital period, and escape velocity. Depends on Math.

### 6. `sociology` — population and social-statistical measures

- **Import / qualified name:** `import math;` then `import sociology;` /
  `sociology.<function>(...)`.
- **Implementation:** `impl/subjects/sociology/sociology_native.cpp`,
  registered with the shared native subject lowering.
- **Reference example:** `lib/ui/example/science/sociology.sleela`.
- **Latest source change:** 2026-10-18.
- **Module version:** not independently declared; repository revision applies.
- **Supported API:** `proportion(part,total)`,
  `rate(events,population,scale)`,
  `growth_rate(oldValue,newValue)`, and
  `transition_probability(transitions,origin)`.
- **Definitions:** arithmetic measures over caller-supplied observations.
  Observed source data must remain distinguishable from modeled relationships;
  association is not a causal claim. Depends on Math.

### 7. `chemistry` — evidence-weighted chemistry support

- **Import / qualified name:** `import math;` then `import chemistry;` /
  `chemistry.<function>(...)`.
- **Implementation:** `impl/subjects/chemistry/chemistry_api.cpp` and
  `impl/subjects/chemistry/chemistry.cpp`; uses its own frontend lowering,
  separate from `native_api.cpp`.
- **Latest source change:** 2026-10-09.
- **Module version:** not independently declared; repository revision applies.
- **Supported executable API:** `ratio(numerator,denominator)`,
  `similarity(a,b)`, `stochastic(seed,weight)`,
  `inference_level(observation,bond,valence,similarity,signal,symmetry)`,
  `confidence(observation,bond,valence,similarity,signal,symmetry)`, and
  `uncertainty(observation,bond,valence,similarity,signal,symmetry)`.
- **Definitions:** evidence-oriented support calculations; the bounded 0–24
  inference level is a model-support scale, not a truth claim or a substitute
  for measured chemical properties. Requires Math for its numerical kernels.

### 8. `financial` — financial valuation and algebra helpers

- **Import / qualified name:** `import financial;` /
  `financial.<function>(...)`.
- **Implementation:** `impl/subjects/finance/financial_api.cpp` and
  `impl/subjects/finance/financial.cpp`; uses its own frontend lowering,
  separate from the shared arithmetic-subject dispatcher.
- **Latest source change:** 2026-10-18.
- **Module version:** not independently declared; repository revision applies.
- **Supported executable API:** `future_value`, `present_value`,
  `annuity_present`, `annuity_future`, `npv4`, `bond_price`, `capm`,
  `wacc`, `determinant2`, `solve2x2_x`, `solve2x2_y`,
  `quadratic_discriminant`, `quadratic_root_plus`,
  `quadratic_root_minus`, and `ratio`. Parameter lists are defined by the
  function declarations in `financial_api.cpp`.
- **Definitions:** deterministic valuation and algebra kernels. Financial
  values require explicit currency identity and rates require an explicit
  period convention; outputs are calculations, not forecasts or advice.

### Import namespaces reserved but not yet backed by subject implementations

The shared native API currently recognizes the names `excel`, `json`,
`crypto`, and `net` in its module-name availability set, but this revision
does not synthesize corresponding `__NativeExcel`, `__NativeJson`,
`__NativeCrypto`, or `__NativeNet` classes in `lowerProgram`. Treat these
as reserved/recognized namespaces, **not as supported callable subject APIs**.
They are not subject entries in `sleelactl`'s registry.

### Subject verification and extension rules

The repository Makefile provides subject checks including
`test-math`, `test-physics`, `test-economics`, `test-inference`,
`test-chemistry`, `test-financial`, `test-astrophysics`, and
`test-sociology`. The October 18, 2026 subject-fix commit records the
quantitative assertion suite passing for Math (27), Physics (15), Economics
(12), Inference (15), Financial (15), and Chemistry (10), plus the
Astrophysics and Sociology smoke checks. Re-run the current tests before
treating that historical result as a fresh build verification:

```sh
make -C impl test-subjects
make -C impl test-astrophysics test-sociology
```

To expose another subject through `sleelactl`, add a `subject` row to
`bin/sleelactl.d/registry.conf` with its real reference-demo path, and ensure
that demo builds and runs with the current `sleela` binary. Do not add a
registry row merely because an import name is reserved or a native source file
exists; its `demo=` must identify a runnable reference demo. Then run
`sleelactl check-registry --strict`.


## Program and module lifecycle: load, combine, change, unload, stop

There are three distinct lifecycle layers. Choose the command that matches the
thing being controlled; these operations are not interchangeable.

| Goal | Native subject program | Handler registry | Long-running daemon |
| --- | --- | --- | --- |
| Load | Add `import <subject>;` to the program, then run it | `controller.load(id)` inside the owning SLeeLa program | `sleelactl start <daemon>` |
| Load several | Import several subjects in one source file | Several `controller.load(id)` calls in that program | Start each daemon (or `start all --yes`) |
| Change | Edit imports/calls and rerun; rebuild runtime if native implementation changed | Load/unload the required ids in the program | Use `restart` or `reload` where supported |
| Unload | End the program; subject code is part of the runtime, not dynamically unloaded | `controller.unload(id)` in the program | `sleelactl stop <daemon>` |
| Inspect | `sleelactl info <subject>`; `sleelactl run <subject>` | `sleelactl list <registry>`; `sleelactl probe <registry>` | `sleelactl status <daemon>` |
| Stop | Ctrl-C in foreground, or terminate the owning process | Stop the program using the registry; handler operations are process-local | `sleelactl stop <daemon>` |

### 1. Load and run one native subject

A subject is selected at source level. The import is compiled/lowered by the
SLeeLa frontend and its native implementation is already part of the runtime.

```sh
sleelactl list subject
sleelactl info math
sleelactl run math
```

The final command runs the registered Math reference program. For a program of
your own, put `import math;` near the top of the `.sleela` source, use
qualified calls such as `math.sqrt(144.0)`, and run that source:

```sh
sleela run ./my-math-program.sleela
```

All eight catalogue subjects are registered with runnable reference demos, so
`sleelactl run physics` (for example) works the same way.

### 2. Multi-load: combine subjects in one program

“Multi-load” for native subjects means importing multiple subject namespaces
into the same SLeeLa program. It does not mean issuing a multi-module
`sleelactl load` command. For example, a source file can declare:

```sleela
#sleela 2.5
import math;
import physics;
import economics;
import astrophysics;

// Calls are qualified by their imported namespace.
math.sqrt(144.0);
physics.force(12.0, 9.8);
economics.future_value(1000.0, 0.05, 3.0);
astrophysics.redshift(656.3, 650.0);
```

Run the combined program once:

```sh
sleela run ./combined-subjects.sleela
```

Add or remove import declarations and their calls to change the set of
subjects used by the program. Follow each subject's API and parameter
conventions in the catalogue above. Importing a subject does not start a
background service; it makes that subject's compiled API available to the
program.

### 3. Change or replace a loaded subject set

For native subjects, change the source file:
1. Add, remove, or edit the relevant `import <name>;` declarations.
2. Update qualified calls to match the APIs listed in the catalogue.
3. Run the source again with `sleela run <file.sleela>`.

If only the SLeeLa source changed, rerun the program; no native library
hot-swap is involved. If a native C/C++ subject implementation or its runtime
integration changed, rebuild the runtime before running the program:

```sh
make -C impl build/sleela
sleela run ./combined-subjects.sleela
```

There is no subject-level `change`, `reload`, or hot-replacement verb in
`sleelactl` today. A source edit affects the next run, not a program that is
already executing.

### 4. Unload and stop native subject programs

Native subject APIs currently have no independent `unload` command. They are
linked into the runtime process. To release them, stop the program that owns
that runtime:
- Foreground run: press `Ctrl-C` in the terminal.
- Background run: stop the process using its recorded PID or the process
  supervisor that launched it; do not guess a PID.
- When the process exits, its runtime and subject state are released by normal
  process teardown.

Do not use `sleelactl unload math` or `sleelactl stop math`: both are invalid
for a `subject`. The subject nature deliberately exposes only `run` and
`info`.

### 5. Registry load/unload is in-program API, not a CLI verb

For codec, compression, and video registries, `load`, `unload`, `enable`,
`disable`, and `canProcess` are calls on a package Controller that exists only
inside one running SLeeLa program; no process holds it between CLI calls. So
the registry nature's CLI verbs are just the ones that can be honest:

```sh
sleelactl list codecs       # declared handlers (handlers=20 -> ids 0..19), sources, pipeline
sleelactl probe codecs      # run the reference Controller pipeline
sleelactl status codecs     # same pipeline; exit status reflects its result
sleelactl load codecs 2     # exit 64: not a CLI verb; the error points at controller.load(id)
```

The reference pipeline exercises load/canProcess/decode/unload in its own
process. For durable or application-specific handler state, the Controller must
be owned by the running SLeeLa application. A resident Controller host (a
daemon that owns registries so `load`/`unload` could work from the CLI) is a
possible future addition, not current behavior.

### 6. Stop, restart, and status for daemons

Only daemon-nature modules are long-running services controlled by systemd.
For example:

```sh
sleelactl status server-edition
sleelactl stop server-edition
sleelactl start server-edition
sleelactl restart server-edition
sleelactl status server-edition
```

System-scope units may require sudo; user-scope units can be selected with
`SLEELACTL_SYSTEMCTL_SCOPE=user`. Use `sleelactl help server-edition` to
confirm the legal verbs for a particular registered module.

### 7. Quick reference and important limits

```sh
sleelactl list subject             # registered native subjects
sleelactl info math                # subject import/API entry point
sleelactl run math                 # run Math reference demo
sleelactl list registry            # registered handler registries
sleelactl probe codecs             # exercise the handler lifecycle flow
sleelactl status server-edition    # inspect daemon
sleelactl stop server-edition      # stop daemon
```

Current limitations:
- Native subjects are compiled into the runtime; no dynamic subject loader,
  per-subject unload, or live hot-swap is exposed.
- Registry load/unload/enable/disable/can are in-program Controller API, not
  CLI verbs (exit 64); `probe` runs the reference pipeline.
- `stop` is a daemon verb, not a generic way to stop arbitrary SLeeLa
  programs. Stop a standalone program through its terminal or process
  supervisor.

## Load and unload across every sleelactl module family

There is no universal `load` or `unload` operation for every module. The
dispatcher checks each module's registered **nature** and refuses verbs that
do not match that nature. Use this table as the authoritative lifecycle map:

| Module nature | Load / activate | Unload / deactivate | What happens today |
| --- | --- | --- | --- |
| `registry` (codecs, compression, video) | `Controller.load(id)` inside a SLeeLa program | `Controller.unload(id)` inside that program | `sleelactl probe <module>` runs the reference lifecycle pipeline. `sleelactl load/unload <module> <id>` validate the id and are refused (exit 3): the Controller is process-local, so a CLI call cannot leave a handler loaded. |
| `daemon` (server-edition, antivirus) | `sleelactl start <module>` (and optionally `enable`) | `sleelactl stop <module>` (and optionally `disable`) | Controls the registered systemd unit. Start/stop affect the running service; enable/disable affect boot activation. |
| `datasource` (database, ledger, connectors) | `sleelactl open <module>` | `sleelactl close <module>` | Reports/drives the package data-source surface. The actual connection and pool lifetime belongs to an `SLDataSource` instance owned by the running application, not a persistent connection held by the CLI. |
| `subject` (math and future registered subjects) | `import <subject>;` in SLeeLa source; then run the program | End the owning SLeeLa process | Subject code is compiled into the runtime. There is no dynamic subject load/unload command. `sleelactl run math` runs the registered Math reference demo; it does not leave Math loaded in a daemon. |
| `tool` (compiler, decompiler, debugger, native-drive) | `sleelactl build <tool>` when a build is needed; `run`/ `inspect` invokes it | The one-shot command exits; remove generated artifacts separately if desired | Tools are invoked on demand. They are not resident modules and do not have load/unload verbs. |
| `protocol` (HTTP, CDMA, Wi-Fi models) | No generic load; use the supported `validate` or `simulate` operation | No generic unload | Specs and reference models are inspected or executed through their own supported operations. |
| `library` | No runtime load command; use its API from a program where supported | No runtime unload command | Informational/library source surface only. |

Examples by family:

```sh
# Handler registry: process-local; list/probe from the CLI, load/unload in-program
sleelactl list codecs
sleelactl probe codecs

# Daemon: service lifecycle, not module memory loading
sleelactl start server-edition
sleelactl status server-edition
sleelactl stop server-edition

# Data source: application-owned connection lifecycle
sleelactl available database
sleelactl open database
sleelactl close database

# Native subject: source import, not sleelactl load/unload
sleelactl info math
sleelactl run math
```

The `all` fan-out is nature-aware, not a force-load switch. For example,
`sleelactl stop all --yes` applies only to modules whose nature permits `stop`
(the daemons), in reverse dependency order; `unload` is not a CLI verb for any
nature. It does not load or unload every subject, tool, protocol,
datasource, or library indiscriminately. Check `sleelactl natures` and
`sleelactl help <module>` before issuing lifecycle commands.

## `@native`, Native Drive, and acceleration: current behavior

These are related native-code facilities, but they are separate mechanisms.
Do not interpret the `@native` marker as a request that automatically loads
a shared library or switches a running VM method to native execution.

### A. `@native` and SLeeLa AOT

`@native` marks code for the supported ahead-of-time (AOT) analysis/emission
path. It is behaviour-neutral during ordinary VM execution: by itself it does
not authorize native code, load a module, launch a process, or accelerate a
method. Inspect and emit the supported subset explicitly:

```sh
sleela aot --list source.sleela
sleela aot --emit source.sleela
```

The current emitter is conservative and fails closed when it encounters
unsupported constructs. Its supported subset includes scalar literals and
variables, arithmetic/comparison, assignment, free calls, conditionals,
loops, return, and print. Arrays, object construction, qualified calls, and
other unsupported constructs are refused. Emission is currently whole-program,
not a standalone native entry point for each annotated method.

### B. Native Drive compilation and execution

Native Drive is the host-toolchain bridge. It can compile C/C++ translation
units with the configured system compiler, build executables or shared
libraries, compile the supported SLeeLa AOT output, and run programs with
explicit argv, bounded output capture, piping, and timeouts.

```sh
# Build a C or C++ executable
sleelactl build native-drive main.c helper.c -o build/app

# Build a shared library and check for the SLeeLa loader ABI exports
sleelactl build native-drive plugin.cpp --kind shared \\
  --require-sleela-abi -o build/plugin.so

# Build supported SLeeLa AOT source through the host C compiler
sleelactl build native-drive source.sleela --language sleela -o build/source-native

# Execute without an implicit shell
sleelactl run native-drive --capture -- build/app arg1 arg2
```

The resulting executable is a separately built artifact. Native Drive does
not silently replace the interpreter or claim a universal speedup; compare
correctness and performance for the specific workload and target platform.

### C. Explicit VM shared-library dispatch

The VM also exposes syntax-3.0 native built-ins for a host-authorized shared
library:

```sleela
// Illustrative calls; the host must install an authorization callback.
let handle = nativeOpen("build/plugin.so");
let result = nativeCall(handle, "calculate", 12, 3.5);
nativeClose(handle);
```

The host authorization callback must approve the candidate path. The handle is
VM-owned and generation-checked; calls cross a versioned typed ABI limited to
null, bool, int64, double, and length-delimited UTF-8 strings. VM arrays,
objects, pointers, and raw VM values are not passed through this ABI. A path
identifies a candidate library; it does not authorize execution. In-process
native code is trusted code, not a sandbox.

### D. What “native acceleration” means today

There are currently three concrete routes:

1. **Built-in native subjects** such as Math: the frontend lowers supported
   qualified calls such as `math.sqrt(...)` to the runtime's compiled
   `__NativeMath` implementation. This is the existing in-process subject
   route; it is not a dynamically loaded plugin.
2. **AOT + Native Drive**: explicitly emit the supported SLeeLa source subset,
   then compile the emitted C with the host compiler and run the resulting
   executable. This is a separate build/run path, not transparent VM
   acceleration.
3. **Authorized native shared libraries**: explicitly open/call/close a
   compatible library through the VM native ABI. The host controls
   authorization and lifetime.

The Pi large-integer worker path is separate again. The IPC frame and
canonical base-1e9 limb codecs exist, but the persistent worker, VM dispatch
integration, and end-to-end Pi benchmark are not yet complete. The current
Math subject route does not accept Pi limb arrays through an external worker.
Do not claim that `@native`, Native Drive, or the existing scalar plugin ABI
has already accelerated the 20,000-digit Pi workload.

For implementation detail and verification commands, see
[`@native` and VM dispatch](../pi-generator/AT-NATIVE.md) and
[Native Drive](../impl/native-drive/README.md).

## Service scope and honest degradation

For systemd, daemon units use the **system** manager by default. Root invokes `systemctl --system` directly; an unprivileged caller uses `sudo systemctl --system` and therefore needs sudo authorization. For a unit installed in the caller's user manager, set `SLEELACTL_SYSTEMCTL_SCOPE=user`; this invokes `systemctl --user` and never sudo. The registry's daemon handles must match the selected manager's unit scope.

- Invalid `SLEELACTL_SYSTEMCTL_SCOPE` values are rejected; only `system` and `user` are accepted.
- No supported service manager, or no `unit=`/`launchd=`/`winsvc=` mapping for the active one → daemon verbs say so and exit 3 (no fake start).
- A module defined by `SLEELACTL_REGISTRY` is never escalated with `sudo` unless `SLEELACTL_TRUST_REGISTRY=1` (exit 2).
- Missing `sudo` for system-scope control by a non-root caller is reported before attempting a command.
- SLeeLa runtime not built → registry verbs say so and point at
  `(cd impl && make build/sleela)` (no fake load/decode).
- Data systems → `sleelactl` reports *availability*; the actual open/close acts
  on an `SLDataSource` instance inside a running SLeeLa program, which the tool
  shows you how to write rather than pretending to hold a connection itself.

## Install

`sleelactl` and its registry are installed by the standard installer:

```sh
make -C install install          # installs sleela, sleelvac, nordshrift, AND sleelactl
```

The command lands in `<prefix>/bin/sleelactl`; its registry in
`<prefix>/lib/sleela/bin/sleelactl.d/registry.conf`.


## Native Drive service units

Native Drive's unit generator is exposed through the tool nature:

```sh
sleelactl inspect native-drive service --name worker --executable /opt/sleela/bin/worker --working-directory /opt/sleela
sleelactl inspect native-drive service --name worker --executable /opt/sleela/bin/worker --working-directory /opt/sleela --install --enable --start
sleelactl inspect native-drive service --name worker --status
sleelactl inspect native-drive service --name worker --rollback
```

The generator defaults to a user unit. Add `--system --user ACCOUNT` for an explicitly scoped system unit (run installation as root). Installation does not enable or start a unit unless the corresponding flags are supplied. Replacement is opt-in and preserves one rollback copy; failed installs attempt to restore the prior unit and undo enable/start side effects. Lifecycle operations are Linux/systemd-only; they do not claim to provide Windows Service Control Manager or macOS launchd integration.


## Regression tests

Run from the repository root (POSIX sh; fake `sleela`, `systemctl`,
`launchctl`, `sc.exe`, `sudo`, and caller UID — no real service is touched):

```sh
bin/sleelactl check-registry --strict        # the real registry is valid
sh tests/sleelactl_cli_test.sh               # registry-driven dispatch, check-registry and
                                             # use-time safety, registry verbs, exit codes,
                                             # diagnostics, help, --dry-run/--yes, after= order,
                                             # systemd/launchd/windows adapters, --json,
                                             # script tools, completion, docs-table drift
sh tests/sleelactl_systemctl_scope_test.sh   # systemd scope, sudo selection, untrusted
                                             # override refusal, legacy format-1 registry
```

CI: `.github/workflows/sleelactl.yml` runs all three on Ubuntu (where `/bin/sh`
is dash, plus an explicit `dash` pass) and macOS (BWK awk).
