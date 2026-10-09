# DEC PDP-12 CPU

SLeeLa profile for the DEC PDP-12, a laboratory computer combining PDP-8-family computing with LINC-compatible capabilities.

- **Status:** profile-driven functional foundation; not a cycle-accurate emulator.
- **Base word model:** 12-bit PDP-8-family data and instruction words.
- **Distinctive feature:** a selectable LINC instruction environment alongside the PDP-8-family instruction environment.
- **Configuration:** machine revision, memory capacity, instruction mode, I/O devices, and timing are profile settings.
- **Compatibility:** LINC and PDP-8 instruction decoding must remain distinct and explicitly selected.

This is a dual-environment model. It must not treat every instruction encoding as belonging to one unified, invented instruction set.