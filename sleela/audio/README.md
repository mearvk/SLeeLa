# SLeeLa Audio Language Layer

Updated 2026-09-28.

This directory is the SLeeLa-language source-of-truth layer for the Audio API. It sits above the existing C/C++/Java/native implementation and defines the portable Audio object model and operating-system boundary.

## Standard .sleela classes

There are **9 standard .sleela files** in the Audio API:

1. Audio.sleela
2. AudioInput.sleela
3. AudioControls.sleela
4. AudioConfiguration.sleela
5. AudioNative.sleela
6. AudioDevice.sleela
7. AudioStream.sleela
8. AudioMixer.sleela
9. AudioSystem.sleela

The first five mirror the established Audio API contract. AudioDevice, AudioStream, AudioMixer, and AudioSystem establish the reusable system/device/mixing boundary without pretending that a platform backend exists where it does not.

## Native counterparts

- c/Audio.h and c/Audio.c provide the C ABI counterpart for the nine contracts.
- cpp/Audio.hpp and cpp/Audio.cpp provide the C++ counterpart and object wrappers.
- The native layer validates the existing 128-input limit and the established controls contract.
- Platform identification is implemented for Linux, macOS, and Windows; actual hardware enumeration remains a driver/backend responsibility.

## Relationship to /audio

The existing /audio implementation remains the rendering implementation. /sleela/audio is the language/API contract layer; it does not replace the native mixer or the Java/JavaFX API.

## Standard boundary

SLeeLa class -> C ABI / C++ object -> Java/native adapter or platform backend -> audio implementation
