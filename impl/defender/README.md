# Sleela Defender Integration

Sleela can provision the MEARVK administrative-defender source appropriate to the host OS.

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

### Windows

The Windows path uses `curl.exe` or `wget.exe` to download the repository archive and then invokes the repository's `build/build.ps1`. The Windows project must contain a valid WDK Visual Studio project; the repository build script intentionally stops when that project is absent. Installation is performed only after a successful build and requires the appropriate administrative privileges and Windows driver-signing policy.

### Linux

The Linux path uses `curl` or `wget`, extracts the repository archive, then runs the kernel module Makefile under `kernel/file-locker/`. Installation invokes `make install`, which requires root privileges and installs the built module through the normal kernel module installation path.

## Safety

This integration does not disable Secure Boot, Defender, UAC, driver signing, or other operating-system security controls. It downloads only the two fixed repositories above and leaves failures visible to the caller.
