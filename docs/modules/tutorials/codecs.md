<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# codecs: Tutorial

| | |
|---|---|
| Nature | registry (`bin/sleelactl help codecs`) |
| Registry summary | Audio codec registry |
| Lives in | `lib/codecs/` (27 `.sleela` files); native counterpart in `codecs/` |
| Writeup type | Tutorial, because you drive the Controller/Verifier/Processor demo with `sleela run` and `sleelactl`, and every step below ran successfully. |
| Verified | 2026-11-13 against `master` e048707fd: `sleela run lib/codecs/codec-pipeline.sleela` exits 0 and prints the expected 11 lines; `sleelactl list/probe/status codecs` all exit 0; an edited copy of the demo ran. |

## What it is

`codecs` is the SLeeLa-side management layer for the 20-entry audio codec registry. It has four roles: a Controller (load/unload/decode/encode), a Verifier (file extension or magic bytes to codec id), a Processor (decides the result code) and a Loader (names and classification). The registry registers `handlers=20` ([`bin/sleelactl.d/registry.conf`](../../../bin/sleelactl.d/registry.conf) line 35). The README says the byte-level work belongs to native plugins in `codecs/`, not to these `.sleela` classes ([`lib/codecs/README.md`](../../../lib/codecs/README.md)).

This tutorial uses the single-file demo `codec-pipeline.sleela`, which inlines the four roles plus a `main`.

## Before you start

You need the repository checkout and the prebuilt `impl/build/sleela`. Do not rebuild `impl/`. Set the environment once:

```sh
cd SLeeLa            # the repository root
export SLEELA_HOME=$PWD SLEELA_LIB=$PWD/lib SLEELA_SHEET=$PWD/SHEET.sheet \
       SLEELA_SHA256_MANIFEST=$PWD/security/sha256-manifest.json
impl/build/sleela version
```

```text
SLeeLa™ Language/Runtime 1.3.2-pro (supported source syntax 1.0 – 3.1; native C/C++ Core VM)
```

## Step 1 - Run the reference pipeline

```sh
timeout 120 impl/build/sleela run lib/codecs/codec-pipeline.sleela
```

Output, abridged (the `[memory-manager]`, `[security]` and `verification: PASS (18667 item(s))` lines are printed before and after the program):

```text
=== SLeeLa audio codec pipeline (Controller/Loader/Verifier/Processor) ===
codecs known: 20
resolve .wav  -> 0 (PCM/WAV)
resolve .flac -> 2 (FLAC)
decode .wav before load: codec not loaded
decode .wav loaded:      ok
encode .wav loaded:      ok
decode .flac no backend: unsupported (backend not present)
decode .flac backend:    ok
probe 'MThd' -> id 17 (MIDI)
loaded count=2
after unloadAll, .wav loaded? false
done
```

Read it against the source ([`codec-pipeline.sleela`](../../../lib/codecs/codec-pipeline.sleela) lines 149-183):

- A codec cannot be used before `load(id)`: result `-5`, text "codec not loaded".
- WAV is *native* (ids 0, 1, 15, 16), so once loaded it returns `0` ("ok").
- FLAC is a *backend* codec. Loaded but with no backend registered it returns `-1`, "unsupported". After `registerBackend(flac)` it returns `0`.

## Step 2 - Ask `sleelactl` about the registry

```sh
timeout 60 bin/sleelactl help codecs
timeout 60 bin/sleelactl list codecs
```

```text
  ✓ codecs — a registry module
  Audio codec registry
  legal verbs:  list probe status

  ✓ codecs — Audio codec registry
  handlers: 20 (ids 0..19)
  sources:  lib/codecs/ (27 .sleela files)
  pipeline: lib/codecs/codec-pipeline.sleela   (run it: sleelactl probe codecs)
  load/unload/enable/disable/canProcess are Controller calls inside a SLeeLa program.
```

Now run the same demo through the control plane:

```sh
timeout 60 bin/sleelactl probe codecs
```

The output is the Step 1 output between the line `codecs reference pipeline: lib/codecs/codec-pipeline.sleela` and a final line:

```text
  ✓ codecs: reference pipeline completed (a demonstration run; no registry state persists)
```

`sleelactl status codecs` prints the same thing (it runs the pipeline again). "No registry state persists" is literal: each run starts from an empty `loaded` table.

## Step 3 - Change the demo

Work on a copy so the repository stays untouched. This changes the second file from `.flac` to `.opus` (Opus is id 8, a backend codec):

```sh
mkdir -p /tmp/sleela-codec-demo
cp lib/codecs/codec-pipeline.sleela /tmp/sleela-codec-demo/my-codec.sleela
sed -i '/void main/,$ s/\.flac/.opus/g; s/nameOf(17)/nameOf(8)/' /tmp/sleela-codec-demo/my-codec.sleela
timeout 120 impl/build/sleela run /tmp/sleela-codec-demo/my-codec.sleela
```

The `sed` range matters. Applied to the whole file, it would also change the Verifier, which then has two `".opus"` branches and returns the FLAC id for `.opus` (I ran that variant by mistake first: `resolve .opus -> 2 (FLAC)`).

```text
codecs known: 20
resolve .wav  -> 0 (PCM/WAV)
resolve .opus -> 8 (codec#8)
decode .wav before load: codec not loaded
decode .wav loaded:      ok
encode .wav loaded:      ok
decode .opus no backend: unsupported (backend not present)
decode .opus backend:    ok
probe 'MThd' -> id 17 (codec#8)
loaded count=2
after unloadAll, .wav loaded? false
done
```

Two things to notice. `resolve .opus -> 8` works because the Verifier knows `.opus` (line 69). `codec#8` appears because the demo's `nameOf` only knows three names (ids 0, 2 and 17, lines 124-129), so every other id prints as `codec#<id>`. The last `probe` line still prints id 17 because `probeMagic("MThd")` is unchanged; my `sed` changed only the name lookup, so that label is wrong on purpose.

Clean up: `rm -rf /tmp/sleela-codec-demo`.

## What you did

You ran the audio codec Controller through load, decode, encode and unload, saw the three result classes (not loaded, no backend, ok), drove the same demo through `sleelactl probe`, and modified a copy.

One caution about what "ok" means. The Processor ([`codec-pipeline.sleela`](../../../lib/codecs/codec-pipeline.sleela) lines 92-105) returns `0` from three booleans only (`loaded`, `native`, `backendReady`). No audio bytes are read or written, and `registerBackend` just sets a flag. The demo shows lifecycle and honest result codes, not real decoding.

## Next steps

- [`lib/codecs/README.md`](../../../lib/codecs/README.md): roles, flow, result codes, the 20-codec table.
- [`lib/codecs/codecs.sleela`](../../../lib/codecs/codecs.sleela): the smaller Loader demo (`sleela run` works; it printed "PCM/WAV [native] -> loaded", and so on).
- [`codecs/CODECS.md`](../../../codecs/CODECS.md) and `codecs/include/sleela_codecs.h`: the native registry. `make -C codecs test` ended with `codecs: all tests passed` (it builds into the git-ignored `codecs/build/`).
- Related: [compression](compression.md), [video](video.md).

## Sources

Files read: `bin/sleelactl.d/registry.conf`, `bin/SLEELACTL.md` (head), `lib/codecs/README.md`, `lib/codecs/codec-pipeline.sleela`, `lib/codecs/codecs.sleela`, `lib/codecs/SLCodecController.sleela` (head), `codecs/include/sleela_codecs.h`.

Commands run (all with `timeout`): `impl/build/sleela version`; `impl/build/sleela run lib/codecs/codec-pipeline.sleela`; `impl/build/sleela run lib/codecs/codecs.sleela`; `bin/sleelactl help|list|probe|status codecs`; the Step 3 copy-and-edit run; `make -C codecs test`.

Not verified: that the 27 `.sleela` files other than the demos run on their own. `sleela check lib/codecs/SLCodecController.sleela` fails (`no 'main' method found`, then repeated `class 'UnsupportedOperationException' declares multiple constructors; constructor overload resolution requires syntax 3.0`). They are described in the README as the "discovery surface", not standalone programs.
