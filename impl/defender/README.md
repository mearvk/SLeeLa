# SLeeLa Defender Integration

SLeeLa can provision the MEARVK administrative-defender source appropriate to the host OS. The SLVM-facing command for this capability is [`bin/OSsupport`](../../bin/OSsupport); the implementation-level interface remains `sleela defender`.

## Supported sources

- Windows: `https://github.com/mearvk/Windows.Admin.Defender`
- Linux: `https://github.com/mearvk/Linux.Admin.Defender`

The `sleela defender` command uses a fixed repository mapping; it does not accept an arbitrary download URL.

## Commands

```text
sleela defender detect
sleela defender fetch [directory]    --allow-defender --sha256 <hex>
sleela defender build [directory]    --allow-defender --sha256 <hex>
sleela defender install [directory]  --allow-defender --sha256 <hex> --allow-root
sleela defender provision [directory] --allow-defender --sha256 <hex> --allow-root
```

`provision` performs fetch, build, and install in sequence.

## Opt-in and integrity gates (required)

Because `fetch`/`build`/`install`/`provision` download a remote source tree and
can install a privileged (kernel) driver, they are **disabled by default** and
each guarded by an explicit gate. `detect` contacts nothing and needs no gate.

| Gate | Flag | Environment variable | Applies to |
|---|---|---|---|
| Opt in to network/build/install | `--allow-defender` | `SLEELA_DEFENDER_OPTIN=1` | fetch, build, install, provision |
| Expected archive SHA-256 | `--sha256 <hex>` | `SLEELA_DEFENDER_SHA256=<hex>` | fetch, build, install, provision |
| Authorize privileged install | `--allow-root` | `SLEELA_DEFENDER_ALLOW_ROOT=1` | install, provision |

Rules enforced by the compiler driver:

- Without `--allow-defender` (or the env opt-in), any action other than
  `detect` is refused before any network access.
- The downloaded archive is hashed and compared against the value supplied via
  `--sha256`/`SLEELA_DEFENDER_SHA256`. If the value is missing or does not
  match, the archive is deleted and the command fails closed. There is **no
  trust-on-first-use**.
- The privileged install step (`sudo make install` on Linux, `pnputil` on
  Windows) is **never** performed implicitly; it requires `--allow-root`
  (or `SLEELA_DEFENDER_ALLOW_ROOT=1`).

Example:

```sh
sleela defender provision \
  --allow-defender \
  --sha256 3f786850e387550fdab836ed7e6dc881de23001b... \
  --allow-root
```

## SLVM OSsupport command

From the repository `bin/` directory, the user-facing command is:

```text
OSsupport
OSsupport detect
OSsupport status
OSsupport fetch [directory]
OSsupport build [directory]
OSsupport install [directory]
OSsupport provision [directory]
OSsupport help
```

With no arguments, `OSsupport` presents numbered choices. With an argument, it is scriptable and preserves the exit status of the delegated `sleela defender` operation. See [`../../OS_SUPPORT.md`](../../OS_SUPPORT.md) for the complete command and native-provisioning specification.

If executable permissions were not preserved after checkout, enable the command locally with:

```sh
chmod +x bin/OSsupport
```

## Windows

The Windows path uses `curl.exe` or `wget.exe` to download the repository archive and then invokes the repository's `build/build.ps1`. The Windows project must contain a valid WDK Visual Studio project; the repository build script intentionally stops when that project is absent. Installation is performed only after a successful build and requires the appropriate administrative privileges and Windows driver-signing policy.

## Linux

The Linux path uses `curl` or `wget`, extracts the repository archive, then runs the kernel module Makefile under `kernel/file-locker/`. Installation invokes `make install`, which requires root privileges and installs the built module through the normal kernel module installation path.

## Result reporting

`OSsupport` reports an explicit terminal result and returns the underlying operation's exit status:

```text
OSsupport result: SUCCESS (provision)
```

or:

```text
OSsupport result: FAILED (build, exit <code>)
```

## Native executable boundary

OSsupport is a shell command wrapper, not itself a Windows PE or Linux ELF executable. It locates the compiled `sleela` runtime at `bin/sleela` when present and otherwise falls back to `impl/build/sleela`. The native OS support build and installation are delegated to the platform-specific repositories.

## Safety

This integration does not disable Secure Boot, Defender, UAC, driver signing, execution-policy protections, or other operating-system security controls. It downloads only the two fixed repositories above and leaves failures visible to the caller.

All network, build, and install actions are additionally disabled by default and require the explicit opt-in, payload-integrity, and privilege-authorization gates described under "Opt-in and integrity gates" above. Downloaded payloads are verified against a caller-supplied SHA-256 before use, and privilege elevation never happens implicitly.
