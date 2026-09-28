# .sleela-debug Session Format

Version: 0.8.0

A session is a versioned diagnostic artifact. Recommended contents:
```
session.json
events.jsonl
breakpoints.json
watchpoints.json
threads.json
stack.json
registers.json
modules.json
symbols.json
memory-map.json
evidence.json
coverage.json
crash.json
replay/
security.json
```

Each artifact records session identity and, where applicable, source revision, executable hash, platform, backend, toolchain and schema version. Readers must reject incompatible schema versions rather than silently interpreting unknown data.