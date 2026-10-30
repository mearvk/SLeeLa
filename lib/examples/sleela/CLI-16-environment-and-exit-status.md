# Environment variables and exit status

| Variable | Meaning |
|---|---|
| `SLEELA_SHA256_MANIFEST` | Trusted SHA-256 manifest path |
| `SLEELA_DEFENDER_OPTIN` | Explicit Defender opt-in |
| `SLEELA_DEFENDER_ALLOW_ROOT` | Explicit privileged-install opt-in |
| `SLEELA_DOCS_DIR` | SleelaDoc corpus directory |
| `SLEELA_FACADE_PATH` | Default facade search root |

The command exits zero on success and non-zero on parse, resolution, semantic,
verification, or runtime failure.

```sh
if sleela check path/to/program.sleela; then
    echo "check passed"
else
    rc=$?
    echo "check failed: $rc" >&2
    exit "$rc"
fi
```

Never bypass a failed SHA-256 verification gate by disabling verification or
substituting an untrusted manifest.
