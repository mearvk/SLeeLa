# SLeeLa Audio GUI

Java 21 / JavaFX 21 presentation layer for SLeeLa Audio/Video.

## Architecture

- JavaFX owns presentation, controls, tables, and preview surfaces.
- AudioMixerModel owns observable presentation state.
- SleelaAudioVideo defines the Java-facing integration contract.
- SleelaAudioVideoSession validates configuration and forwards processing.
- The native SLeeLa runtime owns acquisition, decoding, synchronization, mixing, analysis, and output.

The GUI does not decode media or silently replace the native mixer.

## Current behavior

The application presents Master, Second, and Live Input tracks; per-track gain; bass, mid, treble, master gain, pan; synchronized-input state; waveform and video preview surfaces; and an explicit native integration boundary.

The waveform is a presentation preview. Processing is rejected until a native processor is connected.

## Build

Requirements: JDK 21, Maven 3.9+, and network access for Maven Central/OpenJFX dependencies.

From audio/gui:

    mvn clean test
    mvn clean package
    mvn javafx:run

The JavaFX run target requires a graphical environment. CI runs the headless compile/test target.

## Native integration

Construct SleelaAudioVideoSession with a native processor supplied by the SLeeLa runtime. The processor receives a complete validated MixConfiguration.

## Version

Audio GUI: 0.2.0
