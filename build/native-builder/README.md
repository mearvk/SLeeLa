# SLeeLa OS-specific builder/installer (`slbuild`)

This directory produces a **native binary** — `slbuild` — that is the compiled
front door to the SLeeLa main-package build and installation. It mirrors the
calm, beautiful experience of `install/sleela-clean-install.sh`, but as a real,
OS-aware executable you compile and run per platform.

## Files

| File | Role | Tracked? |
|------|------|----------|
| `slbuild.c` | Source of the native builder/installer binary | yes |
| `make-slbuild.sh` | OS-detecting compile script that builds `slbuild` | yes |
| `README.md` | This document | yes |
| `bin/<os>/slbuild` | The compiled binary (generated) | no (build output) |
| `logs/build-*.log` | Verbatim build logs (generated) | no (build output) |
| `.stage/` | Transient staging tree during install (generated) | no |

> The whole repo already ignores generated `build/` output via `**/build/` in
> the root `.gitignore`. The three source files above are force-tracked (the
> sibling `build/*.sh` entry points are tracked the same way). Do not commit
> `bin/`, `logs/`, or `.stage/`.

## Build the binary (per OS)

```sh
build/native-builder/make-slbuild.sh
```

It detects the host OS and C compiler (`cc` → `gcc` → `clang`) and writes the
binary to an OS-specific path:

| Host | Output |
|------|--------|
| Linux | `build/native-builder/bin/linux/slbuild` |
| macOS | `build/native-builder/bin/macos/slbuild` |
| *BSD  | `build/native-builder/bin/bsd/slbuild` |
| Windows (MSYS/Cygwin) | `build/native-builder/bin/windows/slbuild` |
| other POSIX | `build/native-builder/bin/posix/slbuild` |

## Run the build + install

```sh
build/native-builder/bin/linux/slbuild [options]
```

| Option | Meaning |
|--------|---------|
| `--prefix DIR` | install root (default: `$HOME/.local/sleela`) |
| `--build-only` | build the three core products, do not stage/install |
| `--dry-run` | print the plan only; do not build or install |
| `--plain` / `--no-color` | ASCII only (also honors `NO_COLOR` / non-TTY) |
| `--repo DIR` | repository root (default: auto-detected; `$SLEELA_REPO` honored) |
| `-h`, `--help` | usage |

## What it does

1. **Build** the integrity-gated core products via the real `impl` build —
   `make -C impl build/sleela build/sleelvac build/nordshrift` with
   `SLEELA_SHA256_MANIFEST` set (fail-closed SHA-256 gate). The on-screen view
   is a calm, summarized progress report; the **exact, verbatim** compiler and
   linker output is captured to `build/native-builder/logs/build-<stamp>.log`.
2. **Stage** the main package — the standard library minus the excluded trees
   (servers, IDEs/editors, Wi-Fi, religious/moral/political), identical to the
   shell clean-install policy.
3. **Install** to the chosen prefix, copying binaries, the staged library, the
   integrity manifest + verifier, `SHEET.sheet`, and a generated
   `config/startup.conf` (which records the detected host OS). The build log is
   preserved under `<prefix>/logs/`.
4. **Attest** with a formal *Record of Build*.

After the copy it verifies the install actually materialized (`<prefix>/bin/sleela`
must exist and be executable) rather than trusting the copy's exit code alone.

## OS awareness

`slbuild` detects the host at compile time (`__linux__`, `__APPLE__`, `_WIN32`,
`__FreeBSD__`) and at runtime reports it in the banner, the `Record of Build`,
and `startup.conf`. Paths and defaults adapt to the host.
