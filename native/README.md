<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">






# SLeeLa Native Audio

C++17 native audio foundation behind the JavaFX Audio GUI.

## Implemented

- Stable C ABI in `include/sleela_audio.h`.
- PCM16 mono/stereo WAV input.
- Explicit sample-rate validation.
- Per-input synchronized start offset.
- Per-input gain.
- Master gain and pan.
- Stereo PCM16 WAV output.
- Bounded input count and fail-closed validation.

The GUI invokes the native command adapter rather than pretending that JavaFX itself is the mixer.

## Deliberate boundaries

Live-device capture, non-WAV codecs, DSP EQ, audio analysis, video frames, and platform-specific capture remain explicit adapters. Unsupported capabilities are rejected or remain unavailable rather than being simulated as completed native functionality.

# SLeeLa Native Opcode Bridge

C++17 fetch/dispatch primitives behind the `lib/opcodes` single-opcode classes.

- Stable C ABI in `include/sleela_opcode.h`.
- Canonical mnemonic table of all **103 opcodes** (codes 0–102; base 98 at 0–97).
- `sleela_opcode_vm_next` — the fetch (advance the instruction pointer).
- `sleela_opcode_execute_one` — dispatch exactly one opcode.
- Unknown opcodes are rejected (never a silent no-op); `OP_HALT` stops the stream.

The authoritative execution semantics remain in `/impl/core`; this bridge models
the same one-op-at-a-time discipline so a SLeeLa opcode object can request the
fetch and then its single dispatch.

# SLeeLa Native Opcode-Governance Bridge

C++17 phase/ordering primitives behind the `lib/opcodes/governance` series
(`SLOpcodeRegistrar`, `SLOpcodeListener`, `SLOpcodeEventObserver`,
`SLGovernedExecution`).

- Stable C ABI in `include/sleela_gov.h`.
- `sleela_gov_announce_phase` — notify SLeeLa and the VM of BEFORE / DURING / AFTER.
- `sleela_gov_live_ip` — the live instruction pointer the Listener hears during execution.
- `sleela_gov_current_phase` / `sleela_gov_phase_count` — phase tracking.

The governance *policy* lives in SLeeLa source; this bridge only announces the
phases so both SLeeLa and the VM listen for ordering, and reports the live ip.
Governance never executes an opcode itself — execution stays in `/impl/core`.

## Build

```sh
make
make test
```

`make test` builds and self-tests the audio, opcode, and governance bridges.