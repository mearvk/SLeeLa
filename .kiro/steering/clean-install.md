---
inclusion: always
---

# Clean Install — the guided main-package installer

SLeeLa ships a calm, beautiful command-line installer at
**`install/sleela-clean-install.sh`**, run via **`make clean-install`** from the
repo root. It is the recommended way to put SLeeLa on a user's computer.

## What it installs (the main package)

- The three binaries: **`sleela`** (runtime), **`sleelvac`** (compiler),
  **`nordshrift`** (`.sst` transpiler) — built via the integrity-gated **core**
  target `make -C impl build/sleela build/sleelvac build/nordshrift` and copied
  from `impl/build/`. It deliberately does **not** run the whole-repo `make all`:
  that also builds the excluded subsystems (servers, IDE bridges, the java28 JNI
  bridge, premium VMs) and pulls in heavy external toolchains (e.g. a JDK for
  `jni.h`), which would make a core install fragile and contradict "main package
  only". Building just the core keeps the install fast and dependency-light.
- The **standard library**, **protocols**, and **support modules** — the full
  `lib/` tree *minus* the excluded trees below.
- The SHA-256 integrity manifest (`security/sha256-manifest.json`) and the
  verifier (`tools/verify-before-execution.py`), plus `SHEET.sheet` and a
  generated `config/startup.conf`.

## What it NEVER installs (excluded by design)

The installer stages `lib/` with an explicit exclusion set (`EXCLUDE_LIB` in the
script). Do not remove these exclusions; they are the product policy:

- **Servers:** `http`, `http-1.0`…`http-9.0`, `http-servers`, `server-edition`,
  `web-server-technology`.
- **IDEs / editors:** `ide`, `ide-workbench`, `linux-editors`.
- **Wi-Fi:** `wifi`.
- **Religious / moral / political systems:** `moral-capacitor`, `philosophers`,
  `manifesto`, `politico`, `international-criminal-court`, `psychiatry`,
  `citizen`.

Caveat (do not "fix" it): the HTTP server objects are statically linked **inside**
the `sleela` binary upstream. "No servers" means none of the server/IDE/wifi/
religious-moral *library source trees* are shipped into the installed `lib/`; the
core binary itself is unchanged.

## Experience

1. **One primary decision first:** "Install the SLeeLa main package now?" — then
   a scope choice (user-local `$HOME/.local/sleela`, recommended; or system-wide
   `/usr/local`, which may `sudo`).
2. **On success, optional enhancement modules** are offered from a curated,
   safe, non-excluded set (`audio`, `video`, `signal-processing`, `codecs`,
   `compression`, `gui`, `ui`, `database`, `connector`) — selected by number.

## Build output: summarized on screen, verbatim in a log

The on-screen build is a **calm, summarized** progress report — one step per core
product (`sleela`, `sleelvac`, `nordshrift`), then a one-line summary
(`compiled N units · linked N products · warnings N · log lines N`).

The **exact, verbatim** compiler/linker messages are captured to a persistent,
timestamped log for debugging: `install/logs/build-YYYYMMDD-HHMMSS.log` (the
`*.log` gitignore keeps it out of commits), with a header recording the start
time, the exact `make` command, and the manifest path. On a **successful**
install the log is copied to `<install_root>/logs/`; on **failure** the exact
error tail is shown on screen and the full log is retained and cited by path.
Keep both: the summary is the clean dressing; the log is the debugging record.

## The Record of Build (formal attestation)

The installer closes a successful run with a **Record of Build** — a plainly
worded, orderly attestation suitable for a State Captain / Attorney General:
Product + syntax range, Binaries, Library (included · excluded counts), the
Excluded categories, Scope, Destination, the Build window (start → end),
Integrity ("SHA-256 manifest honored, fail-closed"), and the Verbatim log path,
signed off with "So attested, this build complete and in order." Preserve this
dignified, factual tone; it asserts only what the build actually did.

## Presentation (keep this discipline)

- Brand palette: glimmering green (approx `#00D68F`, 256-color `38;5;43`) on a
  near-black feel, white wordmark, grey labels; box-drawing banner; semantic
  glyphs `[✓] [»] [!] [x]` always paired with color so meaning survives without
  it. Tagline: **THE NOW. THE INGREDIENT.**
- Styling **degrades gracefully**: it turns off automatically when stdout is not
  a TTY, when `NO_COLOR` is set, or with `--plain`, falling back to ASCII box
  chars and `[ok]`-style tags.
- Interactive-prompt output goes to **stderr** so `ask_yn`/`ask_value` results
  captured via `$(...)` are the answer only — keep that when editing.

## Relationship to the other installers

`install/quick-safe-install.sh` (and the macOS/Windows variants) remain the
full-featured installer with the server/HTTP/VM-edition prompts. `clean-install`
is the curated, main-package-only path for users who want a minimal, beautiful
experience. Reuse the `do-install` model (binary set + lib staging + manifest/
verifier) but never re-introduce the server/IDE/wifi/moral trees into it.

**Compiled twin:** `build/native-builder/slbuild` is the native per-OS binary
form of this same clean-install flow (same core products, same `EXCLUDE_LIB`
policy, same integrity gate, same presentation + Record of Build). See
`native-builder.md`. Keep the two in lockstep — a policy change in one belongs
in both.

## When editing

`install/*.sh` is not under the SHA-256 build gate (that gate covers `lib/**` and
`impl/`), so editing the installer does not require regenerating
`security/sha256-manifest.json`. It is tracked, so regenerate the repo digest
records if you change it: `bash tools/generate-all-sha256-md.sh` (and
`tools/generate-important-sha256-manifest.py` for the important-files manifest).
