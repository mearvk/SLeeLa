artifact;
pure;
gasoline;

# trade-economics

The `math/` subject: the economics of a two-party trade.

- `math-primer.sleela` — joint profit maximization, the Nash bargaining split,
  and the bilateral newsvendor critical ratio. Links the sibling subject
  classes in `lib/advantage/math/` (`SLTradeMath`, `SLJointProfitMax`,
  `SLNashBargaining`, `SLBilateralNewsvendor`).
- `trader-example.sleela` — a trader coordinates a buyer and a seller as one
  system, finds the joint-profit optimum `Q*`, and strikes a Nash-fair
  wholesale price (Q*=20, surplus=800, P*=60 splitting 400/400).

```sh
./working-binaries/sleela run lib/advantage/examples/medium/trade-economics/trader-example.sleela
```
