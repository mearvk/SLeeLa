# SleelaTerminal™ Version

**Current version: 1.0.0**

SleelaTerminal™ has reached its first formal release baseline. Version `1.0.0` establishes the initial canonical version for the `sleela-terminal` project and its graphical terminal front end.

## Version Components

SleelaTerminal uses a three-component version:

```text
MAJOR.MINOR.FORGOTTEN
1    .0    .0
```

### Major

The **Major** component identifies a substantial release generation or architectural compatibility boundary.

Current value: **1**

### Minor

The **Minor** component identifies incremental user-facing functionality that remains within the current major architecture.

Current value: **0**

### Forgotten

The **Forgotten** component is the project's third revision component. It identifies small corrections, maintenance changes, presentation refinements, and other changes that do not constitute a new minor feature generation.

Current value: **0**

## Canonical Version

```text
SleelaTerminal™ 1.0.0
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

`VERSION.md` is the initial canonical human-readable version record for SleelaTerminal. Other project files should reference or reproduce `1.0.0` consistently rather than independently inventing a release number.

When automated version propagation is introduced, it should use this document as the documented release baseline and establish a single machine-readable source without changing the meaning of the three components.

---

**SleelaTerminal™ — MEARVK LLC**
