# PDP-12 Registers and State

Separate shared machine state from mode-specific architectural state.

- **PDP-8-family state:** PC, AC, link, and any additional registers supported by the selected profile.
- **LINC-compatible state:** maintain the registers and sequencing state required by the selected LINC model; do not guess names or widths when unspecified.
- **IR:** internal instruction register, with the active decode environment recorded alongside it.
- **Memory state:** configured core-memory abstraction and address bounds.
- **I/O state:** pending device/function request and completion/error status.
- **Mode state:** active instruction environment and valid transition state.

Save/restore must preserve active mode, all relevant mode-specific state, memory, and pending I/O. Keep implementation-only latches separate from programmer-visible registers.