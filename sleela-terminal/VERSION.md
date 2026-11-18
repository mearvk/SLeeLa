# SleelaTerminal™ Version

**Current binary version identity: 1.0121**

The historical SleelaTerminal feature-release line remains 1.1.0; the executable
identity is now 1.0121 under the repository-wide binary identity policy. The GUI
reports this through its compiled version constant, and the shell reports it via
`slsh --version`.

SleelaTerminal™ has advanced to its first incremental feature generation beyond
the `1.0.0` baseline. Feature release `1.1.0` records a round of user-facing
functionality on the GTK 4 / VTE graphical terminal front end — a living
title-bar throbber, recoloured window controls, the brand logo, a Settings
subframe, the `sleela$` default prompt, and the gradient title bar — together
with five shell runtime-bug fixes, all within the current `1.x` architecture.

## Version Components

SleelaTerminal uses a three-component version:

```text
MAJOR.MINOR.FORGOTTEN
1    .1    .0
```

### Major

The **Major** component identifies a substantial release generation or architectural compatibility boundary.

Current value: **1**

### Minor

The **Minor** component identifies incremental user-facing functionality that remains within the current major architecture.

Current value: **1**

### Forgotten

The **Forgotten** component is the project's third revision component. It identifies small corrections, maintenance changes, presentation refinements, and other changes that do not constitute a new minor feature generation.

Current value: **0**

## Canonical Version

```text
SleelaTerminal™ binary 1.0121 (feature line 1.1.0)
```

The version applies to the `sleela-terminal` project as a whole, including:

- `slsh`, the command shell;
- the M1–M5 shell layers;
- the arithmetic, lexer, parser, expansion, and executor components;
- M5 process and signal integration;
- the shell validation and hardening infrastructure; and
- `SleelaTerminal`, the GTK 4 / VTE graphical terminal front end.

## Release Baseline

Version `1.0.0` marks the transition from an evolving `0.x` development identity to a defined first-release baseline. It does not imply that every future capability is complete; it establishes the version from which subsequent compatible development can be measured.

The existing M1–M5 architecture, documented validation facilities, and graphical terminal integration form the basis of this first version.

## 1.1.0 Feature Generation

Version `1.1.0` is the first incremental feature generation on top of the
`1.0.0` baseline. It is a MINOR bump (new user-facing functionality within the
current `1.x` architecture), not a FORGOTTEN maintenance revision. It records:

- a living, left→right flowing title-bar throbber replacing the static bottom
  border;
- recoloured, lit window controls (close / minimize / maximize);
- the trimmed brand logo in the upper-left of the title bar;
- a refined title-bar Settings button that opens Settings as a subframe;
- the default shell prompt changed from `slsh$` to `sleela$`; and
- an elegant gradient title bar.

It also carries five shell runtime-bug fixes (REPL loop, braces, select,
process substitution, trap). The M1–M5 shell architecture is unchanged.

## Versioning Policy

Future versions follow this structure:

| Component | Purpose |
|---|---|
| `MAJOR` | Major architectural or compatibility generation |
| `MINOR` | Incremental feature generation |
| `FORGOTTEN` | Maintenance, correction, refinement, or small revision |

Examples:

```text
1.0.0  First formal SleelaTerminal release baseline
1.0.1  Small correction or maintenance revision
1.0.2  Additional correction or refinement
1.1.0  New incremental feature generation
2.0.0  Major architectural or compatibility generation
```

The third component is intentionally named **Forgotten** for SLeeLa project terminology. It occupies the conventional third numeric position while retaining the project's own name and meaning.

## Version Source

`VERSION.md` is the canonical human-readable record for both the historical feature-release line (`1.1.0`) and the current binary identity (`1.0121`). The GUI executable embeds `1.0121`; the shell exposes the same identity through `slsh --version`. Product feature version and binary artifact identity are separate fields.

Automated version propagation should read the binary identity from the machine-readable registry while preserving the meaning of the three-component feature-release line.

---

**SleelaTerminal™ — MEARVK LLC**
