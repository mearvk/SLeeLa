# SLeeLa SHA-256 Verification Gate

SLeeLa now fails closed before compilation, execution, xclass diagnostics, and OS Defender diagnostics/build/install/provision unless a trusted SHA-256 manifest is supplied.

## Configuration

Set:

```text
SLEELA_SHA256_MANIFEST=/path/to/trusted-manifest.json
```

The manifest uses this structure:

```json
{
  "algorithm": "sha256",
  "files": [
    {"path": "impl/frontend/driver.cpp", "sha256": "..."}
  ]
}
```

Paths must remain inside the selected SLeeLa root. SHA-256 is calculated over the actual file bytes. Any missing file, path escape, malformed manifest, or digest mismatch stops the operation.

## Enforcement points

The native frontend invokes the verification gate before:

- `sleela compile`
- `sleela run`
- `sleela check`
- `sleela xclass`
- `sleela defender detect`
- `sleela defender build`
- `sleela defender install`
- `sleela defender provision`
- direct `.sleela` / `.xclass` execution

The verification implementation is `tools/verify-before-execution.py` and the native integration is `impl/frontend/driver.cpp`.

This is an integrity gate, not publisher authentication. The manifest itself must come from a trusted source. A signed-manifest or other authenticated trust chain can be added separately.
