<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# high-speed-trader: Tutorial

| | |
|---|---|
| Nature | Advantage module (lib/advantage/modules, not in the sleelactl registry) |
| Registry summary | none (not in the sleelactl registry). `SLAdvCatalog` lists it as `high-speed-trader`; module kind `ai.trader`, version `1.0.0` (`trader/SLAdvTraderModule.sleela`). |
| Lives in | `lib/advantage/modules/trader/` (`SLHighSpeedTrader`, `SLHstMarket`, `SLHstPragmatics`, `SLAdvTraderModule`, `HstDemo.sleela`). Courseware: `advantage/artificial-intelligence/04`, `05`, `06`. |
| Writeup type | Tutorial, because you drive it with `sleela run`, both directly (`HstDemo.sleela`) and over the bridge from your own program, and every step below ran successfully. |
| Verified | 2026-11-13 against `master` e048707fd (re-run by a second pass; outputs identical except wall-clock `evals/s`): `HstDemo.sleela` printed `checks passed=31 failed=0`; the host demo's trader sections passed (46/46); a scratch program `TraderTour.sleela` drove the module over the bridge (output below). |

## What it is

A **courseware model** of a high speed trader: pure computation on synthetic data. It places no orders and opens no connection (`SLHighSpeedTrader.sleela` header: "no orders, no network"; README "Honest limits"). It is **not** financial advice, not investment guidance and not a trading system; nothing here says what anyone should buy or sell, and the "threshold", "pnl" and "learning" figures describe a synthetic random stream only; its numbers come from a deterministic random generator (`SLHstMarket`), not from any market.

For each candidate trade (10 numbers) it computes 24 features, each clamped to 0..1, and applies the first P of 24 "pragmatics" in a priority order (you choose P from 8 to 24). The pragmatics come in three phases: logic (L1..L8), use (U1..U8) and ground (G1..G8). Then:

1. A feature below its **hard minimum** is a veto (the reject code is 10 plus the pragmatic's id).
2. Otherwise the weighted mean of the active features must reach a **threshold**.
3. Otherwise a **token bucket** (rate governor) must have a token for the target trades per minute.

Each simulated second the threshold moves by 0.01 to chase the target rate (bounded 0.30..0.98). After an accepted trade in simulation, `learn(realizedBps)` nudges each active weight up or down (bounded 0.25..4.0) and updates a regime prior. If drawdown reaches 95% of its limit the trader **halts** and only a person can `rearm` it. Sources: `SLHighSpeedTrader.sleela` lines 195-343, `SLHstPragmatics.sleela`.

The candidate layout is `o[0..9]` = edgeBps, costBps, evidenceN, spreadBps, volBps, ageMs, qty (fraction of max order), positionFrac, venueTrust, depthRatio (comment at `SLHighSpeedTrader.sleela` line 55).

## Before you start

```sh
cd SLeeLa            # the repository root
export SLEELA_HOME=$PWD SLEELA_LIB=$PWD/lib SLEELA_SHEET=$PWD/SHEET.sheet \
       SLEELA_SHA256_MANIFEST=$PWD/security/sha256-manifest.json
```

Every `run` prints the SHA-256 gate (`verification: PASS (18667 item(s))`) first and three `[memory-manager]` lines last; they are omitted below. Wall-clock figures (`evals/s`) vary by machine.

## Step 1 - Run the shipped demo

```sh
cd $SLEELA_HOME/lib/advantage/modules/trader
timeout 120 $SLEELA_HOME/impl/build/sleela run HstDemo.sleela
```

Real output (55 lines, exit 0), abridged:

```
=== High speed trader courseware model (synthetic data, no orders, no network) ===
-- 1. fail closed configuration --
  PASS  7 pragmatics refused
  PASS  25 pragmatics refused
  PASS  zero trades per minute refused
  PASS  unconfigured trader rejects everything
  PASS  unconfigured trader reports code 1
-- 2. the minimal safe core and the full set --
  P=8  evals/s=118121  accepted(120 sim s)=3663  last full minute=1765  max minute=1898  threshold=0.97  pnl(bps)=38693.5
    vetoes by phase: logic=223306 use=5638 ground=25397  score-rejects=306412  rate-rejects=35584
  PASS  P=8 sustains at least 5000 evaluations per second
  ...
  P=16  evals/s=110018  accepted(120 sim s)=3658  last full minute=1773  max minute=1885  threshold=0.93  pnl(bps)=39840.8
    vetoes by phase: logic=223281 use=5640 ground=41694  score-rejects=295651  rate-rejects=30076
  ...
  P=24  evals/s=99863.5  accepted(120 sim s)=3811  last full minute=1894  max minute=1917  threshold=0.91  pnl(bps)=63947.1
    vetoes by phase: logic=223140 use=5629 ground=42776  score-rejects=295216  rate-rejects=29428
  ...
-- 3. evolution: learning on versus off (same stream, P=24) --
  learning ON                    : trades=3811  pnl(bps)=63947.1  mean=16.7796  halted=false
  learning OFF (guard armed)     : trades=159  pnl(bps)=-376.221  mean=-2.36617  halted=true
  learning OFF (guard disabled)  : trades=4006  pnl(bps)=39317.7  mean=9.8147  halted=false
  weight of L5 plausibility: 4  of L4 evidence: 4
  PASS  learning ON earns a higher mean per trade than learning OFF with the guard disabled
  PASS  the drawdown guard halts a trader that is losing
-- 4. the brilliant person channel --
  PASS  insight 1.5 on L5 accepted
  PASS  insight 5.0 refused (out of range)
  ...
  PASS  insight cannot unlock the kill switch
  PASS  a person can re-arm
-- 5. guards a candidate must clear --
  PASS  baseline candidate accepted
  PASS  malformed input vetoed by L1
  PASS  position at limit vetoed by G4
  PASS  untrusted venue vetoed by G1
  PASS  oversize vetoed by U1
  PASS  negative net edge vetoed by L2
=== checks passed=31 failed=0 ===
```

Reading it: each simulated second carries 5000 candidates (`evalsPerSimSecond`, `HstDemo.sleela` line 55), so 120 seconds is 600,000 candidates and about 3,700 acceptances, roughly 0.6%. Nearly all rejections are score rejects and logic-phase vetoes. The pnl figures are in "bps" of the synthetic stream; they say nothing about real markets. The demo's own check is that learning ON beats learning OFF *with the guard disabled* on the same stream (mean 16.78 vs 9.81 per trade), and that the guarded OFF trader halts.

Standalone, this demo needs only the four trader classes. Copied alone into a scratch directory with the default `SLEELA_LIB`, it also gave `checks passed=31 failed=0`.

## Step 2 - Drive the trader over the bridge from your own program

The module `high-speed-trader` is loaded by name from the catalog. Its topics (README; `SLAdvTraderModule.sleela` line 121 onward): `configure`, `evaluate`, `simulate`, `insight`, `halt`, `rearm`, `status`, `link`, `set-facts`.

Save as `TraderTour.sleela` in a scratch directory **outside `lib/`** (for example `~/sleela-scratch/advantage`):

```
#sleela 3.0
material;
artifact;
pure;
gasoline;
class TraderTour {
  SLAdvMessage req(String topic) {
    SLAdvMessage m = new SLAdvMessage();
    m.addressed("program", "high-speed-trader", topic, 0);
    return m;
  }
  SLAdvMessage cand(double edge, double now) {
    SLAdvMessage m = req("evaluate");
    m.setNum(0, edge); m.setNum(1, 4.0); m.setNum(2, 20.0); m.setNum(3, 4.0); m.setNum(4, 15.0);
    m.setNum(5, 10.0); m.setNum(6, 0.3); m.setNum(7, 0.2); m.setNum(8, 0.9); m.setNum(9, 4.0);
    m.setNum(10, now);
    return m;
  }
  void main() {
    SLAdvMedium med = new SLAdvMedium();
    med.begin();
    print("load: " + med.load("high-speed-trader") + "  seated=" + med.count());
    SLAdvMessage r = med.ask(req("status"));
    print("status: evals=" + r.num(0) + " accepted=" + r.num(1) + " threshold=" + r.num(2) + " pragmatics=" + r.num(3) + " halted=" + r.num(4));
    SLAdvMessage c = req("configure");
    c.setNum(0, 7.0); c.setNum(1, 2000.0);
    r = med.ask(c);
    print("configure 7: status=" + r.status + " text=" + r.text);
    c = req("configure");
    c.setNum(0, 8.0); c.setNum(1, 2000.0);
    r = med.ask(c);
    print("configure 8: status=" + r.status);
    r = med.ask(cand(40.0, 1000.0));
    print("edge 40: decision=" + r.num(0) + " code=" + r.num(1) + " score=" + r.num(2) + " threshold=" + r.num(3));
    r = med.ask(cand(3.0, 2000.0));
    print("edge 3 : decision=" + r.num(0) + " code=" + r.num(1));
    r = med.ask(cand(1.0e30, 3000.0));
    print("edge 1e30: decision=" + r.num(0) + " code=" + r.num(1));
    SLAdvMessage s = req("simulate");
    s.setNum(0, 5.0); s.setNum(1, 1.0);
    r = med.ask(s);
    print("simulate 5s: evals=" + r.num(0) + " accepted=" + r.num(1) + " vetoes=" + r.num(2) + " scoreRej=" + r.num(3) + " rateRej=" + r.num(4) + " pnl=" + r.num(5) + " threshold=" + r.num(6) + " halted=" + r.num(7));
    med.ask(req("halt"));
    r = med.ask(cand(40.0, 900000.0));
    print("after halt: decision=" + r.num(0) + " code=" + r.num(1) + " (27 = G2 kill switch)");
    med.ask(req("rearm"));
    r = med.ask(cand(40.0, 900200.0));
    print("after rearm: decision=" + r.num(0) + " code=" + r.num(1));
    SLAdvMessage ins = req("insight");
    ins.setNum(0, 4.0); ins.setNum(1, 5.0);
    r = med.ask(ins);
    print("insight factor 5.0: status=" + r.status + " text=" + r.text);
    ins = req("insight");
    ins.setNum(0, 4.0); ins.setNum(1, 1.5);
    r = med.ask(ins);
    print("insight factor 1.5: status=" + r.status);
    med.shutdown();
  }
}
```

Run it:

```sh
cd ~/sleela-scratch/advantage
timeout 120 $SLEELA_HOME/impl/build/sleela run TraderTour.sleela
```

Real output (exit 0):

```
load: true  seated=1
status: evals=0 accepted=0 threshold=0.6 pragmatics=16 halted=0
configure 7: status=1 text=refused: pragmatics 8..24, rate above 0
configure 8: status=0
edge 40: decision=1 code=0 score=0.96875 threshold=0.6
edge 3 : decision=0 code=11
edge 1e30: decision=0 code=10
simulate 5s: evals=25000 accepted=174 vetoes=10660 scoreRej=7383 rateRej=6785 pnl=329.05 threshold=0.62 halted=0
after halt: decision=0 code=27 (27 = G2 kill switch)
after rearm: decision=1 code=0
insight factor 5.0: status=1 text=insight refused
insight factor 1.5: status=0
```

How to read it:

- A module starts configured at **16 pragmatics** and 2000 trades per minute (`begin` calls `configure(16, 2000.0)`).
- `configure` with 7 is refused (status 1) and the previous setting stays. The bounds are 8..24 pragmatics and a rate above 0 up to 600,000 (`SLHighSpeedTrader.sleela` line 101).
- The `evaluate` reply carries decision (`num0`: 1 accept, 0 reject), reject code (`num1`), score (`num2`) and threshold (`num3`). Codes: 0 accepted, 1 not ready (unconfigured), 10+id vetoed by pragmatic id (11 = L2 positive net edge: edge 3 minus cost 4 is negative; 10 = L1 well-formed inputs, tripped by 1e30), 100 score below threshold, 101 rate governor.
- `simulate` (seconds 1..600, seed) pushes 5000 candidates per simulated second through the same `evaluate`; here 25,000 candidates gave 174 acceptances.
- `halt` makes the kill switch pragmatic (G2, id 17) fail, so the same strong candidate gets code 27; `rearm` is the human act that restores it.
- `insight(id, factor)` takes a factor in 0.5..2.0 for one weight. 5.0 is **refused**, not clamped (status 1). Hard minimums cannot be changed by it (code comments, `SLHighSpeedTrader.sleela` lines 174-178; demo check "insight cannot unlock the kill switch").

The trader module depends on `SLMoralTrade` (`begin` builds one for its default facts), which lives in `moral/`. This works because the file-name index of `SLEELA_LIB` finds it; see the [advantage-modules tutorial](advantage-modules.md), Step 2.

## Step 3 - Read how the evaluator judges accepted trades (module to module)

There is no new command here: this is the host demo's trader/evaluator linkage, already run in Step 1 of the [advantage-modules tutorial](advantage-modules.md) (46/46); the details below come from reading `SLAdvTraderModule.sleela`. With `link` set to `moral-evaluator`, each accepted candidate is sent on as a `judge` request. The verdict returns to the trader, which tells the program with a `trade-judged` event. A NO withdraws the trade (the status reply counts it). The trader fills the evaluator's 32 facts from `set-facts` plus three live values: lister standing = venue trust, quote age = ageMs/1000, pace = trades per minute; it fixes decision time 0.0001 min, cooling off 0 h and term 1 day (`judgeFacts`, `SLAdvTraderModule.sleela` line 106). So by construction a high speed trade can be YES but never COMPLETE (demo check: "a high speed trade is YES, not COMPLETE"). See the [moral-evaluator tutorial](moral-evaluator.md).

## What you did

- Ran the 31-check demo and read vetoes by phase, learning on vs. off, the halt, and the insight channel.
- Loaded the trader by name, configured it, evaluated three candidates, simulated 5 seconds, halted and re-armed it, and tried a refused insight.

## Next steps

- [Modules README](../../../lib/advantage/modules/README.md).
- Courseware: [lesson 04](../../../advantage/artificial-intelligence/04-the-high-speed-trader.md), [05](../../../advantage/artificial-intelligence/05-pragmatics-and-evolution.md), [06](../../../advantage/artificial-intelligence/06-fail-closed-and-the-brilliant.md). The lessons are under the repository's `advantage/artificial-intelligence/`, not under `lib/advantage/`. Lesson 04's exercises (`evalsPerSimSecond` to 20,000, target 600 trades/min) are applied by editing a scratch copy of `HstDemo.sleela`; those exercises were not run for this writeup.
- Read `SLHstPragmatics.sleela` for the 24 names and the priority order. The first 8 (the minimal core) are L1, G1, G2, U1, G3, L2, U2, G4, matching lesson 05.

## Status and limits

- Works: demo 31/31; module over the bridge as shown; host demo sections 2, 4, 5, 9 (100,000 candidates through the module, about 97,000 evals/s wall on this machine).
- Synthetic only. The model's "realized" outcome is a formula over the synthetic candidate (`SLHstMarket.realized`), so the learning demo shows the mechanism, not skill.
- The lessons say "Targets at least 5000 evaluations per second and about 2000 trades per minute"; the demo checks the first and checks 1500 or more and at most 2000 (plus burst) per minute for each of P=8, 16, 24.
- Reading the code (not run): the trader treats any kind 1 `judge` message as a verdict without checking who sent it (`SLAdvTraderModule.sleela` line 123).

## Related

- [advantage-modules](advantage-modules.md), [moral-evaluator](moral-evaluator.md)

## Sources

Files read: `lib/advantage/modules/README.md`, `trader/SLHighSpeedTrader.sleela`, `trader/SLHstPragmatics.sleela`, `trader/SLHstMarket.sleela`, `trader/SLAdvTraderModule.sleela`, `trader/HstDemo.sleela`, `demo/AdvantageModulesDemo.sleela`, `advantage/artificial-intelligence/04-the-high-speed-trader.md`, `05-pragmatics-and-evolution.md`, `06-fail-closed-and-the-brilliant.md`.

Commands run: `impl/build/sleela run HstDemo.sleela` (in `trader/` and as a lone copy in a scratch dir); `impl/build/sleela run AdvantageModulesDemo.sleela`; `impl/build/sleela run TraderTour.sleela` (scratch).
