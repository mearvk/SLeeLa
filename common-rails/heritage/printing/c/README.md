<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">





# CommonRails Heritage C Printing

This directory provides the C Heritage implementation of the CommonRails printing contract.

PrintLayout: canonical 80-column and 21x21 geometry.
PrintField and PrintLine: stable field padding and word-boundary wrapping.
PrintState and PrintFormatter: state vocabulary and semantic status lines.
PrintProgress: 0..100 normalization and 441-cell conversion.
PrintGlyphs and PrintRenderer: target-independent glyph and square rendering.
PrintComponent: canonical component records.
PrintWriter: output-target wrapper.

Run make check to execute the contract test.