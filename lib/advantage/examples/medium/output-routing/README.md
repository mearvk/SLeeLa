artifact;
pure;
gasoline;

# output-routing

The `output/` emit seam: where the model's output goes.

- `output-primer.sleela` — routes output to the terminal, CommonRails, a
  socket/connector, or silent capture. Links the sibling classes in
  `lib/advantage/output/` (`SLAdvantageSink`, `SLAdvantageEmitter`); naming
  `PrintRail` pulls in `lib/common-rails`.

```sh
./working-binaries/sleela run lib/advantage/examples/medium/output-routing/output-primer.sleela
```
