# Modium™ / Registry™ startup load of the 100,000,000-digit pi constant

This documents the startup load of the large pi catalog constant
([`pi.100000000.digits`](pi.100000000.digits), verified genuine pi in
[`VERIFIED.md`](VERIFIED.md)) by the **Modium** and **Registry** subsystems, the
transient red *Loading* banner, and the install wiring that places the file in
the OS's SLeeLa location.

*MEARVK LLC — 2026-11-08.*

---

## 1. What happens, and when

The large pi value is loaded by the **dedicated subsystem entry points only** —
`sleela registry` and `sleela modium` (and the installer's first run) — **not**
by every `sleela run` of a user program. On those commands, when the output is
an interactive terminal:

1. A red `Loading Modium™` (or `Loading Registry™`) line is printed.
2. That line is **erased in place** (`\r` + `\033[2K`) so it disappears, leaving
   no residue above the command's real output.
3. The subsystem resolves and loads the large pi constant and prints one calm
   line, e.g. `Modium™ catalog pi: loaded 100000000 fractional digits`.
4. The command's normal output follows.

### Strict TTY gate (no log pollution)

The entire banner + pi-report is emitted **only to an interactive terminal**
(`brandColorEnabled`, i.e. `isatty`). Under a pipe, a redirect, or test capture,
**nothing** is written — so machine-parsed registry/modium output is byte-for-byte
unchanged and no ANSI escape codes ever leak. This matches the existing
`brandSleela`/`brandSleelvac` discipline in `common_rails_banner.hpp`.

### Fail-soft

If the large file is not installed on this host, the subsystem prints
`catalog pi: deferred (large reference not installed)` and continues — a missing
large reference never blocks `sleela registry` / `sleela modium`.

---

## 2. Where the value lives (Registry locator, Modium resolution)

Per [`registry/REGISTRY-3.0-MODIUM.md`](../registry/REGISTRY-3.0-MODIUM.md) the
Registry stores **locator metadata, never the bytes**; Modium resolves the
locator to the actual file on the host. Accordingly:

- The **Registry** seeds two defaults under the `Math` key (both in the SLeeLa
  model [`lib/registry/SLRegistry.sleela`](../lib/registry/SLRegistry.sleela)
  `seedDefaults()` and the native seeder
  [`registry/sleela_registry.c`](../registry/sleela_registry.c)
  `sl_reg_seed_defaults`):
  - `piReferenceLargeDigits` = `100000000`
  - `piReferenceLargePath` = `pi-generator/pi.100000000.digits`
- **Modium** (the `sleela modium` startup path) resolves that path on the host
  and loads it.
- [`lib/math/SLPiReference.sleela`](../lib/math/SLPiReference.sleela) — the pi
  catalog static — now exposes the handle so the long-standing "deeper reference
  set … resolved by handle" prose is real:
  - `largePath()` → `"pi-generator/pi.100000000.digits"`
  - `largeDigits()` → `100000000`

### Host resolution order (`resolvePiLargePath`, `impl/frontend/driver.cpp`)

1. `$SLEELA_HOME/pi/pi.100000000.digits` (the installed runtime's own root)
2. `/usr/local/share/sleela/pi/pi.100000000.digits` (system install data dir)
3. `~/.local/share/sleela/pi/pi.100000000.digits` (user install data dir)
4. repo-relative `pi-generator/pi.100000000.digits` (upward walk from CWD)

The load is streaming (it never holds the ~95 MB in one string and does not
re-hash it — integrity is `VERIFIED.md`'s job); it confirms the leading `3.` and
counts the fractional digits.

---

## 3. Install wiring (Standard and Beautiful)

The file is part of **both** install families, landing at `<root>/pi/` so the
resolver above finds it.

| Install | File | What was added |
|---|---|---|
| **Standard** (main `make install` / `install-all`) | [`install/Makefile`](../install/Makefile) `do-install` | copies `pi-generator/pi.100000000.digits` (+ `VERIFIED.md`) to `$(DATA_DIR)/pi/` before the uninstall-tracking `record` step, so it is tracked and removed on uninstall |
| **Beautiful** (guided `make clean-install` / `make build`) | [`install/sleela-clean-install.sh`](../install/sleela-clean-install.sh) | stages the constant into `$STAGE_DIR/pi/` (always, independent of top-level-program selection), before the SHA-256 manifest regenerates, so it is covered by integrity verification; it then lands at `$install_root/pi/` = `$SLEELA_HOME/pi/` |

Both are guarded: if the source tree lacks the 100M file, the step prints a
"deferred" note and the install still succeeds.

---

## 4. Verify it yourself

```sh
# Build the source compiler (needs g++, X11/fontconfig/freetype):
cd impl && SLEELA_SHA256_MANIFEST="$(pwd)/../security/sha256-manifest.json" make build/sleela && cd ..

# Piped == byte-clean (no banner, no escape codes):
impl/build/sleela modium status | cat -v | grep -c 'Loading\|catalog pi'   # => 0

# Interactive (pseudo-TTY) == red banner flashes, disappears, pi reported:
script -qec 'impl/build/sleela modium status' /tmp/out.txt >/dev/null
grep -a 'catalog pi' /tmp/out.txt
#   Modium™ catalog pi: loaded 100000000 fractional digits

# Registry carries the locators:
impl/build/sleela registry get Math piReferenceLargePath    # => pi-generator/pi.100000000.digits
impl/build/sleela registry get Math piReferenceLargeDigits  # => 100000000
```
