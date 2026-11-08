artifact;
pure;
gasoline;

# medium — one subject, a little machinery

Examples that take on a single subject of the package and exercise a bit of its
machinery — still focused, but with real computation behind the result.

| Topic | Folder | Examples | Shows |
|-------|--------|----------|-------|
| Trade economics | [`trade-economics/`](trade-economics/) | `math-primer.sleela`, `trader-example.sleela` | joint profit maximization, Nash bargaining split, bilateral newsvendor critical ratio; a trader finding `Q*` and a Nash-fair wholesale price (Q*=20, surplus=800, P*=60 splitting 400/400) |
| Output routing | [`output-routing/`](output-routing/) | `output-primer.sleela` | routing output to terminal, CommonRails, a socket/connector, or silent capture |
| Supply chains | [`supply-chains/`](supply-chains/) | `helper-example.sleela` | standing up a supply→depot→storefront chain and flowing 8 units one hop downstream |

Run from the repository root, e.g.:

```sh
./working-binaries/sleela run lib/advantage/examples/medium/trade-economics/trader-example.sleela
```
