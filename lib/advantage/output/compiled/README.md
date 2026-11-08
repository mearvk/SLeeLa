artifact;
pure;
gasoline;

# lib/advantage/output/compiled — compiled Core artifacts

Runnable SLeeLa Core artifacts (`.slart`) compiled from the `advantage`
package's five subject demos, laid out in a directory structure that mirrors
the source tree:

| Compiled artifact | Source demo |
|-------------------|-------------|
| `advantage-demo.slart` | `../../advantage-demo.sleela` (onion model) |
| `math/trade-math-demo.slart` | `../../math/trade-math-demo.sleela` |
| `physics/physics-demo.slart` | `../../physics/physics-demo.sleela` |
| `integration/integration-demo.slart` | `../../integration/integration-demo.sleela` |
| `grading/grading-demo.slart` | `../../grading/grading-demo.sleela` |
| `output/output-demo.slart` | `output-demo.sleela` (this subject) |

Each was produced with `sleela compile <demo>.sleela -o <artifact>.slart` and is
a runnable Core artifact (opaque `SLEELA-ART` binary, with a `.annotations`
sidecar). Run one with `sleela run <artifact>.slart`; all six pass their
`ALL CHECKS PASSED` validation.

The `.slart` extension (not `.sleela`) marks these as build output, not source
vocabulary, so the library-inventory generator does not count them as source
units. Only the demos (which carry a `main` and auto-link their subject
siblings) can be compiled; the individual library classes have no `main`.
