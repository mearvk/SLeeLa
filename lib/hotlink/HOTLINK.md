# `hotlink` — the hot-link matrix and its backing model

Which **types of class** may join a running system, how they arrive, who may
bring them, and what they may be **connected** to. The matrix is declared in
XML ([`hotlink-matrix.xml`](hotlink-matrix.xml)) against an XML Schema
([`hotlink-matrix.xsd`](hotlink-matrix.xsd)), and enforced by the SLeeLa model
in this folder. A connection that the matrix does not declare is refused.

```
tools/validate_hotlink_matrix.py          # schema + cross-field rules + drift check
sleela run lib/hotlink/hotlink-demo.sleela  # 102 self-checks of the backing model
```

## Reading the schema

Each `<classType>` answers five questions:

| Attribute | Question | Values |
|---|---|---|
| `codeArrival` | How does the **code** arrive? | `linked` (compiled in, linked at start) · `native` (host-authorized plugin) |
| `instanceArrival` | May an **instance** join while running? | `hot` · `cold` (only from program start) |
| `contract` | How does it talk to the host? | `port` (observer mailbox) · `compartment` (vertical-integration tier) · `native-abi` |
| `minTrust` / `maxInstances` | Who may bring it, and how many? | trust 0 anonymous, 1 worker, 2 executive · 1..64 |
| `gates` | Which safeguards guard it? | `audit trust-floor instance-cap fanout-cap cycle-free quarantine hop-limit depth-cap revision-check authorizer digest-pin` |

plus `status` (`verified` = exercised by a passing self-check, `declared` =
enforced but not yet exercised end to end, `unverified` = depends on something
the committed toolchain lacks), `requiresAuthorizer`, `digestPolicy`
(`none` · `host-pinned`) and, for compartments, `kindFirst`/`kindLast`, the
`SLIntegrationKind` ordinals the type realises.

Each `<link>` declares one permitted connection: `from` → `to`, a `mode`
(`feed` moves value, `subscribe` receives events, `contain` nests), and a
`maxFanout`.

## The standard matrix

| Class type | Code | Instance | Contract | Trust | Max | Kinds | Status |
|---|---|---|---|---|---|---|---|
| `observer.responder` | linked | hot | port | 1 | 14 | — | verified |
| `vi.nested-model` | linked | hot | compartment | 2 | 4 | 0 | verified |
| `vi.onion` | linked | hot | compartment | 1 | 8 | 1 | verified |
| `vi.engineering-bay` | linked | hot | compartment | 1 | 8 | 2 | verified |
| `vi.trial` | linked | hot | compartment | 1 | 8 | 3 | verified |
| `vi.newton-tray` | linked | hot | compartment | 1 | 8 | 4 | verified |
| `vi.plain` | linked | hot | compartment | 1 | 16 | 5–16 | verified |
| `native.extension` | **native** | hot | native-abi | 2 | 2 | — | **unverified** |
| `extends.module` | linked | **cold** | port | 1 | 14 | — | declared |

### How the types connect

```
native.extension ──feed──▶ observer.responder ──feed──▶ vi.plain
                                              ├─feed──▶ vi.onion
                                              ├─feed──▶ vi.engineering-bay ──feed──▶ vi.trial ──feed──▶ vi.newton-tray
                                              ├─feed──▶ vi.trial
                                              └─feed──▶ vi.newton-tray

vi.nested-model ──contain──▶ vi.nested-model (self, different instances)
                ├─contain──▶ vi.plain
                └─contain──▶ vi.engineering-bay

extends.module   (cold: no links; refused while running)
```

## The backing model

| Class | Role |
|---|---|
| `SLHotLinkRow` | One matrix row (data). |
| `SLHotLinkMatrix` | Rows and links, validated as added; `standard()` seeds the XML's matrix and **seals** it. |
| `SLHotLinker` | The gate for instances (`attach`, `attachModule`, `detach`) and connections (`linkPair`, `unlinkPair`). |
| `SLHotLinkIntegration` | Grows a vertical-integration stack through the linker (`expand`) and moves value only along approved connections (`deposit`, `feed`). |

### What the linker enforces (fail closed, every refusal audited)

* **Sealed matrix only.** An unsealed matrix is refused; rows and links cannot be added after sealing.
* **Dry run:** `preflight(type, name, trust, digest)` returns 0 (would attach), 1 (refused for a reason that may pass later) or 2 (refused for good) and changes nothing; `attach` runs the same checks, so they cannot disagree.
* **Instances:** known type · hot type · not `unverified` unless the host permits it · trust at or above the floor · per-type cap · unique, non-reserved name (names are never reused, 64 per linker) · for observer modules, the port name equals the instance name and the observer must admit the port, otherwise the attach is rolled back.
* **Native code:** the host must have granted the authorizer **and** pinned the expected digest (once, immutable), and the presented digest must equal it. Revoking the authorizer detaches every native instance.
* **Connections:** the type pair must be declared · both ends linked · no self-link · no duplicate · fan-out under the limit · no instance-level cycle.
* **Value:** `deposit` needs a live connection; `feed` needs a live **`feed`**-mode connection between **adjacent** tiers. A `contain` link moves no value.
* **Listener lifecycle:** `pauseModule`, `resumeModule`, `alterModule`, `replaceModule` and `reinstateModule` need a linked observer-module instance and the trust floor of its type, then delegate to the observer (see `lib/observer/OBSERVER.md`). A hot-swap keeps the instance name, type and links.
* **Detach:** drops the instance's links and retires an observer module's port.

### Matrix rules enforced when a row or link is added

Vocabulary and ranges; a `native` row must be `native-abi`, require the
authorizer and be `host-pinned`; compartment rows need a kind range that no
other row covers; links join hot types only; no type-level cycle; a self-link
must be `contain`.

## Honest limits

* **The linker approves; it does not load.** Nothing here opens a native
  library. The host calls `nativeOpen` itself, after `attach` approves.
* **`native.extension` is `unverified`.** The committed `working-binaries/sleela`
  does not recognise `nativeOpen` (the source tree does); no compiler was built
  for this work. The row exists so the gates are in place before the first
  plugin; the demo exercises the gates, not a plugin.
* **Digests are compared for equality.** SLeeLa `String` has no length or
  character access, so the model cannot check that a digest is well formed.
* **Class code is still linked at start.** "Hot" means the *instance* joins a
  running system; a type whose code did not exist at launch needs a re-link
  (or a native plugin).
* **`SLGuiDom`/`SLGuiApp` do not link** under the current binary (see
  `lib/observer/OBSERVER.md`), so nothing here depends on them.

## Keeping XML and code together

`SLHotLinkMatrix.standard()` is the executable copy of `hotlink-matrix.xml`.
`tools/validate_hotlink_matrix.py` validates the XML against the schema
(`xmllint`, if installed), applies the cross-field rules the schema cannot
express, and fails if the rows or links differ from `standard()`.

## Authoring notes

`connect` and `sealed` are reserved in SLeeLa 3.0 (a Munction built-in and a
Java-style modifier), so the model uses `linkPair` and `frozen`.
