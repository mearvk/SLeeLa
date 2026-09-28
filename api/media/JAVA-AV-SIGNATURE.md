# SLeeLa Java Audio/Video Signature

This document defines the clean Java-facing signature for the SLeeLa Audio/Video
API. It is an integration contract for Java and JavaFX; the native C/C++ media
and mixer implementation remains authoritative.

## Core Java types

Package:

`com.mearvk.sleela.audio.gui`

Primary interface:

`SleelaAudioVideo`

### Input

```java
record Input(
    String id,
    TrackRole role,
    SourceType sourceType,
    String source,
    long loadAtNs,
    long startAtNs,
    double quality,
    double gainDb
)
```

### Mixer controls

```java
record MixerControls(
    double bassDb,
    double midDb,
    double trebleDb,
    double gainDb,
    double pan,
    List<Double> channelGains
)
```

### Synchronized configuration

```java
record MixConfiguration(
    int sampleRate,
    List<Input> inputs,
    MixerControls controls,
    Path output
)
```

### Analysis

```java
record AudioLevel(
    double level,
    double peak,
    double rms,
    double dominantHz
)

record VideoLevel(
    int width,
    int height,
    double motion,
    double luminance,
    double edgeDensity
)
```

## Operations

```java
boolean validate(MixConfiguration configuration);
SleelaAudioVideo withInput(Input input);
SleelaAudioVideo withControls(MixerControls controls);
void processTo(Path output);
AudioLevel audioLevel();
VideoLevel videoLevel();
```

## Mapping rule

The Java signature is intentionally small. It maps to:

- `SLMediaFrame` for timestamped native audio/video frames.
- `SLAudioMixerControls` for EQ, gain, pan, and channel controls.
- `SLAudioTrackSpec` for native track admission.
- JSON, BODI XML, or standard text mixer configuration for persisted configuration.
- The native synchronized mixer for processing and output.

JavaFX should consume this Java-facing contract rather than reimplementing
codec, device, synchronization, or mixing logic.

## Source of truth

The authoritative implementation remains:

- `impl/core/sleela_media.h/.c`
- `impl/core/sleela_media_formats.h/.c`
- `impl/core/sleela_audio_mixer.h/.c`

The Java signature is the clean interoperability surface above those native
interfaces.
