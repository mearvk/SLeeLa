# SLeeLa + JavaFX

## Purpose

JavaFX is a strictly Java presentation layer while SLeeLa remains authoritative for business operations, computation, media processing, and synchronization.

The common connector system allows JavaFX applications to call SLeeLa through a local process, Java RMI, or HTTP. The same architectural rule now applies to the Audio/Video GUI under audio/gui/.

## Architecture

```text
                 SLeeLa
                    |
          SLeeLa Java Connector
                    |
       +------------+------------+
       |            |            |
     Process       RMI          HTTP
       |            |            |
       +------------+------------+
                    |
               JavaFX UI
                    |
              audio/gui/
                    |
        +-----------+-----------+
        |                       |
   AudioMixerModel       A/V Preview
        |
   future SLeeLa mapping
        |
 native media + synchronized mixer
```

The architectural rule is:

> **JavaFX presents; SLeeLa decides.**

## Audio/Video JavaFX Example

The first clean A/V presentation prototype lives at audio/gui/.

It provides:

- Master, Second, and Live Input tracks.
- Relative quality and timeline offsets.
- Bass, Mid, Treble, Master Gain, and Pan controls.
- Left/right channel controls.
- Synchronized-output action surface.
- Audio waveform preview.
- Video-frame preview surface.
- A Java-only presentation model with no native decoding or mixer duplication.

Run it from audio/gui/ with:

```bash
mvn clean javafx:run
```

The GUI intentionally stops at the integration boundary. The native SLeeLa media provider and synchronized mixer remain responsible for decoding, mixing, synchronization, and output.

## Java Baseline

The A/V GUI uses Java 21 and JavaFX 21 as its baseline. OpenJFX documents JavaFX as a standalone component built on the JDK, with Maven and Gradle support, and JavaFX 21 exposes its UI APIs through named JavaFX modules.

## Common Connector API

The common connector package is:

`com.mearvk.sleela.connector`

The JavaFX layer should normally depend on:

- `SleelaJavaConnector` — transport-neutral Java connector interface.
- `SleelaInvocation` — operation and textual argument envelope.
- `SleelaResult` — normalized success/error result.

A JavaFX controller can therefore use the same call shape regardless of transport.

## Future SLeeLa Mapping

The intended progression for the A/V GUI is:

1. Clean JavaFX controls and presentation model.
2. Clean Java A/V API representing SLeeLa operations.
3. SLeeLa language mapping to those Java operations.
4. Native C/C++ execution behind the mapping.
5. Optional direct SLeeLa GUI declarations once the language-level GUI mapping is mature.

This preserves a normal Java development experience now while leaving a direct SLeeLa-to-Java-to-JavaFX path open for the language.

## Design Principle

The connector layer separates three concerns:

- **Presentation:** JavaFX.
- **Integration:** Java connector and future SLeeLa Java mapping.
- **Authority:** SLeeLa native media/mixer/runtime.

This makes JavaFX a clean desktop experience without turning the presentation layer into a second audio engine.
