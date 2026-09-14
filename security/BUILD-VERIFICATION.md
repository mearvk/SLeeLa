# SLeeLa Verified Build Process

SLeeLa builds are fail-closed with respect to the configured SHA-256 source manifest.

## Required order

1. Resolve a trusted SHA-256 manifest.
2. Verify every manifest entry against the SLeeLa source tree.
3. Stop immediately on a missing file, unexpected path, or digest mismatch.
4. Invoke the normal `impl/Makefile` build only after verification succeeds.
5. Run the resulting test targets normally.

The supported entry point is:

```sh
./tools/build-verified.sh
```

Additional Make targets may be supplied, for example:

```sh
./tools/build-verified.sh all
./tools/build-verified.sh test
```

The manifest may be selected with `SLEELA_SHA256_MANIFEST`. The default location is `security/sha256-manifest.json` (JSON, matching `tools/verify-before-execution.py`).

Regenerate the default manifest after changing any verified source file:

```sh
python3 tools/generate-sha256-manifest.py --root . --output security/sha256-manifest.json
```

The verification step establishes integrity relative to the trusted manifest. SHA-256 alone does not establish who authored or published the manifest; distribution of the manifest remains a trust decision.
