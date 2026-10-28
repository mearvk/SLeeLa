---
inclusion: always
---

# Clean Install — the guided main-package installer

SLeeLa ships a calm, beautiful command-line installer at
**`install/sleela-clean-install.sh`**, run via **`make clean-install`** from the
repo root. It is the recommended way to put SLeeLa on a user's computer.

## What it installs (the main package)

- The three binaries: **`sleela`** (runtime), **`sleelvac`** (compiler),
  **`nordshrift`** (`.sst` transpiler) — built via the integrity-gated `make all`
  and copied from `impl/build/`.
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

## When editing

`install/*.sh` is not under the SHA-256 build gate (that gate covers `lib/**` and
`impl/`), so editing the installer does not require regenerating
`security/sha256-manifest.json`. It is tracked, so regenerate the repo digest
records if you change it: `bash tools/generate-all-sha256-md.sh` (and
`tools/generate-important-sha256-manifest.py` for the important-files manifest).
