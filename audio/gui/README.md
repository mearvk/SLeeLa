# SLeeLa Audio GUI

This directory is the first clean Java/JavaFX presentation layer for the SLeeLa
Audio/Video APIs.

## Purpose

The GUI deliberately sits above the native implementation:

    JavaFX
      |
      +-- audio/gui
      |     |
      |     +-- AudioMixerApp
      |     +-- AudioMixerModel
      |
      +-- future SLeeLa Java mapping
            |
            +-- native media
            +-- synchronized audio mixer
            +-- A/V providers

The JavaFX application presents the same concepts already defined by the native
SLeeLa mixer:

- Master, Second, and Live Input tracks.
- Relative quality.
- Timeline start offsets.
- Bass, Mid, Treble.
- Master gain and pan.
- Per-channel controls.
- Synchronized output.
- Audio waveform and video-frame preview surfaces.

The first GUI intentionally does not decode media itself and does not duplicate
the native mixer. AudioMixerModel is a presentation model that can later be
replaced or backed by a SLeeLa-to-Java mapping.

## Java and JavaFX

SLeeLa already has a Java integration layer, including the Java connector and
JavaFX host under gui/java/. The repository also contains a dedicated
JavaFX.md integration document.

For this A/V GUI, Java 21 and JavaFX 21 are the baseline. OpenJFX documents
JavaFX as a standalone component built on the JDK and supports Maven/Gradle
dependency management. The JavaFX 21 API is modular and should be supplied on
the module path.

## Run

From this directory:

    mvn clean javafx:run

This downloads the platform-specific JavaFX artifacts through Maven and starts
com.mearvk.sleela.audio.gui.AudioMixerApp.

The GUI is a presentation prototype. The Add File, Add Live Input, and Process
actions mark the native integration boundary; they do not replace the native
media provider or mixer.

## Future SLeeLa mapping

The intended progression is:

1. JavaFX controls and view model.
2. Clean Java API representing SLeeLa A/V operations.
3. SLeeLa language mapping to those Java operations.
4. Native C/C++ execution behind the mapping.
5. Optional direct SLeeLa GUI declarations once the language-level GUI mapping
   is mature.

That preserves a normal Java development experience while keeping SLeeLa as
the execution authority.
