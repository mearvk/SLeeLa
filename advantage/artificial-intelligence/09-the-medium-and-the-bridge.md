# Lesson 09 — The medium and the bridge (advanced)

> Series: [index](README.md) · prev: [08](08-tall-science.md)

The AI modules are Advantage™ components. A **user defined program** loads the
**medium** (the center); the medium holds modules and owns the **bridge** that
carries messages in both directions. Reference:
[modules README](../../lib/advantage/modules/README.md). Example program:
[user-program/UserProgram.sleela](user-program/UserProgram.sleela).

## Read it first

Run `UserProgram.sleela`, then read the order of events: the program loads only
the modules it wants; modules announce `ready` on their own; the program asks and
gets replies; the evaluator volunteers a `cardinal-veto` event nobody asked for.

## Three directions

1. Program to module: `ask` waits for the reply; `tell` does not.
2. Module to program: replies plus unsolicited events, handed to your listener on `pump`.
3. Module to module: `link` the trader to the evaluator and each accepted trade is
   judged; a NO withdraws it. The trader reports `trade-judged` upward.

## Safety you should be able to explain

Bounded queues (refuse, count, never grow); reserved names; error replies for
unknown modules and topics; a 100,000-message pump cap against ping-pong; a closed
bridge after `shutdown`. Why is refusing better than blocking here?

## Exercises

1. Write your own module (implement `SLAdvModule`) that counts every `trade-judged`
   event. How does it hear them if events go to `program`?
2. Load only the evaluator and judge 1,000 trades. What does the ledger say?
3. Make two of your modules answer each other and watch `wasSaturated()`.
4. Moral verdicts for high speed trades are YES, never COMPLETE. Which two time
   points fail, and is that the right outcome?
