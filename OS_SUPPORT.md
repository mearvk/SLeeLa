# SLeeLa OS Support and Native Executable Provisioning

This document records the OS-native executable support exposed by SLeeLa and the SLVM command surface used to provision it.

## 1. OSsupport command

The repository root `bin/` contains the command-line entry point:

```text
bin/OSsupport
```

`OSsupport` is the SLVM-facing command wrapper for the SLeeLa Defender integration. It delegates to the compiled `sleela` executable and presents the result of each operation as a clear success or failure status.

When invoked without arguments it presents these choices:

| Choice | Operation | Purpose |
|---:|---|---|
| 1 | `detect` | Detect the host operating system and select the fixed native support repository. |
| 2 | `fetch` | Download the selected OS support source. |
| 3 | `build` | Fetch when needed and compile the native support source. |
| 4 | `install` | Compile when required and install the resulting OS support. |
| 5 | `provision` | Perform fetch, build, and install in sequence. |
| 6 | quit | Leave the command without making changes. |

The same operations are directly runnable from a shell:

```sh
OSsupport detect
OSsupport status
OSsupport fetch [directory]
OSsupport build [directory]
OSsupport install [directory]
OSsupport provision [directory]
OSsupport help
```

After checkout, ensure the command has the local executable bit when the checkout or filesystem does not preserve it:

```sh
chmod +x bin/OSsupport
```

## 2. Native OS mapping

SLeeLa deliberately uses a fixed mapping rather than accepting arbitrary download URLs:

| Host OS | Native support repository |
|---|---|
| Windows | `mearvk/Windows.Admin.Defender` |
| Linux | `mearvk/Linux.Admin.Defender` |

This mapping is implemented in `impl/frontend/driver.cpp` and is also documented in `impl/defender/README.md`.

## 3. SLeeLa CLI

The underlying executable exposes the same provisioning model through:

```text
sleela defender detect
sleela defender fetch [directory]
sleela defender build [directory]
sleela defender install [directory]
sleela defender provision [directory]
```

`OSsupport` is therefore a stable SLVM command surface, while `sleela defender` remains the implementation-level interface.

## 4. Windows behavior

On Windows, SLeeLa uses `curl.exe` or `wget.exe` to retrieve the fixed repository archive and PowerShell `Expand-Archive` to extract it. The build step invokes the support repository's `build/build.ps1`.

A Windows build requires a valid WDK/Visual Studio project in the support repository. The build script is expected to stop if a required `.vcxproj` is absent. Installation uses the normal Windows driver package path (`pnputil.exe`) and remains subject to administrative privileges and applicable driver-signing requirements.

SLeeLa does not bypass execution policy, Secure Boot, UAC, Defender, driver signing, or other Windows security controls.

## 5. Linux behavior

On Linux, SLeeLa uses `curl` or `wget`, extracts the fixed repository archive with `unzip`, and builds the kernel support under `kernel/file-locker/` with its normal Makefile.

Installation uses the repository's normal `make install` path and therefore requires the privileges and kernel-module prerequisites normally required by the host Linux system.

## 6. Result reporting

Every `OSsupport` operation reports a terminal result:

```text
OSsupport result: SUCCESS (detect)
```

or, when the delegated operation fails:

```text
OSsupport result: FAILED (build, exit <code>)
```

The wrapper preserves the delegated exit status so scripts can use it as a normal command-line program.

## 7. Native executable meaning

"Native executable support" means that SLeeLa can identify the host platform and invoke the platform's native support build/install path. It does not mean that one portable shell script is itself a Windows PE or Linux ELF binary.

The compiled SLeeLa runtime remains `impl/build/sleela` unless it is installed into the SLVM `bin/` directory. `OSsupport` is the command-layer entry point that invokes that runtime.

## 8. Security boundary

Provisioning is intentionally administrative rather than a security-control bypass. A failed compiler, missing WDK, missing kernel headers, missing privileges, unsigned driver, Secure Boot restriction, or other platform prerequisite remains a visible failure.

The design goal is predictable native integration while preserving the operating system's normal security model.
