# SLeeLa Linux launcher — sample run

This is a captured, verbatim run of the Linux launcher
[`launchers/slbuild-linux.sh`](launchers/slbuild-linux.sh), which compiles the
native `slbuild` builder/installer binary and then runs it to build the SLeeLa
core products and install the main package.

## Environment

- **OS:** Linux (Amazon Linux 2023), x86_64
- **Toolchain:** `cc` (GCC 11.5.0), GNU Make 4.3, Python 3.11
- **Release:** SLeeLa 0.3.29-dev (supported source syntax 1.0 – 2.5)

## Command

```sh
build/native-builder/launchers/slbuild-linux.sh --plain --prefix /projects/sandbox/sleela-install
```

`--plain` disables the color/box glyphs so the capture is clean ASCII. Exit
status: **0**.

## Output

```text
== SLeeLa Linux launcher: compiling the slbuild binary ==
slbuild: building for Linux using cc
slbuild: built /projects/sandbox/SLeeLa/build/native-builder/bin/linux/slbuild
slbuild: smoke check OK (--help)
slbuild: run it with:  /projects/sandbox/SLeeLa/build/native-builder/bin/linux/slbuild --help
== SLeeLa Linux launcher: running the build + install ==

   +---------------------------------------------------------+
   |                                                         |
   |   S L e e L a    Builder / Installer                    |
   |   THE NOW. THE INGREDIENT.                              |
   |                                                         |
   |   release 0.3.29-dev    syntax 1.0 - 2.5    os Linux    |
   |                                                         |
   +---------------------------------------------------------+

  Builds the SLeeLa core products and installs the main package,
  using this host's native toolchain (Linux).

▐ Plan
------------------------------------------------------------------------
    - repository   /projects/sandbox/SLeeLa
    - host OS      Linux
    - products     sleela, sleelvac, nordshrift
    - install to   /projects/sandbox/sleela-install

▐ Building SLeeLa
------------------------------------------------------------------------
  building the core products; verbatim output captured to the build log
  [..] build sleela
  [ok] sleela built
  [..] build sleelvac
  [ok] sleelvac built
  [..] build nordshrift
  [ok] nordshrift built
  [ok] build complete
    summary  log lines 24
  verbatim log: /projects/sandbox/SLeeLa/build/native-builder/logs/build-20261028-195603.log

▐ Staging the main package
------------------------------------------------------------------------
  [..] copying binaries
    - sleela
    - sleelvac
    - nordshrift
  [..] staging the standard library (excluding servers, IDEs, Wi-Fi, religious/moral)
  [ok] library staged: 81 packages kept, 24 excluded
  [..] staging the integrity manifest and verifier
  [ok] manifest + verifier staged

▐ Installing
------------------------------------------------------------------------
  [ok] installed to /projects/sandbox/sleela-install

▐ Installed
------------------------------------------------------------------------
  [ok] SLeeLa 0.3.29-dev main package is installed.
  Binaries: /projects/sandbox/sleela-install/bin  (sleela · sleelvac · nordshrift)
  Try:      /projects/sandbox/sleela-install/bin/sleela version

▐ Record of Build
------------------------------------------------------------------------
  Be it recorded that the SLeeLa main package was built and installed
  in good order on this Linux host, as attested below.

    Product          SLeeLa 0.3.29-dev (syntax 1.0 - 2.5)
    Host OS          Linux
    Binaries         sleela, sleelvac, nordshrift
    Library          81 packages included · 24 excluded
    Excluded         servers · IDEs/editors · Wi-Fi · religious/moral/political
    Destination      /projects/sandbox/sleela-install
    Build window     2026-10-28 19:56:03 -> 2026-10-28 19:56:06
    Integrity        SHA-256 manifest honored (fail-closed)
    Verbatim log     /projects/sandbox/sleela-install/logs/build-20261028-195603.log

  So attested, this build complete and in order.
  THE NOW. THE INGREDIENT.
```

## Post-run verification

The installed runtime reports its version, confirming a working install:

```text
$ /projects/sandbox/sleela-install/bin/sleela version
SLeeLa™ Language/Runtime 0.3.29-dev (supported source syntax 1.0 – 2.5; native C/C++ Core VM)
```

- 3 binaries installed: `sleela`, `sleelvac`, `nordshrift`
- 81 standard-library packages included, 24 excluded (servers, IDEs/editors,
  Wi-Fi, religious/moral/political) — none of the excluded trees leaked
- Integrity manifest + verifier staged; the SHA-256 build gate was honored

> Paths in this capture (`/projects/sandbox/...`) are from the environment where
> the run was recorded. On a normal host the default prefix is
> `$HOME/.local/sleela`.
