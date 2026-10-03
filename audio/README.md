<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">






# SLeeLa Audio API

**Package:** `audio/`  
**Updated:** 2026-09-28  
**Status:** Native PCM16 WAV foundation implemented; SLVM-to-C/C++ Audio bridge added; Java/native CLI contract aligned; CI build coverage added; final cross-path behavioral/EQ conformance remains.

The SLeeLa Audio API is a layered audio-processing package spanning C11, C++17, Java 21, native processing, and JavaFX presentation.

## Package Layout

| Path | Responsibility |
|---|---|
| `audio/c/` | C11 ABI, validation, and PCM16 WAV mixing |
| `audio/cpp/` | C++17 typed API and PCM16 WAV mixing |
| `audio/java/` | Java 21 orchestration and native-process boundary |
| `audio/native/` | Native command-line media-processing adapter |
| `audio/gui/` | JavaFX audio/video presentation and integration |
| `audio/sleela/` | SLeeLa language-layer Audio classes | 
| `audio/1-2-3-4.md` | Ordered implementation record, dates, completion state, and closure work |
| `.github/workflows/audio-ci.yml` | Audio compile/build integration CI |

## Implemented Audio Capabilities

The current native C/C++ mixer provides:

- PCM16 WAV input;
- mono and stereo input;
- synchronized input offsets;
- per-input gain;
- master gain;
- left/right gain;
- pan;
- stereo PCM16 WAV output;
- sample-rate validation;
- configuration validation;
- malformed/truncated input rejection;
- bounded input count of 128;
- audio-length overflow protection;
- output-size and RIFF-size protection.

The C and C++ APIs provide corresponding audio-processing semantics. The C++ source also received a final source-integrity cleanup removing embedded newline artifacts from the implementation.

## Native Command-Line Adapter

The native adapter now documents and parses the parameterized process contract:

```
sleela-audio-native --output OUTPUT --sample-rate RATE \
  --input PATH START_SECONDS GAIN_DB ... \
  --bass DB --mid DB --treble DB \
  --master-gain DB --pan VALUE \
  --left-gain VALUE --right-gain VALUE
```

The adapter:

- accepts repeated `--input` entries;
- carries per-input start offsets and gain;
- carries master, pan, and channel gain controls;
- carries bass, mid, and treble values at the process boundary;
- rejects unknown or incomplete options;
- invokes the same native C++ mixer implementation used by the C++ API.

EQ values are carried by the process contract, but the native mixer does **not yet implement EQ filtering semantics**. They therefore remain an explicit implementation/conformance item.

## Java API

The Java 21 layer provides:

- `Audio` interface;
- `Input`, `Controls`, and `Configuration` records;
- `NativeAudio` process adapter;
- executable validation;
- configuration validation;
- process exit handling;
- interruption handling.

The Java/native command contract is now aligned with the native adapter. `NativeAudio` emits:

```
--output OUTPUT
--sample-rate SAMPLE_RATE
--input PATH START_SECONDS GAIN_DB
--bass BASS_DB
--mid MID_DB
--treble TREBLE_DB
--master-gain MASTER_GAIN_DB
--pan PAN
--left-gain LEFT_GAIN
--right-gain RIGHT_GAIN
```

This closes the earlier documentation gap in which the Java configuration represented more controls than the process invocation transmitted.

The Java layer remains orchestration; native C/C++ remains responsible for the actual PCM processing.

## GUI

The JavaFX layer provides the presentation and session boundary for the Audio/Video work.

It is intentionally separated from the native processing implementation so that command-line, service, and GUI consumers can use the appropriate layer.

## EQ Status

The public C, C++, and Java models and the native CLI contract expose:

- bass;
- mid;
- treble.

The native PCM16 mixer currently carries these values through the API/CLI boundary but does not apply EQ filtering. EQ is therefore **transported but not implemented as an audio effect**.

## Verification and Conformance

The ordered completion record is maintained in `audio/1-2-3-4.md`.

The required behavioral matrix includes:

1. mono → stereo;
2. stereo → stereo;
3. multiple synchronized offsets;
4. per-input gain;
5. master gain;
6. left/right gain;
7. pan;
8. clipping and saturation;
9. malformed WAV;
10. truncated WAV chunks;
11. sample-rate mismatch;
12. zero/invalid configuration;
13. one-input boundary;
14. 128-input boundary;
15. output-size overflow protection;
16. C/C++ equivalent PCM output for identical fixtures.

The remaining closure work is to execute and record the complete cross-language behavioral matrix and either implement or explicitly finalize the EQ semantics.

## Audio CI

Audio-specific CI is now defined in `.github/workflows/audio-ci.yml`.

The workflow is triggered by changes under `audio/**` or to the workflow itself and covers:

- C audio: `make clean all test`;
- C++ audio: `make clean all test`;
- native audio adapter: `make clean all test`;
- Java 21 Audio API compilation;
- JavaFX audio GUI Maven packaging.

The workflow provides compile/build integration coverage for the complete Audio package. A green CI result should be treated separately from the behavioral conformance matrix until those runtime cases are recorded.

## Build

### C11

```bash
cd audio/c
make clean all test
```

### C++17

```bash
cd audio/cpp
make clean all test
```

### Native adapter

```bash
cd audio/native
make clean all test
```

### Java 21

```bash
cd audio/java
javac -d build/classes src/module-info.java src/com/mearvk/audio/sleela/*.java
```

### JavaFX GUI

```bash
cd audio/gui
mvn -B -DskipTests package
```

## Engineering Record

The Audio API work was developed in stages:

- **2026-09-22:** initial JavaFX Audio/Video GUI and Java API work.
- **2026-09-28:** GUI cleanup and native audio architecture.
- **2026-09-28:** C ABI, native build/documentation, process adapter, and GUI/native boundary.
- **2026-09-28:** C/C++/Java language-layer quality tightening.
- **2026-09-28:** final C WAV buffer-bookkeeping correction and overflow protections.
- **2026-09-28:** Java/native CLI contract aligned with the complete modeled control set.
- **2026-09-28:** Audio API compile/integration CI added.
- **2026-09-28:** C++ source newline artifacts corrected.
- **2026-09-28:** `audio/1-2-3-4.md` maintained as the ordered completion record.

## Current Status

**Implemented:** core native PCM16 WAV foundation, C/C++ APIs, parameterized native CLI boundary, aligned Java process adapter, JavaFX integration, validation, overflow protection, and Audio-specific CI build coverage.

**Remaining closure:** run and record the full cross-language behavioral conformance matrix and implement or explicitly finalize native EQ semantics.

The package is therefore **implemented at its core with its current process contract documented and aligned**, while the remaining behavioral and EQ closure items are kept explicit rather than being described as complete prematurely.


## SLeeLa language layer

The Audio API now has a dedicated `audio/sleela/` contract layer with **9 standard .sleela classes**. All nine declare SLeeLa syntax 1.3 and contain executable method bodies compatible with the current compiler.

The SLVM bridge is handle-based:

`audioNew -> audioAdd -> audioControls -> audioValidate -> audioRender -> audioClose`

The VM owns the bounded job state. `audioRender` crosses a stable C ABI callback into `impl/core/sleela_audio_bridge.cpp`, which invokes the existing C++ WAV renderer. Source-level SLeeLa never receives native pointers or OS handles.

The existing `audio/c/` C ABI remains available, `impl/core/sleela_audio_mixer.c` remains the in-VM float mixer, and `audio/cpp/` remains the concrete WAV implementation.

See `audio/sleela/README.md` and `audio/sleela/SLVM.md` for the complete boundary.