# Advantage™ modules

A way to load AI components into a **user defined program** through a **medium**
and a **bridge**, with conversation in both directions.

```
 user defined program ──loads──▶ SLAdvMedium  (the middle, the center)
        ▲                           │ owns            holds up to 16 modules
        │ events / replies          ▼
        └────────────────────  SLAdvBridge  (the apparent bridge)
                                    │ routes both ways
              ┌─────────────────────┼─────────────────────┐
        high-speed-trader     moral-evaluator       any user defined module
        (SLAdvTraderModule)   (SLAdvMoralModule)    (implements SLAdvModule)
```

## The parts

| Class | Role |
|---|---|
| `SLAdvModule` (interface) | What any module offers: `moduleName`, `moduleKind`, `moduleVersion`, `begin(bridge)`, `handle(message)`, `end`, `running` |
| `SLAdvListener` (interface) | The program callback: `onMessage(message)` |
| `SLAdvMessage` | The envelope: id, from, to, topic, text, up to 48 numbers; kind 0 request, 1 reply, 2 event, 3 error |
| `SLAdvBridge` | Two bounded queues (1024 each): down to modules, up to the program. A refused post returns -1 and is counted |
| `SLAdvMedium` | The center: `begin`, `load(name)`, `attach(module)`, `unload`, `ask`, `tell`, `pump`, `attachListener`, `shutdown` |
| `SLAdvCatalog` | Names the medium can load: `high-speed-trader`, `moral-evaluator` |
| `SLAdvTraderModule` | The high speed trader as a module (`trader/`) |
| `SLAdvMoralModule` | The Moral Evaluator as a module (`moral/`) |

## How a program uses it

```
SLAdvMedium med = new SLAdvMedium();
med.begin();
med.attachListener(myListener);        < modules can speak to the program >
med.load("moral-evaluator");           < load only what you want >
med.load("high-speed-trader");
SLAdvMessage m = new SLAdvMessage();
m.addressed("program", "moral-evaluator", "judge", 0);
m.setNum(0, 1.0);                      < ... the 32 trade facts ... >
SLAdvMessage reply = med.ask(m);       < program to module, and the reply back >
med.pump();                            < hands waiting module messages to the listener >
```

A complete outside-`lib` example is
[`advantage/artificial-intelligence/user-program/UserProgram.sleela`](../../../advantage/artificial-intelligence/user-program/UserProgram.sleela).

## Both directions

- **Program to module:** `ask` (waits for the reply) or `tell` (fire and forget).
- **Module to program:** replies, and unsolicited **events** (`ready`, `halted`,
  `cardinal-veto`, `trade-judged`), delivered to the listener on `pump`.
- **Module to module:** the trader can be `link`ed to the evaluator; each accepted
  trade is sent on for judgement and the verdict comes back. A NO withdraws the trade.

## Safety

- Reserved names `program` and `medium`; names must be unique; max 16 modules.
- A message to an unknown module gets an error reply (status 404); an unknown
  topic gets status 2.
- Queues are bounded; a pump handles at most 100,000 messages, so two modules
  that keep answering each other cannot hang the program (`wasSaturated()`).
- After `shutdown` the bridge refuses every post.

## Topics

**high-speed-trader** — `configure` (n0 pragmatics 8..24, n1 trades/min), `evaluate`
(n0..n9 candidate, n10 now ms), `simulate` (n0 seconds 1..600, n1 seed), `insight`,
`halt`, `rearm`, `status`, `link` (text = module to judge with), `set-facts`.

**moral-evaluator** — `judge` (n0..n31 trade facts in `SLMoralTrade` wire order),
`configure` (n0 points 5..41, n1 states 2 or 3), `seed-catalog`, `register`
(n0 list, text entry), `review`, `ledger`, `status`.

## The two AI modules in brief

- **High speed trader** (`trader/`): 24 pragmatics in three phases (logic, use,
  ground); you choose 8..24 by priority order. Hard minimums veto; otherwise a
  weighted score, a rate governor (target trades per minute), online weight
  learning, a drawdown halt, and a bounded human `insight` channel. Synthetic data.
- **Moral Evaluator** (`moral/`): up to 41 yes/no points in five dimensions
  (trades, time, listers, United States, Tall Science). Five cardinal points
  (life safe, human is the end, consent, no deception, lawful subject) are
  vetoes; then a floor of 0.5 per dimension; then merit against a threshold.
  Verdicts NO / YES / COMPLETE. `SLTallScience` holds the United States as a
  cataloged Item with paired nouns and names; no sale, no profiling.

## Run

```sh
export SLEELA_HOME=<repo root>
cd lib/advantage/modules/demo   && $SLEELA_HOME/working-binaries/sleela run AdvantageModulesDemo.sleela   # 46 checks
cd ../trader                    && $SLEELA_HOME/working-binaries/sleela run HstDemo.sleela                # 31 checks
cd ../moral                     && $SLEELA_HOME/working-binaries/sleela run MoralDemo.sleela              # 39 checks
```

## Honest limits

- Synthetic data throughout; nothing places an order or opens a connection.
- Moral points are rule-based teaching choices on facts the caller supplies.
- `ask` is synchronous (the bridge is routed on the caller thread).
- Interfaces carry no Metadata block; every class does.
