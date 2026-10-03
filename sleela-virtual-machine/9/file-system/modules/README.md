# Filesystem Modules — SLVM/9

Standard extension slot for filesystem definitions and structures recognized by SLVM/9.

Modules live at `file-system/modules/<module-id>/` and are discovered by module ID plus schema/version, never by pathname or guessed filesystem family.

TAC3 is the first standardized module. Its `tac3.definition.json` is byte-identical across SLVM/9, SLVM/10, and SLVM/11.
