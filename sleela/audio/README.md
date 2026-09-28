# SLeeLa Audio Language Layer

Updated 2026-09-28.

This directory is the SLeeLa-language source-of-truth layer for the Audio API. It defines the portable Audio object model and operating-system boundary above the existing C/C++/Java/native implementation.

There are **9 standard .sleela files** in the Audio API: Audio, AudioInput, AudioControls, AudioConfiguration, AudioNative, AudioDevice, AudioStream, AudioMixer, and AudioSystem.

The C counterpart is c/Audio.h + c/Audio.c. The C++ counterpart is cpp/Audio.hpp + cpp/Audio.cpp. Platform identification is implemented for Linux, macOS, and Windows; hardware enumeration remains a platform-driver responsibility.

The existing /audio implementation remains the rendering implementation. /sleela/audio is the language/API contract layer and does not replace the native mixer or Java/JavaFX API.
