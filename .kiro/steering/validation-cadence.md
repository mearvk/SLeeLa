---
inclusion: always
---

# Validation cadence: run the long suites only occasionally

Repo-scoped, always-on. Set by the repository owner on 2026-11-12: the full
validation suites take too long to run on every change. Skip them by default
and run them only occasionally, or when the user asks.

## Default: fast checks only (seconds)

Run these after a change that touches what they cover:

| Check | Command | When |
|---|---|---|
| SHA-256 build gate | `python3 tools/generate-sha256-manifest.py --root . --output security/sha256-manifest.json` then `cd impl && make check-manifest` | after editing `lib/**` or `impl/` source (required, or the build fails) |
| Library inventory | `bash test-suites/test-library-inventory.sh` | after adding or removing a `lib/**/*.sleela` file |
| `/lib/java` file count | `python3 lib/java/validate.py` | after adding or removing a `lib/java` file |
| jdk-28 1:1 index | `python3 lib/java/jdk-28/tools/validate_jdk28.py` | after adding, moving or renaming a `lib/java/jdk-28` file |
| The one program you changed | `impl/build/sleela run <file>.sleela`, or one differential pair (`javac` + `java` + `sleela run` + `diff`) | after changing that program or the class it exercises |

## Occasional: long suites (minutes each), skip unless asked

Do **not** run these as routine verification. Run them only when the user
asks, at a milestone (before a release or a large merge), or after a broad
compiler/VM change. Say in the final summary that they were skipped.

| Suite | Command | Approximate cost |
|---|---|---|
| Every jdk-28 file through the compiler | `bash lib/java/jdk-28/tools/check_jdk28.sh` | 4,721 files; longest of all |
| Compiler test suite | `cd impl && make -k test` (50 targets) | many minutes |
| Accessor contracts | `python3 lib/java/jdk-25/tools/jdk_api_accessors_check.py` | about 3 minutes |
| All differential tests | `bash examples/java/differential/run-differential.sh` | several minutes (17 JDK + VM runs) |
| All conformance programs | `bash examples/java/run-conformance.sh` | several minutes |
| Coverage reports | `python3 lib/java/jdk-25/tools/jdk_api_coverage.py --tier jdk-25|jdk-28 --markdown ...` | regenerate only when publishing new numbers |
| Repo-wide `sleela check` over all main programs | (ad hoc loop) | long |

Running `sleela check` or `run` writes `docs/keyword-supporters/` folders next
to the checked files. Delete these generated folders before committing
(`git status` shows them as untracked) unless the change is meant to include
them.

## Honesty rule

Skipping a suite is allowed; claiming it passed is not. When a long suite was
not run, documents such as `lib/java/STATUS.md` keep its last measured result
with its date, marked "not re-run". They never present it as current.
