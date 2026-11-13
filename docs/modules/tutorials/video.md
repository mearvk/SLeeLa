<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# video: Tutorial

| | |
|---|---|
| Nature | registry (`bin/sleelactl help video`) |
| Registry summary | Video codec registry |
| Lives in | `lib/video/` (21 `.sleela` files); native counterpart in `video/` |
| Writeup type | Tutorial, because you drive the pipeline demo with `sleela run` and `sleelactl`, and every step below ran successfully. |
| Verified | 2026-11-13 against `master` e048707fd: `sleela run lib/video/video-codec-pipeline.sleela` exits 0; `sleelactl list/probe/status video` exit 0; an edited copy ran; `make -C video test` ran its C, C++ and source-inventory tests without error. |

## What it is

`video` is the SLeeLa-side management layer for the 12-codec video registry (`handlers=12`, [`registry.conf`](../../../bin/sleelactl.d/registry.conf) line 37). Ids follow the native header [`video/codecs/include/sleela_video_codecs.h`](../../../video/codecs/include/sleela_video_codecs.h): 0 Raw, 1 H.264, 2 HEVC, 3 AV1, 4 VP8, 5 VP9, 6 MPEG-2, 7 Theora, 8 MJPEG, 9 FFV1, 10 ProRes, 11 DNxHD/HR. Only Raw Video is native; the rest are backend or "qualified backend" (ProRes, DNx) per `video/codecs/src/sleela_video_codecs.c` lines 5-16.

## Before you start

```sh
cd SLeeLa            # the repository root
export SLEELA_HOME=$PWD SLEELA_LIB=$PWD/lib SLEELA_SHEET=$PWD/SHEET.sheet \
       SLEELA_SHA256_MANIFEST=$PWD/security/sha256-manifest.json
```

Use the prebuilt `impl/build/sleela`; do not rebuild `impl/`.

## Step 1 - Run the reference pipeline

```sh
timeout 120 impl/build/sleela run lib/video/video-codec-pipeline.sleela
```

Abridged output (memory-manager and security lines omitted; `verification: PASS (18667 item(s))` comes first):

```text
=== SLeeLa video codec pipeline (Controller/Loader/Verifier/Processor) ===
video codecs known: 12
resolve .yuv -> 0 (Raw Video)
resolve .av1 -> 3 (AV1)
decode .av1 before load: codec not loaded
decode raw loaded (native): ok
decode .av1 no backend: unsupported (backend not present)
decode .av1 backend:    ok
loaded count=2
after unloadAll, raw loaded? false
done
```

Compared with the audio and compression demos ([`video-codec-pipeline.sleela`](../../../lib/video/video-codec-pipeline.sleela)), this one has no `probeMagic` step, no `encode` call in `main`, and `isNative(id)` is just `id == 0`.

## Step 2 - Use `sleelactl`

```sh
timeout 60 bin/sleelactl list video
timeout 60 bin/sleelactl probe video
```

```text
  ✓ video — Video codec registry
  handlers: 12 (ids 0..11)
  sources:  lib/video/ (21 .sleela files)
  pipeline: lib/video/video-codec-pipeline.sleela   (run it: sleelactl probe video)
  load/unload/enable/disable/canProcess are Controller calls inside a SLeeLa program.
```

`probe` prints the Step 1 output and ends `✓ video: reference pipeline completed (a demonstration run; no registry state persists)`. `status video` prints the same.

## Step 3 - Resolve a ProRes file

Edit a copy, changing only `main`:

```sh
mkdir -p /tmp/sleela-video-demo
cp lib/video/video-codec-pipeline.sleela /tmp/sleela-video-demo/my-video.sleela
sed -i '/void main/,$ s/\.av1/.mov/g' /tmp/sleela-video-demo/my-video.sleela
timeout 120 impl/build/sleela run /tmp/sleela-video-demo/my-video.sleela
```

```text
video codecs known: 12
resolve .yuv -> 0 (Raw Video)
resolve .mov -> 10 (Apple ProRes)
decode .mov before load: codec not loaded
decode raw loaded (native): ok
decode .mov no backend: unsupported (backend not present)
decode .mov backend:    ok
loaded count=2
after unloadAll, raw loaded? false
done
```

`.mov` resolves to id 10 (Verifier line `if (ext == ".mov") { return 10; }`). ProRes is a "qualified backend" in the native table, but the SLeeLa demo has only a native/not-native split, so it behaves like AV1. Clean up with `rm -rf /tmp/sleela-video-demo`.

## What you did

You ran the video Controller through resolve, load, decode and unload, saw the three result classes, ran it through `sleelactl`, and changed the extension.

"ok" is bookkeeping, not decoding: the Processor returns `0` from `loaded`, `nativeCodec` and `backendReady` alone, and `registerBackend` sets an array flag.

## Next steps

- [`lib/video/README.md`](../../../lib/video/README.md): only a two-line description of the object model. It does not mention the Controller pipeline or `video-codec-pipeline.sleela`, although the codecs README points to them.
- [`video/codecs/CODECS.md`](../../../video/codecs/CODECS.md): the native registry contract and its security expectations.
- Native tests: `make -C video test` compiles and runs `video/tests/test_video.c` and `.cpp` and ends with `SLeeLa video source inventory: 79 files`. Its Makefile writes the test binaries to `/tmp`, not to a build directory.
- Related: [codecs](codecs.md), [compression](compression.md).

## Sources

Files read: `bin/sleelactl.d/registry.conf`, `lib/video/README.md`, `lib/video/video-codec-pipeline.sleela`, `lib/video/VideoCodecLoader.sleela` (lines 15, 91-93), `lib/video/VideoCodecController.sleela` (lines 68-72), `video/codecs/include/sleela_video_codecs.h`, `video/codecs/src/sleela_video_codecs.c`, `video/codecs/CODECS.md`.

Commands run (with `timeout`): `sleela run lib/video/video-codec-pipeline.sleela` and the edited copy; `bin/sleelactl list|probe|status video`; `make -C video test`.

Not verified: the split `Video*.sleela` files standalone. `sleela check lib/video/VideoCodecController.sleela` reports `no 'main' method found` and `call to unknown method 'configure'` / `'dispatchCode'` (inherited from `SLPluginService`, not visible to a single-file check).
