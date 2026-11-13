<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# ledger: Explanation

| | |
|---|---|
| Nature | datasource (`bin/sleelactl help ledger`) |
| Registry summary | Append-only ledger store |
| Lives in | `lib/ledger/SLPackage.sleela` (the registered directory, one file); the working tool is elsewhere, in `ledger/` |
| Writeup type | Explanation, because the registered module `lib/ledger` is a 54-line package facade with nothing to operate, while the real ledger tool lives outside it and is a build-and-verify utility, not an open/close store. |
| Verified | 2026-11-13 against `master` e048707fd: `bin/sleelactl available/status/open/close ledger` exit 0 and print guidance only; `ledger/ledger.py` and the C++ `ledger_tool` built and verified ledgers, and a tampered ledger was rejected. |

## What it is

Two different things carry the name "ledger", and the registry points at the smaller one.

1. **`lib/ledger/`**, the registered module. It contains only [`SLPackage.sleela`](../../../lib/ledger/SLPackage.sleela). Its class holds a name:

   ```java
   String packageName;
   void configure(String name){packageName=name;}
   String name(){return packageName;}
   boolean available(){return packageName!=null;}
   ```

   The header comment calls it a "Library facade for module package: ledger". It has no append, no storage, and no `open`/`close`/`active`.

2. **The `.ledger` class**, in [`ledger/`](../../../ledger/LEDGER.md) at the repository root: a tamper-evident hash chain of records, one per source file. Each record holds a SHA-256 of the file, a link to the previous record, a UTC ISO-8601 timestamp, and a QR insignia (an SVG). [`tutorial/10-ledger.md`](../../../tutorial/10-ledger.md) teaches it.

The registry entry ([`registry.conf`](../../../bin/sleelactl.d/registry.conf) line 43) says `nature=datasource dir=lib/ledger summary="Append-only ledger store"`. That summary does not describe `lib/ledger/`, which stores nothing.

## How it works

`sleelactl` treats a datasource as an open/close/active surface owned by a running program. For this module it prints, for `status ledger`:

```text
== ledger (datasource) ==
  ledger is an open/close/active data surface (SLDataSource).
  ✓ ledger library present at lib/ledger
  availability is per-connection runtime state: open() → active() == true; close() → false
```

and for `open ledger` the hint `in SLeeLa:  SLDataSource ds = new SLDataSource(); ds.configure("ledger"); ds.open();`. `SLDataSource` is defined in `lib/database`, not in `lib/ledger` (see [database](database.md)), and its `close()` fails at run time with `Munction closeWithReceipt() requires a reach handle`. Nothing links the hint to a ledger.

The ledger itself ([`ledger/LEDGER.md`](../../../ledger/LEDGER.md)):

- The file is JSON Lines: a header line (`"ledger":"sleela","version":1,"algo":"sha256","chain":"prev-hash"`) and one record per file.
- `genesis` is 64 zeros. Record *n*'s `prev` is record *n-1*'s `record_hash`. `record_hash` is the SHA-256 of the record's canonical bytes without that field.
- The timestamp is a local-clock UTC ISO-8601 reading, labelled `"timestamp_kind":"utc-iso8601"`. It is not an RFC 3161 notarised token, and the document says so.
- Two implementations exist: Python (`ledger.py`, `qr.py`) and C++ (`ledger_core.cpp`, `ledger_tool.cpp`, `Makefile`). Each verifies the other's output; I checked the C++-built file with the Python verifier.
- The only commands are `build` and `verify`. `ledger.py` defines exactly those two subparsers. There is no "append to an existing ledger" command, so "append-only" describes the chain format, not an operation you can run.

## Where it lives

| Path | Role |
|---|---|
| `lib/ledger/SLPackage.sleela` | Registered module; package-name facade only |
| `ledger/LEDGER.md` | Format and chain rules |
| `ledger/ledger.py`, `ledger/qr.py` | Python builder/verifier and QR SVG generator |
| `ledger/ledger_core.{h,cpp}`, `ledger/ledger_tool.cpp`, `ledger/Makefile` | C++ implementation (`make ledger_tool`) |
| `tutorial/10-ledger.md` | Lesson that uses the tool |
| `bin/sleelactl.d/registry.conf` line 43 | Registry entry |

## Status and limits

What I ran, all with `timeout`:

```sh
python3 ledger/ledger.py build --out <dir>/demo.ledger --insignia-dir <dir>/insignia --root . \
    examples/symmetry/src/core/Message.sleela
python3 ledger/ledger.py verify <dir>/demo.ledger
```

```text
ledger: wrote .../demo.ledger (1 records)
ledger: OK — 1 records, chain intact
```

Changing `"seq":1` to `"seq":9` in that file made verification fail with exit code 1:

```text
ledger: record 1 (seq 9) record_hash MISMATCH
ledger: FAIL — 1 problem(s)
```

The C++ tool (`make ledger_tool` in `ledger/`, which writes the git-ignored binary `ledger/ledger_tool`) also worked, on two files:

```text
ledger_tool: wrote .../cpp.ledger (2 records)
ledger_tool: OK -- 2 records, chain intact
ledger: OK — 2 records, chain intact      <- Python verifying the C++ output
```

These are the commands from the lesson, so lesson 10 holds for the Python and C++ paths. The lesson's second path, Nordshrift emitting `<artifact>.sleela.ledger` next to each artifact, I did not run, because it would write files into the repository; whether it works here is **unknown**.

Not working or not as described:

- `lib/ledger` has no program to run. `impl/build/sleela check lib/ledger/SLPackage.sleela` fails with `Semantic error: no 'main' method found`.
- The registry summary "Append-only ledger store" and the `sleelactl open ledger` hint do not match anything implemented in `lib/ledger/`.
- `make test` in `ledger/` writes to `/tmp/self.ledger` and `/tmp/insignia`; I did not run it and ran the equivalent commands in a scratch directory instead.

## Related

- [database](database.md): defines `SLDataSource`, the class the `sleelactl` hint refers to.
- [registry-store](../tutorials/registry-store.md): another datasource with a real native core outside its `lib/` directory, which also carries seals (SHA-256).

## Sources

Files read: `bin/sleelactl.d/registry.conf`, `lib/ledger/SLPackage.sleela`, `ledger/LEDGER.md`, `ledger/Makefile`, `ledger/ledger.py` (argument parsing, lines 129-139), `tutorial/10-ledger.md`.

Commands run: `bin/sleelactl help|available|status|open|close ledger`; `impl/build/sleela check lib/ledger/SLPackage.sleela`; `python3 ledger/ledger.py build|verify` (with `PYTHONDONTWRITEBYTECODE=1`); a tampered-copy verify; `make ledger_tool` in `ledger/`; `ledger_tool build|verify`. All ledger files were written to a scratch directory outside the repository and deleted.
