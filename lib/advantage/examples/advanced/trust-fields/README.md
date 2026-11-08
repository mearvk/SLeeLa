artifact;
pure;
gasoline;

# trust-fields

The `physics/` subject: trust modelled as a field (a mathematical analogy).

- `physics-primer.sleela` — the trust metric, the transactional geodesic, the
  announcement contact structure, and the trust-decay SDE. Links the sibling
  subject classes in `lib/advantage/physics/` (`SLPhysicsMath`,
  `SLTrustMetric`, `SLTransactionalGeodesic`, …).
- `healer-example.sleela` — a healer restores trust over time and measures a
  breach via the CIR trust-decay process (reverts 0.3→0.76 toward 0.8; a breach
  drops 0.8→0.4).

> The trust-field "physics" is a mathematical modelling analogy, not a physical
> law.

```sh
./working-binaries/sleela run lib/advantage/examples/advanced/trust-fields/physics-primer.sleela
```
