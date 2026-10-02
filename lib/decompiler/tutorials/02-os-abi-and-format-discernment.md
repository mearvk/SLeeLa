# Tutorial 02 — OS, ABI, and Format Discernment

## Evidence sources

Use format headers, ABI markers, imports/exports, relocation data, calling conventions, section names, runtime metadata, architecture instructions, symbol/debug metadata, and producer/compiler fingerprints.

## Evidence model

Individual evidence -> weighted observations -> compatible evidence set -> conflicts preserved -> discernment report.

A magic number is initial evidence, not proof of the complete environment.

If header evidence conflicts with imports or ABI details, preserve the conflict instead of silently overwriting it.

Seek agreement among architecture, object format, ABI, and operating-system evidence before assigning a strong platform conclusion.

Inspect OSDiscernment.sleela, LanguageReference.sleela, BinaryFormatReference.sleela, and SafetyReference.sleela.

Discernment is analysis and does not grant permission to load or execute the artifact.
