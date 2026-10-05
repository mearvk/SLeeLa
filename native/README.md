<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">






# SLeeLa Native Audio

C++17 native audio foundation behind the JavaFX Audio GUI.

## Implemented

- Stable C ABI in `include/sleela_audio.h`.
- PCM16 mono/stereo WAV input.
- Explicit sample-rate validation.
- Per-input synchronized start offset.
- Per-input gain.
- Master gain and pan.
- Stereo PCM16 WAV output.
- Bounded input count and fail-closed validation.

The GUI invokes the native command adapter rather than pretending that JavaFX itself is the mixer.

## Deliberate boundaries

Live-device capture, non-WAV codecs, DSP EQ, audio analysis, video frames, and platform-specific capture remain explicit adapters. Unsupported capabilities are rejected or remain unavailable rather than being simulated as completed native functionality.

# SLeeLa Native Opcode Bridge

C++17 fetch/dispatch primitives behind the `lib/opcodes` single-opcode classes.

- Stable C ABI in `include/sleela_opcode.h`.
- Canonical mnemonic table of all **103 opcodes** (codes 0–102; base 98 at 0–97).
- `sleela_opcode_vm_next` — the fetch (advance the instruction pointer).
- `sleela_opcode_execute_one` — dispatch exactly one opcode.
- Unknown opcodes are rejected (never a silent no-op); `OP_HALT` stops the stream.

The authoritative execution semantics remain in `/impl/core`; this bridge models
the same one-op-at-a-time discipline so a SLeeLa opcode object can request the
fetch and then its single dispatch.

## Build

```sh
make
make test
```

`make test` builds and self-tests the audio and opcode bridges.