artifact;
pure;
gasoline;

# onion-model

The Sequitur onion model from first principles.

- `onion-primer.sleela` — exercises the onion end to end: the layered dividend
  process, the user's and developer's models, and dynamic update + restitute.
  Links the sibling classes in `lib/advantage/` (`SLAdvantageLayer`,
  `SLAdvantageDividend`, `SLAdvantageSequitur`, …) via the driver's `/lib`
  scan, so no imports are needed.

```sh
./working-binaries/sleela run lib/advantage/examples/novice/onion-model/onion-primer.sleela
```
