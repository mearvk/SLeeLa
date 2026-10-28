# Testing the web server (local, no external CI)

The web-server-technology test suite is a set of **self-contained, runnable
scripts** under `native/`. There is deliberately **no external CI vendor and no
GitHub Actions**: the "CI brand" is offloaded to local scripts you (or any
runner, hook, or cron) invoke directly. Each script exits non-zero on failure,
so they compose into any harness without special plumbing.

## Build + unit tests

```sh
cd web-server-technology/native
make            # builds build/sleela-web-server (links the SQL + slweb cores)
make check      # hardening unit tests + the CPU keep-warm convergence test
```

## End-to-end scripts

| Script | What it proves |
|---|---|
| `smoke-test.sh [ro|rw]` | Side-1 static + `/db` SQL (both dialects) + read-only policy |
| `trav-test.sh` | path-traversal containment (`..%2f..` → 403) |
| `envelopment-test.sh` | Side-2 Scientist gate + warrant routing (meme/static/sql) |
| `hardening-live-test.sh` | rate limit (503), write-token (401/ok), trusted-proxy anti-spoof |
| `config-hardening-test.sh` | the hardening features driven from a config file |
| `warm-live-test.sh` | CPU keep-warm status, `/__sleela/warm` per-core readings, `--no-warm` |
| `stress-fuzz-test.sh [CLIENTS] [REQS]` | concurrency soak + SQL fuzz + HTTP framing fuzz |

Run one:

```sh
sh stress-fuzz-test.sh            # defaults: 8 clients x 25 requests
sh stress-fuzz-test.sh 16 100     # heavier soak
```

## What the stress + fuzz harness checks

`stress-fuzz-test.sh` starts a fresh server (writable `/db`, a write token) and
runs three phases, failing if any regress:

- **A. Concurrency.** Multiple clients hammer `/db` with mixed reads and
  token-authenticated writes (reads driven via `curl --parallel` so load is
  real without forking a process per request). Every request the server accepts
  must return 200, and the server must still be healthy afterward. Confirms no
  torn CSV / no races on the shared engine under concurrent load.
- **B. SQL fuzz.** Gibberish, empty, multi-statement, unknown-table, 16 KiB,
  and binary-ish SQL must each yield a clean **4xx** (e.g. 400/401/413/422) —
  **never a 5xx and never a dropped connection**. Confirms the `/db` surface
  degrades safely on bad input.
- **C. HTTP framing fuzz.** Malformed requests on raw sockets (bad method, no
  HTTP version, missing `Host`, a garbage first line, a 70 KiB header) must be
  rejected with 400/405 or a clean close, and a good request must still succeed
  afterward — i.e. the process survived every malformed frame.

The harness is intentionally conservative about load defaults so it finishes
quickly even where process spawning is expensive; raise `CLIENTS`/`REQS` for a
real soak. It self-reports a clear startup health gate so a port-in-use or
bind failure is diagnosed as such rather than looking like a crash.

## A note on honesty

If a run reports failures, read the per-line output: phases B and C print the
status for each case, and phase A prints `ok/non-200` counts plus the server's
post-run health. A non-200 under phase A is a genuine server defect; a 4xx in
phase B/C is the *expected* safe rejection (those lines are labelled `ok`).
