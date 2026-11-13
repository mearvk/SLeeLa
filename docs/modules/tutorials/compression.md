<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# compression: Tutorial

| | |
|---|---|
| Nature | registry (`bin/sleelactl help compression`) |
| Registry summary | Compression registry (native 0-3) |
| Lives in | `lib/compression/` (22 `.sleela` files); native counterpart in `compression/` |
| Writeup type | Tutorial, because you drive the pipeline demo with `sleela run` and `sleelactl`, and every step below ran successfully. |
| Verified | 2026-11-13 against `master` e048707fd: `sleela run lib/compression/compression-pipeline.sleela` exits 0; `sleelactl list/probe/status compression` exit 0; an edited copy ran; `make -C compression test` printed `compression: all tests passed`. |

## What it is

`compression` is the SLeeLa-side management layer for a 15-algorithm compression registry (`handlers=15`, [`registry.conf`](../../../bin/sleelactl.d/registry.conf) line 36). "native 0-3" in the summary means the algorithms with ids 0 to 3 (Store, RLE, Huffman, LZW) are the *native* ones; ids 4 to 13 are *backend* algorithms and id 14 (ZIP) is a *container* ([`lib/compression/README.md`](../../../lib/compression/README.md), "Algorithm registry"). The same four roles exist as for [codecs](codecs.md): Controller, Loader, Verifier, Processor.

## Before you start

```sh
cd SLeeLa            # the repository root
export SLEELA_HOME=$PWD SLEELA_LIB=$PWD/lib SLEELA_SHEET=$PWD/SHEET.sheet \
       SLEELA_SHA256_MANIFEST=$PWD/security/sha256-manifest.json
```

Use the prebuilt `impl/build/sleela`; do not rebuild `impl/`.

## Step 1 - Run the reference pipeline

```sh
timeout 120 impl/build/sleela run lib/compression/compression-pipeline.sleela
```

Output, abridged (memory-manager and security lines omitted; `verification: PASS (18667 item(s))` is printed first):

```text
=== SLeeLa compression pipeline (Controller/Loader/Verifier/Processor) ===
algorithms known: 15
resolve .rle -> 1 (Run-Length Encoding)
resolve .gz  -> 6 (gzip (RFC 1952))
compress .rle before load: algorithm not loaded
compress .rle loaded:      ok
decompress .rle loaded:    ok
compress .gz no backend:   unsupported (backend not present)
compress .gz backend:      ok
probe 'SHF1' -> id 2 (Canonical Huffman)
loaded count=2
after unloadAll, .rle loaded? false
done
```

Reading it against [`compression-pipeline.sleela`](../../../lib/compression/compression-pipeline.sleela):

- `compress` before `load` gives `-5` ("algorithm not loaded").
- RLE is native (`isNative` is true for ids 0-3), so it returns `0` once loaded.
- gzip is a backend algorithm: loaded without a backend it gives `-1` ("unsupported"); after `registerBackend(gz)` it gives `0`.
- `probeMagic("SHF1")` maps the Huffman signature to id 2. The Verifier knows five signatures: `SHF1`, `SLZW`, `1F8B08`, `28B52FFD`, `PK`.

## Step 2 - Use `sleelactl`

```sh
timeout 60 bin/sleelactl help compression
timeout 60 bin/sleelactl list compression
timeout 60 bin/sleelactl probe compression
```

```text
  ✓ compression — a registry module
  Compression registry (native 0-3)
  legal verbs:  list probe status

  ✓ compression — Compression registry (native 0-3)
  handlers: 15 (ids 0..14)
  sources:  lib/compression/ (22 .sleela files)
  pipeline: lib/compression/compression-pipeline.sleela   (run it: sleelactl probe compression)
  load/unload/enable/disable/canProcess are Controller calls inside a SLeeLa program.
```

`probe` prints the Step 1 output and ends with `✓ compression: reference pipeline completed (a demonstration run; no registry state persists)`. `status` does the same.

## Step 3 - Try Zstandard instead of gzip

Edit a copy, changing only `main` (the Verifier must keep its `.gz` branch):

```sh
mkdir -p /tmp/sleela-comp-demo
cp lib/compression/compression-pipeline.sleela /tmp/sleela-comp-demo/my-comp.sleela
sed -i '/void main/,$ s/\.gz/.zst/g' /tmp/sleela-comp-demo/my-comp.sleela
timeout 120 impl/build/sleela run /tmp/sleela-comp-demo/my-comp.sleela
```

```text
algorithms known: 15
resolve .rle -> 1 (Run-Length Encoding)
resolve .zst  -> 10 (Zstandard)
compress .rle before load: algorithm not loaded
compress .rle loaded:      ok
decompress .rle loaded:    ok
compress .zst no backend:   unsupported (backend not present)
compress .zst backend:      ok
probe 'SHF1' -> id 2 (Canonical Huffman)
...
```

The variable is still called `gz`, but it now holds id 10. Zstandard behaves exactly like gzip because both are backend algorithms. Clean up with `rm -rf /tmp/sleela-comp-demo`.

## What you did

You ran the Controller through load, compress, decompress and unload, saw each result code, drove it through `sleelactl`, and changed the demo.

Be clear about what "ok" means. The Processor ([`compression-pipeline.sleela`](../../../lib/compression/compression-pipeline.sleela), `class Processor`) returns `0` from the three booleans `loaded`, `nativeAlg`, `backendReady`. No bytes are compressed, even for the "native" RLE. The real algorithms are C code in `compression/compressors/`, which this demo never calls.

## Next steps

- [`lib/compression/README.md`](../../../lib/compression/README.md): roles, flow, the result-code table (it has `-6` for "output buffer too small", which the demo's Verifier also knows).
- [`lib/compression/compression.sleela`](../../../lib/compression/compression.sleela): the smaller Loader demo. It ran and printed e.g. `Run-Length Encoding [native] -> loaded`, but names id 10 as `algorithm#10`.
- Native side: `make -C compression test` builds into the git-ignored `compression/build/` and ends `compression: all tests passed`. See [`compression/COMPRESSION.md`](../../../compression/COMPRESSION.md).
- Related: [codecs](codecs.md), [video](video.md).

## Sources

Files read: `bin/sleelactl.d/registry.conf`, `lib/compression/README.md`, `lib/compression/compression-pipeline.sleela`, `lib/compression/compression.sleela` (run), `compression/include/sleela_compression.h` (enum: 15 ids, `STORE` to `ZIP`, then `COUNT`).

Commands run (with `timeout`): `sleela run` on the two `.sleela` files above and on the edited copy; `bin/sleelactl help|list|probe|status compression`; `make -C compression test`.

Not verified: that the split `SLCompression*.sleela` files run on their own. `sleela check lib/compression/SLCompressionController.sleela` reports `no 'main' method found` plus `declares multiple constructors; constructor overload resolution requires syntax 3.0` errors. I also did not run any real compression.
