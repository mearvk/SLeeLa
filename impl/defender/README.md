# SLeeLa Defender Integration

SLeeLa can provision the MEARVK administrative-defender source appropriate to the host OS. The SLVM-facing command for this capability is [`bin/OSsupport`](../../bin/OSsupport); the implementation-level interface remains `sleela defender`.

## Supported sources

- Windows: `https://github.com/mearvk/Windows.Admin.Defender`
- Linux: `https://github.com/mearvk/Linux.Admin.Defender`

The `sleela defender` command uses a fixed repository mapping; it does not accept an arbitrary download URL.

## Commands

```text
sleela defender detect
sleela defender fetch [directory]
sleela defender build [directory]
sleela defender install [directory]
sleela defender provision [directory]
```

`provision` performs fetch, build, and install in sequence.

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
