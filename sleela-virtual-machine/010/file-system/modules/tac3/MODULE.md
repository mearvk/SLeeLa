# TAC3 Filesystem Module

The canonical machine-readable definition is `tac3.definition.json`, intentionally identical across SLVM/9, SLVM/10, and SLVM/11.

## Contract

- Discover by module ID and schema version.
- Validate format major/minor before interpretation.
- Verify identity, generation, integrity, and capabilities.
- Reject unknown mandatory semantics.
- Module presence never grants authority.
- Preserve TAC3 read-only reconstruction versus durable persistence.
- Preserve the complete 33-stat contextual identity model.
- FAT is optional; /swap is backing storage, not canonical filesystem truth.

Formulaically aligned with `Ubuntu.Determinant.Beta.Restricted/tools/tac3`; this is the SLeeLa integration definition, not a replacement for TAC3's source implementation.
