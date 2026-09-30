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

## Build

```sh
make
make test
```