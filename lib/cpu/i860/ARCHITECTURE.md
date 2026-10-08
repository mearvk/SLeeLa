# Intel i860 Architecture

## Execution path

SLI860CPU
-> fetch
-> decode/pairing
-> integer / address execution
-> floating-point / vector execution
-> memory
-> completion

## Dual execution

SLeeLa represents the i860's instruction pairing and execution constraints explicitly rather than applying a generic superscalar model.

## Profiles

XR and XP are separate profiles. Cache, MMU, pipeline, bus, and floating-point/vector details remain implementation-specific where they differ.
