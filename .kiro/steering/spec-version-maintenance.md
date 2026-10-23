---
inclusion: always
---

# Spec Number maintenance (keep documents and binaries in lockstep)

When the **Spec Number** of the language changes — the Sleela source-syntax
spec (`#sleela MAJOR.MINOR`) and/or the Nordshrift `.sst` spec (NS-SST-0001) —
we update the appropriate documents **and** the usage/version output of the
binary executables in the same change. A spec bump is not complete until all of
the places below agree on the new number.

Keep this distinct from the independent **tool/release** versions (Sleelvac™
`kReleaseVersion` in `impl/frontend/driver.cpp`, the Nordshrift tool version in
`impl/nordshrift/nordshrift.cpp`). Those evolve on their own and are **not**
changed by a Spec Number bump. `SLEELA.syntax` states this explicitly: the
syntax version and the compiler/tool version must not be conflated.

## When the Sleela syntax Spec Number changes

1. **Single source of truth** — `impl/frontend/version.h`:
   `maxSupportedSyntax()` (and `minSupportedSyntax()` if the floor moves). The
   `sleela`/`sleelvac` version banners and the usage headers derive the range
   from these, so they update automatically once the binaries are rebuilt.
2. **Normative spec** — `SLEELA.syntax` header: the `Syntax version:` line, the
   compiler version-gate range, the `Status:` range, and the `#sleela X.Y`
   example in the versioning note.
3. **Prose docs** — `README.md` ("documents syntax **X.Y**") and
   `wiki/Language-Reference.md` (the "Supported `.sleela` syntax versions" line).
4. **Binary usage/version output** — confirm `sleela version`, `sleela` (usage
   header), and `sleelvac` (usage header) all report the new range. These read
   from `version.h`; a rebuild is what makes them current.
5. **Version tests** — `impl/tests/version/run_version_tests.sh` and its
   fixtures. The "minor too new" fixture (`minor_ahead.sleela`) must sit one
   minor **above** the new ceiling so it is still rejected.

## When the Nordshrift `.sst` Spec Number changes

1. **Normative spec** — `SST-2.0.model` (NS-SST-0001) header `Revision:` line
   (and `SST.model` where the chain is described).
2. **Binary usage/version output** — `impl/nordshrift/nordshrift.cpp`: the
   `kVersion` string (it names the NS-SST-0001 spec level) and the `usage()`
   header line.
3. **Prose docs** — `ORGANIZATION.md` source-of-truth table and
   `wiki/Nordshrift-Sheet-Reference.md` if they cite the spec revision.

## Verify

After editing, regenerate the SHA-256 manifest and rebuild (the `impl/` build is
integrity-gated), then confirm the binaries report the new number:

```sh
python3 tools/generate-sha256-manifest.py --root . --output security/sha256-manifest.json
cd impl && SLEELA_SHA256_MANIFEST="$(pwd)/../security/sha256-manifest.json" make build/sleela build/nordshrift
./build/sleela version          # must show the new syntax range
./build/sleela                  # usage header must show the new syntax range
./build/nordshrift version      # must show the new NS-SST-0001 spec level
bash tests/version/run_version_tests.sh ./build/sleela   # must PASS
```
