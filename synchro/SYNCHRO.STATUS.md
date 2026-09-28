# SLeeLa Synchro Status

## Test Suite
Run `bash synchro/tests/run_synchro_suite.sh`.

The suite covers C integration smoke, malformed/short/wrong-sequence/timestamp-rejection cases, statistics-window and percentile behavior, C++17 runtime integration, and Java statistics plus a controlled UDP loopback probe.

GitHub Actions runs this suite on pushes and pull requests for `main` and `master`.

## Remaining Verification
VM adapter work and full native multi-platform release verification remain separate tasks.
