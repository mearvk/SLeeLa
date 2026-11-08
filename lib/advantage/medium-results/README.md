artifact;
pure;
gasoline;

# lib/advantage/medium-results — connect + control + execute over US open exchange

Worked SLeeLa source proving the new connectivity medium
([`lib/advantage/connect/`](../connect/)) and the network subject
([`lib/advantage/network/`](../network/)) can **connect, control, and execute**
trades over open exchange in the United States. Each example follows one spine:

1. **Connect** — a national clock (`SLClockConnector`) and an exchange venue
   (`SLNetConnector`).
2. **Control** — scout a venue network (`SLMarketScout`) and negotiate a fair
   clearing price (`SLNegotiationRound`, or the `math/` Nash solution).
3. **Execute** — a cash-flow receipt (`SLCashflow`) settled through a money
   source (`SLMoneyConnector`) and recorded to a database (`SLSqlConnector`).

| Example | Theme | What it shows |
|---------|-------|---------------|
| `business-exchange.sleela` | **Business** | a US small business runs the full connect → control → execute → record loop (in 4500 → out 5000, +500) |
| `trade-exchange.sleela` | **Trade** | cross-venue commodity routing (lumber) with a settlement fee (in 14000 → out 15106, +1106) |
| `economics-exchange.sleela` | **Economics** | maximize the joint surplus then split it fairly (Q*=20, pool 800, Nash 60 splitting 400/400) |
| `youthfulness-exchange.sleela` | **Youthfulness** | a guardrailed first trade: buyer-favored price + a hard spend cap that turns an over-cap spend into a safe, non-retryable decline |

Every example is self-validating and ends in `ALL CHECKS PASSED`.

> `artifact; pure; gasoline;` — illustrative Advantage™ examples with best-guess
> numbers. **No live database, clock, socket, or bank is dialed from inside the
> SLeeLa VM**: every operation that needs a host adapter returns status code 9
> (`NATIVE_ADAPTER_REQUIRED`), no secret lives in SLeeLa (money uses a keystore
> alias; SQL a redacted coordinate), and money is integer cents. This is not
> financial, investment, or legal advice; the youthfulness example is an
> educational guardrail pattern.

```sh
./working-binaries/sleela run lib/advantage/medium-results/business-exchange.sleela
```
