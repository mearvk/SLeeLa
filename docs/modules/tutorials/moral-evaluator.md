<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# moral-evaluator: Tutorial

| | |
|---|---|
| Nature | Advantage module (lib/advantage/modules, not in the sleelactl registry) |
| Registry summary | none (not in the sleelactl registry). `SLAdvCatalog` lists it as `moral-evaluator`; module kind `ai.moral`, version `1.0.0` (`moral/SLAdvMoralModule.sleela`). |
| Lives in | `lib/advantage/modules/moral/` (`SLMoralEvaluator`, `SLMoralTrade`, `SLTallScience`, `SLMoralMeritLedger`, `SLAdvMoralModule`, `MoralDemo.sleela`). Courseware: `advantage/artificial-intelligence/07`, `08`. |
| Writeup type | Tutorial, because you drive it with `sleela run`, directly (`MoralDemo.sleela`) and over the bridge from your own program, and every step below ran successfully. |
| Verified | 2026-11-13 against `master` e048707fd (re-run by a second pass; outputs identical except the throughput line): `MoralDemo.sleela` printed `checks passed=39 failed=0`; the host demo's moral sections passed (46/46); a scratch program `MoralTour.sleela` drove the module over the bridge (output below). |

## What it is

A **rule-based teaching model**, not a source of ethical, legal or financial advice. Its verdicts are outputs of fixed rules over made-up inputs; they are not moral judgements about any real trade or person, and nobody should rely on them to decide what to do. The program describes one trade with 32 facts (`SLMoralTrade`: booleans such as `consent`, numbers such as `priceRatio`). The evaluator turns those facts into up to 41 yes/no **points** with fixed rules written in `SLMoralEvaluator.answers` (for example `yes[5] = priceRatio >= 0.8 && priceRatio <= 1.25`; `yes[14]` = quote age 0..60 s; `yes[15]` = pace at most 2000 trades per minute; `yes[11]` = at least 5 minutes to decide). The README itself calls the points "rule-based teaching choices on facts the caller supplies". It checks nothing about the real world; whatever facts you supply are what it judges.

The 41 points fall in five dimensions: trades (T01..T09), time (M01..M07), listers (L01..L08), United States (U01..U08), tall science (S01..S09). The order of judgement (`SLMoralEvaluator.sleela` line 244):

1. **Cardinal veto.** Five points are vetoes: T01 life is safe, T02 the human is the end, T03 informed consent, T04 no deception, U02 lawful subject. One "no" gives verdict NO regardless of merit.
2. **Dimension floor.** Each dimension that has active points must have at least 0.5 of its points yes, else NO ("dimension below floor").
3. **Merit.** Merit is the mean of the dimension fractions; it must reach the threshold (starts 0.80).
4. Then YES, or COMPLETE if all 41 points are yes and the evaluator is in three-state mode. Risk is reported as 1 minus merit.

`SLTallScience` holds the United States as a catalogued item with four lists (nouns, names, scientific roles, moral roles; each at most 64) and the points S01..S09 read from it; `seed()` fills it with role labels (`scientist (role)`, `ethicist (role)`...), not people. "Respectively" means nouns and names must be equal in count (`paired()`). `SLMoralMeritLedger` keeps counts, mean and recent merit, the worst merit and cardinal-fail counts over many trades.

## Before you start

```sh
cd SLeeLa            # the repository root
export SLEELA_HOME=$PWD SLEELA_LIB=$PWD/lib SLEELA_SHEET=$PWD/SHEET.sheet \
       SLEELA_SHA256_MANIFEST=$PWD/security/sha256-manifest.json
```

Each `run` prints the SHA-256 gate (`verification: PASS (18667 item(s))`) first and three `[memory-manager]` lines last (omitted below). The throughput line varies by machine.

## Step 1 - Run the shipped demo

```sh
cd $SLEELA_HOME/lib/advantage/modules/moral
timeout 120 $SLEELA_HOME/impl/build/sleela run MoralDemo.sleela
```

Real output (64 lines, exit 0), abridged:

```
=== Moral Evaluator courseware model (rule based, synthetic, a teaching model) ===
-- 1. fail closed configuration --
  PASS  4 points refused (cardinals need 5)
  PASS  42 points refused
  PASS  4 verdict states refused
  PASS  unconfigured evaluator says NO
  PASS  unconfigured reason is not ready
-- 2. the Tall Science catalog: the United States as an Item --
  ...
  PASS  nouns and names are paired respectively
  PASS  3 nouns, 3 names, 3 scientific, 3 moral
  PASS  an unmatched noun breaks the respective pairing
  PASS  adding the matching name restores it
-- 3. judging trades (all 41 points, three verdict states) --
  PASS  exemplary trade is COMPLETE
  exemplary: COMPLETE  merit=1  risk=0  reason=complete
  two soft misses: YES  merit=0.946429  risk=0.0535714  reason=merit sufficient
  endangers life: NO  merit=0.977778  risk=0.0222222  reason=cardinal veto
  ...
  no Tall Science catalog: NO  merit=0.8  risk=0.2  reason=dimension below floor
  unfair trade: NO  merit=0.888889  risk=0.111111  reason=dimension below floor
  scattered soft misses: YES  merit=0.814286  risk=0.185714  reason=merit sufficient
-- 4. as few as 5 points, binary or three states --
  PASS  5 cardinal points: a fine trade is YES, never COMPLETE
  PASS  5 cardinal points still veto danger to life
  PASS  binary mode reports an exemplary trade as YES
-- 5. evolution: a human reviewer moves the threshold --
  PASS  a wrong YES raises the threshold
  PASS  wrong NOs lower it again
  PASS  the threshold stays at or below 0.95
  PASS  a cardinal veto is not affected by review
  PASS  an out of range verdict in review is refused
-- 6. overall trades over time: the ledger --
  trades=20000  NO=1787  YES=4202  COMPLETE=14011
  mean merit=0.990484  recent merit=0.992018  worst=0.870635  risk of merit=0.00951625
  cardinal vetoes: life=390 consent=800 lawful=597
  judged 68050.7 trades per second (41 points each)
  ...
=== checks passed=39 failed=0 ===
```

Two lines worth reading closely:

- `endangers life: NO merit=0.977778`: 40 of 41 points are yes, yet the verdict is NO. Merit never buys back a cardinal point (the courseware's lesson 07 says the same).
- `no Tall Science catalog: NO merit=0.8`: with no catalog the nine tall-science points are all "no", so that dimension is 0 of 9, below the floor. Merit 0.8 would otherwise have been enough.

The 20,000-trade ledger in section 6 is made from the demo's own pseudo-random trade generator (`rnd()` in `MoralDemo.sleela`), so those counts describe that generator's choices, not any population of real trades.

The demo's classes are found through `SLEELA_LIB`, not through the directory the entry file sits in (see the [advantage-modules tutorial](advantage-modules.md), Step 2). Check: all of `moral/*.sleela` copied to a scratch directory and run with `SLEELA_LIB` pointing at an empty directory gave `(298 semantic errors)`.

## Step 2 - Drive the module over the bridge

Topics (README; `SLAdvMoralModule.sleela` line 98 onward): `judge` (n0..n31 trade facts in `SLMoralTrade` wire order), `configure` (n0 points 5..41, n1 states 2 or 3), `seed-catalog`, `register` (n0 list 0..3, text entry), `review` (n0 verdict that was given, n1 nonzero if it was correct), `ledger`, `status`. A `judge` reply carries verdict (`num0`: 0 NO, 1 YES, 2 COMPLETE), merit, risk, the failing cardinal point id or -1, the weak dimension or -1, the number of active points, and the reason in `text`. If a cardinal point failed, the module also sends the program an unsolicited `cardinal-veto` event.

Save as `MoralTour.sleela` in a scratch directory **outside `lib/`** (for example `~/sleela-scratch/advantage`):

```
#sleela 3.0
material;
artifact;
pure;
gasoline;
class MoralTour {
  SLAdvMessage judge(SLMoralTrade t) {
    SLAdvMessage m = new SLAdvMessage();
    m.addressed("program", "moral-evaluator", "judge", 0);
    double[] w = new double[32];
    t.toNums(w);
    int i = 0;
    while (i < 32) { m.setNum(i, w[i]); i = i + 1; }
    return m;
  }
  SLAdvMessage req(String topic) {
    SLAdvMessage m = new SLAdvMessage();
    m.addressed("program", "moral-evaluator", topic, 0);
    return m;
  }
  void show(String label, SLAdvMessage r) {
    print(label + ": verdict=" + r.num(0) + " merit=" + r.num(1) + " risk=" + r.num(2) + " cardinal=" + r.num(3) + " weakDim=" + r.num(4) + " points=" + r.num(5) + " reason=" + r.text);
  }
  void main() {
    SLAdvMedium med = new SLAdvMedium();
    med.begin();
    print("load: " + med.load("moral-evaluator"));
    SLMoralTrade t = new SLMoralTrade();
    show("default trade   ", med.ask(judge(t)));
    t.consent = false;
    show("no consent      ", med.ask(judge(t)));
    SLMoralTrade q = new SLMoralTrade();
    q.listerCount = 1;
    q.quoteAgeSeconds = 90.0;
    show("two soft misses ", med.ask(judge(q)));
    SLAdvMessage cf = req("configure");
    cf.setNum(0, 5.0); cf.setNum(1, 3.0);
    SLAdvMessage r = med.ask(cf);
    print("configure 5 points: status=" + r.status);
    show("default, 5 pts  ", med.ask(judge(new SLMoralTrade())));
    cf = req("configure");
    cf.setNum(0, 4.0); cf.setNum(1, 3.0);
    r = med.ask(cf);
    print("configure 4 points: status=" + r.status + " text=" + r.text);
    cf = req("configure");
    cf.setNum(0, 41.0); cf.setNum(1, 2.0);
    med.ask(cf);
    show("default, binary ", med.ask(judge(new SLMoralTrade())));
    SLAdvMessage rv = req("review");
    rv.setNum(0, 1.0); rv.setNum(1, 0.0);
    r = med.ask(rv);
    print("review (YES was wrong): threshold now " + r.num(0));
    r = med.ask(req("ledger"));
    print("ledger: count=" + r.num(0) + " NO=" + r.num(1) + " YES=" + r.num(2) + " COMPLETE=" + r.num(3) + " mean=" + r.num(4) + " recent=" + r.num(5) + " worst=" + r.num(6) + " risk=" + r.num(7));
    r = med.ask(req("status"));
    print("status: active=" + r.num(0) + " threshold=" + r.num(1) + " nouns=" + r.num(2) + " names=" + r.num(3) + " paired=" + r.num(4));
    med.shutdown();
  }
}
```

```sh
cd ~/sleela-scratch/advantage
timeout 120 $SLEELA_HOME/impl/build/sleela run MoralTour.sleela
```

Real output (exit 0):

```
load: true
default trade   : verdict=2 merit=1 risk=0 cardinal=-1 weakDim=-1 points=41 reason=complete
no consent      : verdict=0 merit=0.977778 risk=0.0222222 cardinal=2 weakDim=-1 points=41 reason=cardinal veto
two soft misses : verdict=1 merit=0.946429 risk=0.0535714 cardinal=-1 weakDim=-1 points=41 reason=merit sufficient
configure 5 points: status=0
default, 5 pts  : verdict=1 merit=1 risk=0 cardinal=-1 weakDim=-1 points=5 reason=merit sufficient
configure 4 points: status=1 text=refused: points 5..41, states 2 or 3
default, binary : verdict=1 merit=1 risk=0 cardinal=-1 weakDim=-1 points=41 reason=merit sufficient
review (YES was wrong): threshold now 0.81
ledger: count=5 NO=1 YES=3 COMPLETE=1 mean=0.984841 recent=0.994041 worst=0.946429 risk=0.0151587
status: active=41 threshold=0.81 nouns=3 names=3 paired=1
```

How to read it:

- `cardinal=2` is point T03 (informed consent), the id of the first failing cardinal point; `cardinal=-1` means none.
- With five points only the cardinals are active, so a fine trade is YES and can never be COMPLETE (COMPLETE needs all 41 active). In binary mode (states 2) the exemplary trade is YES too.
- `configure` with 4 points is refused (status 1); the previous configuration stays.
- `review` with verdict 1 and correct = 0 ("that YES was wrong") raised the threshold from 0.80 to 0.81. The threshold is held within 0.60..0.95 and a review never changes cardinal points or the floor.
- The ledger has counted all five judgements made through this module (the module notes every `judge`).

## Step 3 - Read how the catalogue pairing is checked (no new command)

This step has no command of its own; it is taken from the host demo, which ran 46/46 in the [advantage-modules tutorial](advantage-modules.md), Step 1. That demo (section 3) registers a noun without a name (`register` with n0 = 0, text `treaty`); `status` then reports `paired=0` (num4 = 0) until a name is registered too. That is the "respectively" rule of `SLTallScience.paired()` (nouns and names equal in count and at least one). It passed in the 46-check host demo (`an unmatched noun now breaks the respective pairing`).

## What you did

- Ran the 39-check demo and read the order: cardinal veto, dimension floor, merit, verdict.
- Loaded the evaluator by name, judged trades through the bridge, changed the point count and verdict states, applied a human review and read the ledger.

## Next steps

- [Modules README](../../../lib/advantage/modules/README.md).
- Courseware: [lesson 07](../../../advantage/artificial-intelligence/07-the-moral-evaluator.md) and [lesson 08](../../../advantage/artificial-intelligence/08-tall-science.md) (under the repository's `advantage/artificial-intelligence/`, not `lib/advantage/`). The lesson exercises (e.g. "add a 42nd point") were not run for this writeup.
- Let the trader consult it: [high-speed-trader tutorial](high-speed-trader.md), Step 3.

## Status and limits

- Works: demo 39/39; module over the bridge as shown; host demo sections 3-5.
- A trade without a catalogue (`ts == null`) fails all nine tall-science points and is therefore NO ("dimension below floor"), as the demo shows.
- The point rules are fixed constants in source (price ratio 0.8..1.25, quote age at most 60 s, at least 5 minutes to decide, pace at most 2000 per minute, lister standing at least 0.5, at least 2 listers). They are teaching choices (README), not a standard; the model does not verify any fact it is given.
- Lesson 07 says "Which dimension floor catches a trade that is unfair but lawful?": in the demo the `unfair trade` case is stopped by the trades dimension (`weakDim == 0`), merit 0.889.
- Reading the code (not run): `configure` rebuilds the point tables but leaves the learned threshold unchanged.

## Related

- [advantage-modules](advantage-modules.md), [high-speed-trader](high-speed-trader.md)

## Sources

Files read: `lib/advantage/modules/README.md`, `moral/SLMoralEvaluator.sleela`, `moral/SLMoralTrade.sleela`, `moral/SLTallScience.sleela`, `moral/SLMoralMeritLedger.sleela`, `moral/SLAdvMoralModule.sleela`, `moral/MoralDemo.sleela`, `demo/AdvantageModulesDemo.sleela`, `advantage/artificial-intelligence/07-the-moral-evaluator.md`, `08-tall-science.md`.

Commands run: `impl/build/sleela run MoralDemo.sleela` (in `moral/`; also a lone copy with an empty `SLEELA_LIB`); `impl/build/sleela run AdvantageModulesDemo.sleela`; `impl/build/sleela run MoralTour.sleela` (scratch).
