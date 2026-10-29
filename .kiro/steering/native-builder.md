---
inclusion: always
---

# Native builder/installer — `slbuild` (compiled front door)

`build/native-builder/` produces **`slbuild`**, a native per-OS binary that is
the compiled analog of the shell clean-install
(`install/sleela-clean-install.sh`, see `clean-install.md`). It does the same
real work — build the integrity-gated core products, stage the main package,
install, and print a *Record of Build* — but as a shippable, OS-aware
executable rather than a shell script. **Verified: it compiles cleanly and
runs (produce-only, dry-run, and full build+install all confirmed on Linux.)**

## Layout and what is tracked

| File | Role | Tracked? |
|------|------|----------|
| `slbuild.c` | Source of the native builder/installer binary | **yes** |
| `make-slbuild.sh` | OS-detecting compile script (`cc`→`gcc`→`clang`) | **yes** |
| `launchers/slbuild-linux.sh` | Linux launcher (compile + run) | **yes** |
| `launchers/slbuild-macos.sh` | macOS launcher | **yes** |
| `launchers/slbuild-windows10.ps1` | Windows 10+ (PowerShell 5+, MSYS2) | **yes** |
| `launchers/slbuild-windows7.ps1` | Windows 7+ (PowerShell 2.0-safe, TLS 1.2) | **yes** |
| `launchers/slbuild-windowsxp.cmd` | Windows XP+ (classic batch) | **yes** |
| `README.md`, `LINUX-RUN.md` | Docs + a captured verbatim Linux run | **yes** |
| `bin/<os>/slbuild` | The compiled binary | **no — build output** |
| `logs/build-*.log`, `.stage/` | Build logs / transient install staging | **no** |

The whole `build/` tree is gitignored via `**/build/` in the root
`.gitignore`; the 6 source files above are **force-tracked** (same pattern as
the sibling `build/*.sh` entry points). **Never commit `bin/`, `logs/`, or
`.stage/`** — regenerate the binary from source per platform instead. The 6
source files are also recorded in `security/important-sha256-manifest.json`
(the generated `bin/<os>/slbuild` is not).

## How to build and run (Linux, verified)

```sh
# Recommended: one-step launcher (compile the binary, then build + install)
build/native-builder/launchers/slbuild-linux.sh [slbuild options...]

# Produce ONLY the builder/installer binary and stop (prints its path):
build/native-builder/launchers/slbuild-linux.sh --produce-only
#   (also: SLBUILD_PRODUCE_ONLY=1, or -ProduceOnly on PowerShell launchers)

# Or compile the binary directly, then run it:
build/native-builder/make-slbuild.sh          # -> build/native-builder/bin/linux/slbuild
build/native-builder/bin/linux/slbuild --help
```

`make-slbuild.sh` detects the host (`uname -s`) and writes to an OS-specific
path: `bin/{linux,macos,bsd,windows,posix}/slbuild`. It compiles with
`-std=c11 -O2 -Wall -Wextra` (format/stringop-truncation warnings suppressed as
worst-case-theoretical on the bounded `PATH_MAX` buffers) and smoke-tests
`--help`. `slbuild.c` is pure libc + `system()`; no extra libs.

### `slbuild` options

| Option | Meaning |
|--------|---------|
| `--prefix DIR` | install root (default `$HOME/.local/sleela`) |
| `--build-only` | build the 3 core products; do not stage/install |
| `--dry-run` | print the plan only; build/install nothing |
| `--plain` / `--no-color` | ASCII only (also honors `NO_COLOR` / non-TTY) |
| `--repo DIR` | repo root (default: auto-detected; `$SLEELA_REPO` honored) |
| `-h`, `--help` | usage |

## Strategy / invariants to preserve when editing

- **It is the compiled twin of the shell clean-install.** Keep the two in
  lockstep: same 3 core products (`sleela`, `sleelvac`, `nordshrift` via
  `make -C impl build/<product>`), the **same `EXCLUDE_LIB` policy** (servers,
  IDEs/editors, Wi-Fi, religious/moral/political — do not remove exclusions),
  the integrity-gated build (`SLEELA_SHA256_MANIFEST` set, fail-closed), and the
  calm presentation (green/white/grey palette, box banner, `[✓]/[»]/[!]/[x]`
  glyphs paired with color, tagline **THE NOW. THE INGREDIENT.**, graceful
  degrade on `NO_COLOR`/non-TTY/`--plain`). If you change the policy in one,
  change it in both.
- **Summarized on screen, verbatim in a log.** Per-product progress lines on
  screen; the exact compiler/linker output goes to a timestamped
  `build/native-builder/logs/build-*.log` (copied to `<prefix>/logs/` on a
  successful install). Keep both.
- **Repo auto-detection** walks up from the real path of `argv[0]` looking for
  `impl/Makefile` (robust to `bin/<os>/` depth and to PATH invocation); honors
  `--repo` then `$SLEELA_REPO`. Keep it resilient.
- **Fail-closed verification after copy.** It re-checks that `<prefix>/bin/sleela`
  exists and is executable rather than trusting the copy's exit code (overlay/
  sandbox filesystems can report success without persisting). Keep this check.
- **Portability:** `slbuild.c` enables the POSIX/GNU surface under `-std=c11`
  (`realpath`, `setenv`, `popen`, `fileno`, `localtime_r`, `PATH_MAX`) and
  detects the host at compile time (`__linux__`/`__APPLE__`/`_WIN32`/`__FreeBSD__`)
  and reports it at runtime. The Windows launchers run `slbuild` inside MSYS2 /
  Git-Bash because it drives the build through `make` + a POSIX shell.

## When editing these files

`build/native-builder/*` is **not** under the `lib/**`+`impl/` SHA-256 build
gate, so editing it does **not** require regenerating
`security/sha256-manifest.json`. The 6 source files are tracked and recorded in
`security/important-sha256-manifest.json`, so after changing them regenerate the
digest records: `python3 tools/generate-important-sha256-manifest.py` and
`bash tools/generate-all-sha256-md.sh` (SHA256-DIGESTS.md). Rebuilding the
binary (`make-slbuild.sh`) touches only the gitignored `bin/<os>/` — nothing to
commit.
