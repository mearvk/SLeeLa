# CommonRails Heritage Printing

The Heritage layer gives C, C++, and Java implementations of the CommonRails printing contract. The implementations share semantic constants and rendering rules rather than depending on terminal color.

## Components

- PrintLayout — 80-column output, 10-column Object ID, 39-column Current field, and 21x21/441-cell progress geometry.
- PrintField — deterministic fixed-width field padding.
- PrintLine — fixed-width line emission with word-boundary wrapping.
- PrintFormatter — canonical state/status records.
- PrintState — START, WORKING, PROGRESS, COMPLETE, WARN, ERROR.
- PrintProgress — clamps percentages to 0–100 and maps them to 441 cells using floor semantics.
- PrintGlyphs — filled and empty progress glyphs.
- PrintComponent — canonical Object ID / Date / Current / Message component record.
- PrintRenderer — 21x21 progress-square rendering, filled from bottom-right to left and then upward.
- PrintWriter — output-target abstraction.
- PrintContractTest — boundary assertions for width, percentage normalization, and cell conversion.

## Language Heritage

C uses small translation-unit utilities and headers. C++ provides the same contract as a namespaced class/struct layer. Java provides the same contract as nested utility classes under com.sleela.commonrails.heritage.printing.

The implementations preserve the same observable rules so CommonRails output can be reproduced across the three Heritage languages.

## Contract

RequiredSymbols = DocumentedSymbols = TestedSymbols = AcceptedCoreSymbols applies to the printing vocabulary: every documented state and rendering rule has a corresponding implementation or contract assertion.

The authoritative design rules remain in BEAUTIFUL.DESIGN.PRINTING.md and the SLeeLa CommonRails source.
