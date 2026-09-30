<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">

<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">




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