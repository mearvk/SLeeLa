artifact;
pure;
gasoline;

# lib/advantage/examples/primers — subject primers

One runnable primer per `advantage` subject, copied from the package's own
demonstrations so each subject can be run from a single place. Each primer is a
complete, self-validating example (ends in `ALL CHECKS PASSED`).

| Primer | Subject | Shows |
|--------|---------|-------|
| `onion-primer.sleela` | Sequitur onion model | the layered dividend process, user's and developer's models, dynamic update + restitute |
| `math-primer.sleela` | `math/` trade economics | joint profit maximization, Nash bargaining split, bilateral newsvendor critical ratio |
| `physics-primer.sleela` | `physics/` trust fields | trust metric, transactional geodesic, announcement contact structure, trust-decay SDE (a modelling analogy) |
| `integration-primer.sleela` | `integration/` vertical integration | recursive compartments, engineering bays, trials, newton median trays, sophistication score |
| `grading-primer.sleela` | `grading/` circuit grader | scoring modules and ranking winning / losing / grader / absolute sets A–D |
| `output-primer.sleela` | `output/` emit seam | routing output to terminal, CommonRails, a socket/connector, or silent capture |

Run any primer from the repository root:

```sh
./working-binaries/sleela run lib/advantage/examples/primers/math-primer.sleela
```

The primer's subject classes resolve from `lib/advantage/<subject>/` via the
driver's `/lib` identifier scan; no imports are needed. Each primer carries a
unique `*Primer` class name so it never collides with the package's own demo of
the same subject.
