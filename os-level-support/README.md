# OS Level Support

Portable C ABI with a thin C++ facade for read-only, user-mode host services that complement the SLeeLa standard OS model. Existing APIs cover host/process facts, environment, filesystem metadata, CPU/memory, timezone, network metadata, and cron calculation; platform gaps are returned explicitly.

The `runtime/` subdirectory adds an initial, deterministic, in-process C/C++ event-hook registry and cooperative task queue. Hooks are callbacks registered by this runtime only; they do not intercept OS/system calls. Tasks execute only when the caller pumps the queue. This initial layer does not create threads or processes, launch commands, perform I/O, install OS schedules, or alter platform configuration. Callback payloads and user-data remain caller-owned.

The `trust/` directory contains the Global Ring of Trust concept and a C/C++ structural claim model. A well-formed claim is **unverified** and non-authorizing; this code does not perform cryptography, identity verification, revocation lookup, or privilege decisions.

The library intentionally excludes syscall interception, kernel hooks or memory access, driver installation/removal, registry/configuration mutation, privilege elevation, and trust-based authorization. Build and test with `cmake -S os-level-support -B build/osls`, `cmake --build build/osls`, and `ctest --test-dir build/osls --output-on-failure`.
