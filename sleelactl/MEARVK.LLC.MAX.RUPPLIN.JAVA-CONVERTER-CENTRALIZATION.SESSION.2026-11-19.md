# Session 2026-11-19: Java converter source map, branch centralization and measured check

Repository `mearvk/SLeeLa`, branch `master`. All commits were pushed straight to `master`.

## Commits

| Commit | What |
|---|---|
| `e51d7e539` | `.kiro/steering/java-converter-source-map.md`: where the converter source lives |
| `d30a7e3db` | `docs/reference/JAVA.CONVERTER.BRANCH-LEDGER.2026-11-19.md`: status of every converter-related branch |
| `9d3cbfdea` | Port of the SLJIP Java interop work from `work/java-interoperability-hardening` (27 files) |
| `e552ccb9e` | Fixes to the SLeeLa-native converter, plus measured status in `docs/reference/JAVA.CONVERSION.STATUS.md` |

## 1. Finding the source

The first clone timed out during checkout. That left a stale `.git/index.lock` and a working tree that seemed to
have no `sleelactl/` directory. After removing the lock and restoring the tree, the converter was on master.
Its parts:

- `sleelactl/convert/`: the C++ converter (`java_lex.c`, `conv_model.c`, `java_to_sleela.cpp`,
  `javaconv_main.cpp`) and its tests.
- `impl/subjects/conversion/`: the conversion model.
- `lib/java/converter/`: the SLeeLa-native converter.
- Plus `examples/java`, `tests/java_*`, `java28/` and the status documents in `docs/reference/`.

The full map is in `.kiro/steering/java-converter-source-map.md`.

## 2. Branch centralization

I compared 27 remote branches with `java`, `jdk`, `sleelactl`, `convert` or `nov18` in their names against master.
I used the GitHub compare API, then classified each changed file against the point where the branch split from master.
The full table is in `docs/reference/JAVA.CONVERTER.BRANCH-LEDGER.2026-11-19.md`.

- **Fully contained in master (0 commits ahead):** 9 branches. They are safe to delete.
- **Landed on master through other commits:** `package-identity-import-resolution` (PR #374),
  `lower-bound-inference-project-resolution` (ported by PR #379), `qualified-type-fail-closed` (PR #380) and
  `converter-mapping-pass-01`. Master is newer on the shared files, so merging any of them would roll master back.
- **Java string and UTF-16 line (8 branches):** integrated. Master has `impl/core/sleela_java_string.c`.
- **`work/java-api-dependency-closure`:** already on master. Master had moved `lib/java/tools/` and
  `lib/java/implementations/` into `lib/java/jdk-25/`. At those paths the files are identical or newer on master.
- **`work/java-interoperability-hardening`:** this was the one branch with real unmerged work. It was 98 commits
  ahead and 2096 behind, so I ported it file by file instead of merging it (section 3).
- **Open:** PR #378, which only changes workflow files.
- **Not fully checked:** `fix/java-launcher-main-sync` hit a GitHub API rate limit. The two sleelactl native-math
  branches were not compared.

## 3. SLJIP interop port (`9d3cbfdea`)

- **Added:**
  - the SLJIP v1 spec and schema (`lib/java/protocol/`)
  - the native frame codec (`runtime/java_interop_protocol.{c,h}`)
  - the Java endpoint (`SleelaJdkProtocolServer` and its fixture)
  - `lib/java/RUNTIME-INTEGRATION.md` and `impl/JAVA-INTEROP-LOWERING.md`
  - six test files in `test-suites/`
- **Moved to master's layout:** the changes to the two `jdk28_*` tools now apply at `lib/java/jdk-25/tools/`.
- **Combined by hand:**
  - `lib/java/validate.py` uses the branch's exact-count check.
  - `lib/java/MANIFEST.json` records 15,093 files and adds `RUNTIME-INTEGRATION.md` as a control file.
  - The conflicts in `lib/java/README.md` were merged by hand.
- **Left out:** the branch linked the codec into `sleela`/`sleelvac`, but nothing calls it yet. Master has also moved
  Java handling out of `driver.cpp`, so master's driver and link lines were kept. The docs now say the codec is
  tested standalone. `make test-java-runtime` runs the two protocol tests.
- **Bugs fixed:**
  - `test-java-protocol.py` read the endpoint's stderr before every request. That blocks until the process exits,
    so 5 of 7 tests hung.
  - `connector/Makefile` used `$tmp` where it needed `$$tmp`.
  - `SleelaJdkConformance.java` called `add()` on a variable typed `Object`. This was also broken on master.
- **Verified:**
  - `lib/java/validate.py`, the codec test and `make -C connector protocol-test protocol-codec-test` pass.
  - The endpoint tests (7/7) and the library integrity tests (7/7) pass.
  - The JDK conformance check passes when compiled and run directly. `make conformance` needs the Jakarta Servlet jar,
    which the sandbox could not download.

## 4. Measured converter check (`e552ccb9e`)

Run on master `7fbcdd0ae`: Linux x86-64, gcc, Corretto 25.0.5.1, runtime 1.0121 built from source.

| Gate | Result |
|---|---|
| C++ converter build and `--self-check` (model 1.5.0) | PASS, no warnings |
| `generate_ret_rows.py --check` | PASS |
| `tests/javaconv_differential_test.sh` | 108 pass, 0 fail |
| `java.base` subset (884 files) | 259 converted (267 SLeeLa files), 625 failed closed; matches the 2026-11-20 record |
| `tests/javaconv_check_tree.sh` on the subset | 256 of 267 clean; 11 files with 22 errors |
| Native converter `per-member-stub-policy.sh`, `repeat-run-transaction.sh` | PASS after the fixes below; FAIL before |
| `tests/sleelactl_cli_test.sh`, `tests/sleelactl_systemctl_scope_test.sh` | PASS |

The first converter build had over 2,000 compile errors. That came from an intermediate state of master. Newer
commits fixed it, and the table above is from the newest master.

Fixes to the native converter:

- Six two-line comments in `JConvEmitter` (2), `JConvExpr`, `JConvMain`, `JConvModel` and `JConvReport` opened with
  `<`, which is a single-line comment. Their second line parsed as code, and the first such error
  (`JConvMain.sleela:99`) stopped every native conversion. I converted them to `<< ... >>` blocks.
- `JConvTranslator.convertBody` used `outRel`, which is local to `convertType`. It now computes the value itself.
- `security/sha256-manifest.json` was regenerated for exactly these six files, following section 6 of
  `docs/BINARY-SETS.md`.

## Open items

- Not run:
  - the linked check over all of `java.base` (860 output files exceeds the sandbox time limit)
  - the JDK 28 campaign
  - Windows and macOS builds

  The release decision stays at **HOLD**.
- Pre-existing problems on master that this session did not fix:
  - `SleelaHttpConnector.java` declares `maxResponseBytes` twice.
  - `tests/java_api_*_suite.py` and the `validation` section of `lib/java/MANIFEST.json` still point at
    `lib/java/tools/`.
- The 11 files from the subset check that have errors. The open defects are listed in
  `MEARVK.LLC.MAX.RUPPLIN.JAVA-CONVERT.SESSION.2026-11-12.md`.
- Deleting the fully merged branches needs the owner's approval.
