<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">







# SLeeLa Audio Language Layer

Updated 2026-09-28.

The audio/sleela/ directory is the SLeeLa source-of-truth contract layer for Audio. These files use SLeeLa syntax 1.3 and lower through the SLVM rather than bypassing it.

## Execution model

    .sleela source
       -> SLeeLa lexer/parser/semantic analysis
       -> SLeeLa compiler
       -> SLVM bytecode
       -> VM-owned Audio job handle
       -> C SLVM Audio bridge
       -> registered C ABI callback
       -> C++ Audio renderer
       -> operating-system facilities

SLeeLa code never receives a C pointer, C++ object pointer, file descriptor, or OS device handle. The VM exposes only integer handles and typed scalar values.

## Standard .sleela classes

1. Audio.sleela — top-level validate/mix/close facade.
2. AudioConfiguration.sleela — creates and configures VM-owned jobs.
3. AudioControls.sleela — validates and applies mixer controls.
4. AudioDevice.sleela — portable device identity/compatibility boundary.
5. AudioInput.sleela — input path/offset/gain validation.
6. AudioMixer.sleela — handle-oriented mixer facade.
7. AudioNative.sleela — native renderer boundary and platform reporting.
8. AudioStream.sleela — portable stream contract.
9. AudioSystem.sleela — operating-system platform boundary.

All nine contain a #sleela 1.3 declaration and executable method bodies. No file relies on unsupported array types, declaration-only methods, or cross-class type names that the current compiler cannot lower.

## Native bridge

The SLVM core provides bounded Audio job operations:

- audioNew(sampleRate, outputPath)
- audioAdd(handle, path, startSeconds, gainDb)
- audioControls(handle, bassDb, midDb, trebleDb, masterGainDb, pan, leftGain, rightGain)
- audioValidate(handle)
- audioRender(handle)
- audioClose(handle)
- audioPlatform()

The VM stores up to 64 simultaneous jobs, with up to 128 inputs per job. Native paths are copied into VM-owned storage and are never exposed as raw pointers to SLeeLa source.

Audio rendering dispatches through the registered SLAudioNativeRenderFn. The standard SLeeLa executable registers sleela_audio_native_render_bridge, which converts the VM job into the existing C++ sleela::audio::Config and invokes mix_wav().

## C and C++

The boundary is intentionally layered:

- impl/core/sleela_core.c owns VM handles, validation, opcode dispatch, and lifecycle.
- impl/core/sleela_audio_bridge.cpp is the controlled C++ bridge.
- audio/cpp/src/sleela_audio.cpp performs WAV decoding, synchronized mixing, gain/pan processing, clipping, and RIFF output.
- audio/c/ remains the stable C ABI counterpart.
- impl/core/sleela_audio_mixer.c remains the in-VM float mixer for real-time/buffer-oriented use.

This prevents the language layer from pretending that C++ implementation details are part of the SLeeLa source model.

## Hardware and OS boundary

AudioSystem.sleela reports the host platform through the SLVM. Hardware enumeration and device ownership remain driver/backend responsibilities. A platform name is not treated as proof that a physical device is available.

## Conformance rule

A green compile of the nine files is necessary but not sufficient. Runtime conformance must also verify:

- 1 and 128 input jobs;
- mono and stereo WAV;
- synchronized offsets;
- per-input gain;
- master/left/right gain;
- pan;
- malformed/truncated WAV rejection;
- sample-rate mismatch rejection;
- output/RIFF overflow protection;
- VM handle exhaustion and close/reuse;
- C/C++ output equivalence where both paths are intentionally exercised.

EQ fields remain part of the SLeeLa contract. The existing C++ WAV renderer currently transports bass/mid/treble values but does not apply its EQ effect; the in-VM float mixer has an explicit EQ processor. These are kept distinct rather than silently claiming identical semantics.