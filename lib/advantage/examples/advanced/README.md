artifact;
pure;
gasoline;

# advanced — richer, multi-concept subjects

Examples over the package's richer subjects, where several concepts combine and
one of the models is an explicit mathematical analogy rather than a literal law.

| Topic | Folder | Examples | Shows |
|-------|--------|----------|-------|
| Trust fields | [`trust-fields/`](trust-fields/) | `physics-primer.sleela`, `healer-example.sleela` | the trust metric, transactional geodesic, announcement contact structure, and trust-decay SDE; a healer restoring trust over time and measuring a breach via the CIR process (reverts 0.3→0.76 toward 0.8; breach drops 0.8→0.4) |
| Vertical integration | [`vertical-integration/`](vertical-integration/) | `integration-primer.sleela` | recursive compartments, engineering bays, trials, newton median trays, and a sophistication score |

> The trust-field "physics" is a mathematical modelling analogy, not a physical
> law.

Run from the repository root, e.g.:

```sh
./working-binaries/sleela run lib/advantage/examples/advanced/trust-fields/physics-primer.sleela
```
