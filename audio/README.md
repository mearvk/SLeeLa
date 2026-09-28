# SLeeLa Audio API

**Package:** `audio/`  
**Updated:** 2026-09-28  
**Status:** Native foundation implemented; final contract/conformance closure in progress.

The SLeeLa Audio API is a layered audio-processing package spanning C11, C++17, Java 21, native processing, and JavaFX presentation.

## Package Layout

| Path | Responsibility |
|---|---|
| `audio/c/` | C11 ABI, validation, and PCM16 WAV mixing |
| `audio/cpp/` | C++17 typed API and PCM16 WAV mixing |
| `audio/java/` | Java 21 orchestration and native-process boundary |
| `audio/native/` | Native media-processing boundary and build integration |
| `audio/gui/` | JavaFX audio/video presentation and integration |
| `audio/1-2-3-4.md` | Ordered implementation record, dates, completion state, and closure work |

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
- output and audio-length overflow protection.

The C and C++ APIs are intended to provide corresponding audio semantics.

## Java API

The Java 21 layer provides:

- `Audio` interface;
- `Input`, `Controls`, and `Configuration` records;
- `NativeAudio` process adapter;
- executable validation;
- configuration validation;
- process exit handling;
- interruption handling.

The Java layer is orchestration. Native C/C++ remains responsible for the actual parameterized media processing.

## GUI

The JavaFX layer provides the presentation and session boundary for the Audio/Video work.

It is intentionally separated from the native processing implementation so that command-line, service, and GUI consumers can use the appropriate layer.

## Native/Java Contract

The current Java process invocation is:

```
executable OUTPUT SAMPLE_RATE INPUT...
```

The Java configuration model additionally represents input offsets, input gain, EQ, master gain, pan, and channel gain.

Those additional fields are **not all transmitted by the current process invocation**. The complete parameterized behavior therefore remains authoritative in the native C/C++ APIs until the process protocol is expanded.

This distinction is documented deliberately so that the API does not claim a capability that the current process contract does not carry.

## EQ Status

The public API models contain:

- bass;
- mid;
- treble.

The current native mixer does not yet implement those EQ controls. They remain explicit closure items rather than being represented as completed functionality.

## Verification

The final conformance stage is defined in `audio/1-2-3-4.md`.

The required test matrix includes:

1. mono → stereo;
2. stereo → stereo;
3. synchronized offsets;
4. per-input gain;
5. master gain;
6. left/right gain;
7. pan;
8. clipping/saturation;
9. malformed WAV;
10. truncated WAV;
11. sample-rate mismatch;
12. invalid configurations;
13. one-input boundary;
14. 128-input boundary;
15. output-size overflow;
16. equivalent C/C++ PCM output for identical fixtures.

## Build

C:

```bash
cd audio/c
make clean all test
```

C++17:

```bash
cd audio/cpp
make clean all test
```

Java 21:

```bash
cd audio/java
javac -d build/classes src/module-info.java src/com/mearvk/sleela/audio/*.java
```

## Engineering Record

The Audio API work was developed in stages:

- **2026-09-22:** initial JavaFX Audio/Video GUI and Java API work.
- **2026-09-28:** GUI cleanup and native audio architecture.
- **2026-09-28:** C ABI, native build/documentation, process adapter, and GUI/native boundary.
- **2026-09-28:** C/C++/Java language-layer quality tightening.
- **2026-09-28:** final C WAV buffer-bookkeeping correction and overflow protections.
- **2026-09-28:** `audio/1-2-3-4.md` established as the ordered completion record.

## Current Status

**Implemented:** core native PCM16 WAV foundation, C/C++ APIs, Java orchestration boundary, JavaFX integration, validation, and documented build paths.

**Remaining closure:** complete the Java/native parameter protocol, implement or explicitly finalize EQ semantics, and run the full cross-language behavioral conformance matrix.

The package is therefore **implemented at its core and explicitly documented at its remaining boundaries**, rather than being described as complete before those final verification items are closed.
