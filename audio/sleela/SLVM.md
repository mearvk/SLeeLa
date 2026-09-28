# SLVM Audio Boundary

## Purpose

The Audio language layer is not a second runtime. .sleela source is compiled to SLVM bytecode, and Audio native work is entered only through VM-owned handles.

## Boundary invariant

    SLeeLa source -> SLVM -> C ABI -> C++ adapter -> OS/backend

At no point does source-level SLeeLa receive a C pointer, a C++ object pointer, a raw file descriptor, or a native socket/device handle. The VM stores bounded job state and passes validated copies to the native callback.

## Job lifecycle

1. audioNew() allocates a VM-local job.
2. audioAdd() appends an input, capped at 128.
3. audioControls() records the complete control surface.
4. audioValidate() performs VM-side validation.
5. audioRender() dispatches the complete job to the registered renderer.
6. audioClose() releases the VM job.

The VM job table is bounded at 64 jobs. Exhaustion fails closed.

## Renderer

The standard executable registers sleela_audio_native_render_bridge(). The bridge translates the C ABI job into sleela::audio::Config and invokes the existing C++ mix_wav() implementation.

## Contract ownership

- .sleela: language contract.
- impl/core: VM lifecycle and safety boundary.
- impl/core/sleela_audio_bridge.cpp: C-to-C++ transition.
- audio/cpp: concrete WAV implementation.
- audio/c: stable C ABI.
- driver/backend code: physical device enumeration and platform-specific hardware.

**Max Rupplin — MEARVK LLC — 2026**
