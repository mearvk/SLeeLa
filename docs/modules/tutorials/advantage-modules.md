<img align="right" src="../../../images/sleela/sleela-medium-profile-permanent-icon-002.jpg" width="45" height="45" alt="SLeeLa">

# advantage-modules: Tutorial

| | |
|---|---|
| Nature | Advantage module (lib/advantage/modules, not in the sleelactl registry) |
| Registry summary | none (not in the sleelactl registry). The modules README says: a way to load AI components into a user defined program through a medium and a bridge, with conversation in both directions. |
| Lives in | `lib/advantage/modules/` (host: `SLAdvMedium`, `SLAdvBridge`, `SLAdvCatalog`, `SLAdvModule`, `SLAdvListener`, `SLAdvMessage`; demo in `demo/`) |
| Writeup type | Tutorial, because a person writes a program and a module against it, and every step below ran successfully in this checkout. |
| Verified | 2026-11-13 against `master` e048707fd (re-run and corrected by a second pass): `impl/build/sleela run AdvantageModulesDemo.sleela` printed `checks passed=46 failed=0`; `UserProgram.sleela` ran; a user-defined `tally` module written for this tutorial ran in a scratch directory (outputs below). |

## What it is

A user program creates one `SLAdvMedium`. The medium owns a `SLAdvBridge` (two bounded queues of 1024 messages: one toward modules, one toward the program) and seats up to 16 modules. A module is any class that implements the `SLAdvModule` interface. The medium can load two modules by name from `SLAdvCatalog` (`high-speed-trader`, `moral-evaluator`), or the program can `attach` a module object it wrote itself. Messages are `SLAdvMessage` envelopes (id, from, to, topic, text, up to 48 numbers, kind 0 request / 1 reply / 2 event / 3 error). Sources: `lib/advantage/modules/README.md`, `SLAdvMedium.sleela`, `SLAdvBridge.sleela`, `SLAdvMessage.sleela`.

Everything here is synthetic and runs in one process. Nothing opens a network connection (README, "Honest limits").

## Before you start

```sh
cd SLeeLa            # the repository root
export SLEELA_HOME=$PWD SLEELA_LIB=$PWD/lib SLEELA_SHEET=$PWD/SHEET.sheet \
       SLEELA_SHA256_MANIFEST=$PWD/security/sha256-manifest.json
impl/build/sleela version
```

```
[memory-manager] enabled (hard limit 2,147,483,648 bytes)
SLeeLa™ Language/Runtime 1.3.2-pro (supported source syntax 1.0 – 3.1; native C/C++ Core VM)
VM: struct/class layout ceiling 65,536
```

(The Tally program below declares `#sleela 3.0` and runs.)

(The README uses `working-binaries/sleela`; that committed binary also ran the demo with the same 46/46 result. This tutorial uses `impl/build/sleela`.)

Every `run` first prints the SHA-256 gate (`verification: PASS (18667 item(s))`) and ends with three `[memory-manager]` lines. They are left out below.

Make an empty scratch directory **outside `lib/`** for your own files, for example `~/sleela-scratch/advantage`.

## Step 1 - Run the shipped demo

The README says to run it from its own directory.

```sh
cd $SLEELA_HOME/lib/advantage/modules/demo
timeout 120 $SLEELA_HOME/impl/build/sleela run AdvantageModulesDemo.sleela
```

Abridged real output (66 lines in total, exit 0):

```
=== Advantage modules: program, medium, bridge, modules (synthetic, no network) ===
-- 1. the program loads the medium, the medium loads modules --
  PASS  medium begins
  PASS  load high-speed-trader
  PASS  load moral-evaluator
  PASS  two modules seated
  PASS  an unknown name is refused
  PASS  a module already seated is refused
  PASS  both modules told the program they are ready
...
-- 7. a user defined module sits beside the AI modules --
  PASS  the program seats its own module
  PASS  it announced itself
  PASS  the user module answers over the same bridge
  PASS  a user module named program is refused
-- 8. the bridge is bounded and a ping pong cannot hang --
  PASS  posts beyond 1024 pending are refused
  ...
  PASS  the pump gave up after its cap instead of hanging
...
-- 9. the trader at speed, through the bridge --
  PASS  20 simulated seconds evaluated 100000 candidates
  evals=100000  accepted=674  threshold=0.76  wall evals/s=97020.3
  PASS  at least 5000 evaluations per second through the module
...
=== checks passed=46 failed=0 ===
```

The `wall evals/s` figure depends on the machine and will differ on yours.

## Step 2 - See how the demo reaches classes in other directories

`AdvantageModulesDemo.sleela` uses `SLAdvMedium` (parent directory), `SLAdvTraderModule` (reached by the catalog, in `trader/`), `SLMoralTrade` (in `moral/`), and `AdvProgramListener`, `AdvUserEchoModule`, `AdvUserPingModule` (same directory). Its header comment says "run auto-loads the sibling classes by name". Experiments show the real mechanism is different:

| Experiment (all with `timeout 120 impl/build/sleela run`) | Result |
|---|---|
| Copy `demo/*.sleela` to a scratch dir, `SLEELA_LIB=$PWD/lib` | `checks passed=46 failed=0` |
| Same copy, `SLEELA_LIB` unset (`SLEELA_HOME` set) | `checks passed=46 failed=0` |
| Run from the repo root by path (`run lib/advantage/modules/demo/AdvantageModulesDemo.sleela`) | `checks passed=46 failed=0` |
| Run from `/tmp` by absolute path, `SLEELA_LIB` set | `checks passed=46 failed=0` |
| Copy of `demo/*.sleela`, `SLEELA_LIB` = scratch lib holding the host, `trader/` and `moral/` but **not** the three demo classes | `Semantic error: unknown variable type 'AdvProgramListener'`, then `use of undeclared variable 'lis'` ... `(48 semantic errors)`: sibling files next to the entry file are not linked |
| Only `AdvantageModulesDemo.sleela` in a dir, same scratch lib | `Semantic error: unknown variable type 'AdvProgramListener'` |
| Copy of `demo/*.sleela`, `SLEELA_LIB` = an empty dir | `(443 semantic errors)` |

So the linker finds classes by **file name stem** in an index of everything under `SLEELA_LIB` (the index walks the tree recursively: `impl/frontend/library_index.cpp` lines 53 and 61), as described in `impl/docs/SOURCE_LIBRARY_LINKING.md`. It works for `demo/` only because the demo's own helper classes also live under `lib/`. A directory next to your entry file is not searched.

## Step 3 - Write your own module

A module implements the seven methods of `SLAdvModule` (`SLAdvModule.sleela`): `moduleName`, `moduleKind`, `moduleVersion`, `begin(bridge)`, `handle(message)`, `end`, `running`. The program side can implement `SLAdvListener` (`int onMessage(SLAdvMessage m)`).

Save this as `Tally.sleela` in your scratch directory. It holds the module, a listener and the program in one file, the way `advantage/artificial-intelligence/user-program/UserProgram.sleela` does:

```
#sleela 3.0
material;
artifact;
pure;
gasoline;
< A tiny user defined Advantage module: adds up the numbers it is sent. >
class TallyModule implements SLAdvModule {
  SLAdvBridge br;
  boolean on = false;
  int served = 0;
  double total = 0.0;

  String moduleName() { return "tally"; }
  String moduleKind() { return "user.tutorial"; }
  String moduleVersion() { return "0.1.0"; }
  boolean running() { return on; }

  boolean begin(SLAdvBridge bridge) {
    if (bridge == null) { return false; }
    br = bridge;
    on = true;
    SLAdvMessage e = new SLAdvMessage();
    e.addressed("tally", "program", "ready", 2);
    br.post(e);
    return true;
  }

  void end() { on = false; }

  int handle(SLAdvMessage m) {
    if (m.kind != 0) { return 0; }
    served = served + 1;
    if (m.topic == "add") {
      double sum = 0.0;
      int i = 0;
      while (i < m.size()) { sum = sum + m.num(i); i = i + 1; }
      total = total + sum;
      SLAdvMessage r = m.makeReply("tally", "add");
      r.setNum(0, sum);
      r.setNum(1, total);
      r.setNum(2, served * 1.0);
      br.post(r);
      if (total > 100.0) {
        SLAdvMessage ev = new SLAdvMessage();
        ev.addressed("tally", "program", "big-total", 2);
        ev.setNum(0, total);
        ev.text = "running total passed 100";
        br.post(ev);
      }
      return 0;
    }
    br.post(m.makeError("tally", 2, "unknown topic: " + m.topic));
    return 1;
  }
}
class TallyEars implements SLAdvListener {
  int heard = 0;
  int onMessage(SLAdvMessage m) {
    heard = heard + 1;
    print("  [heard] " + m.from + " / " + m.topic + " (kind " + m.kind + ") " + m.text);
    return 0;
  }
}
class TallyProgram {
  SLAdvMessage req(String to, String topic) {
    SLAdvMessage m = new SLAdvMessage();
    m.addressed("program", to, topic, 0);
    return m;
  }
  void main() {
    SLAdvMedium med = new SLAdvMedium();
    TallyEars ears = new TallyEars();
    print("begin: " + med.begin());
    med.attachListener(ears);
    print("attach tally: " + med.attach(new TallyModule()));
    print("attach tally again: " + med.attach(new TallyModule()));
    print("seated: " + med.count() + " first=" + med.nameAt(0));
    med.pump();
    SLAdvMessage a = req("tally", "add");
    a.setNum(0, 1.5); a.setNum(1, 2.5); a.setNum(2, 3.0);
    SLAdvMessage r = med.ask(a);
    print("sum=" + r.num(0) + " total=" + r.num(1) + " served=" + r.num(2));
    SLAdvMessage b = req("tally", "add");
    b.setNum(0, 95.0);
    r = med.ask(b);
    print("sum=" + r.num(0) + " total=" + r.num(1) + " served=" + r.num(2));
    med.pump();
    r = med.ask(req("tally", "subtract"));
    print("kind=" + r.kind + " status=" + r.status + " text=" + r.text);
    r = med.ask(req("nobody", "add"));
    print("kind=" + r.kind + " status=" + r.status + " text=" + r.text);
    med.shutdown();
    print("up after shutdown: " + med.isUp() + "; heard " + ears.heard);
  }
}
```

## Step 4 - Run it

```sh
cd ~/sleela-scratch/advantage
timeout 120 $SLEELA_HOME/impl/build/sleela run Tally.sleela
```

Real output (exit 0):

```
begin: true
attach tally: true
attach tally again: false
seated: 1 first=tally
  [heard] tally / ready (kind 2) 
sum=7 total=7 served=1
sum=95 total=102 served=2
  [heard] tally / big-total (kind 2) running total passed 100
kind=3 status=2 text=unknown topic: subtract
kind=3 status=404 text=no such module: nobody
up after shutdown: false; heard 2
```

What the run shows:

- `attach` returned true once and false for a second module with the same name (`SLAdvMedium.sleela` line 118: a name must be non-empty, not `program`/`medium`, not already seated).
- `begin` posted a `ready` event addressed to `program`. It sat in the program queue until `med.pump()` handed it to the listener (`pump` = `pumpModules` + `deliver`, lines 159-205).
- `ask` stamps `from = "program"`, posts, routes everything pending, then takes the reply whose `inReplyTo` matches (line 206, `SLAdvBridge.takeReply` line 130). Replies are not delivered to the listener; events are.
- The `big-total` event was posted when the total passed 100, and reached the listener on the next `pump`.
- An unknown topic is your module's choice; the medium itself only answers for an unknown module: kind 3, status 404.

## Step 5 - Run the program the repo ships outside `lib`

```sh
cd $SLEELA_HOME/advantage/artificial-intelligence/user-program
timeout 120 $SLEELA_HOME/impl/build/sleela run UserProgram.sleela
```

```
1. load only the modules this program wants
  [module says] moral-evaluator / ready 
  [module says] high-speed-trader / ready 
   seated: 2
2. program asks the moral evaluator to judge a trade
   verdict 0 (0 NO, 1 YES, 2 COMPLETE)  merit 0.977778  reason: cardinal veto
  [module says] moral-evaluator / cardinal-veto T01 life is safe
3. program asks the trader for a status, then simulates 5 seconds
   pragmatics 16  evals so far 0
   evals 25000  accepted 174  threshold 0.64
done; the program heard 3 unsolicited messages
```

Note the three kinds of traffic: replies (returned by `ask`), events (to the listener on `pump`), and the unsolicited `cardinal-veto` event the evaluator sends by itself.

## Step 6 - Two things that do not work (so you do not lose time)

1. **Passing your module in a second file.** `sleela run Program.sleela TallyModule.sleela` (module and program in separate files, lib auto-link needed for `SLAdv*`) failed with `(103 semantic errors)` when the module class (`TallyModule`) was in one file and the listener and program classes in the other (a different split gave 104); the errors are of the form `duplicate method 'SLAdvBridge::getMetaVersion'` (also `SLAdvMessage`, `SLAdvModule`, `SLAdvListener`, `String`). The same two-file form worked only when `SLEELA_LIB` pointed at an empty directory and the program needed no library classes (checked with `HstDemo.sleela` plus `SLHighSpeedTrader`, `SLHstMarket`, `SLHstPragmatics`: 31/31). With `SLEELA_LIB` empty the Tally pair fails with `unknown return type 'SLAdvMessage'`, because it needs the host classes. With lib auto-linking in play, keep your classes in a single file, as above.
2. **Program file alone, module in a second file that is not listed.** `sleela run TallyProgram.sleela` gave `Semantic error: new of unknown class or struct 'TallyModule'` (re-checked); with the module in the first file and the listener in the other, the first error is `unknown variable type 'TallyEars'`: a file next to the entry file is not found (same finding as Step 2).

## What you did

- Ran the host demo (46/46) and saw why it resolves classes in `trader/` and `moral/`: file-name index of `SLEELA_LIB`, not sibling directories.
- Wrote a module (`TallyModule`) and a listener, seated it with `attach`, asked it, and received an event on `pump`.
- Saw the host's own error replies (404 unknown module) and refusal of a duplicate name.

## Next steps

- [`lib/advantage/modules/README.md`](../../../lib/advantage/modules/README.md): parts, topics, safety, limits.
- [High speed trader](high-speed-trader.md) and [Moral evaluator](moral-evaluator.md) tutorials.
- Courseware lessons: [`advantage/artificial-intelligence/09-the-medium-and-the-bridge.md`](../../../advantage/artificial-intelligence/09-the-medium-and-the-bridge.md) and its exercises (a module that counts `trade-judged` events; two modules answering each other; `wasSaturated()`).
- `demo/AdvUserEchoModule.sleela`, `demo/AdvUserPingModule.sleela`, `demo/AdvProgramListener.sleela`: the demo's own user-defined module and listener.

## Status and limits

- The medium refuses modules named `program` or `medium`, duplicates, a null module, and a 17th module (`SLAdvMedium.sleela` lines 118-130). Demo checks cover the first two.
- Posts beyond 1024 pending are refused with -1 and counted. A pump handles at most 100,000 messages and sets `wasSaturated()` (demo section 8: ping-a/ping-b pair).
- `ask` is synchronous: routing runs on the caller thread (README).
- Reading the code (not run): `SLAdvBridge.post` does not check that `from` is the true sender, so a module can write any `from`. The trader accepts a kind 1 `judge` message from any sender as a verdict (`SLAdvTraderModule.sleela` line 123). Treat the bridge as a cooperative in-process mechanism, not a security boundary.

## Related

- [high-speed-trader](high-speed-trader.md), [moral-evaluator](moral-evaluator.md)
- `impl/docs/SOURCE_LIBRARY_LINKING.md`

## Sources

Files read: `lib/advantage/modules/README.md`, `SLAdvModule.sleela`, `SLAdvListener.sleela`, `SLAdvMessage.sleela`, `SLAdvBridge.sleela`, `SLAdvMedium.sleela`, `SLAdvCatalog.sleela`, `demo/AdvantageModulesDemo.sleela`, `demo/AdvUserEchoModule.sleela`, `demo/AdvUserPingModule.sleela`, `demo/AdvProgramListener.sleela`, `advantage/artificial-intelligence/README.md`, `09-the-medium-and-the-bridge.md`, `user-program/UserProgram.sleela`, `impl/docs/SOURCE_LIBRARY_LINKING.md`, `impl/frontend/library_index.cpp`.

Commands run: `impl/build/sleela version`; `impl/build/sleela run` on `AdvantageModulesDemo.sleela` (in place, as a scratch copy with three different `SLEELA_LIB` settings, from the repo root and from `/tmp`); `working-binaries/sleela run AdvantageModulesDemo.sleela`; `UserProgram.sleela`; the scratch `Tally.sleela`; the two-file and single-file failure cases above.
