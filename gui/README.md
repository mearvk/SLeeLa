<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">






# SLeeLa Audio GUI

Java 21 / JavaFX 21 presentation layer for SLeeLa Audio/Video.

Architecture:

    JavaFX → Java contract → native adapter → SLeeLa native audio → output

Implemented:
- Observable mixer state and per-track gain.
- Explicit synchronized-input contract.
- Native PCM16 mono/stereo WAV mixer under audio/native.
- Java native-process adapter with fail-closed diagnostics.
- Sample-rate and input validation.
- Dedicated Java and native CI.

The GUI does not claim live-device capture, arbitrary codecs, DSP analysis, or video processing until those native adapters exist.

Build Java:

    cd audio/gui
    mvn clean test

Build native:

    cd audio/native
    make clean all test

Run with native processing:

    mvn javafx:run -Dsleela.audio.native=/path/to/sleela-audio-native

Current native scope is deliberately narrow and testable: PCM16 WAV input/output, synchronized start offsets, per-input gain, master gain, and pan.

Next production adapters are platform device capture, broader codecs, real audio analysis/DSP, RGB/RGBA video frames, direct JNI where appropriate, and signed Linux/Windows/macOS packaging.

Version: Audio GUI 0.3.0; Native Audio 0.1.0

## GUI integration bridge (`native/`) and the SLeeLa `lib/gui` contract

Beyond the audio GUI above, this directory carries the **GUI integration layer**
that connects SLeeLa to a desktop host over three complementary paths — a Java
host, SLeeLa-owned GUI intent, and a shared native C ABI — plus a 1..14
document-change listener that refreshes a running window on an OS call. See
[`INTEGRATION.md`](INTEGRATION.md) and [`DOCUMENT_LISTENER.md`](DOCUMENT_LISTENER.md).

- **Native bridge (Path 3):** `native/sleela_gui_bridge.{h,cpp}` is the
  C-compatible callback boundary (`slgui_bridge_create` / `_call` /
  `_on_document_change` / `_document_changed` / `_destroy`). Build and test it:

      cd gui/native
      make clean all test

  The round-trip test (`slgui_bridge_test.cpp`) exercises create/call, the
  document-change refresh hook, NULL-argument normalization, and the documented
  `SLGUI_MIN_DOCUMENTS`..`SLGUI_MAX_DOCUMENTS` (1..14) bound, built with
  `-Werror`.

- **SLeeLa-facing contract:** the `.sleela` vocabulary a SLeeLa program uses to
  express this model lives in [`../lib/gui/`](../lib/gui/) — `SLGuiBackend`,
  `SLGuiWindow`, `SLGuiAction`, `SLGuiRuntime` (Path 2), `SLGuiHost` (Path 1),
  `SLGuiDocument`, `SLGuiDocumentListener`, `SLGuiBridge` (Path 3), and
  `SLGuiIntegration`, with a runnable `gui-demo.sleela`. See
  [`../lib/gui/ARCHITECTURE.md`](../lib/gui/ARCHITECTURE.md).

- **CI:** [`.github/workflows/gui-ci.yml`](../.github/workflows/gui-ci.yml)
  builds and tests the native bridge and validates the `lib/gui` source layer on
  every change under `gui/**` or `lib/gui/**`.