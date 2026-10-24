---
inclusion: always
---

# The `bin/` profile (staging for SLeeLa's runnable products)

`bin/` is more than the launcher scripts. The working model — recorded in full
in [`bin/SUPPOSITIONS.md`](../../bin/SUPPOSITIONS.md) — is that **`bin/` is the
in-tree collection/staging point for SLeeLa's runnable products**:

1. the launcher/entry **scripts** (`bin/SLeeLa`, `bin/OSsupport`,
   `bin/install-modules`) — shipped;
2. the **paced iterative module installer** (`bin/Makefile.modules` +
   `bin/install-modules.sh`) that walks every module's `install`/`all` over a
   ~10-20 minute window; and
3. the **compiled C/C++ executables** produced when SLeeLa is built — whether an
   individual module is compiled or the whole main program is built.

This is **in addition to** (never instead of) the normal binary-install
locations: user-local (`~/.local/bin`), system (`/usr/local/bin`, `/usr/bin`),
and other profile `bin` paths. `bin/` is the build/staging profile; those are
the deployment targets. A product can live in both.

## Rules of thumb

- Only **final linked executables** stage into `bin/`. Objects (`.o`) and static
  archives (`.a`) do not; libraries/headers go to `lib/`/`include/` or the
  pkg-config path (see the Ballum install pattern).
- The launchers already **resolve** a `sleela` binary next to the script
  (`bin/sleela`) before `impl/build/sleela`, so staging compiled output into
  `bin/` is the already-supported mechanism — see `bin/SLeeLa` / `bin/OSsupport`.
- The **OS Defender** (`bin/OSsupport` → `sleela defender`) is SLeeLa's
  OS-centric native-support layer: it fetches/builds/installs a privileged C/C++
  backend per host OS (signed WDK driver on Windows, kernel module on Linux;
  macOS is detect-only). It provisions native support; it does not bypass the
  OS's own security controls.
- Compiled products that touch the display/OS link the OS-centric libraries at
  build time (Linux `-lX11 -lXrandr`; macOS frameworks; Windows `-lgdi32
  -luser32`); modules record these in their installed `.pc` `Libs.private`.

## When adding a binary to the picture

Update [`bin/SUPPOSITIONS.md`](../../bin/SUPPOSITIONS.md): add the supposed
`bin/` name, how it is built (which Makefile/target), and mark it **(shipped)**
or **(supposed)**. Keep that file honest — it is a working model, not a
contract; the Makefiles and `bin/README.md` remain authoritative.
